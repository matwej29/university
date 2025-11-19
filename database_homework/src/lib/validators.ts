import { z } from "zod";

export const subjectSchema = z.object({
  name: z.string().min(1, "Название предмета обязательно")
});

export const reviewerSchema = z.object({
  fullName: z.string().min(3, "ФИО слишком короткое"),
  subjectNames: z.array(z.string()).default([])
});

export const taskSchema = z.object({
  body: z.string().min(5),
  correctAnswer: z.string().min(1),
  subjectName: z.string().min(1)
});

export const solutionSchema = z.object({
  solverId: z.number().int().nonnegative(),
  taskId: z.number().int().positive(),
  answerText: z.string().min(1),
  submittedAnswer: z.string().min(1)
});

export const assignSchema = z.object({
  reviewerId: z.number().int().positive()
});

export const gradeSchema = z.object({
  reviewerId: z.number().int().positive(),
  score: z.number().int().min(0).max(100),
  comment: z.string().max(2000).optional()
});

export const queueSchema = z.object({
  subjectName: z.string().min(1).optional()
});
