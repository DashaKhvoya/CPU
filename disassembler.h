#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct String {
  size_t size;
  char* buf;
  size_t numb;
  char** cmd;
};

size_t SizeOfBuf(const char* file_name);
void ReadBinFile(const char* input, String* string);
void WriteFile(const char* output, const String* string);

void MainArguments(int argc, char* argv[], const char** input, const char** output);

void Translation(const char* input, const char* output);
void TranslationByteToAsm(const String* asmbly, String* byte);
