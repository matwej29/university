nor $3, $2, $1
loop:
addu $11, $10, $12
addiu $12, $zero, 2020
lw $11, 4($12)
sw $11, 8($12)
addiu $10, $zero, -8
label1:
addiu $10, $10, 8
beq $zero, $10, label1
j loop
