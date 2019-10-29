#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;
const int WORD_LENGTH = 32;

void find_instr(const std::string &instr, uint32_t reg[]);
void addu(const std::string &instr, uint32_t reg[]);
void subu(const std::string &instr, uint32_t reg[]);
void mul(const std::string &instr, uint32_t reg[]);

int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);

  uint32_t registers[32] = {0};
  uint32_t pc = IMEM_OFFSET;

  char memblock[100];
  int sizeI = 0;

  std::ifstream binStream;

  try {

    binStream.open("assebly.mips.bin", std::ios::binary | std::ios::in);
    binStream.seekg (0, std::ios::end);
    std::streampos size = binStream.tellg();
    sizeI = size;
    binStream.seekg (0, std::ios::beg);
    while (!binStream.eof()) {
      binStream.read(memblock, size);
    }
    std::cerr << sizeI << std::endl;
    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }


  uint32_t instructions[sizeI/4] = {0};

  for (int i = 0; i < sizeI; i++){
    uint32_t tmp = static_cast<uint32_t>(memblock[i]) << 8 * (3-(i%4));
    instructions[i / 4] += tmp;
  }

  for (int i = 0; i < sizeI/4; i++) {
    std::cerr << std::bitset<32>(instructions[i]) << std::endl;
  }

  //find_instr(instructions[0], registers);

  // while (1) {
  //   uint32_t instruction;
  //   if (pc == 0) {
  //     std::cerr << "Program completed successfully" << std::endl;
  //     exit(registers[2]); //program executed properly and exited
  //   }
  //   else if (pc < IMEM_OFFSET + IMEM_LENGTH) {
  //     instruction = instr_mem[pc/4];
  //   }
  //   else {
  //     std::cerr << "Accessing out of range memory" << std::endl;
  //     exit(-11); //memory out of address range - exited with error
  //   }
  // }

  return 0;
}

void find_instr(const std::string &instr, uint32_t reg[]) {
  std::string opcode = instr.substr(0, 6);
  bool rType = 1, jType = 1;
  std::cerr << instr << std::endl;
  std::cerr << opcode << std::endl;
  for (int i = 0; i < 6; i++) {
    if (opcode[i] != '0') {
      rType = 0;
    }
    if ((i != 5) && (opcode[i] != 0)) {
      jType = 0;
    }
    if ((i == 5) && (opcode[i] != 1)) {
      jType = 0;
    }
  }

  std::cerr << rType << std::endl;
  std::cerr << jType << std::endl;

  if (rType) {
    std::string fn = instr.substr(26, 32);
    std::cerr << "R type" << std::endl;
    if (fn == "100001") {
      std::cerr << "addu" << std::endl;
      addu(instr, reg);
    }
  }
  else if (jType) {
    std::cout << "J type" << std::endl;
  }
  else { //if not r or j type then it must be a i type instruction
    std::cerr << "I type" << std::endl;
  }
}

void addu(const std::string &instr, uint32_t reg[]) {
  std::string dest = instr.substr(6, 5);
  std::string r1 = instr.substr(11, 5);
  std::string r2 = instr.substr(16, 5);
  // reg[10] = 8;
  // reg[11] = 4294967295;
  // reg[9] = 4294967295;
  reg[std::stoi(dest, nullptr, 2)] = reg[std::stoi(r1, nullptr, 2)] + reg[std::stoi(r2, nullptr, 2)];
  // std::cerr << std::stoi(dest, nullptr, 2) << " = " << std::stoi(r1, nullptr, 2) << " + " << std::stoi(r2, nullptr, 2) << std::endl;
  // std::cerr << reg[10] << std::endl;
}

void subu(const std::string &instr, uint32_t reg[]) {
  std::string dest = instr.substr(6, 5);
  std::string r1 = instr.substr(11, 5);
  std::string r2 = instr.substr(16, 5);
  // reg[10] = 8;
  // reg[11] = 0;
  // reg[9] = 4294967295;
  reg[std::stoi(dest, nullptr, 2)] = reg[std::stoi(r1, nullptr, 2)] - reg[std::stoi(r2, nullptr, 2)];
  // std::cerr << std::stoi(dest, nullptr, 2) << " = " << std::stoi(r1, nullptr, 2) << " - " << std::stoi(r2, nullptr, 2) << std::endl;
  // std::cerr << reg[10] << std::endl;
}

void mul(const std::string &instr, uint32_t reg[]) {
  std::string dest = instr.substr(6, 5);
  std::string r1 = instr.substr(11, 5);
  std::string r2 = instr.substr(16, 5);
  // reg[10] = 8;
  // reg[11] = 0;
  // reg[9] = 4294967295;
  reg[std::stoi(dest, nullptr, 2)] = reg[std::stoi(r1, nullptr, 2)] * reg[std::stoi(r2, nullptr, 2)];
  // std::cerr << std::stoi(dest, nullptr, 2) << " = " << std::stoi(r1, nullptr, 2) << " * " << std::stoi(r2, nullptr, 2) << std::endl;
  // std::cerr << reg[10] << std::endl;
}
