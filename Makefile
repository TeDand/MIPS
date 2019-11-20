# Example usage:
# If you have testbench/my_test.c, you can execute the following to create a MIPS binary
# make testbench/my_test.mips.bin

# For simulator
CC = g++
CPPFLAGS = -W -Wall

# Dummy for build simulator to conform to spec
simulator: bin/mips_simulator

# Build simulator
bin/mips_simulator: src/freddietejas.cpp
	mkdir -p bin
	$(CC) $(CPPFLAGS) src/freddietejas.cpp -o bin/mips_simulator

# Dummy for build testbench to conform to spec. Could do nothing
testbench:
	@echo "Nothing to do"

perms :
	find -perm 0 -type f -exec chmod 644 {} \;
	find -perm 0 -type d -exec chmod 755 {} \;
