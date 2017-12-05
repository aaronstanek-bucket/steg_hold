#ifndef STEG_BIG
#define STEG_BIG

#include "steg_close.h"
#include "push_many.h"

char osv(vector<char> * v, unsigned long long index) {
  if (index<v->size()) {
    return v->at(index);
  }
  return 10;
}

bool forward_match(vector<char> * v, unsigned long long spot, const char * m) {
  unsigned long long i = 0;
  while (true) {
    if (*(m+i)==0) {
      return true;
    }
    if (osv(v,spot+i)!=*(m+i)) {
      return false;
    }
    i += 1;
  }
}

gav<unsigned long long> find_lines(vector<char> * ivec) {
  unsigned long long siz = ivec->size();
  gav<unsigned long long> ou;
  ou.make_new();
  vector<unsigned long long> * ovec = ou.p();
  ovec->push_back(0);
  for (unsigned long long i=0;i<siz;i++) {
    if (osv(ivec,i)==10) {
      if (i+1<siz) {
        ovec->push_back(i+1);
      }
    }
  }
  return ou;
}

struct alg_stack_element {
  char typ;
  unsigned int ind;
};

void annotate_scan(vector<char> * ivec, vector<char> * ovec, unsigned long long & contentstart) {
  char my;
  while(true) {
    my = osv(ivec,contentstart);
    if ((my==10) || (my==13)) {
      break;
    }
    ovec->push_back(my);
    contentstart += 1;
  }
}

unsigned long long func_insert(vector<char> * ivec, unsigned long long contentstart) {
  unsigned long long i = contentstart;
  char my;
  while (osv(ivec,i)!='(') {
    i += 1;
  }
  while (true) {
    my = osv(ivec,i);
    if ((my>=65) && (my<=90)) {
      break;
    }
    if ((my>=97) && (my<=122)) {
      break;
    }
    if (my==95) {
      break;
    }
    i -= 1;
  }
  while (true) {
    my = osv(ivec,i);
    if ((my==32) || (my==9)) {
      break;
    }
    i -= 1;
  }
  return i;
}

gav<char> annotate(gav<char> inp, gav<unsigned long long> lines) {
  vector<char> * ivec = inp.p();
  gav<char> ou;
  ou.make_new();
  vector<char> * ovec = ou.p();
  vector<alg_stack_element> alg_stack;
  unsigned long long lnstart;
  unsigned long long contentstart;
  char my;
  for (unsigned linenum=0;linenum<lines.p()->size();linenum++) {
    lnstart = lines.p()->at(linenum);
    contentstart = lnstart;
    while (true) {
      my = osv(ivec,contentstart);
      if ((my!=32) && (my!=9)) {
        break;
      }
      ovec->push_back(my);
      contentstart += 1;
    }
    if ((my==10) || (my==13)) {
      ovec->push_back(my);
      continue;
    }
    while (true) {
      if (alg_stack.size()==0) {
        break;
      }
      if (alg_stack.back().ind>=contentstart-lnstart) {
        steg_close(ovec,alg_stack.back().typ);
        alg_stack.pop_back(); // ok, because previous line
      }
      else {
        break;
      }
    }
    alg_stack.push_back(alg_stack_element());
    alg_stack.back().typ = '$';
    alg_stack.back().ind = contentstart-lnstart;
    if (my=='#') {
      annotate_scan(ivec,ovec,contentstart);
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"namespace ")) {
      alg_stack.back().typ = 'n';
      annotate_scan(ivec,ovec,contentstart);
      ovec->push_back('{');
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"if ")) {
      alg_stack.back().typ = 'i';
      push_many(ovec,"if(");
      contentstart += 3;
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec,"){");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"else")) {
      my = osv(ivec,contentstart+4);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = 'e';
        annotate_scan(ivec,ovec,contentstart);
        ovec->push_back('{');
        ovec->push_back(10);
        continue;
      }
    }
    if (forward_match(ivec,contentstart,"elif ")) {
      alg_stack.back().typ = 's';
      push_many(ovec,"else if(");
      contentstart += 5;
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec,"){");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"for ")) {
      alg_stack.back().typ = 'f';
      push_many(ovec,"for(");
      contentstart += 4;
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec,"){");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"while ")) {
      alg_stack.back().typ = 'w';
      push_many(ovec,"while(");
      contentstart += 6;
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec,"){");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"set_template ")) {
      alg_stack.back().typ = 'T';
      unsigned long long hold = contentstart;
      push_many(ovec,"_?r1");
      contentstart += 13;
      annotate_scan(ivec,ovec,contentstart);
      ovec->push_back(';');
      contentstart = hold;
      push_many(ovec,"_?r3");
      contentstart += 13;
      annotate_scan(ivec,ovec,contentstart);
      ovec->push_back(';');
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"set_class ")) {
      alg_stack.back().typ = 'L';
      push_many(ovec,"_?r2");
      contentstart += 10;
      annotate_scan(ivec,ovec,contentstart);
      ovec->push_back(';');
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"func ")) {
      alg_stack.back().typ = 'F';
      push_many(ovec,"_?C_?1 ");
      contentstart += 4;
      unsigned long long fi = func_insert(ivec,contentstart);
      while(true) {
        my = osv(ivec,contentstart);
        if ((my==10) || (my==13)) {
          break;
        }
        ovec->push_back(my);
        if (contentstart==fi) {
          push_many(ovec," _?2 ");
        }
        contentstart += 1;
      }
      push_many(ovec," _?f { _?d");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"class ")) {
      alg_stack.back().typ = 'C';
      push_many(ovec,"_?c_?3");
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec," { _?C");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"struct ")) {
      alg_stack.back().typ = 'S';
      push_many(ovec,"_?c_?3");
      annotate_scan(ivec,ovec,contentstart);
      push_many(ovec," {");
      ovec->push_back(10);
      continue;
    }
    if (forward_match(ivec,contentstart,"dec")) {
      my = osv(ivec,contentstart+3);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = '1';
        push_many(ovec,"_?c private:");
        ovec->push_back(10);
        continue;
      }
    }
    if (forward_match(ivec,contentstart,"private")) {
      my = osv(ivec,contentstart+7);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = '2';
        push_many(ovec,"_?c private:");
        ovec->push_back(10);
        continue;
      }
    }
    if (forward_match(ivec,contentstart,"public")) {
      my = osv(ivec,contentstart+6);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = '3';
        push_many(ovec,"_?c public:");
        ovec->push_back(10);
        continue;
      }
    }
    if (forward_match(ivec,contentstart,"honly")) {
      my = osv(ivec,contentstart+5);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = '[';
        push_many(ovec,"_?c");
        ovec->push_back(10);
        continue;
      }
    }
    if (forward_match(ivec,contentstart,"cpponly")) {
      my = osv(ivec,contentstart+7);
      if ((my==10) || (my==13) || (my==32) || (my==9)) {
        alg_stack.back().typ = ']';
        push_many(ovec,"_?h");
        ovec->push_back(10);
        continue;
      }
    }
    // not a keyword
    annotate_scan(ivec,ovec,contentstart);
    ovec->push_back(';');
    ovec->push_back(10);
  }
  while (alg_stack.size()!=0) {
    steg_close(ovec,alg_stack.back().typ);
    alg_stack.pop_back();
  }
  return ou;
}

gav<char> steg_big(gav<char> inp) {
  gav<unsigned long long> lines = find_lines(inp.p());
  return annotate(inp,lines);
}

#endif
