# 254
#
#
# Tejas

ori $3, $0, 0x0007
lui $4, 0xffff
ori $4, $4, 0xfffd
div $3, $4
mflo $2
jr $0
nop

# testing potitive/negative quotient
