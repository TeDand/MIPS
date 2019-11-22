# 215
#
#
# Freddie

ori $2, $0, 0x0007
bne $0, $2, end
ori $2, $0, 0x00d7
end: jr $0
nop

# branch delay slot
