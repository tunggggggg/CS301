main:
    addi $sp, $sp, -4096
    addi $a0, $0, 123
    jal print 
    j end

print:
    bne $a0, $0, endif
    jr $ra

endif:
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $k1, 4($sp)
    addi $k1, $0, 10
    div $a0, $k1
    mflo $a0
    mfhi $k1
    jal print

    addi $k1, $k1, 48
    sw $k1, -256($0)
    lw $ra, 0($sp)
    lw $k1, 4($sp)
    addi $sp, $sp, 8
    jr $ra

end:
    j end