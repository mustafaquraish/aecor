#include "tokens.hh"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
  // read "test.ae" file into std::string
  FILE *fp = fopen("test.ae", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return 1;
  }
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = new char[size + 1];
  fread(buffer, 1, size, fp);
  fclose(fp);
  buffer[size] = '\0';
  string source(buffer);

  auto tokens = lex(source);

  for (auto token:  tokens) {
    cout << token << endl;
  }

  return 0;
}
