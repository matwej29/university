import { ReviewerLogin } from "@/components/ReviewerLogin";

export default function ReviewerLoginPage() {
  return (
    <div style={{ padding: "2rem" }}>
      <h1 style={{ textAlign: "center", marginBottom: "2rem" }}>
        Вход для проверяющего
      </h1>
      <ReviewerLogin />
    </div>
  );
}
