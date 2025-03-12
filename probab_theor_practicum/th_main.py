import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats
from scipy.optimize import minimize
from scipy.stats import chi2_contingency

np.random.seed(2025)  # устанавливаем seed для детерминированной генерации выборок

# 1. Генерация выборок и вычисление выборочных характеристик

a = 3
sigma2 = 0.81
gamma = 0.99
alpha = 0.01
a0 = 3
sigma02 = 1

sigma = np.sqrt(sigma2)
sigma0 = np.sqrt(sigma02)

n = 100


# Первая выборка
x = np.random.normal(loc=a, scale=sigma, size=n)

print("Первая выборка:\n", x)

# Вторая выборка
y = np.random.normal(loc=a, scale=sigma, size=n)

print("Вторая выборка:\n", y)


char_titles = {
    "Mean": "Среднее",
    "Variance": "Дисперсия",
    "Median": "Медиана",
    "Min": "x_min",
    "Max": "x_max",
    "Range": "Размах",
    "CV": "Коэффициент вариации",
    "Oscilation": "Коэффициент осцилляции",
    "Fixed variance": "Исправленная дисперсия",
}


def sample_characteristics(sample):
    mean = np.mean(sample)  # выборочное среднее
    variance = np.var(sample)  # оценка дисперсии
    fixed_variance = np.var(sample, ddof=1)  # исправленная оценка дисперсии
    median = np.median(sample)
    min_val = np.min(sample)
    max_val = np.max(sample)
    range_val = max_val - min_val
    cv = np.std(sample, ddof=1) / mean * 100  # коэффициент вариации
    oscilation = (max_val - min_val) / mean * 100  # коэффициент осцилляции

    return {
        "Mean": mean,
        "Variance": variance,
        "Fixed variance": fixed_variance,
        "Median": median,
        "Min": min_val,
        "Max": max_val,
        "Range": range_val,
        "CV": cv,
        "Oscilation": oscilation,
    }


x_char = sample_characteristics(x)

y_char = sample_characteristics(y)

print("Характеристики выборки X:")

for t, val in x_char.items():
    print(char_titles[t], f"{val:.5f}")

print()

print("Характеристики выборки Y:")

for t, val in y_char.items():
    print(char_titles[t], f"{val:.5f}")

print()

# 2. Эмпирическая функция распределения, гистограмма и сглаживание
# (для первой выборки x)


# Эмпирическая функция распределения
def ecdf(sample):
    x_val = np.sort(sample)
    y_val = np.arange(1, len(sample) + 1) / len(sample)

    return x_val, y_val


x_val_ecdf, y_val_ecdf = ecdf(x)

plt.figure()

plt.step(x_val_ecdf, y_val_ecdf)

plt.title("Эмпирическая функция распределения (X)")

plt.xlabel("Значение")
plt.ylabel("Частота")

# Гистограмма и сглаживание
plt.figure()

num_bins = int(1 + 3.322 * np.log10(n))  # Правило Стерджесса

plt.hist(
    x, bins=num_bins, density=True, alpha=0.7, label="Гистограмма"
)  # Используем правило Стерджесса для определения числа интервалов

# Сглаживание с помощью плотности нормального распределения
xmin, xmax = plt.xlim()
x_smooth = np.linspace(xmin, xmax, 300)

p = stats.norm.pdf(x_smooth, x_char["Mean"], np.sqrt(x_char["Variance"]))

plt.plot(x_smooth, p, "k", linewidth=2, label="Сглаженная кривая")

plt.title("Гистограмма и сглаживание (X)")

plt.xlabel("Значение")

plt.ylabel("Плотность")

plt.legend()

# 3. Доверительные интервалы (для первой выборки x)
print("Вычисление доверительных интвералов")
# Доверительный интервал для среднего при известной дисперсии
mu_gamma = stats.norm.ppf(1 - alpha / 2)

print("mu_gamma", mu_gamma)

ci_mean_known_var = (
    x_char["Mean"] - mu_gamma * sigma / np.sqrt(n),
    x_char["Mean"] + mu_gamma * sigma / np.sqrt(n),
)

# Доверительный интервал для среднего при неизвестной дисперсии
t_gamma = stats.t.ppf(1 - alpha / 2, n - 1)

