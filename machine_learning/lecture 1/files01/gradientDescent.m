function [theta, J_history] = gradientDescent(X, y, theta, alpha, num_iters)
%GRADIENTDESCENT Использует градиентный спуск для поиска параметров theta,
%   используя num_iters итераций и скорость обучения alpha

% Инициализация полезных переменных
m = length(y); % количество обучающих примеров
J_history = zeros(num_iters, 1);

for iter = 1:num_iters

    % ====================== НАЧАЛО ВАШЕГО КОДА ======================
    % Instructions: Выполните один шаг градиентного спуска по вектору
    %               параметров theta. 
    %
    % Hint: Во время отладки будет полезно выводить значения функции
    %       стоимости (computeCost) и градиента.
    %







    % ====================== КОНЕЦ ВАШЕГО КОДА ======================

    % Сохраняем значения J на каждой итерации
    J_history(iter) = computeCost(X, y, theta);

end

end
