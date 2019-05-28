#include <iostream>
#include "scanner.h"
#include "LRparser.h"
#include "Translator.h"
using namespace std;

int main() {
  string path;
  cout << "Input the path to the file to be analyzed: ";
  cin >> path;

//  Scanner scanner(path);
//  int over = 1;
//  Word* _word = new Word;
//
//  cout << "----Analysis result---- " << endl;
//  while(true) {
//    _word = scanner.next_word();
//    if(_word->type_id == -2 || _word->type_id >= 1000)  // if encouter an error
//      break;
//      cout << "(" << _word->type_id << ", " << _word->word << ")" << endl;
//  }

//  LRparser lrparser(path, true); // print_origin_file=true
//  lrparser.parse();

//  LRparser lrparser(path, false); // print_origin_file=true
//  if(lrparser.parse()) { // if successfully parsed, call the translator
//    Translator translator(path);
//    translator.translate();
//  }
  Translator translator(path);
  translator.translate();
}






































