import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";

// GET /api/solutions/unassigned - получить непроверенные решения по предметам текущего проверяющего
export async function GET(req: NextRequest) {
    try {
        const examiner_id = req.cookies.get("examiner_id");

        if (!examiner_id) {
            return NextResponse.json(
                { error: "Not authorized" },
                { status: 401 }
            );
        }

        const { searchParams } = new URL(req.url);
        const subject = searchParams.get("subject");
        const limit = parseInt(searchParams.get("limit") || "10");

        let query: string;
        let params: (string | number)[];

        if (subject) {
            // Фильтруем по конкретному предмету
            query = `
                SELECT 
                    s.solver_number,
                    s.task_number,
                    s.solution_text,
                    s.answer as student_answer,
                    t.text as task_text,
                    t.answer as correct_answer,
                    t.subject
                FROM solutions s
                JOIN task t ON s.task_number = t.task_number
                JOIN examiners_subjects es ON t.subject = es.subjects_name
                WHERE s.examiner_number IS NULL
                  AND es.examiners_id = $1
                  AND t.subject = $2
                ORDER BY s.task_number
                LIMIT $3;
            `;
            params = [examiner_id.value, subject, limit];
        } else {
            // Все предметы, которые может проверять текущий проверяющий
            query = `
                SELECT 
                    s.solver_number,
                    s.task_number,
                    s.solution_text,
                    s.answer as student_answer,
                    t.text as task_text,
                    t.answer as correct_answer,
                    t.subject
                FROM solutions s
                JOIN task t ON s.task_number = t.task_number
                JOIN examiners_subjects es ON t.subject = es.subjects_name
                WHERE s.examiner_number IS NULL
                  AND es.examiners_id = $1
                ORDER BY t.subject, s.task_number
                LIMIT $2;
            `;
            params = [examiner_id.value, limit];
        }

        const res = await pool.query(query, params);

        return NextResponse.json({
            data: res.rows,
        });
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}
