#include <stdlib.h>
#include <stdio.h>

#include "micro6502.h"
//=========//
//  Memory //
//=========//
void MEMORY::init()
{
  for (u32 i = 0; i < MAX_MEM; i++)
    data[i] = 0;
}

Byte &MEMORY::operator[](u32 addr)
{
  //See if addr < MAX_MEX
  return data[addr];
}

void MEMORY::writeWord(u32 &cycles, u32 address, Word data)
{
  this->data[address] = data & 0xFF;
  this->data[address + 1] = (data >> 8);

  cycles -= 2;
}

//========//
//  CPU   //
//========//
void CPU::reset(MEMORY &memory)
{
  PC = 0xFFFC;
  SP = 0x0100;

  //flags
  D = Z = I = D = B = V = N = 0;

  //registros
  A = X = Y = 0;

  memory.init();
}
//============//
//Byte related//
//============//
Byte CPU::fetchByte(MEMORY &memory, u32 &cycles)
{
  Byte data = memory[PC++];
  cycles--;
  return data;
}

Byte CPU::readByte(MEMORY &memory, u32 &cycles, Byte address)
{
  Byte data = memory[address];
  cycles--;
  return data;
}

//============//
//Word related//
//============//
Word CPU::fetchWord(MEMORY &memory, u32 &cycles)
{
  // 6502 uses little endian
  Word data = memory[PC++];
  data |= (memory[PC++] << 8);

  //Expandir el uso de big endian en un futuro

  cycles -= 2;
  return data;
}

void CPU::LDAsetStatus()
{
  Z = (A == 0);
  N = (A & 0b1000000) > 0;
}

void CPU::exec(MEMORY &memory, u32 cycles)
{
  while (cycles)
  {
    Byte instruction = fetchByte(memory, cycles); //fetch next instruction from memory

    switch (instruction)
    {
    case INS_LDA_IM:
    {
      Byte value = fetchByte(memory, cycles);

      A = value;
      LDAsetStatus();
    }
    break;

    case INS_LDA_ZP:
    {
      Byte zeroPageAddr = fetchByte(memory, cycles);

      A = readByte(memory, cycles, zeroPageAddr);
      LDAsetStatus();
    }
    break;

    case INS_LDA_ZPX:
    {
      Byte zeroPageAddr = fetchByte(memory, cycles);

      zeroPageAddr += X;
      cycles--;

      A = readByte(memory, cycles, zeroPageAddr);
      LDAsetStatus();
    }
    break;

    case INS_JSR:
    {
      Word subAddr = fetchWord(memory, cycles);

      memory.writeWord(cycles, PC - 1, SP);

      ++PC = subAddr;
      cycles--;
    }
    break;

    default:
    {
      printf("Instruction not hanlded &d", instruction);
    }
    break;
    }
  }
}