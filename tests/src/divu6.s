# 0
#
#
# Tejas

lui $3, 0xffff
ori $3, $3, 0xfff9
lui $4, 0xffff
ori $4, $4, 0xfffd
divu $3, $4
mflo $2
jr $0
nop

# testing negative/negative quotient
