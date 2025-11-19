import type { ReactNode } from "react";
import "./globals.css";

export const metadata = {
  title: "ЕГЭ Проверка API",
  description: "Backend сервис для проверки решений ЕГЭ",
};

export default function RootLayout({ children }: { children: ReactNode }) {
  return (
    <html lang="ru">
      <body>{children}</body>
    </html>
  );
}
