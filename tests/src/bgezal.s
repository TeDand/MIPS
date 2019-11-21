# 10

ori $2, $0, 0x0007
bgezal $2, add
nop
jr $0
nop
add: addi $2, 0x0003
jr $31

# basic functionality
