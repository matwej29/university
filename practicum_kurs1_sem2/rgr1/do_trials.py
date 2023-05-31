from arrayGenerator import generate_array


def do_trials(func, array_len, sort_type, measure_type = 'time', trials = 1):
    """
        Выполняет trials испытаний функции func (для measure_type равного 'time') следующим образом:
         1) создается список массивов длины array_len с типом упорядоченности sort_type,
         2) засекается время начала процесса,
         3) на созданных массивах поочередно вызывается функция сортировки func,
         4) засекается время окончания процесса,
         5) разница между временем окончания и начала процесса делится на количество испытаний и возвращается в качестве ответа.
        "Засечь" время можно, вызывав функцию time.process_time() - она вернет количество прошедших после запуска секунд

        Случай measure_type == 'counter' пока не обрабатывайте.

        Вход:
        - func: (функция) функция сортировки
        - array_len: (целое число) размер массивов, на которых проводятся испытания
        - sort_type: тип создаваемого массива:
         - 'random': случайный
         - 'reverse': в обратном порядке
         - 'almost': почти упорядоченный
         - 'normal': упорядоченный
        - measure_type: тип проводимых измерений:
         - 'time': измерять время выполнения функций
         - 'counter': измерять количество выполняемых функциями "шагов"
        - trials: (целое число) количество проводимых испытаний

        Выход:
        Среднее значение измеряемой величины.
        """

    global counter  # переменная понадобится вам позже, при первоначальной реализации оставьте ее как есть
    result = 0

    ### НАЧАЛО ВАШЕГО КОДА
    arrays = [generate_array(array_len, sort_type) for _ in range(array_len)]  # must be trials

    if measure_type == 'time':
        from time import time

        time_start = time()
        for arr in arrays:
            func(arr)
        time_end = time()

        # время в миллисекундах
        result = (time_end - time_start) / array_len / 1000
    elif measure_type == 'counter':
        counter = 0

        for arr in arrays:
            func(arr)

        result = counter / trials
    else:
        raise ValueError(f"Not knows measure_type: {measure_type}")
    ### КОНЕЦ ВАШЕГО КОДА

    return result
