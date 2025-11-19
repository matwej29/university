"use client";

import { useEffect, useState } from "react";
import { useRouter } from "next/navigation";
import { Reviewer } from "@/lib/types";
import styles from "./ReviewerLogin.module.css";

export function ReviewerLogin() {
  const [reviewers, setReviewers] = useState<Reviewer[]>([]);
  const [selectedId, setSelectedId] = useState<string>("");
  const router = useRouter();

  useEffect(() => {
    fetch("/api/reviewers")
      .then((res) => res.json())
      .then((data) => setReviewers(data.reviewers))
      .catch((err) => console.error(err));
  }, []);

  const handleLogin = () => {
    if (selectedId) {
      router.push(`/reviewer/${selectedId}`);
    }
  };

  return (
    <div className={styles.container}>
      <h2 className={styles.title}>Выберите профиль</h2>
      <select
        className={styles.select}
        value={selectedId}
        onChange={(e) => setSelectedId(e.target.value)}
      >
        <option value="">-- Выберите проверяющего --</option>
        {reviewers.map((r) => (
          <option key={r.id} value={r.id}>
            {r.fullName} ({r.subjects.join(", ")})
          </option>
        ))}
      </select>
      <button
        className={styles.button}
        onClick={handleLogin}
        disabled={!selectedId}
      >
        Войти
      </button>
    </div>
  );
}
