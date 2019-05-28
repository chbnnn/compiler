#include "LRparser.h"

LRparser::LRparser(std::string path, bool print_origin_file) {
  scner = new Scanner(path, print_origin_file);
}

void LRparser::_raise(std::string error_string, int line_count) {
  if(line_count != -1)
    std::cout << "\nException captured at line " << line_count;
  else
    std::cout << "\nException captured at line " << scner->line_count;

  std::cout << ": Error: " << error_string;

  exit(-1);
}

bool LRparser::_match(std::string tag, std::string err_str) {
  if(_word->word == tag) return true;
  if(err_str != "")
    _raise(err_str);
  return false;
}

bool LRparser::_is_factor() {
  _word = scner->next_word();
  std::cout << _word->word << " ";
  if(_word->type_id == 10 || _word->type_id == 20) // if is identifier or number
    return true;
  else if(_word->type_id == 26) { // if is '('
    if(!_is_expression()) {_raise("invalid expression"); return false;}

//    std::cout << _word->word << " ";
    if(_word->type_id == 27) return true;  // if is ')'
  }
  return false;
}

bool LRparser::_is_term() {
  bool _flag;
  do {
    if(!_is_factor()) {
      _raise("invalid factor");
      return false;
    }
    _word = scner->next_word();

    _flag = (_word->type_id == 24 || _word->type_id == 25);  // if is * or /
    if(_flag) std::cout << _word->word << " ";
  } while(_flag);  // if is * or /
  return true;
}

bool LRparser::_is_expression() {
  do {
    if(!_is_term()) {
      _raise("invalid term");
      return false;
    }
//    _word = scner->next_word();
    if(_word->type_id != 34 && _word->word != ")") std::cout << _word->word << " "; // if is not ';', i.e. it's either a '+' or a '-'
  } while(_word->type_id == 22 || _word->type_id == 23);  // if is + or -
  if(_word->type_id == 10 || _word->type_id == 20) _raise("invalid operator");
  return true;
}

bool LRparser::_is_condition() {
//  _match("(", "expected '('");
  if(!_is_expression())
    _raise("invalid expr");

  if(!(_word->type_id >= 35 && _word->type_id <= 40))
    _raise("invalid relational operator");

  if(!_is_expression())
    _raise("invalid expr");
  _match(")", "expected ')'");
  std::cout << _word->word << " ";
  return true;
}

bool LRparser::_is_statement() {
  _word = scner->next_word();
//  if(_word->word == "}") _word = scner->next_word();
  if(_word->word != "{" && _word->word != "}") std::cout << _word->word << " "; // if is not 'end', print the term
  if(_word->type_id == 10) { // if is identifier, i.e. variable, reserverd word, etc
    _word = scner->next_word();
    std::cout << _word->word << " ";
    if(_word->type_id == 21) { // if is '='
      if(_is_expression()) {
        return true;
      }
      else _raise("expr error");
    }
    else {
      _raise("missing '=' error");
    }
  } else if(_word->word == "if" || _word->word == "while") {
    _word = scner->next_word(); // '('
    _match("(", "expected '('");
    std::cout << _word->word << " ";
    if(!_is_condition()) {
      _raise("invalid condition statement");
      return false;
    }
    _word = scner->next_word(); // '{'
    _match("{", "expected '{");
    std::cout << _word->word << "\n";
    _parse();
//    std::cout << "returned\n";
//    _word = scner->next_word();
//    std::cout << _word->word << "==";
    _match("}", "expected '}' after '{'");
//    _word = scner->next_word();
//    std::cout << _word->word << "\n";
    return true;
  }
  return false;
}

bool LRparser::_parse() {
  do {
    if(!_is_statement()) {
      if(_word->word == "}")
        return true;
      _raise("invalid statement");
      return false;
    }

//    if(_word->word != "}")
      std::cout << _word->word << "\n";
  } while(_word->type_id == 34 || _word->word == "}");  // if is ';' or '}'

  return true;
}

bool LRparser::parse() {
    _word = scner->next_word();
      std::cout << "----start parsing-----" << std::endl;
    if(_word->word == "main") {  // if is 'main'
      std::cout << _word->word << " ";
      _word = scner->next_word();  // (
      _match("(", "expected '(' after main");
      std::cout << _word->word << " ";
      _word = scner->next_word();  // )
      _match(")", "expected ')' after '('");
      std::cout << _word->word << " ";
      _word = scner->next_word();  // {
      _match("{", "expected '{'");
      std::cout << _word->word << "\n";
      if(!_parse()) {  // start parsing
        _raise("parsing stopped");
        return false;
      }

//      _word = scner->next_word();  // }
      std::cout << _word->word << "\n";
      std::cout << "\nStatements successfully parsed" << std::endl;
    }
    else
      _raise("'main' EXPECTED at bos");
    return false;
}
