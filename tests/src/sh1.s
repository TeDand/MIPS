# 0

lui $3, 0x2000
ori $4, $3, 0x88ff
sw $4, 0($3)
lw $2, 0($3)
srl $2, $2, 0x10
jr $0

# storing only half a word
