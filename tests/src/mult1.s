# 21
#
#
# Tejas

lui $3, 0x0007
lui $4, 0x0003
mult $3, $4
mfhi $2
jr $0
nop

# testing HI register
