#include "StringFunctions.h"

size_t SizeOfBuf(const char* file_name) {
  assert (file_name);

  struct stat stbuf = {};
  int res = stat (file_name, &stbuf);

  return stbuf.st_size;
}

void ReadFile(const char* input, Text* text) {
  assert(input);
  assert(text);

  size_t size_of_buf = SizeOfBuf(input);
  text -> buf = (char*)calloc(size_of_buf, sizeof(char));

  FILE* file = fopen(input, "r");
  assert(file);
  size_of_buf = fread(text -> buf, sizeof(char), size_of_buf, file);
  fclose(file);

  text -> size = size_of_buf;
}

void WriteBinFile(const char* output, const Text* text) {
  assert(output);
  assert(text);

  FILE* file = fopen(output, "wb");
  fwrite(text -> buf, sizeof(char), text -> size + 1, file);
  fclose(file);
}

void SplitToStrings(Text* text) {
  assert(text);

  text -> numb_of_strings = 1;
  for (size_t i = 0; i < text -> size; i++) {
    if(text -> buf[i] == '\n') {
      text -> numb_of_strings++;
    }
  }

  text -> strings = (String*)calloc(text -> numb_of_strings, sizeof(String));

  char* token = strtok(text -> buf, "\n");
  for (size_t i = 0; i < text -> numb_of_strings; i++) {
    if (token) {
      text -> strings[i].size = strlen(token);
      text -> strings[i].buf = (char*)calloc(text -> strings[i].size + 1, sizeof(char));
      strcpy(text -> strings[i].buf, token);
    } else {
      text -> numb_of_strings--;
      i--;
    }

    token = strtok(NULL, "\n");
  }

  for (size_t i = 0; i < text -> numb_of_strings; i++) {
    SplitToTokens(&text -> strings[i]);
  }
}

void SplitToTokens(String* string) {
  assert(string);
  char* temp = (char*)calloc(string -> size + 1, sizeof(char));
  strcpy(temp, string -> buf);

  string -> numb_of_tokens = 1;
  for (size_t i = 0; i < string -> size; i++) {
    if(string -> buf[i] == ' ' || string -> buf[i] == '+') {
      string -> numb_of_tokens++;
    }
  }

  string -> tokens = (char**)calloc(string -> numb_of_tokens, sizeof(char*));

  char* token = strtok(string -> buf, " +");
  for (size_t i = 0; i < string -> numb_of_tokens; i++) {
    if (token) {
      string -> tokens[i] = (char*)calloc(strlen(token) + 1, sizeof(char));
      strcpy(string -> tokens[i], token);
    } else {
      string -> numb_of_tokens--;
      i--;
    }

    token = strtok(NULL, " +\n");
  }

  free(string -> buf);
  string -> buf = temp;
}
