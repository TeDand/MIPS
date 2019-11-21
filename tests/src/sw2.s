# 245

lui $3, 0x2000
ori $4, $0, 0x88ff
sw $4, -1($3)
jr $0

# storing below data memory boundary
