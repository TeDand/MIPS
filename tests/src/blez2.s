# 3

ori $2, $0, 0x1
blez $2, end
nop
addi $2, 0x1
end:addi $2, 0x1
    jr $0
nop

# check no branch if greater than zero
