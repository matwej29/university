import { Pool } from "pg";
import fs from "fs";
import path from "path";

console.log(process.env.POSTGRES_URL);

export const pool = new Pool({
    connectionString: process.env.POSTGRES_URL,
    max: 20,
    idleTimeoutMillis: 30000,
    connectionTimeoutMillis: 2000,
    ssl: false,
});

const applySchema = async () => {
    console.log("appying schema");
    const schemaFilePath = path.join(process.cwd(), "/sql/schema.sql");

    const dbInitQuery = fs.readFileSync(schemaFilePath);
    await pool.query(String(dbInitQuery));
};

// applySchema();
