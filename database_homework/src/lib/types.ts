export interface Subject {
  name: string;
}

export interface Reviewer {
  id: number;
  fullName: string;
  subjects: string[];
}

export interface Task {
  id: number;
  text: string;
  correctAnswer: string;
  subject: string;
  createdAt: string;
}

export interface Solution {
  id: number;
  solverId: number;
  taskId: number;
  taskText: string;
  subject: string;
  answerText: string;
  submittedAnswer: string;
  reviewerId: number | null;
  score: number | null;
  comment: string | null;
  assignedAt: string | null;
  reviewedAt: string | null;
  createdAt: string;
}
