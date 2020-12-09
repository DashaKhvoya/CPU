#include "counter.h"

const char* initials = "BD";
const char* version = "1";


void Counter(const char* command_file) {
  CPU cpu = {};
  CpuConstruct(&cpu, command_file);

  #define DEF_CMD(name, num, arg, code) \
    case num: CPU_##name(&cpu); break;

  while(cpu.run) {
  switch ((unsigned int)cpu.code[cpu.pc]) {
      #include "commands.h"
      ;
    }
    ++cpu.pc;
    //WriteVRAM(&cpu);
  }
  WriteVRAM(&cpu);

  #undef DEF_CMD
}

void WriteVRAM(CPU *cpu) {
  for (size_t i = 0; i < vram_height; i++) {
    for (size_t j = 0; j < vram_width; j++) {
      if (cpu -> ram[vram_begin + i * vram_width + j]) {
        printf("%c", cpu -> ram[vram_begin + i * vram_width + j]);
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void CpuConstruct(CPU* cpu, const char* command_file) {
  construct(cpu -> stk);
  construct(cpu -> call_stk);

  ReadCommand(command_file, cpu);
  cpu -> pc = cpu -> code[0];

  cpu -> ram = (char*)calloc(size_of_memory, sizeof(char));

  cpu -> run = true;

  memset(cpu -> registers, 0, numb_of_reg * sizeof(double));
}

void ReadCommand(const char* command_file, CPU* cpu) {
  String temp = {};
  ReadBinFile(command_file, &temp);

  if (strncmp(temp.buf + 1, initials, 2)) {
    printf("Error file format\n");
    return;
  }

  if (strncmp(temp.buf + 3, version, 1)) {
    printf("Error version\n");
    return;
  }

  cpu -> code = temp.buf;
}

int IsOk(const CPU* cpu) {
  if (IsOk(&cpu -> stk) != 0) {
    return 1;
  }
  if (!cpu -> code) {
    return 1;
  }
  return 0;
}

void Dump(const CPU* cpu) {
  #ifdef DEBUG
  FILE* log_file = fopen ("log_file_counter.txt", "a");

  fprintf(log_file, "code =\n");
  for (size_t i = 0; i < cpu -> pc; i++) {
    fprintf(log_file, "\t  [%c]\n", cpu -> code[i]);
  }
  fprintf(log_file, "\t->[%c]\n", cpu -> code[cpu -> pc]);

  for (size_t i = 1; i < 5; i++) {
    fprintf(log_file, "registers[%lu] = %lg\n", i, cpu -> registers[i]);
  }

  fprintf (log_file, "stack =\n");
  for (int i = 0; i < cpu -> stk.capacity; i++) {
    fprintf (log_file, "\tdata[%d] = %lg\n", i, cpu -> stk.data[i]);
  }

  fclose (log_file);
  #endif
}

#define DEF_CMD(name, num, arg, code) void (CPU_##name)(CPU* cpu) { CPU_ASSERT_OK(cpu); code CPU_ASSERT_OK(cpu); }

#include "commands.h"

#undef DEF_CMD

void MainArguments(int argc, char* argv[], const char** input) {
  assert(input);

  if(argc>1) {
    for(int i = 1; i < argc; i++) {
      if(strcmp(argv[i], "--input")  == 0 || strcmp(argv[i], "-i") == 0) {
        i++;
        *input = argv[i];
      }
    }
  }
}

void ReadBinFile(const char* input, String* string) {
  assert(input);
  assert(string);

  struct stat stbuf = {};
  int res = stat (input, &stbuf);

  string -> size =  stbuf.st_size;
  string -> buf = (char*)calloc(string -> size, sizeof(char));

  FILE* file = fopen(input, "rb");
  fread(string -> buf, sizeof(char), string -> size, file);
  fclose(file);
}
