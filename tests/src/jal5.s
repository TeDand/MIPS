# 12

ori $2, $0, 0x0007
jal add
add $2, $31,$0
add: addi $2,$0, 0x0003
jr $0

# checking when link register written to
