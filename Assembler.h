#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "StringFunctions.h"

#define DEF_CMD(name, num, arg, code)\
  CMD_##name = num,

enum Commands {
  #include "commands.h"
};

#undef DEF_CMD

struct FileHeader {
  char offset;
  char name;
  char surname;
  char version;
};

struct Label {
  char* label_name;
  size_t addr;
};

struct LabelArr {
  Label* labels;
  size_t numb_of_lab;
};

void Translation(const char* input, const char* output);
void TranslationAsmToByte(const Text* asmbly, Text* byte, LabelArr* label_arr);

size_t ArgChek(String* string, Text* byte, size_t ofs, LabelArr* label_arr);

enum Arguments {
  MEMORY   = 0x1,
  REGISTER = 0x2,
  IMMED    = 0x4,
  LABEL    = 0x8
};

bool IsNumber(char* str);
bool IsRegister(char* str);
bool IsLabel(char* str);

size_t WriteNumber(Text* byte, char* token, size_t ofs);
size_t WriteRegister(Text* byte, char* token, size_t ofs);
size_t WriteLabel(Text* byte, char* token, size_t ofs, LabelArr* label_arr);

size_t GetLabel(LabelArr* label_arr, char* label);
void AddLabel(LabelArr* label_arr, char* label_name, size_t pos);
