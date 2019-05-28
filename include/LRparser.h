#ifndef LRPARSER_H
#define LRPARSER_H

#include "scanner.h"

class LRparser {
private:
  Scanner *scner;
  Word *_word;
  void _raise(std::string error_string, int line_count=-1);
  bool _match(std::string tag, std::string err_str);
  bool _is_factor();
  bool _is_term();
  bool _is_expression();
  bool _is_condition();
  bool _is_statement();
  bool _parse();

public:
  LRparser(std::string path, bool print_origin_file=false);
  bool parse();
};

#endif // LRPARSER_H
