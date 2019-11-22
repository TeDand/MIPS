# 2

lui $3, 0xF000
ori $2, $0, 0x1
blez $3, end
nop
addi $2, $2, 0x1
end: addi $2, $2, 0x1
jr $0
nop

# check branch if less than zero
