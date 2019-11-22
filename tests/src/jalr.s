# 4

lui $3, 0x1000
<<<<<<< HEAD
addi $3, $3, 0x0020
jalr $4, $3
addi $2, $2, 0x0001
=======
addi $3, 0x0020
jalr add, $4
addi $2, 0x0001
>>>>>>> d1671fe5ee43d7d0bae9bc66b3efebf8d30954c1
end: jr $0
nop
add: addi $2, $2, 0x0003
jr $4

# basic functionality
