# 83
#
#
# Tejas

lui $3, 0xffff
ori $3, $3, 0xfff9
ori $4, $0, 0x0003
divu $3, $4
mflo $2
jr $0
nop

# testing negative/positive quotient
