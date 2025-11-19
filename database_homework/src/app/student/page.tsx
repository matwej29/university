"use client";

import { useEffect, useState } from "react";
import Link from "next/link";
import { Task } from "@/lib/types";
import styles from "./Student.module.css";

export default function StudentPage() {
  const [tasks, setTasks] = useState<Task[]>([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetch("/api/tasks?limit=50")
      .then((res) => res.json())
      .then((data) => setTasks(data.tasks))
      .catch(console.error)
      .finally(() => setLoading(false));
  }, []);

  if (loading) return <div className={styles.loading}>Загрузка заданий...</div>;

  return (
    <div className={styles.container}>
      <div className={styles.header}>
        <h1 className={styles.title}>Доступные задания</h1>
        <Link href="/" className={styles.backButton}>
          На главную
        </Link>
      </div>

      <div className={styles.taskList}>
        {tasks.map((task) => (
          <Link
            key={task.id}
            href={`/student/task/${task.id}`}
            className={styles.taskCard}
          >
            <div className={styles.taskHeader}>
              <span className={styles.subjectTag}>{task.subject}</span>
              <span>ID: {task.id}</span>
            </div>
            <div className={styles.taskBody}>
              {task.text.length > 100
                ? task.text.slice(0, 100) + "..."
                : task.text}
            </div>
          </Link>
        ))}
      </div>
    </div>
  );
}
