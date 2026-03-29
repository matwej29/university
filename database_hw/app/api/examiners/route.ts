import { pool } from "@/lib/db";
import { NextResponse } from "next/server";

type Examiner = {
    id: number;
    name: string;
};

export async function GET() {
    try {
        const res: Examiner[] = (
            await pool.query("SELECT id, name FROM examiners;")
        ).rows as Examiner[];
        return NextResponse.json(res);
    } catch (e) {
        console.error(e);
        return NextResponse.json(
            { error: `DB get error: ${e}` },
            { status: 500 }
        );
    }
}
