import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query, withTransaction } from "@/lib/db";
import { jsonCreated, jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { reviewerSchema } from "@/lib/validators";

type ReviewerRow = {
  id: number;
  full_name: string;
  subjects: string[];
};

export async function GET() {
  try {
    const result = await query<ReviewerRow>(
      `SELECT r.id, r.full_name,
              COALESCE(json_agg(rs.subject_name) FILTER (WHERE rs.subject_name IS NOT NULL), '[]') AS subjects
         FROM reviewers r
         LEFT JOIN reviewer_subjects rs ON rs.reviewer_id = r.id
        GROUP BY r.id
        ORDER BY r.full_name`
    );
    const reviewers = result.rows.map((row) => ({
      id: row.id,
      fullName: row.full_name,
      subjects: row.subjects ?? []
    }));
    console.log("Reviewers fetched:", reviewers.length);
    return jsonOk({ reviewers });
  } catch (error) {
    return jsonServerError(error);
  }
}

export async function POST(req: NextRequest) {
  try {
    const payload = reviewerSchema.parse(await req.json());

    const reviewer = await withTransaction(async (client) => {
      const reviewerResult = await client.query<{ id: number; full_name: string }>(
        "INSERT INTO reviewers (full_name) VALUES ($1) RETURNING id, full_name",
        [payload.fullName]
      );
      const inserted = reviewerResult.rows[0];

      if (payload.subjectNames.length) {
        const values = payload.subjectNames.map((name, idx) => `($1, $${idx + 2})`).join(", ");
        await client.query(
          `INSERT INTO reviewer_subjects (reviewer_id, subject_name)
           VALUES ${values}
           ON CONFLICT (reviewer_id, subject_name) DO NOTHING`,
          [inserted.id, ...payload.subjectNames]
        );
      }

      return inserted;
    });

    return jsonCreated({
      id: reviewer.id,
      fullName: reviewer.full_name,
      subjects: payload.subjectNames
    });
  } catch (error) {
    if (error instanceof ZodError) {
      return jsonError(error.issues.map((i) => i.message).join(", "), 400);
    }
    if (error instanceof Error) {
      return jsonError(error.message, 400);
    }
    return jsonServerError(error);
  }
}
