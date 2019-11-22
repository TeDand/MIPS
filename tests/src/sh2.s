# 245
#
#
# Tejas

lui $3, 0x2000
ori $4, $0, 0x88ff
sh $4, 1($3)
jr $0

# misaligned address
