"use client";

import { useState } from "react";
import { Solution } from "@/lib/types";
import styles from "./GradingInterface.module.css";

interface GradingInterfaceProps {
  solution: Solution;
  reviewerId: number;
  onComplete: () => void;
}

export function GradingInterface({
  solution,
  reviewerId,
  onComplete,
}: GradingInterfaceProps) {
  const [score, setScore] = useState<string>("");
  const [comment, setComment] = useState<string>("");
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError(null);

    try {
      const response = await fetch(`/api/solutions/${solution.id}/grade`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          reviewerId,
          score: Number(score),
          comment: comment || undefined,
        }),
      });

      if (!response.ok) {
        const data = await response.json();
        throw new Error(data.error || "Ошибка при сохранении оценки");
      }

      onComplete();
    } catch (err) {
      setError(err instanceof Error ? err.message : "Неизвестная ошибка");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className={styles.grading}>
      <div className={styles.taskSection}>
        <span className={styles.label}>Задание:</span>
        <p className={styles.text}>{solution.taskText}</p>
      </div>

      <div className={styles.answerSection}>
        <span className={styles.label}>Решение ученика:</span>
        <p className={styles.text}>{solution.answerText}</p>
      </div>

      <div className={styles.submittedSection}>
        <span className={styles.label}>Ответ ученика:</span>
        <p className={styles.text}>{solution.submittedAnswer}</p>
      </div>

      {error && <div className={styles.error}>{error}</div>}

      <form className={styles.form} onSubmit={handleSubmit}>
        <div className={styles.formGroup}>
          <label className={styles.label} htmlFor="score">
            Оценка (0-100):
          </label>
          <input
            id="score"
            type="number"
            min="0"
            max="100"
            value={score}
            onChange={(e) => setScore(e.target.value)}
            className={styles.input}
            required
          />
        </div>

        <div className={styles.formGroup}>
          <label className={styles.label} htmlFor="comment">
            Комментарий (необязательно):
          </label>
          <textarea
            id="comment"
            value={comment}
            onChange={(e) => setComment(e.target.value)}
            className={styles.textarea}
            placeholder="Добавьте комментарий к проверке..."
          />
        </div>

        <div className={styles.actions}>
          <button
            type="submit"
            className={styles.button}
            disabled={loading || !score}
          >
            {loading ? "Сохранение..." : "Сохранить оценку"}
          </button>
          <button
            type="button"
            className={`${styles.button} ${styles.buttonCancel}`}
            onClick={onComplete}
            disabled={loading}
          >
            Отмена
          </button>
        </div>
      </form>
    </div>
  );
}
