#include <iostream>

#include "ga_rc.h"
#include "quick_save.h"
#include "remove_comments.h"
#include "steg_big.h"

gav<char> protect(gav<char> inp) {
  vector<char> * ivec = inp.p();
  gav<char> ou;
  ou.make_new();
  vector<char> * ovec = ou.p();
  unsigned long long siz = ivec->size();
  unsigned long long i = 0;
  while (i<siz) {
    if (siz-i>1) {
      if (ivec->at(i)=='_') {
        if (ivec->at(i+1)=='?') {
          ovec->push_back('_');
          ovec->push_back('?');
          ovec->push_back('!');
          i += 2;
          continue;
        }
      }
    }
    ovec->push_back(ivec->at(i));
    i += 1;
  }
  return ou;
}

int main(int argc, char * argv[]) {
  if (argc<3) {
    cout << "need more args" << endl;
    exit(0);
  }
  cout << "booted" << endl;
  gav<char> source = quick_save::readfile(argv[1]);
  cout << "file loaded" << endl;
  remove_comments(source.p());
  cout << "comments removed" << endl;
  source = protect(source);
  cout << "source protected" << endl;
  source = steg_big(source);
  cout << "converted" << endl;
  quick_save::writefile(argv[2],source);
  cout << "data output" << endl;
  return 0;
}
