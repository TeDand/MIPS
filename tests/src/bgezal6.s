# 4
#
#
# Tejas

ori $2, $0, 0x0007
bgezal $2, add
ori $2, $0, 0x0001
jr $0
nop
add: addi $2, $2, 0x0003
jr $31

# branch delay slot
