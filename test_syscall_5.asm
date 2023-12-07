main:
    addi $sp, $sp, -4096
    jal syscall5
    addi $s0, $v0, 0
    j end

syscall5:
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $v0, 4($sp)
    sw $t0, 8($sp)
    addi $t0, $0, 10

read:
    lw $v0, -240($0)
    beq $v0, $0, read
    lw $v0, -236($0)
    addi $k1, $0, 48
    slt $k1, $k1, $v0
    beq $k1, $0, 3
    addi $k1, $0, 57
    slt $k1, $v0, $k1
    bne $k1, $0, endif
    jr $ra
endif:
    lw $k1, 4($sp)
    mult $k1, $t0
    mflo $k1
    addi $v0, $v0, -48
    add $v0, $k1, $v0
    sw $v0, 4($sp)
    sw $0, -240($0)
    jal read

    lw $ra, 0($sp)
    lw $v0, 4($sp)
    lw $t0, 8($sp)
    addi $sp, $sp, 12
    jr $ra

end:
    j end