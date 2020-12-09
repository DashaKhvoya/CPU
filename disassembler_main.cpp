#include "disassembler.h"

int main(int argc, char* argv[]) {
  const char* input = "input.bin";
  const char* output = "output.txt";
  MainArguments(argc, argv, &input, &output);
  Translation(input, output);
  return 0;
}
