.text
main:
    li $v0, 5
    syscall
    move $t0, $v0

    addi $t0, $t0, 1

    li $v0, 1
    move $a0, $t0
    syscall

    li $v0, 10
    syscall
