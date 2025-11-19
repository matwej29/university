import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query } from "@/lib/db";
import { jsonCreated, jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { solutionSchema } from "@/lib/validators";

type SolutionRow = {
  id: number;
  solver_id: number;
  task_id: number;
  answer_text: string;
  submitted_answer: string;
  reviewer_id: number | null;
  score: number | null;
  comment: string | null;
  assigned_at: string | null;
  reviewed_at: string | null;
  created_at: string;
  subject_name: string;
  task_body: string;
};

const STATUS_CONDITIONS: Record<string, string> = {
  pending: "s.reviewer_id IS NULL",
  assigned: "s.reviewer_id IS NOT NULL AND s.score IS NULL",
  graded: "s.score IS NOT NULL"
};

export async function GET(req: NextRequest) {
  try {
    const { searchParams } = new URL(req.url);
    const status = searchParams.get("status") ?? "pending";
    const reviewerId = searchParams.get("reviewerId");
    const subject = searchParams.get("subject");
    const limit = Math.min(Number(searchParams.get("limit")) || 25, 100);

    const conditions: string[] = [];
    const values: (string | number)[] = [];

    const statusCondition = STATUS_CONDITIONS[status];
    if (statusCondition) {
      conditions.push(statusCondition);
    }

    if (reviewerId) {
      conditions.push(`s.reviewer_id = $${values.length + 1}`);
      values.push(Number(reviewerId));
    }

    if (subject) {
      conditions.push(`t.subject_name = $${values.length + 1}`);
      values.push(subject);
    }

    const where = conditions.length ? `WHERE ${conditions.join(" AND ")}` : "";
    const result = await query<SolutionRow>(
      `SELECT s.*, t.subject_name, t.body AS task_body
         FROM solutions s
         JOIN tasks t ON t.id = s.task_id
        ${where}
        ORDER BY s.created_at ASC
        LIMIT $${values.length + 1}`,
      [...values, limit]
    );

    const solutions = result.rows.map((row) => ({
      id: row.id,
      solverId: row.solver_id,
      taskId: row.task_id,
      taskText: row.task_body,
      subject: row.subject_name,
      answerText: row.answer_text,
      submittedAnswer: row.submitted_answer,
      reviewerId: row.reviewer_id,
      score: row.score,
      comment: row.comment,
      assignedAt: row.assigned_at,
      reviewedAt: row.reviewed_at,
      createdAt: row.created_at
    }));

    return jsonOk({ solutions });
  } catch (error) {
    return jsonServerError(error);
  }
}

export async function POST(req: NextRequest) {
  try {
    const payload = solutionSchema.parse(await req.json());
    const inserted = await query<SolutionRow>(
      `INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer)
       VALUES ($1, $2, $3, $4)
       RETURNING *,
                 (SELECT subject_name FROM tasks WHERE id = $2) AS subject_name,
                 (SELECT body FROM tasks WHERE id = $2) AS task_body`,
      [payload.solverId, payload.taskId, payload.answerText, payload.submittedAnswer]
    );

    const row = inserted.rows[0];
    return jsonCreated({
      id: row.id,
      solverId: row.solver_id,
      taskId: row.task_id,
      subject: row.subject_name,
      taskText: row.task_body,
      answerText: row.answer_text,
      submittedAnswer: row.submitted_answer,
      reviewerId: row.reviewer_id,
      score: row.score,
      comment: row.comment,
      createdAt: row.created_at
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
