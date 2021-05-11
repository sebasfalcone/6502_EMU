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

void MEMORY::writeByte(u32 &cycles, u32 address, Byte data)
{
  this->data[address] = data & 0xFF;

  cycles--;
}

//========//
//  CPU   //
//========//
void CPU::reset(MEMORY &memory)
{
  PC = 0xFFF0; //Memory addres where the program start
  SP = 0x00FF; //Start of stack pointer

  //flags
  D = Z = I = D = B = V = N = 0;

  //registros
  A = X = Y = 0;

  memory.init();
}
//============//
//Byte related//
//============//
Byte CPU::fetchByteMemory(MEMORY &memory, u32 &cycles)
{
  Byte data = memory[PC++];
  cycles--;
  return data;
}

Byte CPU::readByteMemory(MEMORY &memory, u32 &cycles, Word address)
{
  Byte data = memory[address];
  cycles--;
  return data;
}

Byte CPU::fetchRegister(u32 &cycles, Byte &reg)
{
  Byte data = reg;

  PC++;
  cycles--;
  return data;
}

Byte CPU::readRegister(Byte &reg)
{
  Byte data = reg;
  return data;
}

void CPU::writeRegister(Byte &reg, Byte data)
{
  reg = data;
}

//============//
//Word related//
//============//
Word CPU::fetchWordMemory(MEMORY &memory, u32 &cycles)
{
  // 6502 uses little endian
  Word data = memory[PC++];
  data |= (memory[PC++] << 8);

  //Expandir el uso de big endian en un futuro

  cycles -= 2;
  return data;
}

Word CPU::readWordMemory(MEMORY &memory, u32 &cycles, Word address)
{
  Word data = memory[address++];
  data |= (memory[address] << 8);

  cycles -= 2;
  return data;
}

//LOAD SET STATUS//
void CPU::loadSetStatus(Byte reg)
{
  Z = (reg == 0);
  N = (reg & 0b1000000) > 0;
}

//INCREMENT SET STATUS//
void CPU::incrementSetStatus(Byte reg)
{
  Z = (reg == 0);
  N = (reg & 0b1000000) > 0;
}

void CPU::exec(MEMORY &memory, u32 &cycles)
{
  while (cycles)
  {
    Byte instruction = fetchByteMemory(memory, cycles); //fetch next instruction from memory

    switch (instruction)
    {
    //----------------//
    //Load Acumulator//
    //--------------//
    case INS_LDA_IM:
    {
      Byte value = fetchByteMemory(memory, cycles);

      writeRegister(A, value);

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      writeRegister(A, readByteMemory(memory, cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ZPX:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      writeRegister(A, readByteMemory(memory, cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      writeRegister(A, readByteMemory(memory, cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABSX:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      writeRegister(A, readByteMemory(memory, cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABSY:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(Y);
      cycles--;

      writeRegister(A, readByteMemory(memory, cycles, addr));

      loadSetStatus(A);
    }
    break;

    //-----------------//
    //Load Y Register //
    //---------------//
    case INS_LDY_IM:
    {
      Byte value = fetchByteMemory(memory, cycles);

      writeRegister(Y, value);

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      writeRegister(Y, readByteMemory(memory, cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ZPX:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      writeRegister(Y, readByteMemory(memory, cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      writeRegister(Y, readByteMemory(memory, cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ABSX:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      writeRegister(Y, readByteMemory(memory, cycles, addr));

      loadSetStatus(Y);
    }
    break;

    //-----------------//
    //Load Y Register //
    //---------------//
    case INS_LDX_IM:
    {
      Byte value = fetchByteMemory(memory, cycles);

      writeRegister(X, value);

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      writeRegister(X, readByteMemory(memory, cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ZPY:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(Y);
      cycles--;

      writeRegister(X, readByteMemory(memory, cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      writeRegister(X, readByteMemory(memory, cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ABSY:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(Y);
      cycles--;

      writeRegister(X, readByteMemory(memory, cycles, addr));

      loadSetStatus(X);
    }
    break;

    //---------------------//
    //Increment X register//
    //-------------------//
    case INS_INX:
    {
      writeRegister(X, fetchRegister(cycles, X) + 1);

      incrementSetStatus(X);
    }
    break;

    //---------------------//
    //Increment Y register//
    //-------------------//
    case INS_INY:
    {
      writeRegister(Y, fetchRegister(cycles, Y) + 1);

      incrementSetStatus(Y);
    }
    break;

    //-----------.-----//
    //Increment Memory//
    //---------------//
    case INS_INC_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      memory.writeByte(cycles, addr, readByteMemory(memory, cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ZPX:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readByteMemory(memory, cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      memory.writeByte(cycles, addr, readByteMemory(memory, cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ABSX:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readByteMemory(memory, cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    //---------------//
    //Jump Subrutine//
    //-------------//
    case INS_JSR:
    {
      Word subAddr = fetchWordMemory(memory, cycles);

      memory.writeWord(cycles, PC - 1, SP);

      PC = subAddr;
      PC++;
      cycles--;
    }

    //-----------------//
    //Store Acumulator//
    //---------------//
    //STA DONT AFFECT FLAGS
    case INS_STA_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ZPX:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABSX:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABSY:
    {
      Word addr = fetchWordMemory(memory, cycles);

      addr += readRegister(Y);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_INDX:
    {
    }
    break;

    case INS_STA_INDY:
    {
    }
    break;

    //-----------------//
    //Store X register//
    //---------------//
    //STX DONT AFFECT FLAGS
    case INS_STX_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;

    case INS_STX_ZPY:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(Y);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;

    case INS_STX_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;
    //-----------------//
    //Store Y register//
    //---------------//
    //STY DONT AFFECT FLAGS
    case INS_STY_ZP:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(Y));
    }
    break;

    case INS_STY_ZPX:
    {
      Byte addr = fetchByteMemory(memory, cycles);

      addr += readRegister(X);

      memory.writeByte(cycles, addr, readRegister(Y));
    }
    break;

    case INS_STY_ABS:
    {
      Word addr = fetchWordMemory(memory, cycles);

      memory.writeByte(cycles, addr, readRegister(Y));
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