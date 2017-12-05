#ifndef __SIMPLETRACEBACKDEBUGGER_INCLUDED__
#define __SIMPLETRACEBACKDEBUGGER_INCLUDED__
#include <iostream>
#include <vector>
#include <exception>
#include <csignal>
#include <cstdlib>
using namespace std;
namespace SIMPLETRACEBACKDEBUGGER {
struct datapoint {
const char * fdata;
unsigned long long linenum;
};
vector<datapoint> stack;
void update(unsigned long long ln) ;
class coin {
public:
bool one_byte_of_memory;
coin(const char * fdata) ;
~coin() ;
};
void do_traceback() ;
void deal_with_exceptions() ;
void deal_with_segfault(int err) ;
class setup {
public:
setup() ;
};
}
#endif
