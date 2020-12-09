#include "counter.h"

int main(int argc, char* argv[]) {
  const char* input = "input.txt";
  MainArguments(argc, argv, &input);
  Counter(input);
  return 0;
}
