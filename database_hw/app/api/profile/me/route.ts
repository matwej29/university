import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";

// GET /api/profile/me - получить информацию о текущем проверяющем
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
            `SELECT e.id, e.name, array_agg(es.subjects_name) as subjects
             FROM examiners e
             LEFT JOIN examiners_subjects AS es ON e.id = es.examiners_id
             WHERE e.id = $1
             GROUP BY e.id;`,
            [examiner_id.value]
        );

        if (res.rows.length === 0) {
            return NextResponse.json(
                { error: "Examiner not found" },
                { status: 404 }
            );
        }

        return NextResponse.json({ data: res.rows[0] });
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}
