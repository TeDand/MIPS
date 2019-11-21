# 4

lui $3, 0x2400
ori $4, $0, 0x04
sw $4, -4($3)
lw $2, -4($3)
jr $0

# test load at edge of data memory
