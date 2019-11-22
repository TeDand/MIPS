# 245
#
#
# Freddie

lui $3, 0x0fff
ori $3, $3, 0xfffc
jalr $4, $3
nop
jr $0

# jump below instruction memory
