%% Initialization
clear ; close all; clc


%% ======================= Part 1: Plotting =======================
fprintf('Визуализируем данные ...\n')
data = load('ex1data1.txt');
X = data(:, 1); y = data(:, 2);
m = length(y); % number of training examples

% Plot Data
% Note: You have to complete the code in plotData.m
plotData(X, y);

fprintf('Нажмитие Enter, чтобы продолжить.\n');
pause;

%% =================== Part 2: Cost and Gradient descent ===================

X = [ones(m, 1), data(:,1)]; % Add a column of ones to x
theta = zeros(2, 1); % initialize fitting parameters

% Some gradient descent settings
iterations = 1500;
alpha = 0.01;

fprintf('\nТестируем функцию стоимости ...\n')
% compute and display initial cost
J = computeCost(X, y, theta);
fprintf('С theta = [0 ; 0]\nвычисленная стоимость = %f\n', J);
fprintf('Ожидаемая стоимость (примерно) 32.07\n');

% further testing of the cost function
J = computeCost(X, y, [-1 ; 2]);
fprintf('\nС theta = [-1 ; 2]\nвычисленная стоимость = %f\n', J);
fprintf('Ожидаемая стоимость (примерно) 54.24\n');

fprintf('Нажмитие Enter, чтобы продолжить.\n');
pause;

fprintf('\nВыполняем градиентный спуск ...\n')
% run gradient descent
theta = gradientDescent(X, y, theta, alpha, iterations);

% print theta to screen
fprintf('Theta, найденное градиентным спуском:\n');
fprintf('%f\n', theta);
fprintf('Ожидаемое значение theta (примерно)\n');
fprintf(' -3.6303\n  1.1664\n\n');

% Plot the linear fit
hold on; % keep previous plot visible
plot(X(:,2), X*theta, '-')
legend('Обучающая выборка', 'Линейная регрессия')
hold off % don't overlay any more plots on this figure

% Predict values for population sizes of 35,000 and 70,000
predict1 = [1, 3.5] *theta;
fprintf('Для населения = 35,000, мы прогнозируем прибыль %f\n',...
    predict1*10000);
predict2 = [1, 7] * theta;
fprintf('Для населения = 70,000, мы прогнозируем прибыль %f\n',...
    predict2*10000);

fprintf('Нажмитие Enter, чтобы продолжить.\n');
pause;

%% ============= Part 3: Visualizing J(theta_0, theta_1) =============
fprintf('Визуализируем J(theta_0, theta_1) ...\n')

% Grid over which we will calculate J
theta0_vals = linspace(-10, 10, 100);
theta1_vals = linspace(-1, 4, 100);

% initialize J_vals to a matrix of 0's
J_vals = zeros(length(theta0_vals), length(theta1_vals));

% Fill out J_vals
for i = 1:length(theta0_vals)
    for j = 1:length(theta1_vals)
	  t = [theta0_vals(i); theta1_vals(j)];
	  J_vals(i,j) = computeCost(X, y, t);
    end
end


% Because of the way meshgrids work in the surf command, we need to
% transpose J_vals before calling surf, or else the axes will be flipped
J_vals = J_vals';
% Surface plot
figure;
surf(theta0_vals, theta1_vals, J_vals)
xlabel('\theta_0'); ylabel('\theta_1');

% Contour plot
figure;
% Plot J_vals as 15 contours spaced logarithmically between 0.01 and 100
contour(theta0_vals, theta1_vals, J_vals, logspace(-2, 3, 20))
xlabel('\theta_0'); ylabel('\theta_1');
hold on;
plot(theta(1), theta(2), 'rx', 'MarkerSize', 10, 'LineWidth', 2);
