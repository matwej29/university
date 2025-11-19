import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query } from "@/lib/db";
import { jsonCreated, jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { taskSchema } from "@/lib/validators";

type TaskRow = {
  id: number;
  body: string;
  correct_answer: string;
  subject_name: string;
  created_at: string;
};

export async function GET(req: NextRequest) {
  try {
    const { searchParams } = new URL(req.url);
    const subject = searchParams.get("subject");
    const limit = Math.min(Number(searchParams.get("limit")) || 50, 200);

    const conditions: string[] = [];
    const values: (string | number)[] = [];

    if (subject) {
      conditions.push(`subject_name = $${values.length + 1}`);
      values.push(subject);
    }

    const whereClause = conditions.length ? `WHERE ${conditions.join(" AND ")}` : "";
    const result = await query<TaskRow>(
      `SELECT id, body, correct_answer, subject_name, created_at
         FROM tasks
        ${whereClause}
        ORDER BY created_at DESC
        LIMIT $${values.length + 1}`,
      [...values, limit]
    );

    const tasks = result.rows.map((row) => ({
      id: row.id,
      text: row.body,
      correctAnswer: row.correct_answer,
      subject: row.subject_name,
      createdAt: row.created_at
    }));

    return jsonOk({ tasks });
  } catch (error) {
    return jsonServerError(error);
  }
}

export async function POST(req: NextRequest) {
  try {
    const payload = taskSchema.parse(await req.json());
    const inserted = await query<TaskRow>(
      `INSERT INTO tasks (body, correct_answer, subject_name)
       VALUES ($1, $2, $3)
       RETURNING id, body, correct_answer, subject_name, created_at`,
      [payload.body, payload.correctAnswer, payload.subjectName]
    );

    const task = inserted.rows[0];
    return jsonCreated({
      id: task.id,
      text: task.body,
      correctAnswer: task.correct_answer,
      subject: task.subject_name,
      createdAt: task.created_at
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
