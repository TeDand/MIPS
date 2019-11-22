# 3

lui $3, 0xF000
ori $2, $0, 0x0001
bltzal $3, add
nop
addi $2, $2, 0x0001
jr $0
nop
add: addi $2, $2, 0x0001
jr $31

# check link reg when less than 0
