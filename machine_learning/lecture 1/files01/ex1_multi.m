%% Initialization

%% ================ Part 1: Feature Normalization ================

%% Clear and Close Figures
clear ; close all; clc

fprintf('Загружаем данные ...\n');

%% Load Data
data = load('ex1data2.txt');
X = data(:, 1:2);
y = data(:, 3);
m = length(y);

% Print out some data points
fprintf('Первые 10 примеров из обучающей выборки: \n');
fprintf(' x = [%.0f %.0f], y = %.0f \n', [X(1:10,:) y(1:10,:)]');

fprintf('Нажмитие Enter, чтобы продолжить.\n');
pause;

% Scale features and set them to zero mean
fprintf('Нормализуем признаки ...\n');

[X mu sigma] = featureNormalize(X);

% Add intercept term to X
X = [ones(m, 1) X];


%% ================ Part 2: Gradient Descent ================

% ====================== YOUR CODE HERE ======================
% Instructions: We have provided you with the following starter
%               code that runs gradient descent with a particular
%               learning rate (alpha). 
%
%               Your task is to first make sure that your functions - 
%               computeCost and gradientDescent already work with 
%               this starter code and support multiple variables.
%
%               After that, try running gradient descent with 
%               different values of alpha and see which one gives
%               you the best result.
%
%               Finally, you should complete the code at the end
%               to predict the price of a 1650 sq-mt, 3 br house.
%
% Hint: By using the 'hold on' command, you can plot multiple
%       graphs on the same figure.
%
% Hint: At prediction, make sure you do the same feature normalization.
%

fprintf('Проводим градиентный спуск ...\n');

% Choose some alpha value
alpha = 0.01;
num_iters = 400;

% Init Theta and Run Gradient Descent 
theta = zeros(3, 1);
[theta, J_history] = gradientDescentMulti(X, y, theta, alpha, num_iters);

% Plot the convergence graph
figure;
plot(1:numel(J_history), J_history, '-b', 'LineWidth', 2);
xlabel('Количество итераций');
ylabel('Стоимость J');

% Display gradient descent's result
fprintf('Theta, вычисленное градиентным спуском: \n');
fprintf(' %f \n', theta);
fprintf('\n');

% Вычислите прогноз стоимости дома с площадью 1650 кв.м. и тремя спальнями
% ====================== НАЧАЛО ВАШЕГО КОДА ======================
% Помните, что первый столбец матрицы X состоит из одних единиц. Поэтому,
% его не требуется нормализовать.
price = 0; % Вам нужно изменить это значение


% ====================== КОНЕЦ ВАШЕГО КОДА ======================

fprintf(['Прогнозируемая стоимость дома с площадью 1650 кв.м. и тремя спальнями ' ...
         '(используя градиентный спуск):\n $%f\n'], price);

fprintf('Нажмитие Enter, чтобы продолжить.\n');
pause;

%% ================ Part 3: Normal Equations ================

fprintf('Решаем с помощью нормального уравнения...\n');

% ====================== YOUR CODE HERE ======================
% Instructions: The following code computes the closed form 
%               solution for linear regression using the normal
%               equations. You should complete the code in 
%               normalEqn.m
%
%               After doing so, you should complete this code 
%               to predict the price of a 1650 sq-mt, 3 br house.
%

%% Load Data
data = csvread('ex1data2.txt');
X = data(:, 1:2);
y = data(:, 3);
m = length(y);

% Add intercept term to X
X = [ones(m, 1) X];

% Calculate the parameters from the normal equation
theta = normalEqn(X, y);

% Display normal equation's result
fprintf('Theta, вычисленное с помощью нормального уравнения: \n');
fprintf(' %f \n', theta);
fprintf('\n');


% Estimate the price of a 1650 sq-mt, 3 br house
% ====================== YOUR CODE HERE ======================
price = 0; % You should change this


% ============================================================

fprintf(['Прогнозируемая стоимость дома с площадью 1650 кв.м. и тремя спальнями ' ...
         '(с помощью нормального уравнения):\n $%f\n'], price);

