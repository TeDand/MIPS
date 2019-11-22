# 112
# p
# 
# Freddie

ori $2, $0, 0x0070
lui $3, 0x3000
addi $3, $3, 0x0004
sw $2, 0($3)
jr $0

# testing putchar with p
