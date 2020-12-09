DEF_CMD(in, 11, 0,
  do {
    Elem_t value;
    scanf("%lg", &value);
    StackPush(&cpu -> stk, value);
  } while(false);)

DEF_CMD(out, 12, 0,
  do {
    Elem_t value = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    printf("%lg\n", value);
  } while(false);)

DEF_CMD(push, 13, 1,
  do {
    cpu -> pc++;
    char args = cpu -> code[cpu -> pc];
    cpu -> pc++;

    double value = 0;
    if (IMMED & args) {
      value += *(double*)(cpu -> code + cpu -> pc);
      cpu -> pc += sizeof(double);
    }
    if (REGISTER & args) {
      value += cpu -> registers[cpu -> code[cpu -> pc]];
      cpu -> pc++;
    }
    if (MEMORY & args) {
      StackPush(&cpu -> stk, *(double*)(cpu -> ram + (unsigned int)value));
    } else {
      StackPush(&cpu -> stk, value);
    }

    cpu -> pc--;
  } while(false);)

DEF_CMD(pop, 14, 1,
  do {
    cpu -> pc++;
    char args = cpu -> code[cpu -> pc];
    cpu -> pc++;

    if (MEMORY & args) {
      int value = 0;
      if (IMMED & args) {
        value += *(double*)(cpu -> code + cpu -> pc);
        cpu -> pc += sizeof(double);
      }
      if (REGISTER & args) {
        value += cpu -> registers[cpu -> code[cpu -> pc]];
        cpu -> pc++;
      }
      if (value >= vram_begin && value <= vram_end) {
        cpu -> ram[value] = StackTop(&cpu -> stk);
      } else {
        *(double*)(cpu -> ram + value) = StackTop(&cpu -> stk);
      }
    } else {
      cpu -> registers[cpu -> code[cpu -> pc]] = StackTop(&cpu -> stk);
    }

    StackPop(&cpu -> stk);
    cpu -> pc--;
  } while(false);)

DEF_CMD(add, 15, 0,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, arg1 + arg2);
  }while(false);)

DEF_CMD(sub, 16, 0,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, arg2 - arg1);
  } while(false);)

DEF_CMD(mul, 17, 0,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, arg1 * arg2);
  } while(false);)

DEF_CMD(div, 18, 0,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, arg2 / arg1);
  } while(false);)

DEF_CMD(sin, 19, 0,
  do {
    Elem_t arg = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, sin(arg));
  } while(false);)

DEF_CMD(cos, 20, 0,
  do {
    Elem_t arg = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, cos(arg));
  } while(false);)

DEF_CMD(sqrt, 21, 0,
  {
    Elem_t arg = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, sqrt(arg));
  })

DEF_CMD(pow, 22, 0,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    StackPush(&cpu -> stk, pow(arg2, arg1));
  } while(false);)

DEF_CMD(hlt, 23, 0,
  do {
    cpu -> run = false;
  } while(false);)

DEF_CMD(ja, 24, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (arg2 > arg1) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu ->pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(jae, 25, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (arg2 >= arg1) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu ->pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(jb, 26, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (arg2 < arg1) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu ->pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(jbe, 27, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (arg2 <= arg1) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu ->pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(je, 28, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (fabs(arg2 - arg1) < 0.0000001) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu ->pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(jne, 29, 1,
  do {
    Elem_t arg1 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    Elem_t arg2 = StackTop(&cpu -> stk);
    StackPop(&cpu -> stk);
    if (fabs(arg2 - arg1) >= 0.0000001) {
      cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
    } else {
      cpu -> pc += sizeof(size_t) + 1;
    }
  } while(false);)

DEF_CMD(jmp, 30, 1,
   do {
     cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
  } while(false);)

DEF_CMD(call, 31, 1,
   do {
     StackPush(&cpu -> call_stk, cpu -> pc + 1 + sizeof(size_t));
     cpu -> pc = *(size_t*)(cpu -> code + cpu -> pc + 2);
   } while(false);)

DEF_CMD(ret, 32, 0,
   do {
     cpu -> pc = (size_t)StackTop(&cpu -> call_stk);
     StackPop(&cpu -> call_stk);
   } while(false);)

DEF_CMD(rnd, 33, 0,
   do {
     Elem_t tmp = StackTop(&cpu -> stk);
     StackPop(&cpu -> stk);
     StackPush(&cpu -> stk, floor(tmp));
   } while(false);)
