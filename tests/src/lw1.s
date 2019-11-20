# 255

lui $3, 0x2000
lui $4, 0x88ff
sw $4, 0($3)
lw $2, 0($3)
srl $2, $2, 0x10
jr $0

# basic functionality
