# 255

lui $5, 0x8000
lui $3, 0x2000
sw  $5, 0($3)
lh $2, 0($3)
srl $2, $2, 0x10
jr $0

# check sign extend
