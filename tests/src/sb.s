# 255
#
#
# Tejas

lui $3, 0x2000
ori $4, $0, 0x88ff
sb $4, 3($3)
lw $2, 0($3)
jr $0

# basic functionality
