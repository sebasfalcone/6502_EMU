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

#define loadValue1 0xA0
#define loadValue2 0x0C

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

MU_TEST(test_incx)
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

MU_TEST(test_incy)
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
  CPU cpu;
  MEMORY memory;

  cpu.reset(memory);

  //start inline program Load instruction set
  memory[0x000A] = 0x03;

  memory[0xFFFA] = CPU::INS_LDX_ZP; //3 cycles
  memory[0xFFFB] = 0x09;
  memory[0x0009] = 0x01;

  memory[0xFFFC] = CPU::INS_CPX_ZP; //3 cycles
  memory[0xFFFD] = 0X0A;
  //end inline program Load instruction set

  u32 cycles = CYCLE_AMOUNT;
  cpu.exec(memory, cycles);

  //start test code
  testCycles = cycles;
  testCpu = cpu;

  MU_RUN_TEST(test_jsr);
  MU_RUN_TEST(test_ldy);
  MU_RUN_TEST(test_ldx);
  MU_RUN_TEST(test_incx);
  MU_RUN_TEST(test_incy);

  MU_RUN_TEST(test_cycles);
  MU_REPORT();
  //end test code

  return MU_EXIT_CODE;
}