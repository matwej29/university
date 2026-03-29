"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import styles from "./page.module.css";

type Examiner = {
    id: number;
    name: string;
};

export default function LoginPage() {
    const [examiners, setExaminers] = useState<Examiner[]>([]);
    const [selectedId, setSelectedId] = useState<string>("");
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState("");
    const router = useRouter();

    useEffect(() => {
        fetch("/api/examiners")
            .then((res) => res.json())
            .then((data) => setExaminers(data))
            .catch(() => setError("Ошибка загрузки списка проверяющих"));
    }, []);

    const handleLogin = async () => {
        if (!selectedId) return;

        setLoading(true);
        setError("");

        try {
            const res = await fetch("/api/auth/login", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ id: parseInt(selectedId) }),
            });

            if (res.ok) {
                router.push("/dashboard");
            } else {
                const data = await res.json();
                setError(data.error || "Ошибка входа");
            }
        } catch {
            setError("Ошибка подключения к серверу");
        } finally {
            setLoading(false);
        }
    };

    return (
        <div className={styles.container}>
            <h1 className={styles.title}>Проверка заданий ЕГЭ</h1>
            <div className={styles.loginForm}>
                <label className={styles.label}>Выберите проверяющего:</label>
                <select
                    className={styles.select}
                    value={selectedId}
                    onChange={(e) => setSelectedId(e.target.value)}
                >
                    <option value="">-- Выберите --</option>
                    {examiners.map((examiner) => (
                        <option key={examiner.id} value={examiner.id}>
                            {examiner.name}
                        </option>
                    ))}
                </select>
                <button
                    className={styles.button}
                    onClick={handleLogin}
                    disabled={!selectedId || loading}
                >
                    {loading ? "Вход..." : "Войти"}
                </button>
                {error && <p className={styles.error}>{error}</p>}
            </div>
        </div>
    );
}
