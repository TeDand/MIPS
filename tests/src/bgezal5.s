# 12

lui $2, 0xf000
bgezal $2, add
nop
add $2, $0, $31
jr $0
nop
add: addi $2, $2, 0x0003
jr $0

# check link register when less than zero
