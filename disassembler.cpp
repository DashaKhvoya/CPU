#include "disassembler.h"

void MainArguments(int argc, char* argv[], const char** input, const char** output) {
  assert(input);
  assert(output);

  if(argc>1) {
    for(int i = 1; i < argc; i++) {
      if(strcmp(argv[i], "--input")  == 0 || strcmp(argv[i], "-i") == 0) {
        i++;
        *input = argv[i];
      }

      if(strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
        i++;
        *output = argv[i];
      }
    }
  }
}

void Translation(const char* input, const char* output) {
  String byte = {};
  ReadBinFile(input, &byte);
  String asmbly = {};

  TranslationByteToAsm(&byte, &asmbly);
  WriteFile(output, &asmbly);
}

void TranslationByteToAsm(const String* byte, String* asmbly) {
  assert(byte);
  assert(asmbly);

  if (byte -> buf[0] != 'B' || byte -> buf[1] != 'D') {
    printf ("File Format Error");
    return;
  }
  if (byte -> buf[2] != '1') {
    printf ("False CPU version");
    return;
  }

  asmbly -> buf = (char*)calloc((byte -> size)*6, sizeof(char));
  size_t ofs = 0;

  #define DEF_CMD(name, num, arg, code)                     \
    if ((unsigned char)byte -> buf[i] == num) {             \
      ofs += sprintf(asmbly -> buf + ofs, "%s", #name);     \
      if (arg != 0) {                                       \
        if (byte -> buf[i + 1] == 0) {                      \
          double temp = *(double*)(byte -> buf + i + 2);    \
          ofs += sprintf(asmbly -> buf + ofs, "%lg", temp); \
          i += sizeof(double);                              \
        } else {                                            \
          i++;                                              \
        }                                                   \
      }                                                     \
      ofs += sprintf(asmbly -> buf + ofs, "\n");            \
    } else

  for (size_t i = 3; i < byte -> size; i++) {
    #include "commands.h"
    ;
  }

  #undef DEF_CMD

  asmbly -> size = ofs;
  asmbly -> buf[ofs] = '\0';
}

size_t SizeOfBuf(const char* file_name) {
  assert (file_name);

  struct stat stbuf = {};
  int res = stat (file_name, &stbuf);

  return stbuf.st_size;
}

void ReadBinFile(const char* input, String* string) {
  assert(input);
  assert(string);

  string -> size = SizeOfBuf(input);
  string -> buf = (char*)calloc(string -> size, sizeof(char));

  FILE* file = fopen(input, "rb");
  fread(string -> buf, sizeof(char), string -> size, file);
  fclose(file);
}

void WriteFile(const char* output, const String* string) {
  assert(string);
  assert(output);

  FILE* file = fopen(output, "w");
  fwrite(string -> buf, sizeof(char), string -> size, file);
  fclose(file);
}
