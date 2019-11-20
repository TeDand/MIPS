# 136

lui $3, 0x2000
ori $4, $0, 0x88ff
sw $4, 0($3)
lw $2, 0($3)
srl $2, $2, 0x8
jr $0

# basic functionality
