import { NextRequest } from "next/server";
import { ZodError } from "zod";
import { query } from "@/lib/db";
import { jsonCreated, jsonError, jsonOk, jsonServerError } from "@/lib/http";
import { subjectSchema } from "@/lib/validators";

export async function GET() {
  try {
    const result = await query<{ name: string }>(
      "SELECT name FROM subjects ORDER BY name"
    );
    return jsonOk({ subjects: result.rows });
  } catch (error) {
    return jsonServerError(error);
  }
}

export async function POST(req: NextRequest) {
  try {
    const payload = subjectSchema.parse(await req.json());
    const inserted = await query<{ name: string }>(
      "INSERT INTO subjects (name) VALUES ($1) ON CONFLICT (name) DO UPDATE SET name = EXCLUDED.name RETURNING name",
      [payload.name]
    );
    return jsonCreated(inserted.rows[0]);
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
