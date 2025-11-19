const { Pool } = require('pg');
const fs = require('fs');
const path = require('path');
const { env } = require('process');

// Load .env manually since we might not have dotenv installed
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
    console.log('Environment variables loaded from .env file');
    // log
}

const connectionString = process.env.DATABASE_URL || "postgres://postgres:postgres@localhost:5432/ege";
const sslRequired = process.env.DB_SSL?.toLowerCase() === "true";

const pool = new Pool({
  connectionString,
  ssl: sslRequired ? { rejectUnauthorized: false } : false
});

pool.connect();

async function seed() {
  try {
    const seedSql = fs.readFileSync(path.join(__dirname, '../db/seed.sql'), 'utf8');
    console.log("seed.sql length:", seedSql.length);
    console.log('Running seed...');
    await pool.query(seedSql);
    console.log('Seed completed successfully');
  } catch (err) {
    console.error('Error seeding database:', err);
    process.exit(1);
  } finally {
    await pool.end();
  }
}

seed();
