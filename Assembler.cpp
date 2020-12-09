#include "Assembler.h"

#define dbg printf("%d\n", __LINE__);

void Translation(const char* input, const char* output) {
  Text asmbly = {};
  ReadFile(input, &asmbly);
  SplitToStrings(&asmbly);

  Text byte = {};
  LabelArr label_arr = {};
  label_arr.labels = (Label*)calloc(asmbly.size, sizeof(Label));

  TranslationAsmToByte(&asmbly, &byte, &label_arr);
  TranslationAsmToByte(&asmbly, &byte, &label_arr);

  WriteBinFile(output, &byte);
}

void CreateHeader(FileHeader* header) {
  header -> offset = 4;
  header -> name = 'B';
  header -> surname = 'D';
  header -> version = '1';
}

void TranslationAsmToByte(const Text* asmbly, Text* byte, LabelArr* label_arr) {
  assert(asmbly);
  assert(byte);

  byte -> buf = (char*)calloc((asmbly -> size) * 2, sizeof(char));
  size_t ofs = 0;

  FileHeader header = {};
  CreateHeader(&header);

  *(FileHeader*)(byte -> buf + ofs) = header;
  ofs += sizeof(FileHeader);

  #define DEF_CMD(name, num, arg, code)                             \
    if (strcmp(#name, asmbly -> strings[i].tokens[0]) == 0) {       \
      byte -> buf[ofs++] = num;                                     \
      if (arg > 0) {                                                \
        ofs = ArgChek(&asmbly -> strings[i], byte, ofs, label_arr); \
      }                                                             \
    } else

  for (size_t i = 0; i < asmbly -> numb_of_strings; i++) {
    #include "commands.h"
    /*else*/ if (IsLabel(asmbly -> strings[i].tokens[0])) {
      AddLabel(label_arr, asmbly -> strings[i].tokens[0], ofs - 1);
    } else {
      if (asmbly -> strings[i].tokens[0]) {
        printf("Syntax error \"%s\" on line %lu\n", asmbly -> strings[i].tokens[0], i);
      }
    }
  }

  #undef DEF_CMD

  byte -> size = ofs;
}

size_t ArgChek(String* string, Text* byte, size_t ofs, LabelArr* label_arr) {
  char* token = string -> tokens[0];
  size_t control_ofs = ofs++;

  byte -> buf[control_ofs] = 0;
  if(strcmp(token, "push") == 0 || strcmp(token, "pop") == 0) {
    token = string -> tokens[1];

    if (token[0] == '[') {
      byte -> buf[control_ofs] += MEMORY;
      token++;
    }
    if (token[strlen(token) - 1] == ']') {
      token[strlen(token) - 1] = '\0';
    }

    if (IsRegister(token)) {
      byte -> buf[control_ofs] += REGISTER;
      ofs = WriteRegister(byte, token, ofs);
    }

    if (IsNumber(token)) {
      byte -> buf[control_ofs] += IMMED;
      ofs = WriteNumber(byte, token, ofs);

      if (string -> numb_of_tokens > 2) {
        token = string -> tokens[2];
        if (token[strlen(token) - 1] == ']') {
          token[strlen(token) - 1] = '\0';
        }
        if (IsRegister(token)) {
          byte -> buf[control_ofs] += REGISTER;
          ofs =  WriteRegister(byte, token, ofs);
        }
      }
    }
  } else {
    token = string -> tokens[1];
    byte -> buf[control_ofs] += LABEL;
    ofs = WriteLabel(byte, token, ofs, label_arr);
  }
  return ofs;
}

bool IsNumber(char* str) {
  size_t length = strlen(str);
  for (size_t i = 0; i < length; i++) {
    if (!(isdigit(str[i]) || str[i] == '.')) {
      return false;
    }
  }
  return true;
}

bool IsRegister(char* str) {
  return (strcmp(str, "rax") == 0) || (strcmp(str, "rbx") == 0) ||
         (strcmp(str, "rcx") == 0) || (strcmp(str, "rdx") == 0);
}

bool IsLabel(char* str) {
  size_t length = strlen(str);
  for (size_t i = 0; i < length; i++) {
    if (str[i] == ':') {
      return true;
    }
  }
  return false;
}

size_t WriteNumber(Text* byte, char* token, size_t ofs) {
  *(double*)(byte -> buf + ofs) = atof(token);
  ofs += sizeof(double);
  return ofs;
}

size_t WriteRegister(Text* byte, char* token, size_t ofs) {
  byte -> buf[ofs++] = (char)(token[1] - 'a' + 1);
  return ofs;
}

size_t WriteLabel(Text* byte, char* token, size_t ofs, LabelArr* label_arr) {
  *(size_t*)(byte -> buf + ofs) = GetLabel(label_arr, token);
  ofs += sizeof(size_t);
  return ofs;
}

size_t GetLabel(LabelArr* label_arr, char* label) {
  for(size_t i = 0; i < label_arr -> numb_of_lab; i++) {
    if (strcmp(label_arr -> labels[i].label_name, label) == 0) {
      return label_arr -> labels[i].addr;
    }
  }
  return -1;
}

void AddLabel(LabelArr* label_arr, char* label_name, size_t pos) {
  size_t len = strlen(label_name);
  label_arr -> labels[label_arr -> numb_of_lab].label_name = (char*)calloc(len, sizeof(char));
  for (size_t j = 0; j < len - 1; j++) {
    label_arr -> labels[label_arr -> numb_of_lab].label_name[j] = label_name[j];
  }
  label_arr -> labels[label_arr -> numb_of_lab].label_name[len - 1] = '\0';
  label_arr -> labels[label_arr -> numb_of_lab].addr = pos;
  label_arr -> numb_of_lab++;
}
