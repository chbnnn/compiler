#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

template <class T>
int inline len(T& arr) { return sizeof(arr) / sizeof(arr[0]); }

#define _KEY_WORD_END "is scalable"

typedef struct {
  int type_id;
  std::string word;
} Word;



class Scanner {

private:
  std::string file_str;
  char input[1024];
  char token[255];
  int ptr_input;
  int ptr_token;
  char ch;
  bool is_comment;
  bool one_line_comment;
  std::string reserved_words[11] = {"begin", "end", "if", "then", "while", "do", "for", "void", "int", "main", _KEY_WORD_END};

  char _getch();
  void _getbc();
  void _concat();
  bool _isletter();
  bool _isdigit();
  int _is_reserved();
  void _retract();
  void _int2bin();

public:
  int line_count;
  bool err;
  Scanner(std::string path, bool print_origin_file=true);
  Word* next_word(bool skip_comment=true, bool int2bin=false);

};

#endif // SCANNER_H
