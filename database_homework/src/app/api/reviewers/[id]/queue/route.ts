import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { withTransaction } from "@/lib/db";
import { jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { queueSchema } from "@/lib/validators";

type QueueRow = {
  id: number;
  solver_id: number;
  task_id: number;
  answer_text: string;
  submitted_answer: string;
  subject_name: string;
  task_body: string;
};

export async function POST(
  req: NextRequest,
  { params }: { params: { id: string } }
) {
  try {
    const reviewerId = Number(params.id);
    if (!reviewerId) {
      return jsonError("Некорректный идентификатор проверяющего", 400);
    }

    let parsedBody: unknown = {};
    try {
      parsedBody = await req.json();
    } catch {
      parsedBody = {};
    }

    const { subjectName } = queueSchema.parse(parsedBody);

    const payload = await withTransaction(async (client) => {
      const subjectsResult = await client.query<{ subject_name: string }>(
        `SELECT subject_name FROM reviewer_subjects WHERE reviewer_id = $1`,
        [reviewerId]
      );
      const allowedSubjects = subjectsResult.rows.map((row) => row.subject_name);

      if (!allowedSubjects.length) {
        throw new Error("У проверяющего не назначено ни одного предмета");
      }

      const filteredSubjects = subjectName
        ? allowedSubjects.filter((subject) => subject === subjectName)
        : allowedSubjects;

      if (!filteredSubjects.length) {
        throw new Error("Предмет не входит в список проверяющего");
      }

      const result = await client.query<QueueRow>(
        `SELECT s.id,
                s.solver_id,
                s.task_id,
                s.answer_text,
                s.submitted_answer,
                t.subject_name,
                t.body AS task_body
           FROM solutions s
           JOIN tasks t ON t.id = s.task_id
          WHERE s.reviewer_id IS NULL
            AND s.score IS NULL
            AND t.subject_name = ANY($1)
          ORDER BY s.created_at ASC
          LIMIT 1`,
        [filteredSubjects]
      );

      if (!result.rowCount) {
        return null;
      }

      const solution = result.rows[0];
      await client.query(
        `UPDATE solutions
            SET reviewer_id = $1,
                assigned_at = NOW()
          WHERE id = $2`,
        [reviewerId, solution.id]
      );

      return solution;
    });

    if (!payload) {
      return jsonOk({ message: "Нет доступных решений" });
    }

    return jsonOk({
      id: payload.id,
      solverId: payload.solver_id,
      taskId: payload.task_id,
      taskText: payload.task_body,
      subject: payload.subject_name,
      answerText: payload.answer_text,
      submittedAnswer: payload.submitted_answer
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
