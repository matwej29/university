; i8080
;ORG 8000H

ID_1: DB 15 ; a
ID_2: DB 40 ; b
ID_3: DB 0 ; c

;ORG 0H
MAIN:
    ; подготовка стекового фрема для вызова gcd(a, b):
    ; копируем значения в параметры функции
    ; a
    LDA ID_1
    PUSH PSW
    
    ; b
    LDA ID_2
    PUSH PSW
    
    ; место под результат
    LDA ID_3
    PUSH PSW
    
    CALL GCD

    ; записали результат в ID_3
    POP PSW
    STA ID_3

    ; удалили параметры a и b со стека
    POP B
    POP B
    
    HLT

GCD:
    
WHILE:
    ; условие цикла a > 0 && b > 0
    ; Загружаем b со стека в регистр A
    LXI H, 5
    DAD SP
    MOV A, M
    SUI 0 ; отнимаем ноль от регистра A для того, чтобы обновить флаги

    ; Если b <= 0, заканчиваем цикл
    JM END_WHILE
    JZ END_WHILE

    ; Загружаем b со стека в регистр A
    LXI H, 7
    DAD SP
    MOV A, M
    SUI 0

    ; Если a <= 0, заканчиваем цикл
    JM END_WHILE
    JZ END_WHILE

    ; тело цикла
    ; Проверка условия  if (a - b > 0)
    LXI H, 5
    DAD SP
    MOV B, M

    LXI H, 7
    DAD SP
    MOV A, M

    ; a - b
    SUB B

    ; сравнение a - b > 0
    JP THEN
    JMP ELSE
THEN:
    ; a = a - b
    LXI H, 7
    DAD SP
    MOV M, A

    JMP WHILE
ELSE:
    ; b
    LXI H, 5
    DAD SP
    MOV A, M

    ; a
    LXI H, 7
    DAD SP
    MOV B, M

    ; b - a
    SUB B

    ; b = b - a
    LXI H, 5
    DAD SP
    MOV M, A

    JMP WHILE

END_WHILE:
    ; Вышли из цикла
    ; Считаем a + b
    LXI H, 5
    DAD SP
    MOV B, M

    LXI H, 7
    DAD SP
    MOV A, M

    ADD B
    
    ; записываем результат в предназначенное место на стеке
    LXI H, 3
    DAD SP
    MOV M, A

    RET
