#ifndef __UNEASY_VALUES_INCLUDED__
#define __UNEASY_VALUES_INCLUDED__

#include <vector>

using namespace std;

char uv(vector<char> * inp, unsigned long long i) {
  if (i>=inp->size()) {
    return 10;
  }
  return inp->at(i);
}

#endif
