import { ReviewerDashboard } from "@/components/ReviewerDashboard";

export default function ReviewerPage({ params }: { params: { id: string } }) {
  return <ReviewerDashboard reviewerId={params.id} />;
}
