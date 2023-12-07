# Run this test once you have hooked up your terminal and memory
.data
    functions: .word end f
    info: .word 10 33 100 108 114 111 119 32 44 111 108 108 101 72 0
.text
.globl main
main:
    addi $sp, $sp, -4096
    la $s0, functions
begin:
    lw $t0, 4($s0)
    la $a0, info
    jalr $t0
    lw $t0, 0($s0)
    jalr $t0
f:
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s1, 4($sp)
    lw $s1, 0($a0)
    beq $s1, $0, endf
    addi $a0, $a0, 4
    jal f
    sw $s1, -256($0)
endf:
    lw $s1, 4($sp)
    lw $ra, 0($sp)
    addi $sp, $sp, 8
    jr $ra
end:
    j end