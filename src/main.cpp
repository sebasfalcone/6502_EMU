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
#include "minunit.h"

//Cycles needed for the program to execute
#define CYCLE_AMOUNT 5

#define loadValue 0x41

#define LDA
//#define JSR

//-------------------//
//START UNIT TESTING//
//-----------------//
u32 testCycles = 0;
MEMORY testMemory = {};
CPU testCpu = {};

MU_TEST(test_cycles)
{
  mu_check(testCycles == 0);
}

MU_TEST(test_lda)
{
  if (testCpu.A == 0)
    mu_check(testCpu.Z == 1);
  else
    mu_check(testCpu.Z == 0);

  if ((testCpu.A & 0b1000000) > 0)
    mu_check(testCpu.N == 1);
  else
    mu_check(testCpu.N == 0);
}

MU_TEST(test_ldx)
{
  if (testCpu.X == 0)
    mu_check(testCpu.Z == 1);
  else
    mu_check(testCpu.Z == 0);

  if ((testCpu.X & 0b1000000) > 0)
    mu_check(testCpu.N == 1);
  else
    mu_check(testCpu.N == 0);
}

MU_TEST(test_ldy)
{
  if (testCpu.Y == 0)
    mu_check(testCpu.Z == 1);
  else
    mu_check(testCpu.Z == 0);

  if ((testCpu.Y & 0b1000000) > 0)
    mu_check(testCpu.N == 1);
  else
    mu_check(testCpu.N == 0);
}

MU_TEST(test_jsr)
{
}

//-----------------//
//END UNIT TESTING//
//---------------//

int main()
{
  MEMORY memory;
  CPU cpu;

  cpu.reset(memory);

#ifdef JSR
  //start inline program Jump Sub Rutine
  memory[0xFFFC] = CPU::INS_JSR;
  memory[0xFFFD] = 0x42;
  memory[0xFFFE] = 0x42;
  memory[0x4242] = CPU::INS_LDA_IM;
  memory[0x4243] = 0x84;
//end inline program Jump Sub Rutine
#endif
#ifdef LDA
  //start inline program Load Acumulator Zero page
  cpu.X = 0x01;
  memory[0xFFFC] = CPU::INS_LDY_ABSX;
  memory[0xFFFD] = 0x08;
  memory[0xFFFE] = 0x00;
  memory[0x0009] = loadValue;
//end inline program Load Acumulator Zero page
#endif

  u32 cycles = CYCLE_AMOUNT;
  cpu.exec(memory, cycles);

  //start test code
  testCycles = cycles;
  testMemory = memory;
  testCpu = cpu;

#ifdef JSR
  MU_RUN_TEST(test_jsr);
#endif
#ifdef LDA
  MU_RUN_TEST(test_ldy);
#endif

  MU_RUN_TEST(test_cycles);
  MU_REPORT();
  //end test code

  return MU_EXIT_CODE;
}