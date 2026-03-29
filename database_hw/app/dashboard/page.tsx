"use client";

import { useState, useEffect, useCallback } from "react";
import { useRouter } from "next/navigation";
import styles from "./page.module.css";

type Subject = {
    name: string;
};

type Solution = {
    solver_number: number;
    task_number: number;
    solution_text: string;
    student_answer: string;
    correct_answer: string;
    task_text: string;
    subject: string;
    grade?: number | null;
};

type Profile = {
    id: number;
    name: string;
    subjects: string[];
};

export default function DashboardPage() {
    const [profile, setProfile] = useState<Profile | null>(null);
    const [subjects, setSubjects] = useState<Subject[]>([]);
    const [activeTab, setActiveTab] = useState<"unassigned" | "my">(
        "unassigned"
    );
    const [solutions, setSolutions] = useState<Solution[]>([]);
    const [selectedSubject, setSelectedSubject] = useState<string>("");
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [grades, setGrades] = useState<Record<string, string>>({});
    const router = useRouter();

    // Загрузка профиля
    useEffect(() => {
        fetch("/api/profile/me")
            .then((res) => {
                if (res.status === 401) {
                    router.push("/");
                    return null;
                }
                return res.json();
            })
            .then((data) => {
                if (data?.data) {
                    setProfile(data.data);
                }
            })
            .catch(() => setError("Ошибка загрузки профиля"));
    }, [router]);

    // Загрузка предметов
    useEffect(() => {
        fetch("/api/subjects")
            .then((res) => res.json())
            .then((data) => setSubjects(data))
            .catch(() => setError("Ошибка загрузки предметов"));
    }, []);

    // Загрузка решений
    const loadSolutions = useCallback(async () => {
        setLoading(true);
        setError("");

        try {
            let url =
                activeTab === "unassigned"
                    ? "/api/solutions/unassigned"
                    : "/api/solutions/my";
            if (activeTab === "unassigned" && selectedSubject) {
                url += `?subject=${encodeURIComponent(selectedSubject)}`;
            }

            const res = await fetch(url);
            if (res.status === 401) {
                router.push("/");
                return;
            }

            const data = await res.json();
            setSolutions(data.data || []);
        } catch {
            setError("Ошибка загрузки решений");
        } finally {
            setLoading(false);
        }
    }, [activeTab, selectedSubject, router]);

    useEffect(() => {
        loadSolutions();
    }, [loadSolutions]);

    const handleLogout = async () => {
        await fetch("/api/auth/login", { method: "DELETE" });
        router.push("/");
    };

    const handleTake = async (solverNumber: number, taskNumber: number) => {
        try {
            const res = await fetch(
                `/api/solutions/${solverNumber}/${taskNumber}/take`,
                {
                    method: "POST",
                }
            );

            if (res.ok) {
                loadSolutions();
            } else {
                const data = await res.json();
                alert(data.error || "Ошибка при взятии задания");
            }
        } catch {
            alert("Ошибка подключения");
        }
    };

    const handleGrade = async (solverNumber: number, taskNumber: number) => {
        const key = `${solverNumber}-${taskNumber}`;
        const gradeValue = grades[key];

        if (!gradeValue) {
            alert("Введите оценку");
            return;
        }

        const grade = parseInt(gradeValue);
        if (isNaN(grade) || grade < 0 || grade > 100) {
            alert("Оценка должна быть числом от 0 до 100");
            return;
        }

        try {
            const res = await fetch(
                `/api/solutions/${solverNumber}/${taskNumber}/grade`,
                {
                    method: "POST",
                    headers: { "Content-Type": "application/json" },
                    body: JSON.stringify({ grade }),
                }
            );

            if (res.ok) {
                loadSolutions();
                setGrades((prev) => ({ ...prev, [key]: "" }));
            } else {
                const data = await res.json();
                alert(data.error || "Ошибка при выставлении оценки");
            }
        } catch {
            alert("Ошибка подключения");
        }
    };

    const isAnswerCorrect = (studentAnswer: string, correctAnswer: string) => {
        return (
            studentAnswer?.trim().toLowerCase() ===
            correctAnswer?.trim().toLowerCase()
        );
    };

    return (
        <div className={styles.container}>
            <header className={styles.header}>
                <h1 className={styles.title}>Проверка заданий ЕГЭ</h1>
                <div className={styles.userInfo}>
                    {profile && (
                        <span className={styles.userName}>{profile.name}</span>
                    )}
                    <button
                        className={styles.logoutButton}
                        onClick={handleLogout}
                    >
                        Выйти
                    </button>
                </div>
            </header>

            <div className={styles.tabs}>
                <button
                    className={`${styles.tab} ${
                        activeTab === "unassigned" ? styles.tabActive : ""
                    }`}
                    onClick={() => setActiveTab("unassigned")}
                >
                    Доступные для проверки
                </button>
                <button
                    className={`${styles.tab} ${
                        activeTab === "my" ? styles.tabActive : ""
                    }`}
                    onClick={() => setActiveTab("my")}
                >
                    Мои задания
                </button>
            </div>

            {activeTab === "unassigned" && (
                <div className={styles.filterSection}>
                    <span className={styles.filterLabel}>
                        Фильтр по предмету:
                    </span>
                    <select
                        className={styles.filterSelect}
                        value={selectedSubject}
                        onChange={(e) => setSelectedSubject(e.target.value)}
                    >
                        <option value="">Все предметы</option>
                        {subjects
                            .filter((s) => profile?.subjects?.includes(s.name))
                            .map((subject) => (
                                <option key={subject.name} value={subject.name}>
                                    {subject.name}
                                </option>
                            ))}
                    </select>
                </div>
            )}

            {error && <p className={styles.error}>{error}</p>}

            {loading ? (
                <div className={styles.loading}>Загрузка...</div>
            ) : solutions.length === 0 ? (
                <div className={styles.emptyState}>
                    {activeTab === "unassigned"
                        ? "Нет доступных заданий для проверки"
                        : "У вас нет назначенных заданий"}
                </div>
            ) : (
                <div className={styles.solutionsList}>
                    {solutions.map((solution) => {
                        const key = `${solution.solver_number}-${solution.task_number}`;
                        const correct = isAnswerCorrect(
                            solution.student_answer,
                            solution.correct_answer
                        );

                        return (
                            <div key={key} className={styles.solutionCard}>
                                <div className={styles.solutionHeader}>
                                    <div className={styles.solutionMeta}>
                                        <span className={styles.solutionId}>
                                            Ученик {solution.solver_number} •
                                            Задание {solution.task_number} •
                                            Предмет {solution.subject}
                                        </span>
                                    </div>
                                </div>

                                <p className={styles.taskText}>
                                    {solution.task_text}
                                </p>
                                <div className={styles.solutionText}>
                                    {solution.solution_text}
                                </div>

                                <div className={styles.answers}>
                                    <div className={styles.answer}>
                                        <span className={styles.answerLabel}>
                                            Ответ ученика:
                                        </span>
                                        <span className={styles.answerValue}>
                                            {solution.student_answer}
                                        </span>
                                    </div>
                                    <div className={styles.answer}>
                                        <span className={styles.answerLabel}>
                                            Правильный ответ:
                                        </span>
                                        <span className={styles.answerValue}>
                                            {solution.correct_answer}
                                        </span>
                                    </div>
                                </div>

                                <div className={styles.actions}>
                                    {activeTab === "unassigned" ? (
                                        <button
                                            className={styles.takeButton}
                                            onClick={() =>
                                                handleTake(
                                                    solution.solver_number,
                                                    solution.task_number
                                                )
                                            }
                                        >
                                            Взять на проверку
                                        </button>
                                    ) : solution.grade !== null &&
                                      solution.grade !== undefined ? (
                                        <span className={styles.gradeBadge}>
                                            Оценка: {solution.grade}
                                        </span>
                                    ) : (
                                        <div className={styles.gradeForm}>
                                            <input
                                                type="number"
                                                min="0"
                                                max="100"
                                                className={styles.gradeInput}
                                                placeholder="0-100"
                                                value={grades[key] || ""}
                                                onChange={(e) =>
                                                    setGrades((prev) => ({
                                                        ...prev,
                                                        [key]: e.target.value,
                                                    }))
                                                }
                                            />
                                            <button
                                                className={styles.gradeButton}
                                                onClick={() =>
                                                    handleGrade(
                                                        solution.solver_number,
                                                        solution.task_number
                                                    )
                                                }
                                            >
                                                Поставить оценку
                                            </button>
                                        </div>
                                    )}
                                </div>
                            </div>
                        );
                    })}
                </div>
            )}
        </div>
    );
}
