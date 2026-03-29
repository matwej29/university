import { NextResponse } from "next/server";
import { pool } from "@/lib/db";

type Subject = {
    name: string;
};

// GET /api/subjects - получить список всех предметов
export async function GET() {
    try {
        const res = await pool.query("SELECT name FROM subjects;");
        const subjects: Subject[] = res.rows as Subject[];
        return NextResponse.json(subjects);
    } catch (e) {
        console.error(e);
        return NextResponse.json(
            { error: `DB get error: ${e}` },
            { status: 500 }
        );
    }
}
