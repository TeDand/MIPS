/*#include <iostream>
#include <fstream>

using namespace std;

int main () {
  streampos size;
  streampos fPoint = 0;
  char * memblock;
  ifstream file ("file.bin", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << *(memblock+32) <<endl;

    delete[] memblock;
  }
  else cout << "Unable to open file";
  return 0;
}*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;

int main(int argc, char *argv[]) {

  char memblock[100];
  int sizeI =0;
  /*std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);

  std::vector<uint32_t> registers = {0};
  uint32_t pc = IMEM_OFFSET;

  std::string binFile = argv[1];*/

  std::ifstream binStream;

  try {
    
    binStream.open("file.bin", std::ios::binary | std::ios::in);
    binStream.seekg (0, std::ios::end);
    std::streampos size = binStream.tellg();
    sizeI = size;
    //memblock = new char [size];
    binStream.seekg (0, std::ios::beg);
    while (!binStream.eof()) {
      binStream.read(memblock, size);

    }
    std::cout<<sizeI<<std::endl;
    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }

  int wordlen = 32;
  int index = 0;
  int aIn = 0;
  int memArray[50];
  int curWord = 0;

  for (int i; i < sizeI; i++){
    int bit = memblock[i] - '0';
    if(index == wordlen){
      memArray[aIn] = curWord;
      aIn++;
      curWord = bit;
      index = 0;

    }
    else{
      curWord = curWord << 1;
      curWord = curWord | bit;
    }
    index++;
  }
  std::cout<<std::endl;
  for (int i = 0; i < aIn; i++){
    for int(j = 0; j <32)
  }


  //binStream.seek(seek_end);
  //binSize.tell();
/*
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
*/
//delete[] memblock;

  return 0;
}
