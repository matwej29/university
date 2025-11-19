"use client";

import { useEffect, useState } from "react";
import Link from "next/link";
import { Subject } from "@/lib/types";
import styles from "./CreateTask.module.css";

export default function CreateTaskPage() {
  const [subjects, setSubjects] = useState<Subject[]>([]);
  const [subjectName, setSubjectName] = useState("");
  const [body, setBody] = useState("");
  const [correctAnswer, setCorrectAnswer] = useState("");
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState(false);

  useEffect(() => {
    fetch("/api/subjects")
      .then((res) => res.json())
      .then((data) => {
        setSubjects(data.subjects);
        if (data.subjects.length > 0) {
          setSubjectName(data.subjects[0].name);
        }
      })
      .catch(console.error);
  }, []);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setSuccess(false);

    try {
      const res = await fetch("/api/tasks", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          subjectName,
          body,
          correctAnswer,
        }),
      });

      if (!res.ok) {
        const data = await res.json();
        throw new Error(data.error || "Ошибка создания задания");
      }

      setSuccess(true);
      setBody("");
      setCorrectAnswer("");
    } catch (err) {
      setError(err instanceof Error ? err.message : "Ошибка");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className={styles.container}>
      <h1 className={styles.title}>Создание нового задания</h1>

      {success && (
        <div className={styles.success}>
          Задание успешно создано! Можно добавить еще одно.
        </div>
      )}

      {error && <div className={styles.error}>{error}</div>}

      <form onSubmit={handleSubmit} className={styles.form}>
        <div className={styles.formGroup}>
          <label className={styles.label}>Предмет</label>
          <select
            className={styles.select}
            value={subjectName}
            onChange={(e) => setSubjectName(e.target.value)}
            required
          >
            {subjects.map((s) => (
              <option key={s.name} value={s.name}>
                {s.name}
              </option>
            ))}
          </select>
        </div>

        <div className={styles.formGroup}>
          <label className={styles.label}>Текст задания</label>
          <textarea
            className={styles.textarea}
            value={body}
            onChange={(e) => setBody(e.target.value)}
            required
            placeholder="Введите условие задачи..."
          />
        </div>

        <div className={styles.formGroup}>
          <label className={styles.label}>Правильный ответ</label>
          <input
            type="text"
            className={styles.input}
            value={correctAnswer}
            onChange={(e) => setCorrectAnswer(e.target.value)}
            required
            placeholder="Ответ для автоматической сверки (если нужно)"
          />
        </div>

        <button type="submit" className={styles.button} disabled={loading}>
          {loading ? "Создание..." : "Создать задание"}
        </button>

        <Link
          href="/"
          style={{
            textAlign: "center",
            color: "#94a3b8",
            textDecoration: "none",
          }}
        >
          Вернуться на главную
        </Link>
      </form>
    </div>
  );
}
