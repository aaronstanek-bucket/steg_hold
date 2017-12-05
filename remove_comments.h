#ifndef __REMOVE_COMMENTS_INCLUDED__
#define __REMOVE_COMMENTS_INCLUDED__

#include <vector>
#include "uneasy_values.h"

using namespace std;

void remove_comments(vector<char> * inp) {
  bool in_comment = false;
  bool in_block_comment = false;
  bool in_single_quote = false;
  bool in_double_quote = false;
  bool skip = false;
  unsigned long long siz = inp->size();
  char now;
  for (unsigned long long i=0;i<siz;i++) {
    now = uv(inp,i);
    if (now==10) {
      in_comment = false;
    }
    else if (now==13) {
      (inp->at(i)) = 32;
    }
    else if (in_comment) {
      (inp->at(i)) = 32;
    }
    else if (in_block_comment) {
      if (now=='*') {
        if (uv(inp,i+1)=='/') {
          (inp->at(i+1)) = 32;
          in_block_comment = false;
        }
      }
      (inp->at(i)) = 32;
    }
    else if (in_single_quote) {
      if (now=='\'') {
        if (skip) {
          skip = false;
        }
        else {
          in_single_quote = false;
        }
      }
      if (now=='\\') {
        if (uv(inp,i+1)=='\'') {
          skip = true;
        }
      }
    }
    else if (in_double_quote) {
      if (now=='"') {
        if (skip) {
          skip = false;
        }
        else {
          in_double_quote = false;
        }
      }
      if (now=='\\') {
        if (uv(inp,i+1)=='"') {
          skip = true;
        }
      }
    }
    else {
      if (now=='\'') {
        in_single_quote = true;
      }
      else if (now=='"') {
        in_double_quote = true;
      }
      else if ((now=='/') && (uv(inp,i+1)=='/')) {
        in_comment = true;
        (inp->at(i)) = 32;
      }
      else if ((now=='/') && (uv(inp,i+1)=='*')) {
        in_block_comment = true;
        (inp->at(i)) = 32;
      }
    }
  }
}

#endif
