# 7

ori $2, $0, 0x7
ori $3, $0, 0x0
bne $0, $3, end
nop
jr $0
nop
end: addi $2, $2, 0x1 
jr $0
nop

# check no branch if equal
