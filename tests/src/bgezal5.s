# 12

lui $2, 0xF000
bgezal $2, add
nop
add $2, $0, $31
jr $0
nop
add: addi $2, 0x0003
jr $0

# chack link register when less than zero
