# 246
#
#
# Freddie

lui $3, 0x7fff
ori $3, $3, 0xffff
lui $4, 0xffff
ori $4, $4, 0xffff
sub $2, $3, $4
jr $0
nop

# positive overflow
