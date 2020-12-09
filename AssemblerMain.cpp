#include "Assembler.h"

void MainArguments(int argc, char* argv[], const char** input, const char** output);

int main(int argc, char* argv[]) {
  const char* input = "input.txt";
  const char* output = "output.bin";
  MainArguments(argc, argv, &input, &output);
  Translation(input, output);
  return 0;
}

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
