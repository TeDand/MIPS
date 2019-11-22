# 16
#
#
# Freddie

addi $5, $5, 0x1000
lui $3, 0x1000
lw $2, 0($3)
srl $2, $2, 0x8
jr $0

# test load from instruction
