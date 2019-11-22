# 1
#
#
# Tejas

ori $2, $0, 0x0007
beq $0, $2, end
nop
ori $2, $0, 0x01
end: jr $0
nop

# check doesnt branch when not equal
