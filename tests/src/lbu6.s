# 1

addi $5, $0, 0x80
lui $3, 0x2000
sw  $5, 0($3)
lbu $2, 3($3)
srl $2, $2, 0x7
jr $0

# check sign extend
