#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct String {
  size_t size;
  char* buf;
  size_t numb_of_tokens;
  char** tokens;
};

struct Text {
  size_t size;
  char* buf;
  size_t numb_of_strings;
  String* strings;
};

size_t SizeOfBuf(const char* file_name);
void ReadFile(const char* input, Text* text);
void WriteBinFile(const char* output, const Text* text);
void SplitToStrings(Text* text);
void SplitToTokens(String* string);
