import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";

// POST /api/solutions/[solver_number]/[task_number]/take - взять решение на проверку
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

        // Проверяем, что решение еще не взято на проверку
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

        if (checkRes.rows[0].examiner_number !== null) {
            return NextResponse.json(
                { error: "Solution is already taken by another examiner" },
                { status: 409 }
            );
        }

        // Проверяем, что проверяющий может проверять этот предмет
        const subjectCheckRes = await pool.query(
            `SELECT 1 FROM task t
             JOIN examiners_subjects es ON t.subject = es.subjects_name
             WHERE t.task_number = $1 AND es.examiners_id = $2;`,
            [task_number, examiner_id.value]
        );

        if (subjectCheckRes.rows.length === 0) {
            return NextResponse.json(
                { error: "You are not allowed to check this subject" },
                { status: 403 }
            );
        }

        const res = await pool.query(
            `UPDATE solutions 
             SET examiner_number = $3 
             WHERE task_number = $1 AND solver_number = $2
             RETURNING task_number, solver_number, examiner_number;`,
            [task_number, solver_number, examiner_id.value]
        );

        return NextResponse.json({
            data: res.rows[0],
        });
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}
