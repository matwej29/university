import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";

// GET /api/solutions/my - получить решения, назначенные текущему проверяющему
export async function GET(req: NextRequest) {
    try {
        const examiner_id = req.cookies.get("examiner_id");

        if (!examiner_id) {
            return NextResponse.json(
                { error: "Not authorized" },
                { status: 401 }
            );
        }

        const res = await pool.query(
            `SELECT 
                s.solver_number,
                s.task_number,
                s.solution_text,
                s.answer as student_answer,
                s.grade,
                t.text as task_text,
                t.answer as correct_answer,
                t.subject
             FROM solutions s
             JOIN task t ON s.task_number = t.task_number
             WHERE s.examiner_number = $1
             ORDER BY s.grade NULLS FIRST, s.task_number;`,
            [examiner_id.value]
        );

        return NextResponse.json({
            data: res.rows,
        });
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}
