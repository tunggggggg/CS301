.data
    methodtable: .word 10 12 12 13
    #array1: .asciiz "work please"
    .text
    .globl main
main:
    addi $a0, $0, 10
    la $t0, array1
    lw $t0, 0($t0)
    add $s0, $v0, $t0
    addi $v0, $0, 10
    syscall
