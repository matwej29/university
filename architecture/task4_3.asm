.text
main:
    addi $v0, $zero, 5
    syscall
    move $s0, $v0

    addi $v0, $zero, 5
    syscall
    move $s1, $v0

    div $s0, $s1
    mfhi $s2

    addi $v0, $zero, 1
    move $a0, $s2
    syscall
    addi $v0, $zero, 10
    syscall
