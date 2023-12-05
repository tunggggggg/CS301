addi $sp, $0, -4096
addi $sp, $sp, -48
addi $t0, $0, 256 #blue
addi $s1, $0, 0 #y-coordinate
addi $s2, $0, 255 #color
addi $s0, $0, 85  #x-coordinate
addi $t1, $0, 1
addi $t2, $0, 2
for1:
    
    beq $s1, $t0, endfor1
    sw $s0, -224($0)
    sw $s1, -220($0)
    sw $s2, -216($0)
    sw $0, -212($0)
    addi $s1, $s1, 1
    j for1

endfor1:
    beq $t1, $t2, horizontal
    addi $s0, $s0, 85
    addi $s1, $0, 0 #y-coordinate
    addi $t1, $t1, 1
    j for1

horizontal:
addi $t1, $0, 1
addi $t2, $0, 2
addi $t0, $0, 255 #limit
addi $s0, $0, 0 #x-coordinate
addi $s2, $0, 263 #color cyan
addi $s1, $0, 85  #y-coordinate
for2:
    
    beq $s0, $t0, endfor2
    sw $s0, -224($0)
    sw $s1, -220($0)
    sw $s2, -216($0)
    sw $0, -212($0)
    addi $s0, $s0, 1
    j for2

endfor2:
    beq $t1, $t2, end
    addi $s1, $s1, 85
    addi $s0, $0, 0 #y-coordinate
    addi $t1, $t1, 1
    j for2
end:
    j end




