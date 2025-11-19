import { Pool, PoolClient, QueryResult, QueryResultRow } from "pg";

type QueryParams = Array<string | number | boolean | null>;

type GlobalPool = typeof globalThis & { __dbPool?: Pool };

const globalForDb = globalThis as GlobalPool;

const connectionString =
  process.env.DATABASE_URL ?? "postgres://postgres:postgres@localhost:5432/ege";

if (!globalForDb.__dbPool) {
  const sslRequired = process.env.DB_SSL?.toLowerCase() === "true";
  globalForDb.__dbPool = new Pool({
    connectionString,
    ssl: sslRequired ? { rejectUnauthorized: false } : false
  });
}

const pool = globalForDb.__dbPool;

pool.on("error", (err: Error) => {
  console.error("Unexpected PostgreSQL pool error", err);
});

export async function query<T extends QueryResultRow = QueryResultRow>(
  text: string,
  params: QueryParams = []
): Promise<QueryResult<T>> {
  return pool.query<T>(text, params);
}

export async function withTransaction<T>(
  handler: (client: PoolClient) => Promise<T>
): Promise<T> {
  const client = await pool.connect();
  try {
    await client.query("BEGIN");
    const result = await handler(client);
    await client.query("COMMIT");
    return result;
  } catch (error) {
    await client.query("ROLLBACK");
    throw error;
  } finally {
    client.release();
  }
}

export type DbClient = PoolClient;
