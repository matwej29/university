# ЕГЭ Проверка — backend

Простой REST API сервис на Next.js (App Router) и TypeScript для потока проверки решений ЕГЭ. Использует PostgreSQL с чистыми SQL-запросами (можно подключить SQLite, поменяв `DATABASE_URL`).

## Быстрый старт

```bash
cd /Users/matvej29/edu_projects/university/database_homework
cp .env.example .env   # укажите DATABASE_URL
yarn install
yarn dev
```

## Настройка базы

1. Создайте базу и пользователя в Postgres.
2. Выполните миграцию:

```bash
psql "$DATABASE_URL" -f db/schema.sql
```

Переменные окружения:

- `DATABASE_URL` — строка подключения Postgres (по умолчанию `postgres://postgres:postgres@localhost:5432/ege`).
- `DB_SSL=true` — включает SSL (например, для Render/Neon).

## Основные сущности

- `subjects` — каталог предметов.
- `reviewers`, `reviewer_subjects` — проверяющие и их предметы.
- `tasks` — задания с правильным ответом и предметом.
- `solutions` — решения учеников, статус отслеживается по `reviewer_id` и `score`.

## REST API

Все ответы в JSON.

### `/api/subjects`
- `GET` — список предметов.
- `POST` — создать или обновить предмет `{ name }`.

### `/api/reviewers`
- `GET` — вернуть проверяющих с их предметами.
- `POST` — создать проверяющего `{ fullName, subjectNames[] }`.

### `/api/reviewers/:id/queue`
- `POST` — взять следующее неподтверждённое решение по предметам проверяющего. Тело `{ subjectName? }`. Автоматически назначает решение.

### `/api/tasks`
- `GET` — список заданий, фильтр `?subject=`.
- `POST` — добавить задание `{ body, correctAnswer, subjectName }`.

### `/api/solutions`
- `GET` — список решений. Параметры `status=pending|assigned|graded`, `subject`, `reviewerId`, `limit`.
- `POST` — создать решение `{ solverId, taskId, answerText, submittedAnswer }`.

### `/api/solutions/:id/assign`
- `POST` — вручную назначить решение проверяющему `{ reviewerId }`.

### `/api/solutions/:id/grade`
- `POST` — выставить оценку `{ reviewerId, score, comment? }`.

## Проверка

```bash
yarn lint
yarn typecheck
```

## Дальнейшие шаги

- Добавить seed-скрипты для тестовых данных.
- Реализовать фронтенд, используя описанный API.
- Подключить настоящую авторизацию (при необходимости).
