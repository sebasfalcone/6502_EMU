/**
 * @file main.cpp
 * @author Falcone Sebastian
 * @brief Emulador de micro 6502
 * @version 0.1
 * @date 2021-05-06
 * 
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "micro6502.h"

int main()
{
  MEMORY memory;
  CPU cpu;

  cpu.reset(memory);

  //start inline program
  memory[0xFFFC] = CPU::INS_JSR;
  memory[0xFFFD] = 0x42;
  memory[0xFFFE] = 0x42;
  memory[0x4242] = CPU::INS_LDA_IM;
  memory[0x4243] = 0x84;
  //end inline program

  cpu.exec(memory, 9);

  return 0;
}