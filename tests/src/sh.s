# 255

lui $3, 0x2000
ori $4, $0, 0x88ff
sh $4, 2($3)
lh $2, 2($3)
jr $0

# basic functionality
