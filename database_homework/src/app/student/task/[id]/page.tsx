"use client";

import { useEffect, useState } from "react";
import Link from "next/link";
import { useRouter } from "next/navigation";
import { Task } from "@/lib/types";
import styles from "./SolveTask.module.css";

export default function SolveTaskPage({ params }: { params: { id: string } }) {
  const [task, setTask] = useState<Task | null>(null);
  const [solverId, setSolverId] = useState("");
  const [answerText, setAnswerText] = useState("");
  const [submittedAnswer, setSubmittedAnswer] = useState("");
  const [loading, setLoading] = useState(true);
  const [submitting, setSubmitting] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState(false);

  useEffect(() => {
    fetch(`/api/tasks/${params.id}`)
      .then((res) => {
        if (!res.ok) throw new Error("Задание не найдено");
        return res.json();
      })
      .then(setTask)
      .catch((err) => setError(err.message))
      .finally(() => setLoading(false));
  }, [params.id]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setSubmitting(true);
    setError(null);

    try {
      const res = await fetch("/api/solutions", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          solverId: Number(solverId),
          taskId: Number(params.id),
          answerText,
          submittedAnswer,
        }),
      });

      if (!res.ok) {
        const data = await res.json();
        throw new Error(data.error || "Ошибка отправки решения");
      }

      setSuccess(true);
    } catch (err) {
      setError(err instanceof Error ? err.message : "Ошибка");
    } finally {
      setSubmitting(false);
    }
  };

  if (loading) return <div className={styles.container}>Загрузка...</div>;
  if (!task) return <div className={styles.container}>Задание не найдено</div>;

  if (success) {
    return (
      <div className={styles.container}>
        <div className={styles.success}>
          <h2>Решение отправлено!</h2>
          <p>Ваше решение принято в обработку.</p>
          <div style={{ marginTop: "1rem" }}>
            <Link href="/student" className={styles.link}>
              Вернуться к списку заданий
            </Link>
          </div>
        </div>
      </div>
    );
  }

  return (
    <div className={styles.container}>
      <div className={styles.card}>
        <div className={styles.taskInfo}>
          <span className={styles.label}>
            {task.subject} • Задание #{task.id}
          </span>
          <div className={styles.taskText}>{task.text}</div>
        </div>

        {error && <div className={styles.error}>{error}</div>}

        <form onSubmit={handleSubmit} className={styles.form}>
          <div className={styles.formGroup}>
            <label className={styles.label}>Ваш ID (номер ученика)</label>
            <input
              type="number"
              required
              className={styles.input}
              value={solverId}
              onChange={(e) => setSolverId(e.target.value)}
              placeholder="Например: 101"
            />
          </div>

          <div className={styles.formGroup}>
            <label className={styles.label}>Краткий ответ</label>
            <input
              type="text"
              required
              className={styles.input}
              value={submittedAnswer}
              onChange={(e) => setSubmittedAnswer(e.target.value)}
              placeholder="Ответ на задание"
            />
          </div>

          <div className={styles.formGroup}>
            <label className={styles.label}>Полное решение</label>
            <textarea
              required
              className={styles.textarea}
              value={answerText}
              onChange={(e) => setAnswerText(e.target.value)}
              placeholder="Напишите решение"
            />
          </div>

          <div style={{ display: "flex", gap: "1rem" }}>
            <button
              type="submit"
              className={styles.button}
              disabled={submitting}
              style={{ flex: 1 }}
            >
              {submitting ? "Отправка..." : "Отправить решение"}
            </button>
            <Link
              href="/student"
              className={styles.button}
              style={{
                background: "#475569",
                textAlign: "center",
                textDecoration: "none",
              }}
            >
              Отмена
            </Link>
          </div>
        </form>
      </div>
    </div>
  );
}
