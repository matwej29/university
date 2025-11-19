import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query } from "@/lib/db";
import { jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { assignSchema } from "@/lib/validators";

type SolutionRow = {
  id: number;
  reviewer_id: number | null;
  subject_name: string;
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

    const payload = assignSchema.parse(await req.json());

    const result = await query<SolutionRow>(
      `UPDATE solutions s
          SET reviewer_id = $1,
              assigned_at = NOW()
        WHERE s.id = $2
          AND (s.reviewer_id IS NULL OR s.reviewer_id = $1)
          AND s.score IS NULL
        RETURNING s.id, s.reviewer_id,
                  (SELECT subject_name FROM tasks WHERE id = s.task_id) AS subject_name`,
      [payload.reviewerId, solutionId]
    );

    if (!result.rowCount) {
      return jsonError("Решение уже назначено или проверено", 409);
    }

    return jsonOk({
      id: result.rows[0].id,
      reviewerId: result.rows[0].reviewer_id,
      subject: result.rows[0].subject_name
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
