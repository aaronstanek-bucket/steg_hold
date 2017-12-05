#ifndef STEG_CLOSE_INCLUDED
#define STEG_CLOSE_INCLUDED

#include "push_many.h"

void steg_close(vector<char> * ovec, char typ) {
  // $
  // n i e s f w
  // T L F C S 1 2 3
  if (typ=='$') {
    return;
  }
  if ((typ>=97) && (typ<=122)) {
    ovec->push_back('}');
    return;
  }
  // eventually the next bit will be usable, in future versions
  if (typ=='T') {
    //push_many(ovec,"_?r1;");
    return;
  }
  if (typ=='L') {
    //push_many(ovec,"_?r2;");
    return;
  }
  if (typ=='F') {
    push_many(ovec," _?D } _?F ");
    return;
  }
  if ((typ=='C') || (typ=='S')) {
    push_many(ovec," _?c }; _?C _?r1;_?r2;_?r3;_?r4;");
    return;
  }
  if ((typ>=48) && (typ<=57)) {
    return;
  }
  if (typ=='[') {
    push_many(ovec," _?C");
    return;
  }
  if (typ==']') {
    push_many(ovec," _?H");
    return;
  }
  throw "internal error";
}

#endif
