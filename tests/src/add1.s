# 246

lui $3, 0x7fff
ori $3, $3, 0xffff
ori $4, $0, 0x1
add $2, $3, $4
jr $0
nop

# positive overflow
