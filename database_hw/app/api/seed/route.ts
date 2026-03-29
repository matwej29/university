import { NextRequest, NextResponse } from "next/server";
import { pool } from "@/lib/db";
import fs from "fs";
import path from "path";

export async function GET(_req: NextRequest) {
    try {
        const seedFilePath = path.join(process.cwd(), "/sql/seed.sql");

        const dbSeedQuery = fs.readFileSync(seedFilePath);
        await pool.query(String(dbSeedQuery));

        return NextResponse.json({ data: {} }, { status: 200 });
    } catch (e) {
        console.log(e);

        return NextResponse.json({ error: `Got error: ${e}` }, { status: 500 });
    }

    return NextResponse.json({ error: "Internal error" }, { status: 500 });
}
