#include <string>
#include <fstream>
#include <iostream>
#include <vector>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;

int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);

  std::vector<uint32_t> registers = {0};
  uint32_t pc = IMEM_OFFSET;

  std::string binFile = argv[1];

  std::ifstream binStream;

  try {
    binStream.open(argv[1], std::ios::binary | std::ios::in);
    int begin = binStream.tellg();
    binStream.seekg (0, std::ios::end);
    int end = binStream.tellg();
    int binSize = begin - end;
    while (!binStream.eof()) {
      binStream.read(instr_mem, binSize);
      //binStream.read(&tmp[0], binSize);
    }

    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }

  //binStream.seek(seek_end);
  //binSize.tell();

  while (1) {
    uint32_t instruction;
    if (pc == 0) {
      std::cerr << "Program completed successfully" << std::endl;
      exit(registers[2]); //program executed properly and exited
    }
    else if (pc < IMEM_OFFSET + IMEM_LENGTH) {
      instruction = instr_mem[pc/4];
    }
    else {
      std::cerr << "Accessing out of range memory" << std::endl;
      exit(-11); //memory out of address range - exited with error
    }
  }

  return 0;
}
