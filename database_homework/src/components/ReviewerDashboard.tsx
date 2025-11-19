"use client";

import { useEffect, useState } from "react";
import { useRouter } from "next/navigation";
import Link from "next/link";
import { Reviewer, Solution } from "@/lib/types";
import { SolutionCard } from "@/components/SolutionCard";
import { GradingInterface } from "@/components/GradingInterface";
import styles from "@/components/ReviewerDashboard.module.css";

interface ReviewerDashboardProps {
  reviewerId: string;
}

export function ReviewerDashboard({ reviewerId }: ReviewerDashboardProps) {
  const router = useRouter();
  const [reviewer, setReviewer] = useState<Reviewer | null>(null);
  const [activeSolution, setActiveSolution] = useState<Solution | null>(null);
  const [assignedSolutions, setAssignedSolutions] = useState<Solution[]>([]);
  const [gradedSolutions, setGradedSolutions] = useState<Solution[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  const loadReviewer = async () => {
    try {
      const res = await fetch("/api/reviewers");
      const data = await res.json();
      const found = data.reviewers.find(
        (r: Reviewer) => r.id === Number(reviewerId)
      );
      if (!found) {
        setError("Проверяющий не найден");
        return;
      }
      setReviewer(found);
    } catch (err) {
      setError("Ошибка загрузки данных проверяющего");
    }
  };

  const loadSolutions = async () => {
    try {
      const [assignedRes, gradedRes] = await Promise.all([
        fetch(
          `/api/solutions?status=assigned&reviewerId=${reviewerId}&limit=10`
        ),
        fetch(`/api/solutions?status=graded&reviewerId=${reviewerId}&limit=20`),
      ]);

      const assignedData = await assignedRes.json();
      const gradedData = await gradedRes.json();

      setAssignedSolutions(assignedData.solutions || []);
      setGradedSolutions(gradedData.solutions || []);
    } catch (err) {
      setError("Ошибка загрузки решений");
    }
  };

  const fetchNext = async () => {
    setError(null);
    try {
      const res = await fetch(`/api/reviewers/${reviewerId}/queue`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({}),
      });

      const data = await res.json();

      if (!res.ok) {
        throw new Error(data.error || "Ошибка получения следующего решения");
      }

      if (data.message) {
        setError(data.message);
        return;
      }

      setActiveSolution(data);
      await loadSolutions();
    } catch (err) {
      setError(err instanceof Error ? err.message : "Неизвестная ошибка");
    }
  };

  const handleGradingComplete = async () => {
    setActiveSolution(null);
    await loadSolutions();
  };

  useEffect(() => {
    const init = async () => {
      setLoading(true);
      await Promise.all([loadReviewer(), loadSolutions()]);
      setLoading(false);
    };
    init();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [reviewerId]);

  if (loading) {
    return <div className={styles.loading}>Загрузка...</div>;
  }

  if (!reviewer) {
    return (
      <div className={styles.dashboard}>
        <div className={styles.error}>Проверяющий не найден</div>
        <Link href="/" className={styles.backButton}>
          На главную
        </Link>
      </div>
    );
  }

  return (
    <div className={styles.dashboard}>
      <div className={styles.header}>
        <div>
          <h1 className={styles.title}>{reviewer.fullName}</h1>
          <p className={styles.subtitle}>
            Предметы: {reviewer.subjects.join(", ")}
          </p>
        </div>
        <Link href="/" className={styles.backButton}>
          Выйти
        </Link>
      </div>

      {error && <div className={styles.error}>{error}</div>}

      <div className={styles.actions}>
        <button
          className={styles.button}
          onClick={fetchNext}
          disabled={!!activeSolution}
        >
          Взять следующее задание
        </button>
        <button
          className={`${styles.button} ${styles.buttonSecondary}`}
          onClick={loadSolutions}
        >
          Обновить список
        </button>
      </div>

      {activeSolution && (
        <div className={styles.card}>
          <h2 className={styles.sectionTitle}>Текущая проверка</h2>
          <GradingInterface
            solution={activeSolution}
            reviewerId={Number(reviewerId)}
            onComplete={handleGradingComplete}
          />
        </div>
      )}

      {assignedSolutions.length > 0 && (
        <div className={styles.card}>
          <h2 className={styles.sectionTitle}>Назначенные решения</h2>
          {assignedSolutions.map((solution) => (
            <SolutionCard key={solution.id} solution={solution} />
          ))}
        </div>
      )}

      <div className={styles.card}>
        <h2 className={styles.sectionTitle}>История проверок</h2>
        {gradedSolutions.length > 0 ? (
          gradedSolutions.map((solution) => (
            <SolutionCard key={solution.id} solution={solution} />
          ))
        ) : (
          <div className={styles.emptyState}>Нет проверенных решений</div>
        )}
      </div>
    </div>
  );
}
