# 0
#
#
# Tejas

lui $3, 0x2000
ori $4, $0, 0x88ff
sb $4, 3($3)
lw $2, 0($3)
srl $2, $2, 8
jr $0

# only storing a byte
