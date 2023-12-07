#This is starter code, so that you know the basic format of this file.
#Use _ in your system labels to decrease the chance that labels in the "main"
#program will conflict

.data
.text
_syscallStart_:
    la $k1, _END_OF_STATIC_MEMORY_
    sw $k1, -200($0)
    beq $v0, $0, _syscall0 #jump to syscall 0
    addi $k1, $0, 1
    beq $v0, $k1, _syscall1 #jump to syscall 1
    addi $k1, $0, 5
    beq $v0, $k1, _syscall5 #jump to syscall 5
    addi $k1, $0, 9
    beq $v0, $k1, _syscall9 #jump to syscall 9
    addi $k1, $0, 10
    beq $v0, $k1, _syscall10 #jump to syscall 10
    addi $k1, $0, 11
    beq $v0, $k1, _syscall11 #jump to syscall 11
    addi $k1, $0, 12
    beq $v0, $k1, _syscall12 #jump to syscall 12
    #Error state - this should never happen - treat it like an end program
    j _syscall10

#Do init stuff
_syscall0:
    addi $sp, $sp, -4096
    la $k1, _END_OF_STATIC_MEMORY_
    j _syscallEnd_

#Print Integer
_syscall1:
print:
    bne $a0, $0, endif1
    jr $ra
endif1:
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
    jr $k0

#Read Integer
_syscall5:
    # Read Integer code goes here

    addi $sp, $sp, -12
    sw $ra, 0($sp)
    addi $t0, $v0, 0
    addi $v0, $0, 0
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
    bne $k1, $0, endif5
    jr $ra
endif5:
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
    jr $k0

#Heap allocation
_syscall9:
    lw $k1, -200($0)
    sub $k1, $k1, $a0
    addi $v0, $k1, 0
    sw $k1, -200($0)
    jr $k0

#"End" the program
_syscall10:
    j _syscall10

#print character
_syscall11:
    # print character code goes here
    sw $a0, -256($0)
    jr $k0

#read character
_syscall12:
    # read character code goes here
    loop:
        lw $v0, -240($0)
        bne $v0, $0, 1
        j loop
        lw $v0, -236($0)
        sw $s0, -240($0)
    jr $k0

#extra credit syscalls go here?

_syscallEnd_:
