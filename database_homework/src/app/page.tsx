import Link from "next/link";

export default function Home() {
  return (
    <main style={{ padding: "2rem", maxWidth: "800px", margin: "0 auto" }}>
      <h1 style={{ textAlign: "center", marginBottom: "2rem" }}>
        ЕГЭ Проверка
      </h1>
      <p style={{ textAlign: "center", marginBottom: "3rem", color: "#94a3b8" }}>
        Сервис для подготовки и проверки заданий ЕГЭ
      </p>

      <div
        style={{
          display: "flex",
          flexDirection: "column",
          gap: "1rem",
          alignItems: "center"
        }}
      >
        <Link
          href="/student"
          style={{
            padding: "1.5rem",
            background: "#1e293b",
            borderRadius: "8px",
            textDecoration: "none",
            color: "white",
            width: "100%",
            textAlign: "center",
            border: "1px solid #334155"
          }}
        >
          <h2 style={{ margin: "0 0 0.5rem" }}>🎓 Я Ученик</h2>
          <p style={{ margin: 0, color: "#94a3b8" }}>
            Решать задания и получать оценки
          </p>
        </Link>

        <Link
          href="/reviewer/login"
          style={{
            padding: "1.5rem",
            background: "#1e293b",
            borderRadius: "8px",
            textDecoration: "none",
            color: "white",
            width: "100%",
            textAlign: "center",
            border: "1px solid #334155"
          }}
        >
          <h2 style={{ margin: "0 0 0.5rem" }}>👨‍🏫 Я Проверяющий</h2>
          <p style={{ margin: 0, color: "#94a3b8" }}>
            Проверять решения и ставить оценки
          </p>
        </Link>

        <Link
          href="/admin/tasks/create"
          style={{
            padding: "1.5rem",
            background: "#1e293b",
            borderRadius: "8px",
            textDecoration: "none",
            color: "white",
            width: "100%",
            textAlign: "center",
            border: "1px solid #334155"
          }}
        >
          <h2 style={{ margin: "0 0 0.5rem" }}>⚙️ Администратор</h2>
          <p style={{ margin: 0, color: "#94a3b8" }}>
            Создавать новые задания
          </p>
        </Link>
      </div>
    </main>
  );
}
