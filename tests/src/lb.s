# 100
# 
#
# Tejas

lui $3, 0x2000
lui $5, 0x2000
addi $5, $5, 0x4
ori $4, $0, 0x64
sw $4, 0($5)
lb $2, 7($3)
jr $0

# basic functionality
