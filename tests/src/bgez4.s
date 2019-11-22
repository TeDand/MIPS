# 221
#
#
# Tejas

ori $2, $0, 0x0007
bgez $2, end
ori $2, $0, 0x00dd
end: jr $0
nop

# branch delay slot
