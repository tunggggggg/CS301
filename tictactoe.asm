.data
    victory: .word 87 105 110 110 101 114 32 80 108 97 121 101 114 32 0
.text
.globl main
main:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal drawboard
#______________________________set up memory for game board
    addi $sp, $sp, -44
    sw $ra, 0($sp)          
    sw $0, 4($sp)               #1
    sw $0, 8($sp)               #2
    sw $0, 12($sp)              #3
    sw $0, 16($sp)              #4
    sw $0, 20($sp)              #5
    sw $0, 24($sp)              #6
    sw $0, 28($sp)              #7
    sw $0, 32($sp)              #8
    sw $0, 36($sp)              #9
    sw $0, 40($sp)              #current player

play:
#_______________________________loading and rotating player
    lw $s0, 40($sp)
    bne $s0, $0, 3
    addi $s0, $0, 1             #first move set player to 1
    sw $s0, 40($sp)
    j gameplay
    addi $t0, $0, 1             #starting from second move, rotate players            
    bne $s0, $t0, 3             #last player was not 1, branch to set back to 1
    addi $s0, $0, 2             #else change to 2
    sw $s0, 40($sp)
    j gameplay
    addi $s0, $0, 1             #change to 1
    sw $s0, 40($sp)
#_______________________________finish rotating players, continue playing
gameplay:
    addi $v0, $0, 5
        syscall
    add $t0, $v0, $0            #read move
    sll $t0, $s0, 2
    add $t0, $t0, $sp
    sw $s0, 0($t0)              #stores player number 1 or 2 to the right cell
    add $a0, $0, $v0            #load cell number to a0
    #jal drawplayer              #draw X for player 1 and O for player 2


    jal check
    beq $v0, $0, play           #continue playing
    la $a0, victory
    lw $a1, 40($sp)             #load victorious player
    addi $t0, $0, 0             #counter for printing
    jal printVictory
    addi $v0, $0, 10            #end
        syscall
#_______________________________functions to check if the new move makes a player the winner
check:
    lw $t0, 20($sp)             #load cell 5
    lw $t1, 40($sp)             #load current player
    bne $t0, $t1, endcell5
    lw $t0, 4($sp)              #check cell 1
    bne $t0, $t1, 4
    lw $t0, 4($sp)              #check cell 9
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
    lw $t0, 8($sp)              #check cell 2
    bne $t0, $t1, 4
    lw $t0, 32($sp)             #check cell 8
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
    lw $t0, 12($sp)             #check cell 3
    bne $t0, $t1, 4
    lw $t0, 28($sp)             #check cell 7
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
    lw $t0, 16($sp)             #check cell 4
    bne $t0, $t1, 4
    lw $t0, 24($sp)             #check cell 6
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
#_______________________________if none of the pattern that crosses cell 5(middle cell), check 123 and 789
endcell5:
    lw $t0, 4($sp)              #load cell 1
    lw $t1, 40($sp)
    bne $t0, $t1, endcell1
    lw $t0, 8($sp)              #check cell 2
    bne $t0, $t1, 4
    lw $t0, 12($sp)             #check cell 3
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
endcell1:
    lw $t0, 28($sp)             #load cell 7
    lw $t1, 40($sp)
    bne $t0, $t1, endcell7
    lw $t0, 32($sp)             #check cell 8
    bne $t0, $t1, 4
    lw $t0, 36($sp)             #check cell 9
    bne $t0, $t1, 2
    addi $v0, $0, 1             #current player wins
    jr $ra
endcell7:
    addi $v0, $0, 0             #not win yet
    jr $ra
#_______________________________print victory message and the winning player
printVictory:
    addi $v0, $0, 11
        syscall
    addi $t0, $0, 4
    la $a0, victory
    add $a0, $a0, $t0
    lw $a0, 0($a0)
    bne $a0, $0, printVictory
    addi $a0, $a1, 0            #print winning player
    addi $v0, $0, 11
        syscall
    addi $a0, $0, 33            #print winning player
    addi $v0, $0, 11            #print !
        syscall
    jr $ra
#_______________________________setup the board
drawboard:
    addi $t0, $0, 121           #max size
    addi $s0, $0, 40            #x-coordinate
    addi $s1, $0, 0             #y-coordinate
    addi $s2, $0, 80            #color
for1:
    slt $t1, $t0, $s0
    bne $t1, $0, horizontal
    beq $s1, $t0, endfor1
    sw $s0, -224($0) #load x-coordinate
    sw $s1, -220($0) #load y-coordinate
    sw $s2, -216($0) #load color
    sw $0, -212($0) #draw
    addi $s1, $s1, 1
    j for1
endfor1:
    addi $s0, $s0, 41
    addi $s1, $0, 0             #y-coordinate
    j for1
horizontal:
    addi $t0, $0, 121           #limit
    addi $s0, $0, 0             #x-coordinate
    addi $s1, $0, 40            #y-coordinate
    addi $s2, $0, 263           #color cyan
for2: 
    slt $t1, $t0, $s1
    bne $t1, $0, endSetUp   
    beq $s0, $t0, endfor2
    sw $s0, -224($0)
    sw $s1, -220($0)
    sw $s2, -216($0)
    sw $0, -212($0)
    addi $s0, $s0, 1
    j for2
endfor2:
    addi $s1, $s1, 41
    addi $s0, $0, 0 #y-coordinate
    j for2
endSetUp:
    jr $ra