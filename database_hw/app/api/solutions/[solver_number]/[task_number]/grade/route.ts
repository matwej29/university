import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";

// POST /api/solutions/[solver_number]/[task_number]/grade - поставить оценку решению
export async function POST(
    req: NextRequest,
    ctx: { params: Promise<{ solver_number: string; task_number: string }> }
) {
    const { task_number, solver_number } = await ctx.params;

    try {
        const examiner_id = req.cookies.get("examiner_id");

        if (!examiner_id) {
            return NextResponse.json(
                { error: "Not authorized" },
                { status: 401 }
            );
        }

        const body = await req.json();
        const { grade } = body;

        if (grade === undefined || grade === null) {
            return NextResponse.json(
                { error: "Grade is required" },
                { status: 400 }
            );
        }

        if (typeof grade !== "number" || grade < 0 || grade > 100) {
            return NextResponse.json(
                { error: "Grade must be a number between 0 and 100" },
                { status: 400 }
            );
        }

        // Проверяем, что решение назначено текущему проверяющему
        const checkRes = await pool.query(
            `SELECT examiner_number FROM solutions 
             WHERE task_number = $1 AND solver_number = $2;`,
            [task_number, solver_number]
        );

        if (checkRes.rows.length === 0) {
            return NextResponse.json(
                { error: "Solution not found" },
                { status: 404 }
            );
        }

        if (checkRes.rows[0].examiner_number !== parseInt(examiner_id.value)) {
            return NextResponse.json(
                { error: "Solution is not assigned to you" },
                { status: 403 }
            );
        }

        // Устанавливаем оценку
        const res = await pool.query(
            `UPDATE solutions 
             SET grade = $3 
             WHERE task_number = $1 AND solver_number = $2
             RETURNING solver_number, task_number, grade;`,
            [task_number, solver_number, grade]
        );

        return NextResponse.json({
            data: res.rows[0],
        });
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}
