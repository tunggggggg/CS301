.data
          barray: .word 4 f
          array: .word 3 4 5 #This comment must be ignored
.text
.globl main
main:
  #This comment must be ignored
  addi $s0, $zero, 100 #this comment needs to be ignored
loop:
  addi $t0, $0, 4 #call f with parameter i
  addi $s0, $v0, 0 #i = f(i)
  addi $s0, $0, 4 #($s0)
  sw $t1, 4($s0)
  lw $t0, 4($s1) 
  bne $t0, $0, f
  sll $t0, $t0, 2
  srl $t0, $t0, 2
  jal f
  sll $t0, $t0, 2
  srl $t0, $t0, 2
f:
  addi $t4, $0, 0
  syscall
