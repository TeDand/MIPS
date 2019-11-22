# 20
#
#
# Freddie

ori $2, $0, 0x0007
lui $3, 0x1000
ori $3, $3, 0x0018
jalr $4, $3
add $2, $4, $0
addi $2, $2, 0x0001
add: jr $0

# checking when link register written to
