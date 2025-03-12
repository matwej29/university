import scipy
import numpy as np
import matplotlib.pyplot as plt

np.random.seed(42)

# Задаем параметры нормального распределения
mu = 3              # среднее
variance = 0.81        # дисперсия


sigma = np.sqrt(variance)  # стандартное отклонение
n = 200            # размер выборки

# Генерируем выборку
sample1 = np.random.normal(mu, sigma, n)
sample2 = np.random.normal(mu, sigma, n)


ymax = 0.6

# Создаем подграфики
plt.figure(figsize=(10, 5))

# Визуализируем выборку
plt.subplot(1, 2, 1)  # 1 строка, 2 колонки, 1-й подграфик
plt.hist(sample1, bins=15, density=True, alpha=0.6, color='g')

plt.ylim(0.0, ymax)

# Добавляем линию плотности нормального распределения
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)
p = np.exp(-0.5 * ((x - mu) / sigma) ** 2) / (sigma * np.sqrt(2 * np.pi))
plt.plot(x, p, 'k', linewidth=2)


# Визуализируем вторую выборку
plt.subplot(1, 2, 2)  # 1 строка, 2 колонки, 2-й подграфик
plt.hist(sample2, bins=15, density=True, alpha=0.6, color='b')

plt.ylim(0.0, ymax)

# добавляем линию плотности и на второй график
plt.plot(x, p, 'k', linewidth=2)

plt.title('Гистограмма двух выборок из нормального распределения')

plt.tight_layout()  # Автоматически подгоняет подграфики
plt.show()
