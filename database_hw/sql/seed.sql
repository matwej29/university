BEGIN;

-- 1. Заполняем предметы (Справочник)
INSERT INTO public.subjects (name) VALUES
    ('Математика'),
    ('Информатика'),
    ('Русский язык')
ON CONFLICT (name) DO NOTHING;

-- 2. Заполняем экзаменаторов (Проверяющие)
INSERT INTO public.examiners (id, name) VALUES
    (1, 'Петров Петр Петрович'),   -- Проверяет Информатику
    (2, 'Иванова Мария Ивановна'), -- Проверяет Математику
    (3, 'Сидоров Сидор Сидорович') -- Универсал: Математика и Информатика
ON CONFLICT (id) DO NOTHING;

-- 3. Связываем экзаменаторов с предметами (Компетенции)
INSERT INTO public.examiners_subjects (examiners_id, subjects_name) VALUES
    (1, 'Информатика'),
    (2, 'Математика'),
    (3, 'Математика'),
    (3, 'Информатика')
ON CONFLICT (examiners_id, subjects_name) DO NOTHING;

-- 4. Создаем задания (Банк заданий)
INSERT INTO public.task (task_number, text, answer, subject) VALUES
    (101, 'Решите уравнение: 2x = 4', '2', 'Математика'),
    (102, 'Найдите площадь квадрата со стороной 5', '25', 'Математика'),
    (201, 'Переведите число 5 в двоичную систему счисления', '101', 'Информатика'),
    (202, 'Сколько бит в одном байте?', '8', 'Информатика')
ON CONFLICT (task_number) DO NOTHING;

-- 5. Создаем решения (Пользовательский контент)

-- Сценарий А: Решение проверено и оценено (Математика, проверил id=2)
INSERT INTO public.solutions (solver_number, task_number, solution_text, answer, examiner_number, grade)
VALUES
    (1001, 101, 'x = 4 / 2; x = 2', '2', 2, 100)
ON CONFLICT (solver_number, task_number) DO NOTHING;

-- Сценарий Б: Решение проверено, оценка низкая (Информатика, проверил id=1)
INSERT INTO public.solutions (solver_number, task_number, solution_text, answer, examiner_number, grade)
VALUES
    (1002, 201, 'Это будет 100', '100', 1, 0) -- Ошибка ученика
ON CONFLICT (solver_number, task_number) DO NOTHING;

-- Сценарий В: Решение ожидает проверки (examiner_number IS NULL)
INSERT INTO public.solutions (solver_number, task_number, solution_text, answer, examiner_number, grade)
VALUES
    (1003, 202, 'В байте 8 бит', '8', NULL, NULL)
ON CONFLICT (solver_number, task_number) DO NOTHING;

-- Сценарий Г: Взято на проверку универсалом (id=3), но оценка еще не стоит
INSERT INTO public.solutions (solver_number, task_number, solution_text, answer, examiner_number, grade)
VALUES
    (1004, 102, '5 умножить на 5', '25', 3, NULL)
ON CONFLICT (solver_number, task_number) DO NOTHING;

COMMIT;
