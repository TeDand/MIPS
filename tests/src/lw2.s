# 255

lui $3, 0x88ff
ori $4, $0, 0x88ff
sw $4, 0($3)
lw $2, 3($3)
ori $5, 0x0010
srl $2, $5
jr $0

# basic functionality
