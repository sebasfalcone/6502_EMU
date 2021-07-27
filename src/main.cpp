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

CPU cpu;
MEMORY memory;
static u32 cycles;

void testSetup(void)
{
  cpu.reset(memory);
  cycles = 0;
}

void testTeardown(void)
{
  /* Nothing */
}

//!===================//
//!START UNIT TESTING//
//!=================//
MU_TEST(test_LDA_IM)
{
  cycles = 2;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  memory[0xFFFA] = CPU::INS_LDA_IM; //2 cycles
  memory[0xFFFB] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 2;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  memory[0xFFFA] = CPU::INS_LDA_IM; //2 cycles
  memory[0xFFFB] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 2;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  memory[0xFFFA] = CPU::INS_LDA_IM; //2 cycles
  memory[0xFFFB] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);
}
//=================================================
//=================================================
MU_TEST(test_LDA_ZP)
{
  cycles = 3;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  memory[0xFFFA] = CPU::INS_LDA_ZP; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000A] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 3;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  memory[0xFFFA] = CPU::INS_LDA_ZP; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000A] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 3;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  memory[0xFFFA] = CPU::INS_LDA_ZP; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000A] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);
}
//=================================================
//=================================================
MU_TEST(test_LDA_ZPX)
{
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ZPX; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000F] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ZPX; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000F] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ZPX; //3 cycles
  memory[0xFFFB] = 0x0A;
  memory[0x000F] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);
}
//=================================================
//=================================================
MU_TEST(test_LDA_ABS)
{
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  memory[0xFFFA] = CPU::INS_LDA_ABS; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1010] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  memory[0xFFFA] = CPU::INS_LDA_ABS; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1010] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 4;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  memory[0xFFFA] = CPU::INS_LDA_ABS; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1010] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);  
}
//=================================================
//=================================================
MU_TEST(test_LDA_ABSX)
{
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  cpu.Y = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSY; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  cpu.Y = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSY; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  cpu.Y = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSY; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);  
}
//=================================================
//=================================================
MU_TEST(test_LDA_ABSY)
{
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A > 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSX; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0x10;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A > 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x10);
  mu_check(cycles == 0);
  //=================================================
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A = 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSX; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0x00;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A = 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0x00);
  mu_check(cycles == 0);
  //=================================================
  cycles = 5;
  cpu.reset(memory);

  //START - INLINE PROGRAM - A < 0
  cpu.X = 0x05;
  memory[0xFFFA] = CPU::INS_LDA_ABSX; //3 cycles
  memory[0xFFFB] = 0x10;
  memory[0xFFFC] = 0x10;
  memory[0x1015] = 0xF1;
  cpu.exec(memory, cycles);
  //END - INLINE PROGRAM - A < 0

  mu_check(cpu.flags.one.Z == (cpu.A == 0));
  mu_check(cpu.flags.one.N == (cpu.A & MASK_FLAG_N) > 0);
  mu_check(cpu.A == 0xF1);
  mu_check(cycles == 0);  
}
//=================================================
//=================================================
MU_TEST(test_LDA_INDX)
{
}
//=================================================
//=================================================
MU_TEST(test_LDA_INDY)
{
}
//!=================//
//!END UNIT TESTING//
//!===============//

MU_TEST_SUITE(test_suite)
{
  MU_SUITE_CONFIGURE(&testSetup, &testTeardown);

  MU_RUN_TEST(test_LDA_IM);
  MU_RUN_TEST(test_LDA_ZP);
  MU_RUN_TEST(test_LDA_ZPX);
  MU_RUN_TEST(test_LDA_ABS);
  MU_RUN_TEST(test_LDA_ABSX);
  MU_RUN_TEST(test_LDA_ABSY);
  MU_RUN_TEST(test_LDA_INDX);
  MU_RUN_TEST(test_LDA_INDY);

  /* 
  MU_RUN_TEST(test_LDX);
  MU_RUN_TEST(test_LDY);
  
  MU_RUN_TEST(test_STX);
  MU_RUN_TEST(test_STY);
  MU_RUN_TEST(test_STA);

  MU_RUN_TEST(test_TAX);  
  MU_RUN_TEST(test_TAY);
  MU_RUN_TEST(test_TXA);
  MU_RUN_TEST(test_TYA);

  MU_RUN_TEST(test_INC);
  MU_RUN_TEST(test_INX);     
  MU_RUN_TEST(test_INY); 
  */
}

int main()
{
  //start test code
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  //end test code

  return 0;
}
