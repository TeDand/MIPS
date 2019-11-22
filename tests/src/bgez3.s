# 7
#
#
# Tejas

ori $3, $0, 0x0
ori $2, $0, 0x7
bgez $3, end
nop
ori $2, $0, 0x00dd
end: jr $0
nop

# check if equal to zero
