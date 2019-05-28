#ifndef TRANSLATOR
#define TRANSLATOR_H

#include <string>
#include "scanner.h"

typedef struct {
  std::string result;
  std::string ag1;
  std::string op;
  std::string ag2;
} Quad;

class Translator {
private:
  Scanner *scner;
  Word *_word;
  Quad quad[60];
  int _brace[60];
  int _sp;
  Quad _quad;
  int _idx;
  int _temp_count;

  void _push();
  void _pop(bool is_while=false);
  void _jmp_back();
  void print(int i);
  void display();
  bool match(std::string tag, std::string err_str="");
  void _emit(Quad _quad);
  void _emit(std::string result, std::string ag1, std::string op="", std::string ag2="");
  std::string _new_temp();
  void _raise(std::string error_string, int line_count=-1);
  std::string _parse_factor();
  std::string _parse_term();
  std::string _parse_expression();
  void _parse_condition();
  void _parse_statement();
  void _translate();
  void _translate_statement_block(bool is_while=false);

public:
  Translator(std::string path);
  void translate();
};

#endif // TRANSLATOR_H
