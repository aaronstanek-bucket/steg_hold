#include "ga_rc.h"
#include "quick_save.h"

#include <iostream>
#include <string>

void pushchar(vector<char> * r, const char * inp) {
  int i = 0;
  char m;
  while (true) {
    m = *(inp+i);
    if (m==0) {
      return;
    }
    r->push_back(m);
    i += 1;
  }
}

gav<char> apply_debug(gav<char> inp) {
  vector<char> * ivec = inp.p();
  gav<char> ou;
  ou.make_new();
  vector<char> * ovec = ou.p();
  pushchar(ovec,"_?c#include \"SIMPLETRACEBACKDEBUGGER.h\"");
  ovec->push_back(10);
  pushchar(ovec,"_?C");
  char ch1,ch2,ch3;
  bool debug_active = false;
  unsigned long long linenum = 1;
  unsigned long long term = ivec->size();
  unsigned long long i = 0;
  while (true) {
    if (i>=term) {
      return ou;
    }
    ch1 = ivec->at(i);
    if (term-i>2) {
      ch2 = ivec->at(i+1);
      ch3 = ivec->at(i+2);
      if ((ch1=='_') && (ch2=='?')) {
        if (ch3=='d') {
          pushchar(ovec,"SIMPLETRACEBACKDEBUGGER::coin(\"");
          i += 3;
          while (ivec->at(i)!=';') {
            ovec->push_back(ivec->at(i));
            i += 1;
          }
          pushchar(ovec,"\");");
          i += 1;
          debug_active = true;
          continue;
        }
        if (ch3=='D') {
          i += 3;
          debug_active = false;
          continue;
        }
      }
    }
    if (ch1==10) {
      linenum += 1;
      if (debug_active) {
        ovec->push_back(ch1);
        i += 1;
        pushchar(ovec,"SIMPLETRACEBACKDEBUGGER::update(");
        pushchar(ovec,to_string(linenum).c_str());
        pushchar(ovec,");");
        continue;
      }
    }
    ovec->push_back(ch1);
    i += 1;
  }
}

void do_the_copy(vector<char> * s, vector<char> * h, vector<char> * c) {
  bool ih = true;
  bool ic = true;
  vector<string> rem(4);
  char ch1,ch2,ch3;
  unsigned long long term = s->size();
  unsigned long long i = 0;
  while (true) {
    if (i>=term) {
      return;
    }
    ch1 = s->at(i);
    if (term-i>2) {
      ch2 = s->at(i+1);
      ch3 = s->at(i+2);
      if ((ch1=='_') && (ch2)=='?') {
        if (ch3=='H') {
          ih = true;
          i += 3;
          continue;
        }
        if (ch3=='h') {
          ih = false;
          i += 3;
          continue;
        }
        if (ch3=='C') {
          ic = true;
          i += 3;
          continue;
        }
        if (ch3=='c') {
          ic = false;
          i += 3;
          continue;
        }
        if (ch3=='f') {
          h->push_back(';');
          ih = false;
          ic = true;
          i += 3;
          continue;
        }
        if (ch3=='F') {
          ih = true;
          i += 3;
          continue;
        }
        if (ch3=='r') {
          i += 3;
          ch1 = s->at(i);
          int ind = (int)(ch1-49);
          if ((ind<0) || (ind>3)) {
            cout << "variable index out of range" << endl;
            exit(0);
          }
          rem[ind].resize(0);
          i += 1;
          while (true) {
            ch1 = s->at(i);
            if (ch1==';') {
              break;
            }
            rem[ind].push_back(ch1);
            i += 1;
          }
          i += 1;
          continue;
        }
        if ((ch3>=49) && (ch3<=52)) {
          int ind = (int)(ch3-49);
          if ((ih) && (ch3>=51)) {
            for (int q=0;q<rem[ind].size();q++) {
              h->push_back(rem[ind][q]);
            }
          }
          if (ic) {
            for (int q=0;q<rem[ind].size();q++) {
              c->push_back(rem[ind][q]);
            }
          }
          i += 3;
          continue;
        }
        if (ch3=='!') {
          if (ih) {
            h->push_back('_');
            h->push_back('?');
          }
          if (ic) {
            c->push_back('_');
            c->push_back('?');
          }
          i += 3;
          continue;
        }
        // if none
        i += 3;
        continue;
      }
    }
    // not a command sequence
    if (ih) {
      h->push_back(ch1);
    }
    if (ic) {
      c->push_back(ch1);
    }
    i += 1;
  }
}

gav<char> remove_extra_space(gav<char> inp) {
  vector<char> * ivec = inp.p();
  gav<char> ou;
  ou.make_new();
  vector<char> * ovec = ou.p();
  bool flag = false;
  unsigned long long siz = ivec->size();
  char here;
  for (unsigned long long i=0;i<siz;i++) {
    here = ivec->at(i);
    if (here==10) {
      if (flag) {
        ovec->push_back(here);
      }
      flag = false;
      continue;
    }
    if (flag) {
      ovec->push_back(here);
    }
    else {
      if ((here!=32) && (here!=9)) {
        ovec->push_back(here);
        flag = true;
      }
    }
  }
  return ou;
}

int main(int argc, char * argv[]) {
  if (argc<4) {
    cout << "not enough arguments" << endl;
    cout << "ex: input.splitcies output.h output.cpp" << endl;
    exit(0);
  }
  gav<char> source = quick_save::readfile(argv[1]);
  if (argc==5) {
    source = apply_debug(source);
  }
  gav<char> h;
  h.make_new();
  gav<char> c;
  c.make_new();
  do_the_copy(source.p(),h.p(),c.p());
  h = remove_extra_space(h);
  c = remove_extra_space(c);
  quick_save::writefile(argv[2],h);
  quick_save::writefile(argv[3],c);
  return 0;
}
