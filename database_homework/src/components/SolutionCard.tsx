import { Solution } from "@/lib/types";
import styles from "./SolutionCard.module.css";

interface SolutionCardProps {
  solution: Solution;
}

export function SolutionCard({ solution }: SolutionCardProps) {
  const getStatus = () => {
    if (solution.score !== null) return "graded";
    if (solution.reviewerId !== null) return "assigned";
    return "pending";
  };

  const status = getStatus();

  return (
    <div className={styles.solution}>
      <div className={styles.solutionHeader}>
        <div className={styles.solutionMeta}>
          <span>
            <span className={styles.label}>ID:</span> {solution.id}
          </span>
          <span>
            <span className={styles.label}>Предмет:</span> {solution.subject}
          </span>
          <span>
            <span className={styles.label}>Ученик:</span> {solution.solverId}
          </span>
        </div>
        <span
          className={`${styles.status} ${
            status === "pending"
              ? styles.statusPending
              : status === "assigned"
              ? styles.statusAssigned
              : styles.statusGraded
          }`}
        >
          {status === "pending"
            ? "Ожидает"
            : status === "assigned"
            ? "Назначено"
            : "Проверено"}
        </span>
      </div>

      <div className={styles.solutionBody}>
        <div className={styles.taskText}>
          <span className={styles.label}>Задание:</span> {solution.taskText}
        </div>
        <div className={styles.answerText}>
          <span className={styles.label}>Решение:</span> {solution.answerText}
        </div>
        <div className={styles.submittedAnswer}>
          <span className={styles.label}>Ответ ученика:</span>{" "}
          {solution.submittedAnswer}
        </div>
      </div>

      {solution.score !== null && (
        <div>
          <span className={styles.label}>Оценка:</span>{" "}
          <span className={styles.score}>{solution.score}</span>
          {solution.comment && (
            <div className={styles.comment}>
              <span className={styles.label}>Комментарий:</span>{" "}
              {solution.comment}
            </div>
          )}
        </div>
      )}
    </div>
  );
}
