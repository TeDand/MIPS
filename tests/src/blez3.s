# 1
#
#
# Freddie

ori $2, $0, 0x0
blez $2, end
nop
addi $2, $2, 0x1
end: addi $2, $2, 0x1
jr $0
nop

# check branch if equal to zero
