# 1

lui $2, 0xF000
ori $2, $2, 0x0007
bgezal $2, end
nop
ori $2, $0, 0x001
end: jr $0
nop

# check when less that zero
