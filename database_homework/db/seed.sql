-- Subjects
INSERT INTO subjects (name) VALUES 
('Математика'),
('Информатика'),
('Русский язык')
ON CONFLICT (name) DO NOTHING;

-- Reviewers
INSERT INTO reviewers (full_name) VALUES 
('Иванов Иван Иванович'),
('Петров Петр Петрович'),
('Сидорова Анна Сергеевна')
ON CONFLICT DO NOTHING;

-- Reviewer Subjects
-- Иванов: Математика, Информатика
INSERT INTO reviewer_subjects (reviewer_id, subject_name)
SELECT id, 'Математика' FROM reviewers WHERE full_name = 'Иванов Иван Иванович'
ON CONFLICT DO NOTHING;

INSERT INTO reviewer_subjects (reviewer_id, subject_name)
SELECT id, 'Информатика' FROM reviewers WHERE full_name = 'Иванов Иван Иванович'
ON CONFLICT DO NOTHING;

-- Петров: Русский язык
INSERT INTO reviewer_subjects (reviewer_id, subject_name)
SELECT id, 'Русский язык' FROM reviewers WHERE full_name = 'Петров Петр Петрович'
ON CONFLICT DO NOTHING;

-- Сидорова: Математика
INSERT INTO reviewer_subjects (reviewer_id, subject_name)
SELECT id, 'Математика' FROM reviewers WHERE full_name = 'Сидорова Анна Сергеевна'
ON CONFLICT DO NOTHING;

-- Tasks
INSERT INTO tasks (body, correct_answer, subject_name) VALUES
('Решите уравнение: 2x + 5 = 15', '5', 'Математика'),
('Напишите программу, которая выводит "Hello World"', 'print("Hello World")', 'Информатика'),
('Вставьте пропущенную букву: к_рова', 'о', 'Русский язык'),
('Вычислите интеграл от x^2 dx от 0 до 3', '9', 'Математика'),
('Переведите число 10 из десятичной системы в двоичную', '1010', 'Информатика');

-- Solutions
-- Pending solutions
INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer)
SELECT 101, id, 'Я думаю это 5', '5' FROM tasks WHERE body = 'Решите уравнение: 2x + 5 = 15';

INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer)
SELECT 102, id, 'print("Hello World")', 'print("Hello World")' FROM tasks WHERE body = 'Напишите программу, которая выводит "Hello World"';

INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer)
SELECT 103, id, 'кАрова', 'а' FROM tasks WHERE body = 'Вставьте пропущенную букву: к_рова';

-- Assigned solutions (assigned to Ivanov)
INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer, reviewer_id, assigned_at)
SELECT 104, t.id, 'Это 9', '9', r.id, NOW()
FROM tasks t, reviewers r
WHERE t.body = 'Вычислите интеграл от x^2 dx от 0 до 3' AND r.full_name = 'Иванов Иван Иванович';

-- Graded solutions
INSERT INTO solutions (solver_id, task_id, answer_text, submitted_answer, reviewer_id, assigned_at, score, comment, reviewed_at)
SELECT 105, t.id, '1010', '1010', r.id, NOW() - INTERVAL '1 hour', 100, 'Отлично!', NOW()
FROM tasks t, reviewers r
WHERE t.body = 'Переведите число 10 из десятичной системы в двоичную' AND r.full_name = 'Иванов Иван Иванович';
