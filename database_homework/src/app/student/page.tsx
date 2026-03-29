"use client";

import { useEffect, useState } from "react";
import Link from "next/link";
import { Task } from "@/lib/types";

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

  if (loading)
    return <div style={{ padding: "2rem", textAlign: "center" }}>Загрузка...</div>;

  return (
    <div style={{ maxWidth: "800px", margin: "0 auto", padding: "2rem" }}>
      <div
        style={{
          display: "flex",
          justifyContent: "space-between",
          alignItems: "center",
          marginBottom: "2rem"
        }}
      >
        <h1 style={{ margin: 0 }}>Доступные задания</h1>
        <Link
          href="/"
          style={{
            padding: "0.5rem 1rem",
            background: "#475569",
            color: "white",
            textDecoration: "none",
            borderRadius: "4px"
          }}
        >
          На главную
        </Link>
      </div>

      <div style={{ display: "flex", flexDirection: "column", gap: "1rem" }}>
        {tasks.map((task) => (
          <Link
            key={task.id}
            href={`/student/task/${task.id}`}
            style={{
              display: "block",
              background: "#1e293b",
              padding: "1.5rem",
              borderRadius: "8px",
              textDecoration: "none",
              color: "inherit",
              border: "1px solid #334155"
            }}
          >
            <div
              style={{
                display: "flex",
                justifyContent: "space-between",
                marginBottom: "0.5rem",
                color: "#94a3b8",
                fontSize: "0.9rem"
              }}
            >
              <span
                style={{
                  background: "#334155",
                  padding: "0.25rem 0.75rem",
                  borderRadius: "12px",
                  color: "#cbd5e1"
                }}
              >
                {task.subject}
              </span>
              <span>ID: {task.id}</span>
            </div>
            <div style={{ fontSize: "1.1rem", lineHeight: 1.5 }}>
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
