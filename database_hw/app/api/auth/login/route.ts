import { pool } from "@/lib/db";
import { NextRequest, NextResponse } from "next/server";

// POST /api/auth/login - войти как проверяющий
export async function POST(req: NextRequest) {
    try {
        const body = await req.json();
        const { id } = body;

        if (!id) {
            return NextResponse.json(
                { error: "Examiner ID is required" },
                { status: 400 }
            );
        }

        const res = await pool.query(
            "SELECT id, name FROM examiners WHERE id = $1;",
            [id]
        );

        if (res.rows.length === 0) {
            return NextResponse.json(
                { error: "Examiner not found" },
                { status: 404 }
            );
        }

        const response = NextResponse.json({ data: res.rows[0] });
        response.cookies.set("examiner_id", String(id), {
            httpOnly: true,
            secure: process.env.NODE_ENV === "production",
            sameSite: "lax",
        });

        return response;
    } catch (e) {
        console.error(e);
        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }
}

// DELETE /api/auth/login - выйти из системы
export async function DELETE() {
    const response = NextResponse.json({ message: "Logged out" });
    response.cookies.delete("examiner_id");
    return response;
}
