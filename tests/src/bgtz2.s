# 1
#
#
# Freddie

ori $2, $0, 0x0007
lui $3, 0xF000
bgtz $3, end
nop
ori $2, $0, 0x01
end: jr $0
nop

# check no branch when -ve
