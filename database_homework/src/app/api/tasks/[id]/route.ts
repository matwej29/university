import { NextRequest } from "next/server";
import { query } from "@/lib/db";
import { jsonError, jsonOk, jsonServerError } from "@/lib/http";

export async function GET(
  req: NextRequest,
  { params }: { params: { id: string } }
) {
  try {
    const id = Number(params.id);
    if (!id) {
      return jsonError("Некорректный ID задания", 400);
    }

    const result = await query(
      "SELECT id, body, subject_name, created_at FROM tasks WHERE id = $1",
      [id]
    );

    if (result.rows.length === 0) {
      return jsonError("Задание не найдено", 404);
    }

    const row = result.rows[0];
    return jsonOk({
      id: row.id,
      text: row.body,
      subject: row.subject_name,
      createdAt: row.created_at
    });
  } catch (error) {
    return jsonServerError(error);
  }
}
