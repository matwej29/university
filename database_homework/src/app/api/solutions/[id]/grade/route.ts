import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query } from "@/lib/db";
import { jsonCreated, jsonError, jsonServerError } from "@/lib/http";
import { gradeSchema } from "@/lib/validators";

type SolutionRow = {
  id: number;
  reviewer_id: number | null;
  score: number | null;
  comment: string | null;
  reviewed_at: string | null;
};

export async function POST(
  req: NextRequest,
  { params }: { params: { id: string } }
) {
  try {
    const solutionId = Number(params.id);
    if (!solutionId) {
      return jsonError("Некорректный идентификатор решения", 400);
    }

    const payload = gradeSchema.parse(await req.json());

    const result = await query<SolutionRow>(
      `UPDATE solutions
          SET reviewer_id = COALESCE(reviewer_id, $1),
              score = $2,
              comment = $3,
              reviewed_at = NOW()
        WHERE id = $4
        RETURNING id, reviewer_id, score, comment, reviewed_at`,
      [payload.reviewerId, payload.score, payload.comment ?? null, solutionId]
    );

    if (!result.rowCount) {
      return jsonError("Решение не найдено", 404);
    }

    const row = result.rows[0];
    return jsonCreated({
      id: row.id,
      reviewerId: row.reviewer_id,
      score: row.score,
      comment: row.comment,
      reviewedAt: row.reviewed_at
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
