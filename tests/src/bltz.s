# 0

lui $2, 0x00ff
sub $3, $0, $2
bltz $3, end
nop
end: jr $0
nop

# basic functionality
