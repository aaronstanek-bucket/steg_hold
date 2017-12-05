#include "SIMPLETRACEBACKDEBUGGER.h"
using namespace std;
namespace SIMPLETRACEBACKDEBUGGER {
void update(unsigned long long ln)  {
stack.back().linenum = ln;
} 
coin::coin(const char * fdata)  {
datapoint dp;
dp.fdata = fdata;
dp.linenum = 0;
stack.push_back(dp);
} 
coin::~coin()  {
stack.pop_back();
} 
void do_traceback()  {
cout << endl << endl;
cout << "START ERROR TRACEBACK" << endl;
for (unsigned long long i=0;i<stack.size();i++) {
cout << stack[i].fdata << " : " << stack[i].linenum << endl;
}
cout << "END ERROR TRACEBACK" << endl;
} 
void deal_with_exceptions()  {
do_traceback();
cout << "..." << endl;
cout << "UNCAUGHT EXCEPTION";
exit(0);
} 
void deal_with_segfault(int err)  {
do_traceback();
cout << "..." << endl;
cout << "SEGFAULT";
exit(0);
} 
setup::setup()  {
set_terminate(deal_with_exceptions);
signal(SIGSEGV,deal_with_segfault);
} 
}
