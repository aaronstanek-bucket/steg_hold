#ifndef PUSH_MANY_INCLUDED
#define PUSH_MANY_INCLUDED

void push_many(vector<char> * ovec, const char * da) {
  char my;
  unsigned long long i = 0;
  while (true) {
    my = *(da+i);
    if (my==0) {
      return;
    }
    ovec->push_back(my);
    i += 1;
  }
}

#endif
