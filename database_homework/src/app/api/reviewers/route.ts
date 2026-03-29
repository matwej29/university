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
    // Получаем плоский список (проверяющий -> предмет)
    const result = await query<{ id: number; full_name: string; subject_name: string | null }>(
      `SELECT r.id, r.full_name, rs.subject_name
         FROM reviewers r
         LEFT JOIN reviewer_subjects rs ON rs.reviewer_id = r.id
        ORDER BY r.full_name`
    );

    // Группируем в JavaScript
    const reviewersMap = new Map<number, { id: number; fullName: string; subjects: string[] }>();

    for (const row of result.rows) {
      if (!reviewersMap.has(row.id)) {
        reviewersMap.set(row.id, {
          id: row.id,
          fullName: row.full_name,
          subjects: []
        });
      }
      if (row.subject_name) {
        reviewersMap.get(row.id)!.subjects.push(row.subject_name);
      }
    }

    return jsonOk({ reviewers: Array.from(reviewersMap.values()) });
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
