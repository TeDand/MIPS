# 7

ori $2, $0, 0x0007
beq $0, $0, end
nop
ori $2, $0, 0x00dd
end: jr $0
nop

# check branch if equal
