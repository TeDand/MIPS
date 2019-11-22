# 7
#
#
# Tejas

ori $3, $0, 0x0007
lui $4, 0xffff
ori $4, $4, 0xfffd
divu $3, $4
mfhi $2
jr $0
nop

# testing positive/negative remainder