print("t_gamma", t_gamma)

ci_mean_unknown_var = (
    x_char["Mean"] - t_gamma * np.sqrt(x_char["Fixed variance"]) / np.sqrt(n),
    x_char["Mean"] + t_gamma * np.sqrt(x_char["Fixed variance"]) / np.sqrt(n),
)

# Доверительный интервал для дисперсии
chi2_alpha1 = stats.chi2.ppf(alpha / 2, n - 1)

chi2_alpha2 = stats.chi2.ppf(1 - alpha / 2, n - 1)

print("chichi2", chi2_alpha1, chi2_alpha2)

ci_var = (
    (n - 1) * x_char["Fixed variance"] / chi2_alpha2,
    (n - 1) * x_char["Fixed variance"] / chi2_alpha1,
)

print("Доверительный интервал для среднего (известная дисперсия):", ci_mean_known_var)

print(
    "Доверительный интервал для среднего (неизвестная дисперсия):", ci_mean_unknown_var
)

print("Доверительный интервал для дисперсии:", ci_var)

# 4. Критерий хи-квадрат для проверки нормальности (простая гипотеза)
print("4. Проверка согласия эмпирических данных с нормальным распределением")

# Определение границ интервалов для построения гистограммы

num_bins = int(1 + 3.322 * np.log10(n))  # Правило Стерджесса

hist, bin_edges = np.histogram(x, bins=num_bins, density=False)


def normal_mnp(bin_edges, hist):
    def neg_log_likelihood(params):
        a, sigma = params
        # Вероятности попадания в каждый интервал для N(a, sigma^2)
        p = stats.norm.cdf((bin_edges[1:] - a) / sigma) - stats.norm.cdf(
            (bin_edges[:-1] - a) / sigma
        )
        p = np.maximum(p, 1e-15)  # избегаем log(0)
        return -np.sum(hist * np.log(p))

    # Начальные приближения для a и sigma
    a_init = x_char["Mean"]
    sigma_init = np.sqrt(x_char["Variance"])

    # Минимизируем минус логарифм правдоподобия
    res = minimize(
        neg_log_likelihood, x0=[a_init, sigma_init], bounds=[(None, None), (1e-9, None)]
    )
    a_hat, sigma_hat = res.x
    return a_hat, sigma_hat**2


mean_grouped, variance_grouped = normal_mnp(bin_edges, hist)


print("Среднее и дисперсия для сгруппированных данных")
print(mean_grouped, variance_grouped)

print("Гистограмма сгруппированных данных")
print(bin_edges)
print(hist)

# Вычисление теоретических вероятности для каждого интервала
expected_freqs = np.diff(stats.norm.cdf(bin_edges, loc=mean_grouped, scale=np.sqrt(variance_grouped)))

print("Теоретические вероятности\n", expected_freqs)
print("Сумма вероятностей:", np.sum(expected_freqs))

# Вычисление статистики хи-квадрат
chi2_stat = np.sum((hist - expected_freqs * n) ** 2 / (expected_freqs * n))

# Определение критического значения хи-квадрат
df = (
    num_bins - 1 - 2
)  # Степени свободы (число интервалов - 1 - число оцененных параметров (2))

print("Степени свободны", df)
critical_value = stats.chi2.ppf(gamma, df)

print(f"Хи-квадрат статистика: {chi2_stat}")

print(f"Критическое значение: {critical_value}")

if chi2_stat > critical_value:
    print("Отвергаем гипотезу о нормальном распределении")
else:
    print("Не отвергаем гипотезу о нормальном распределении")

# 5. Критерий хи-квадрат для проверки однородности
print("5. Критерий хи-квадрат для проверки однородности")
# Объединяем выборки и определяем интервалы
combined_sample = np.concatenate([x, y])
num_bins_homogeneity = num_bins
hist_x, _ = np.histogram(x, bins=num_bins_homogeneity)
hist_y, _ = np.histogram(y, bins=num_bins_homogeneity)

# Создаем таблицу сгруппированных данных
grouped_table = np.array([hist_x, hist_y])

print("Таблица сгруппированных данных")
print("Границы интервалов")
print(bin_edges)
print(grouped_table)
print(
    [
        int(grouped_table[0, i] + grouped_table[1, i])
        for i in range(num_bins_homogeneity)
    ]
)

