. text
main :
# Запрос на ввод целого числа
li $v0 , 4
la $a0 , prompt
syscall
# Прочитать целое число и сохраните его в $s0
li $v0 , 5
syscall
move $s0 , $v0
# Инкрементировать s0
addiu $s0, %s0, 1
# Вывести текст
li $v0 , 4
la $a0 , output
syscall
# Вывести введенное число
li $v0 , 1
move $a0 , $s0
syscall
# Завершение программы
li $v0 , 10
syscall
. data
prompt : . asciiz " Please enter an integer: "
output : . asciiz "\ nResult: "