# 11

.set noreorder
ori $2, $0, 0x0007
j add
addi $2, $2, 0x0001
end: jr $0
nop
add: addi $2, $2, 0x0003
j end

# basic functionality
