# 4
#
#
# Tejas

lui $3, 0x2400
ori $4, $0, 0x04
sw $4, -4($3)
lb $2, -1($3)
jr $0

# test load at edge of data memory
