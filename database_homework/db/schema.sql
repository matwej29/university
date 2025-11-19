-- Subjects available for tasks and reviewers
CREATE TABLE IF NOT EXISTS subjects (
    name TEXT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS reviewers (
    id SERIAL PRIMARY KEY,
    full_name TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS reviewer_subjects (
    reviewer_id INTEGER NOT NULL REFERENCES reviewers(id) ON DELETE CASCADE,
    subject_name TEXT NOT NULL REFERENCES subjects(name) ON DELETE CASCADE,
    PRIMARY KEY (reviewer_id, subject_name)
);

CREATE TABLE IF NOT EXISTS tasks (
    id SERIAL PRIMARY KEY,
    body TEXT NOT NULL,
    correct_answer TEXT NOT NULL,
    subject_name TEXT NOT NULL REFERENCES subjects(name) ON DELETE RESTRICT,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS solutions (
    id SERIAL PRIMARY KEY,
    solver_id INTEGER NOT NULL,
    task_id INTEGER NOT NULL REFERENCES tasks(id) ON DELETE CASCADE,
    answer_text TEXT NOT NULL,
    submitted_answer TEXT NOT NULL,
    reviewer_id INTEGER REFERENCES reviewers(id) ON DELETE SET NULL,
    score SMALLINT CHECK (score BETWEEN 0 AND 100),
    comment TEXT,
    assigned_at TIMESTAMPTZ,
    reviewed_at TIMESTAMPTZ,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX IF NOT EXISTS idx_solutions_task_id ON solutions(task_id);
CREATE INDEX IF NOT EXISTS idx_solutions_reviewer_id ON solutions(reviewer_id);
CREATE INDEX IF NOT EXISTS idx_tasks_subject ON tasks(subject_name);
