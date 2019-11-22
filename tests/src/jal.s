# 11

ori $2, $0, 0x0007
jal add
addi $2, $2, 0x0001
end: jr $0
nop
add: addi $2, $2, 0x0003
jr $31

# basic functionality
