#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;
const int WORD_LENGTH = 32;

void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc,bool& isDelay);
void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t & pc);
void do_jType(const uint32_t &instr, uint32_t reg[],uint32_t& pc);
void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft);
int addu(const int &r1, const int &r2);
void subu(const std::string &instr, uint32_t reg[]);
void mul(const std::string &instr, uint32_t reg[]);
int jr(const uint32_t &r1);
uint32_t j(uint32_t index);


int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);
  //char* memblock = new char[sizeI];
  uint32_t registers[32] = {0};
  uint32_t pc = 0;

  char memblock[100];
  int sizeI = 0;

  std::ifstream binStream;
 
  try {
    binStream.open("simplJump.mips.bin", std::ios::binary | std::ios::in);
    
    binStream.seekg (0, std::ios::end);
    std::streampos size = binStream.tellg();
    sizeI = size;
    binStream.seekg (0, std::ios::beg);
     
    while (!binStream.eof()) {
      binStream.read(memblock, size);
    }
    binStream.close();
    binStream.clear();
    
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }
  std::cerr << "Got here"<<std::endl;
  for (int i = 0; i < sizeI; i++){
    uint32_t tmp = static_cast<uint32_t>(memblock[i]) << 8 * (3-(i%4));
    instr_mem[i / 4] += tmp;
  }

  /*for (int i = 0; i < sizeI/4; i++) {
    std::cerr << std::bitset<32>(instr_mem[i]) << std::endl;
  }*/
  registers[21] = 0x10000004;
  registers[20] = 0x10000008;
  registers[22] = 0x1000000C;

  bool isDelay = false; //true if executing next 
  uint32_t dealySlot;


  while (1) {
    std::string tBlock;
    std::cout << std::endl;
    std::cout << "Program counter = "<<std::hex << pc*4 + IMEM_OFFSET<<std::endl;
    std::cout << "Vector index = "<<std::hex << pc<<std::endl;
    std::cout << "And instruction is: " << std::bitset<32>(instr_mem[pc])<<std::endl;
    std::cout << std::endl;
    std::cin >> tBlock;

    if(isDelay && (pc +1 < sizeI/4)){
      isDelay = false;
      find_instr(instr_mem[pc], registers,pc,isDelay);
      if(isDelay){
         dealySlot = instr_mem[pc];
       }

      pc+=1;

    }
    else if(dealySlot !=0){
        find_instr(dealySlot, registers,pc, isDelay);
        dealySlot = 0;
        pc +=1;
    }
     else if (pc +1 < sizeI/4) {
       find_instr(instr_mem[pc], registers,pc, isDelay);
       if(isDelay){
         dealySlot = instr_mem[pc];
       }
       pc += 1;
     }
     else if(pc +1 == sizeI/4){
      std::cerr << "Program completed successfully" << std::endl;
      exit(registers[2]); //program executed properly and exited
     }
     else {
       std::cerr << "Accessing out of range memory" << std::endl;
       exit(-11); //memory out of address range - exited with error
     }
   }

   /*
    while (1) {
    
    if(isDelay && (pc +1 < sizeI/4)){
      isDelay = false;
      find_instr(instr_mem[pc], registers,pc,isDelay);
      if(isDelay){
         dealySlot = instr_mem[pc];
       }

      pc+=1;

    }
    else if(dealySlot !=0){
        find_instr(dealySlot, registers,pc, isDelay);
        dealySlot = 0;
        pc +=1; jo
    }
     else if (pc +1 < sizeI/4) {
       find_instr(instr_mem[pc], registers,pc, isDelay);
       if(isDelay){
         dealySlot = instr_mem[pc];
       }
       pc += 1;
     }
     else if(pc +1 == sizeI/4){
      std::cerr << "Program completed successfully" << std::endl;
      exit(registers[2]); //program executed properly and exited
     }
     else {
       std::cerr << "Accessing out of range memory" << std::endl;
       exit(-11); //memory out of address range - exited with error
     }
   }
   */

  return 0;
}


// so if branch -> jump goes into delaySlot -> execute next -> execute delaySlot
void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc,bool& isDelay) {

  uint32_t opcode = instr & 0b11111100000000000000000000000000;
  if(instr == 0){

  }
  else if (opcode == 0b00000000000000000000000000000000) {
    do_rType(instr, reg,pc);
  }
  else if ((opcode == 0b00001100000000000000000000000000) || (opcode == 0b00001000000000000000000000000000)) {
    std::cerr<< "ITS A J"<< std::endl;
    if(isDelay){
      do_jType(instr,reg,pc);
      isDelay = false;
    }
    else{
      isDelay = true;
    }
      
      
    
  }
  else{
    std::cerr<<"itype"<<std::endl;
  }

}

void do_jType(const uint32_t &instr, uint32_t reg[],uint32_t & pc) {
  
  std::cerr << "Do J Type" << std::endl;
  uint32_t jType = instr >> 26;
  if(jType == 0b10){
    pc = j(instr && 0X3FFFFFF) - (IMEM_OFFSET/4);
  }
  else{
    std::cerr<<"No implemented instruction!!"<<std::endl;
  }
  
}

void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t & pc) {
  int r1, r2, dest, shft;
  uint32_t fn = instr & 0b00000000000000000000000000111111;

  decode_rType(instr, r1, r2, dest, shft);

  if (fn == 0b00000000000000000000000000100000) {
    std::cerr << "addu" << std::endl;
    reg[dest] = addu(reg[r1], reg[r2]);
  }
  else if(fn == 0b1000){
    pc = jr(reg[r1]);
  }

}


void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft) {
  uint32_t mask = 0b00000000000000000000000000011111;

  shft = (instr >> 6) & mask;

  dest = (instr >> 11) & mask;

  r2 = (instr >> 16) & mask;

  r1 = (instr >> 21) & mask;

}

int addu(const int &r1, const int &r2) {
  return r1 + r2;
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

int jr(const uint32_t &r1){
    //000000 rs 0x15 001000
    int target  = r1 - IMEM_OFFSET;
    //target is the memory location - the instruction memory offset
    if(target < 0 || target > IMEM_LENGTH){
      std::cerr << "This should be an error message? This test has failed!!"<<std::endl;
    }
    else{
      return (target/4);
    }
    std::cerr << "This is the target reg: "<< std::bitset<32>(target)<<std::endl;
 

}

uint32_t j(uint32_t index){
  //opcode index
  //what about branch branch add
  // aslo check this uses the delay slot
  return index;
}
