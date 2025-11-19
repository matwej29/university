function [theta, J_history] = gradientDescentMulti(X, y, theta, alpha, num_iters)
%GRADIENTDESCENTMULTI Использует градиентный спуск для поиска параметров theta

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
