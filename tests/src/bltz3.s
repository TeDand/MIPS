# 221

lui $2, 0x00ff
sub $3, $0, $2
bltz $3, end
ori $2, $0, 0x00dd
end: jr $0
nop

# branch delay slot
