#ifndef __QUICK_SAVE_INCLUDED__
#define __QUICK_SAVE_INCLUDED__

#include "ga_rc.h"
#include <fstream>

using namespace std;

namespace quick_save {

  gav<char> readfile(const char * filename) {
    gav<char> ou;
    ou.make_new();
    vector<char> * da = ou.p();
    fstream infile;
    infile.open(filename,std::fstream::in|std::fstream::binary);
    if (infile.is_open()==false) {
      throw "file dne";
    }
    infile.seekg(0,ios::end);
    unsigned long long filesize = infile.tellg();
    if (filesize==0) {
      da->resize(0);
      infile.close();
      return ou;
    }
    da->resize(filesize);
    infile.seekg(0,ios::beg);
    infile.read(&(da->at(0)),filesize);
    infile.close();
    return ou;
  }

  void writefile(const char * filename, const char * data, unsigned long long datasize) {
    fstream outfile;
    outfile.open(filename,std::fstream::out|std::fstream::binary|std::fstream::trunc);
    outfile.seekp(0,ios::beg);
    outfile.write(data,datasize);
    outfile.close();
  }

  void writefile(const char * filename, gav<char> tofile) {
    if (tofile.isdef()==false) {
      throw "input to writefile function must be defined";
    }
    unsigned long long datasize;
    datasize = tofile.p()->size();
    if (datasize==0) {
      writefile(filename,"",0);
    }
    else {
      writefile(filename,&(tofile.p()->at(0)),datasize);
    }
  }

  void writefile(const char * filename, vector<char> * tofile) {
    unsigned long long datasize;
    datasize = tofile->size();
    if (datasize==0) {
      writefile(filename,"",0);
    }
    else {
      writefile(filename,&(tofile->at(0)),datasize);
    }
  }

}

#endif
