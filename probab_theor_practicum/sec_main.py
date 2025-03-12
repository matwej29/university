import numpy as np

import matplotlib.pyplot as plt

import scipy.stats as stats

np.random.seed(42)

# Параметры для генерации выборок

a = 3

sigma2 = 0.81
sigma = np.sqrt(sigma2)

n = 200  # Объем выборки

# Генерация выборок

np.random.seed(0)  # Для воспроизводимости

sample1 = np.random.normal(a, sigma, n)

sample2 = np.random.normal(a, sigma, n)

# Вычисление статистик для выборок

def calculate_statistics(sample):

    mean = np.mean(sample)

    variance = np.var(sample, ddof=1)

    median = np.median(sample)

    min_value = np.min(sample)

    max_value = np.max(sample)

    range_value = max_value - min_value

    variation_coefficient = np.std(sample, ddof=1) / mean

    oscillation_coefficient = (max_value - min_value) / mean

    return (mean, variance, median, min_value, max_value, range_value, variation_coefficient, oscillation_coefficient)

stats1 = calculate_statistics(sample1)

stats2 = calculate_statistics(sample2)

# Печать статистик

print("Statistics for Sample 1:", stats1)

print("Statistics for Sample 2:", stats2)

# Построение графиков

def plot_sample(sample):

    plt.hist(sample, bins='sturges', density=True, alpha=0.6, color='g', label='Histogram')

    xmin, xmax = plt.xlim()

    x = np.linspace(xmin, xmax, 100)

    p = stats.norm.pdf(x, np.mean(sample), np.std(sample, ddof=1))

    plt.plot(x, p, 'k', linewidth=2, label='Normal distribution')

    plt.title('Sample Distribution')

    plt.xlabel('Value')

    plt.ylabel('Density')

    plt.legend()

    plt.show()

plot_sample(sample1)

# Построение эмпирической функции распределения

def plot_ecdf(sample):

    n = len(sample)

    x = np.sort(sample)

    y = np.arange(1, n+1) / n

    plt.step(x, y, where='mid', label='ECDF')

    plt.title('Empirical Cumulative Distribution Function')

    plt.xlabel('Value')

    plt.ylabel('ECDF')

    plt.grid(True)

    plt.show()

plot_ecdf(sample1)

# Доверительные интервалы

confidence_level = 0.99

alpha = 1 - confidence_level

mean_confidence_interval = stats.norm.interval(confidence_level, loc=np.mean(sample1), scale=np.std(sample1, ddof=1)/np.sqrt(n))

variance_confidence_interval = ((n - 1) * np.var(sample1, ddof=1) / stats.chi2.ppf(1 - alpha / 2, n - 1),

                                (n - 1) * np.var(sample1, ddof=1) / stats.chi2.ppf(alpha / 2, n - 1))

print("Confidence interval for mean:", mean_confidence_interval)

print("Confidence interval for variance:", variance_confidence_interval)

# Проверка гипотез

a0 = 3

sigma0 = 1

# Гипотеза о среднем

t_statistic, p_value = stats.ttest_1samp(sample1, a0)

print("T-statistic:", t_statistic, "P-value:", p_value)

# Гипотеза о дисперсии

chi2_statistic = (n - 1) * np.var(sample1, ddof=1) / sigma0**2

p_value_variance = stats.chi2.sf(chi2_statistic, n - 1)

print("Chi-square statistic:", chi2_statistic, "P-value for variance:", p_value_variance)

# Гипотеза о равенстве средних

t_statistic_ind, p_value_ind = stats.ttest_ind(sample1, sample2)

print("T-statistic for equal means:", t_statistic_ind, "P-value:", p_value_ind)

# Гипотеза о равенстве дисперсий

f_statistic, p_value_f = stats.levene(sample1, sample2)

print("F-statistic for equal variances:", f_statistic, "P-value:", p_value_f)
