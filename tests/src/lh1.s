# 136

lui $3, 0x2000
ori $4, $0, 0x88ff
sw $4, 0($3)
lh $2, 2($3)
ori $5, 0x008
srl $2, $5
jr $0

# basic functionality
