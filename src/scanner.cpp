#include "scanner.h"

Scanner::Scanner(std::string path, bool print_origin_file) {
  std::ifstream infile(path);

  std::istreambuf_iterator<char> begin(infile);
  std::istreambuf_iterator<char> end;
  std::string s(begin, end);

  strcpy(input, s.c_str());

  err = false;
  line_count = 1;
  ptr_input = 0;
  is_comment = false;
  if(print_origin_file)
    std::cout << "----Your file---- " << std::endl
              << input << std::endl;
}

char Scanner::_getch() {
  ch = input[ptr_input++];
  return (ch);
}

void Scanner::_getbc() {
  while(ch == ' ') {
    ch = input[ptr_input++];
  }
}

void Scanner::_concat() {
  token[ptr_token++] = ch;
  token[ptr_token] = '\0';
}

bool Scanner::_isletter() {
  if(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') return true;
  return false;
}

bool Scanner::_isdigit() {
  if(ch >= '0' && ch <= '9') return true;
  return false;
}

int Scanner::_is_reserved() {
  int n = len(reserved_words);
  for(int i = 0; i < n; i++) {
    if(token == reserved_words[i])
      return i;
  }
  return 10;
}

void Scanner::_retract() {
  ptr_input--;
}

void Scanner::_int2bin() {
  int num = atoi(token);
  int quotient = num, remainder;
  ptr_token = 0;
  std::string res="";
  int count = 0;
  do {
    num = quotient;
    quotient = num / 2;
    remainder = num % 2;
    std::string tmp = "x";
    tmp.at(0) = (char)(48+remainder);
    res = tmp + res;
    count++;
  } while (quotient != 0);
  for(int i=0; i<count; i++) {
    ch = res.at(i);
    _concat();
  }
}

Word* Scanner::next_word(bool skip_comment, bool int2bin) {
  Word* _word = new Word;
  _word->type_id = 10;
  _word->word = "";
  ptr_token = 0;
  _getch();
  _getbc();


  if(_isletter()) {
    // 如果以letter开头，则一直读完letter(letter | digit)*
    while(_isletter() || _isdigit()) {
      _concat();
      _getch();
    } _retract();

    if(is_comment) _word->type_id = -1;
    else _word->type_id = Scanner::_is_reserved();  // 判断标识符是否为保留字（关键字）

    _word->word = token;

  } else if(_isdigit()) {
    // 如果是数字，则一直读完digit digit*
    while(_isdigit()) {
      _concat();
      _getch();
    } _retract();

    if(int2bin) _int2bin();
    _word->word = token;

    if(is_comment) _word->type_id = -1;
    else _word->type_id = 20;

  } else switch(ch) {
      // 否则为运算符或界符
    case '=':
              _getch();
              if(ch == '=') {
                _word->word = "==";
                if(is_comment)
                  _word->type_id = -1;
                else
                  _word->type_id = 39;
                break;
              } _retract();

              _word->word = "=";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 21;
              break;
    case '+':
              _word->word = "+";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 22;
              break;
    case '-':
              _word->word = "-";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 23;
              break;
    case '*':
              // 判断是否为注释代码块结束标记
              _getch();
              if(ch == '/') {
                _word->word = "*/";
                _word->type_id = -1;
                is_comment = false;
                break;
              } _retract();

              _word->word = "*";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 24;
              break;
    case '/':
              // 判断是否为注释起始标志，并标记是否为单行注释
              _getch();
              if(ch == '/') {
                _word->type_id = -1;
                _word->word = "//";
                is_comment = true;
                one_line_comment = true;
                break;
              } else if(ch == '*') {
                _word->type_id = -1;
                _word->word = "/*";
                is_comment = true;
                one_line_comment = false;
                break;
              } _retract();

              _word->word = "/";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 25;
              break;
    case '(':
              _word->word = "(";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 26;
              break;
    case ')':
              _word->word = ")";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 27;
              break;
    case '[':
              _word->word = "[";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 28;
              break;
    case ']':
              _word->word = "]";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 29;
              break;
    case '{':
              _word->word = "{";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 30;
              break;
    case '}':
              _word->word = "}";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 31;
              break;
    case ',':
              _word->word = ",";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 32;
              break;
    case ':':
              _word->word = ":";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 33;
              break;
    case ';':
              _word->word = ";";
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 34;
              break;
    case '>':
              _getch();
              if(ch == '=') {
                _word->word = ">=";
                if(is_comment)
                  _word->type_id = -1;
                else
                  _word->type_id = 37;
                break;
              } _retract();

              _word->word = '>';
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 35;
              break;
    case '<':
              _getch();
              if(ch == '=') {
                _word->word = "<=";
                if(is_comment)
                  _word->type_id = -1;
                else
                  _word->type_id = 38;
                break;
              }
              _retract();
              _word->word = '<';
              if(is_comment)
                _word->type_id = -1;
              else
                _word->type_id = 36;
              break;
    case '!':
              _getch();
              if(ch == '=') {
                _word->word = "!=";
                if(is_comment)
                  _word->type_id = -1;
                else
                  _word->type_id = 40;
                break;
              }
              _retract();
              _word->word = "ERROR";
              _word->type_id = -1;
              break;
    case '\n':
               _word->word = "LF";
               _word->type_id = -1;
               ++line_count;
               if(is_comment && !one_line_comment) _word->type_id = -1;
               if(is_comment && one_line_comment) is_comment = false;
               break;
    case '\0':
               _word->word = "OVER";
               _word->type_id = 1000;
               break;
    case '#':
               _word->word = "finished";
               _word->type_id = 100;
               break;
    default:
             _word->word = "ERROR";
             _word->type_id = -2;
             err = true;
    //             std::cout << "\nError:\n  exception captured at line " << line_count << std::endl;
  }
  if(skip_comment && _word->type_id == -1) return next_word();
  else return _word;

}
