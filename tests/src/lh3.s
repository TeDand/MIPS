# 16
#
#
# Tejas

addi $5, $5, 0x1000
lui $3, 0x1000
lh $2, 2($3)
srl $2, $2, 0x8
jr $0

# test load from instruction
