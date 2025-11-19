const { Client } = require('pg');
const fs = require('fs');
const path = require('path');

// Загрузка .env
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

const connectionString = process.env.DATABASE_URL;
console.log(`Trying to connect to: ${connectionString?.replace(/:([^:@]+)@/, ':****@')}`); // Скрываем пароль в логах

const client = new Client({
  connectionString,
  ssl: process.env.DB_SSL === 'true' ? { rejectUnauthorized: false } : false,
  connectionTimeoutMillis: 5000, // Таймаут 5 секунд
});

async function testConnection() {
  try {
    await client.connect();
    console.log('✅ Успешное подключение!');
    const res = await client.query('SELECT NOW() as now, inet_server_addr() as ip');
    console.log('Время на сервере:', res.rows[0].now);
    console.log('IP сервера:', res.rows[0].ip);
  } catch (err) {
    console.error('❌ Ошибка подключения:');
    console.error('Message:', err.message);
    console.error('Code:', err.code);
    if (err.address) console.error('Target IP:', err.address);
    if (err.port) console.error('Target Port:', err.port);
    
    console.log('\nВозможные причины:');
    console.log('1. Неверный IP адрес или порт в DATABASE_URL.');
    console.log('2. Postgres не настроен слушать внешние подключения (listen_addresses).');
    console.log('3. Файрвол на сервере блокирует порт 5432.');
    console.log('4. pg_hba.conf не разрешает подключение с вашего IP.');
  } finally {
    await client.end();
  }
}

testConnection();
