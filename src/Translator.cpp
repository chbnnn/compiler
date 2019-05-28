#include "Translator.h"

Translator::Translator(std::string path) {
  scner = new Scanner(path);
  _idx = 0;
  _sp = 0;
  _temp_count = 0;
  _quad.result = ""; _quad.ag1 = ""; _quad.op = ""; _quad.ag2 = "";
}

void Translator::_raise(std::string error_string, int line_count) {
  if(line_count != -1)
    std::cout << "\nException captured at line " << line_count;
  else
    std::cout << "\nException captured at line " << scner->line_count;

  std::cout << ": Error: " << error_string;

  exit(-1);
}

void Translator::_jmp_back() {
  Quad _q = {"jmp", "", "", ""};
  _q.ag1 = "item" + std::to_string(_brace[_sp]);
  _emit(_q);
}

void Translator::_push() {
  if(_sp + 1 < 60 && _idx > 1)
    _brace[_sp++] = _idx-1;
}

void Translator::_pop(bool is_while) {
  if(_sp - 1 >= 0) {
    if(is_while) {
      quad[_brace[--_sp]].ag1 = "item" + std::to_string(_idx+2);
      _jmp_back();
    }
    else
      quad[_brace[--_sp]].ag1 = "item" + std::to_string(_idx+1);
  }
}


void Translator::print(int i) {
//  std::cout << i << ". (" << quad[i].result << ", " << quad[i].ag1 << ", " << quad[i].op << ", " << quad[i].ag2 << ")\n";
  if(quad[i].result == "jmp") {
    std::cout << i+1 << ":  " << quad[i].result << " " << quad[i].ag1 << std::endl;
    return;
  } else if(quad[i].op == ">" || quad[i].op == ">=" || quad[i].op == "==" || quad[i].op == "<" || quad[i].op == "<=") {
    std::cout << i+1 << ":  jmp " << quad[i].result << " " << quad[i].ag1;
  }
  else
    std::cout << i+1 << ":  " << quad[i].result << " = " << quad[i].ag1;

  if(quad[i].op == "")
    std::cout << "\n";
  else
    std::cout << " " << quad[i].op << " " << quad[i].ag2 << "\n";
}

void Translator::display() {
  for(int i=0; i<_idx; i++)
    print(i);
  std::cout << _idx+1 << ". return 0\n";
}

bool Translator::match(std::string tag, std::string err_str) {
  if(_word->word == tag) return true;
  if(err_str != "")
    _raise(err_str);
  return false;
}

void Translator::_emit(Quad _quad) {
  if(_idx >= 60) {_raise("idx of quad out of bound"); return;}
  quad[_idx].result = _quad.result;
  quad[_idx].ag1 = _quad.ag1;
  quad[_idx].op = _quad.op;
  quad[_idx].ag2 = _quad.ag2;
  ++_idx;
//  std::cout << _idx << ". " << _quad.result << " = " << _quad.ag1;
//  if(_quad.op == "")
//    std::cout << "\n";
//  else
//    std::cout << " " << _quad.op << " " << _quad.ag2 << "\n";
//  std::cout << "(" << _quad.result << ", " << _quad.ag1 << ", " << _quad.op << ", " << _quad.ag2 << ")\n";
}

void Translator::_emit(std::string result, std::string ag1, std::string op, std::string ag2) {
  if(_idx >= 60) {_raise("idx of quad out of bound"); return;}
  quad[_idx].result = result;
  quad[_idx].ag1 = ag1;
  quad[_idx].op = op;
  quad[_idx].ag2 = ag2;
  ++_idx;

  std::cout << "(" << result << ", " << ag1 << ", " << op << ", " << ag2 << ")\n";
}

std::string Translator::_new_temp() {
  return "t" + std::to_string(_temp_count++);
}

std::string Translator::_parse_factor() {
  _word = scner->next_word();

  if(_word->type_id == 10 || _word->type_id == 20) {  // if is identifier or number
    return _word->word;
  } else if(_word->type_id == 26) { // if is '('
    std::string expr = _parse_expression();

    return expr;
  }
  return "";  // if not the above two (not gonna happen in fact)
}

std::string Translator::_parse_term() {
  bool _flag;
  int _component_count = -1;
  Quad _quad = {"", "", "", ""};

  do {
    std::string factor = _parse_factor();
    if(factor != "") {
      _component_count = ++_component_count % 2;
      switch(_component_count) {
        case 0: _quad.ag1 = factor; break;
        case 1: _quad.ag2 = factor; _quad.result = _new_temp(); _emit(_quad); _quad.ag1 = _quad.result;
      }
    }

    _word = scner->next_word();
    _flag = (_word->type_id == 24 || _word->type_id == 25);  // if is * or /
    if(_flag) {
//      std::cout << _word->word << " ";
      _quad.op = _word->word;
    }
  } while(_flag);  // if is * or /
  return _quad.ag1;
}

std::string Translator::_parse_expression() {
  Quad _quad = {"", "", "", ""};
  int term_count = -1;
  do {
    std::string term = _parse_term();
    if(term != "") {
      term_count = ++term_count % 2;
      if(term_count == 0) _quad.ag1 = term;
      else if(term_count == 1) {_quad.ag2 = term; _quad.result = _new_temp(); _emit(_quad); _quad.ag1 = _quad.result;}
    }

    _quad.op = _word->word;
  } while(_word->type_id == 22 || _word->type_id == 23);  // if is + or -
  return _quad.ag1;
}

void Translator::_parse_condition() {
  Quad _quad = {"", "", "", ""};
  _quad.ag1 = _parse_expression();

  if(_word->type_id >= 35 && _word->type_id <= 40)
    _quad.op = _word->word;
  else _raise("invalid relational operator");

  _quad.ag2 = _parse_expression();
  _quad.result = std::to_string(_idx + 3);
  _emit(_quad);

  Quad _quad2 = {"jmp", std::to_string(_idx - 2), "", ""};
  _emit(_quad2);
}

void Translator::_parse_statement() {
  Quad _quad = {"", "", "", ""};
  switch(_word->type_id) {
    case 10:  // if is identifier
             _quad.result = _word->word;
             _word = scner->next_word();
             if(_word->type_id == 21) { // if is '='
                std::string expr = _parse_expression();
                if(expr != "") {
                  _quad.ag1 = expr;
                  _emit(_quad);
                }
             } else _raise("expect '='");
             break;
    case  2:   // if is 'if'
             _word = scner->next_word();
             match("(", "missing '('");
             _parse_condition();
             match(")", "missing ')'");

             _word = scner->next_word();
             _translate_statement_block();
             break;
    case  4:  // if is 'while'
             _word = scner->next_word();
             match("(", "missing '('");
             _parse_condition();
             match(")", "missing ')'");

             _word = scner->next_word();
             _translate_statement_block(true);
             break;
  }
}


void Translator::_translate() {
  _word = scner->next_word();
  while(_word->type_id == 10 || _word->word == "if" || _word->word == "while") {
    _parse_statement();

    _word = scner->next_word();
  }
}

void Translator::_translate_statement_block(bool is_while) {
  match("{", "expected '{'");
  _push();

  _translate();

  match("}", "expected '}'");
  _pop(is_while);
}

void Translator::translate() {
    _word = scner->next_word();
    match("main", "expected main");

    _word = scner->next_word();
    match("(", "expected '('");

    _word = scner->next_word();
    match(")", "expected ')'");

    std::cout << "-----translated result-----" << std::endl;

    _word = scner->next_word();
    _translate_statement_block();

    display();

    std::cout << "Script file successfully translated" << std::endl;

}
