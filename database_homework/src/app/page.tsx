import Link from "next/link";
import styles from "./Home.module.css";

export default function Home() {
  return (
    <main className={styles.container}>
      <h1 className={styles.title}>ЕГЭ Проверка</h1>
      <p className={styles.subtitle}>
        Сервис для подготовки и проверки заданий ЕГЭ
      </p>

      <div className={styles.grid}>
        <Link href="/student" className={styles.card}>
          <div className={styles.icon}>🎓</div>
          <h2 className={styles.cardTitle}>Я Ученик</h2>
          <p className={styles.cardDesc}>
            Решайте задания по разным предметам и получайте оценки от экспертов.
          </p>
        </Link>

        <Link href="/reviewer/login" className={styles.card}>
          <div className={styles.icon}>👨‍🏫</div>
          <h2 className={styles.cardTitle}>Я Проверяющий</h2>
          <p className={styles.cardDesc}>
            Проверяйте решения учеников, выставляйте оценки и давайте
            комментарии.
          </p>
        </Link>

        <Link href="/admin/tasks/create" className={styles.card}>
          <div className={styles.icon}>⚙️</div>
          <h2 className={styles.cardTitle}>Администратор</h2>
          <p className={styles.cardDesc}>
            Создавайте новые задания и управляйте контентом платформы.
          </p>
        </Link>
      </div>
    </main>
  );
}
