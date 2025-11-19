const { Pool } = require('pg');
const fs = require('fs');
const path = require('path');

// Load .env manually
const envPath = path.join(__dirname, '../.env');
if (fs.existsSync(envPath)) {
  const envConfig = fs.readFileSync(envPath, 'utf8');
  envConfig.split('\n').forEach(line => {
    const match = line.match(/^([^=]+)=(.*)$/);
    if (match) {
      const key = match[1].trim();
      const value = match[2].trim().replace(/^["'](.*)["']$/, '$1');
      process.env[key] = value;
    }
  });
}

const connectionString = process.env.DATABASE_URL || "postgres://postgres:postgres@localhost:5432/ege";
const sslRequired = process.env.DB_SSL?.toLowerCase() === "true";

const pool = new Pool({
  connectionString,
  ssl: sslRequired ? { rejectUnauthorized: false } : false
});

async function migrate() {
  try {
    const schemaSql = fs.readFileSync(path.join(__dirname, '../db/schema.sql'), 'utf8');
    console.log('Running migration...');
    await pool.query(schemaSql);
    console.log('Migration completed successfully');
  } catch (err) {
    console.error('Error running migration:', err);
    process.exit(1);
  } finally {
    await pool.end();
  }
}

migrate();