# Вычисляем статистику хи-квадрат и табличное значение
chi2_stat_homogeneity = 0
for k in range(num_bins_homogeneity):
    for j in range(2):
        chi2_stat_homogeneity += (
            (grouped_table[j,k] - np.sum(grouped_table[j,:] * np.sum(grouped_table[:,k]) / n)) ** 2
            / (np.sum(grouped_table[j,:]) * np.sum(grouped_table[:,k]))
        )

df_homogeneity = num_bins_homogeneity - 1
print("Степени свободы", df_homogeneity)
critical_value_homogeneity = stats.chi2.ppf(gamma, df_homogeneity)

print(f"Хи-квадрат статистика: {chi2_stat_homogeneity}")

print(f"Критическое значение: {critical_value_homogeneity}")

if chi2_stat_homogeneity > critical_value_homogeneity:
    print("Отвергаем гипотезу об однородности выборок")
else:
    print("Не отвергаем гипотезу об однородности выборок")

# 6. Проверка гипотез о параметрах
print("6. Проверка гипотиз")
# H0: a_x = a0 против H1: a_x != a0
t_stat_mean = (x_char["Mean"] - a0) / (np.sqrt(x_char["Variance"]) / np.sqrt(n))
critical_value_mean = stats.t.ppf(1 - alpha / 2, n - 1)
print(f"\nПроверка гипотезы о среднем:")
print(f"t-статистика: {t_stat_mean}")
print(f"Критическое значение: {critical_value_mean}")

if abs(t_stat_mean) > critical_value_mean:
    print("Отвергаем H0: a_x = a0")
else:
    print("Не отвергаем H0: a_x = a0")

# H0: sigma_x^2 = sigma0^2 против H1: sigma_x^2 != sigma0^2
chi2_stat_var = (n - 1) * x_char["Fixed variance"] / sigma02
critical_value_var1 = stats.chi2.ppf(alpha / 2, n - 1)
critical_value_var2 = stats.chi2.ppf(1 - alpha / 2, n - 1)
print(f"\nПроверка гипотезы о дисперсии:")
print(f"Хи-квадрат статистика: {chi2_stat_var}")
print(f"Критические значения: {critical_value_var1}, {critical_value_var2}")

if chi2_stat_var < critical_value_var1 or chi2_stat_var > critical_value_var2:
    print("Отвергаем H0: sigma_x^2 = sigma0^2")
else:
    print("Не отвергаем H0: sigma_x^2 = sigma0^2")

# H0: a_x = a_y против H1: a_x != a_y
t_stat_means_eq = (x_char["Mean"] - y_char["Mean"]) / np.sqrt(
    ((n-1)*x_char["Variance"] + (n-1)*y_char["Variance"]) / (2*n -2)
) * np.sqrt(n / 2)

critical_value_means_eq = stats.t.ppf(1 - alpha / 2, 2 * n - 2)
print(f"\nПроверка гипотезы о равенстве средних:")
print(f"t-статистика: {t_stat_means_eq}")
print(f"Критическое значение: {critical_value_means_eq}")

if abs(t_stat_means_eq) > critical_value_means_eq:
    print("Отвергаем H0: a_x = a_y")
else:
    print("Не отвергаем H0: a_x = a_y")

# H0: sigma_x^2 = sigma_y^2 против H1: sigma_x^2 != sigma_y^2
f_stat_vars_eq = x_char["Variance"] / y_char["Variance"]
critical_value_vars_eq1 = stats.f.ppf(alpha / 2, n - 1, n - 1)
critical_value_vars_eq2 = stats.f.ppf(1 - alpha / 2, n - 1, n - 1)

print(f"\nПроверка гипотезы о равенстве дисперсий:")
print(f"F-статистика: {f_stat_vars_eq}")
print(f"Критические значения: {critical_value_vars_eq1}, {critical_value_vars_eq2}")

if f_stat_vars_eq < critical_value_vars_eq1 or f_stat_vars_eq > critical_value_vars_eq2:
    print("Отвергаем H0: sigma_x^2 = sigma_y^2")
else:
    print("Не отвергаем H0: sigma_x^2 = sigma_y^2")

plt.show()
