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

Byte MEMORY::fetchByte(u32 &cycles, Word &PC)
{
  Byte value = data[PC++];
  cycles--;
  return value;
}

Byte MEMORY::readByte(u32 &cycles, Word address)
{
  Byte value = data[address];
  cycles--;
  return value;
}

Word MEMORY::fetchWord(u32 &cycles, Word &PC)
{
  // 6502 uses little endian
  Word value = data[PC++];
  value |= (data[PC++] << 8);

  //Expandir el uso de big endian en un futuro

  cycles -= 2;
  return value;
}

Word MEMORY::readWord(u32 &cycles, Word address)
{
  Word value = data[address++];
  value |= (data[address] << 8);

  cycles -= 2;
  return value;
}

//========//
//  CPU   //
//========//
void CPU::reset(MEMORY &memory)
{
  PC = 0xFFFA; //Memory addres where the program start
  SP = 0x00FF; //Start of stack pointer

  //flags
  D = Z = I = D = B = V = N = 0;

  //registros
  A = X = Y = 0;

  memory.init();
}

//Byte related//
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

//-------------//
//FLAG UPDATES//
//-----------//

//LOAD SET STATUS//
void CPU::loadSetStatus(const Byte reg)
{
  Z = (reg == 0);
  N = (reg & 0b1000000) > 0;
}

//INCREMENT SET STATUS//
void CPU::incrementSetStatus(const Byte reg)
{
  Z = (reg == 0);
  N = (reg & 0b1000000) > 0;
}

//TRANSFET SET STATUS
void CPU::transferSetStatus(const Byte reg)
{
  Z = (reg == 0);
  N = (reg & 0b1000000) > 0;
}

void CPU::cmpSetStatus(const Byte test)
{
  if (test == 0)
  {
    Z = 1;
    C = 1;
  }
  else if (test > 0)
  {
    Z = 0;
    C = 1;
  }
  else
  {
    Z = 0;
    C = 0;
  }

  N = (test & 0b1000000) > 0;
}

void CPU::exec(MEMORY &memory, u32 &cycles)
{
  while (cycles)
  {
    Byte instruction = memory.fetchByte(cycles, PC); //fetch next instruction from memory

    switch (instruction)
    {
    //----------------//
    //Load Acumulator//
    //--------------//
    case INS_LDA_IM:
    {
      Byte value = memory.fetchByte(cycles, PC);

      writeRegister(A, value);

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      writeRegister(A, memory.readByte(cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ZPX:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(X);
      cycles--;

      writeRegister(A, memory.readByte(cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      writeRegister(A, memory.readByte(cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABSX:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(X);
      cycles--;

      writeRegister(A, memory.readByte(cycles, addr));

      loadSetStatus(A);
    }
    break;

    case INS_LDA_ABSY:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(Y);
      cycles--;

      writeRegister(A, memory.readByte(cycles, addr));

      loadSetStatus(A);
    }
    break;

    //-----------------//
    //Load Y Register //
    //---------------//
    case INS_LDY_IM:
    {
      Byte value = memory.fetchByte(cycles, PC);

      writeRegister(Y, value);

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      writeRegister(Y, memory.readByte(cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ZPX:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(X);
      cycles--;

      writeRegister(Y, memory.readByte(cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      writeRegister(Y, memory.readByte(cycles, addr));

      loadSetStatus(Y);
    }
    break;

    case INS_LDY_ABSX:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(X);
      cycles--;

      writeRegister(Y, memory.readByte(cycles, addr));

      loadSetStatus(Y);
    }
    break;

    //-----------------//
    //Load Y Register //
    //---------------//
    case INS_LDX_IM:
    {
      Byte value = memory.fetchByte(cycles, PC);

      writeRegister(X, value);

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      writeRegister(X, memory.readByte(cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ZPY:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(Y);
      cycles--;

      writeRegister(X, memory.readByte(cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      writeRegister(X, memory.readByte(cycles, addr));

      loadSetStatus(X);
    }
    break;

    case INS_LDX_ABSY:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(Y);
      cycles--;

      writeRegister(X, memory.readByte(cycles, addr));

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
      Byte addr = memory.fetchByte(cycles, PC);

      memory.writeByte(cycles, addr, memory.readByte(cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ZPX:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, memory.readByte(cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      memory.writeByte(cycles, addr, memory.readByte(cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    case INS_INC_ABSX:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, memory.readByte(cycles, addr) + 1);

      incrementSetStatus(memory[addr]);
    }
    break;

    //---------------//
    //Jump Subrutine//
    //-------------//
    case INS_JSR:
    {
      Word subAddr = memory.fetchWord(cycles, PC);

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
      Byte addr = memory.fetchByte(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ZPX:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABSX:
    {
      Word addr = memory.fetchWord(cycles, PC);

      addr += readRegister(X);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(A));
    }
    break;

    case INS_STA_ABSY:
    {
      Word addr = memory.fetchWord(cycles, PC);

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

    //----------//
    //Compare X//
    //--------//
    case INS_CPX_IM:
    {
      Byte addr = memory.readByte(cycles, PC);

      cmpSetStatus(X - memory[addr]);
    }
    break;

    case INS_CPX_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      cmpSetStatus(X - memory[addr]);
    }
    break;

    case INS_CPX_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      cmpSetStatus(X - memory[addr]);
    }
    break;

    //-----------------//
    //Store X register//
    //---------------//
    //STX DONT AFFECT FLAGS
    case INS_STX_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;

    case INS_STX_ZPY:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(Y);
      cycles--;

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;

    case INS_STX_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(X));
    }
    break;
    //-----------------//
    //Store Y register//
    //---------------//
    //STY DONT AFFECT FLAGS
    case INS_STY_ZP:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(Y));
    }
    break;

    case INS_STY_ZPX:
    {
      Byte addr = memory.fetchByte(cycles, PC);

      addr += readRegister(X);

      memory.writeByte(cycles, addr, readRegister(Y));
    }
    break;

    case INS_STY_ABS:
    {
      Word addr = memory.fetchWord(cycles, PC);

      memory.writeByte(cycles, addr, readRegister(Y));
    }
    break;

    //----------------//
    //Transfer Y to A//
    //--------------//
    case INS_TYA:
    {
      A = readRegister(Y);
      cycles--;

      transferSetStatus(A);
    }
    break;

    //----------------//
    //Transfer X to A//
    //--------------//
    case INS_TXA:
    {
      A = readRegister(X);
      cycles--;

      transferSetStatus(A);
    }
    break;

    //----------------//
    //Transfer A to X//
    //--------------//
    case INS_TAX:
    {
      X = readRegister(A);
      cycles--;

      transferSetStatus(X);
    }
    break;

    //----------------//
    //Transfer A to Y//
    //--------------//
    case INS_TAY:
    {
      Y = readRegister(A);
      cycles--;

      transferSetStatus(Y);
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