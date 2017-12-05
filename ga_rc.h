#ifndef __GA_RC_INCLUDED__
#define __GA_RC_INCLUDED__

#include <cstdlib>
#include <vector>

using namespace std;

template <class T>
class ga_rc_datahold {
  public:
    unsigned short c;
    T data;
    void add_ref() {
      if (c>65000) {
        throw "too many refs";
      }
      c += 1;
    }
    void rm_ref() {
      if (c<2) {
        delete this;
      }
      else {
        c -= 1;
      }
    }
    ga_rc_datahold() {
      c = 0;
    }
};

template <class T>
class ga {
  private:
    ga_rc_datahold<T> * poi;
  public:
    ~ga() {
      if (poi!=NULL) {
        poi->rm_ref();
      }
    }
    ga() {
      poi = NULL;
    }
    ga(const ga<T> & s) {
      poi = s.poi;
      if (poi!=NULL) {
        poi->add_ref();
      }
    }
    ga(const T & s) {
      poi = new ga_rc_datahold<T>;
      (poi->data) = s;
      poi->add_ref();
    }
    void operator=(const ga<T> & s) {
      if (s.poi!=NULL) {
        s.poi->add_ref();
      }
      if (poi!=NULL) {
        poi->rm_ref();
      }
      poi = s.poi;
    }
    void clear() {
      if (poi!=NULL) {
        poi->rm_ref();
        poi = NULL;
      }
    }
    void make_new() {
      clear();
      poi = new ga_rc_datahold<T>;
      poi->add_ref();
    }
    bool isdef() {
      if (poi==NULL) {
        return false;
      }
      return true;
    }
    T * p() { // potential segfault
      return &(poi->data);
    }
    bool sameref(const ga<T> & s) {
      if (poi==s.poi) {
        return true;
      }
      return false;
    }
    bool samevalue(const ga<T> & s) { // potential segfault
      if (poi->data==s.poi->data) {
        return true;
      }
      return false;
    }
    unsigned short refcount() { // potential segfault
      return poi->c;
    }
};

template <class T>
class gav {
  private:
    ga_rc_datahold< vector<T> > * poi;
  public:
    ~gav() {
      if (poi!=NULL) {
        poi->rm_ref();
      }
    }
    gav() {
      poi = NULL;
    }
    gav(const gav<T> & s) {
      poi = s.poi;
      if (poi!=NULL) {
        poi->add_ref();
      }
    }
    gav(const T & s) {
      poi = new ga_rc_datahold< vector<T> >;
      (poi->data) = s;
      poi->add_ref();
    }
    void operator=(const gav<T> & s) {
      if (s.poi!=NULL) {
        s.poi->add_ref();
      }
      if (poi!=NULL) {
        poi->rm_ref();
      }
      poi = s.poi;
    }
    void clear() {
      if (poi!=NULL) {
        poi->rm_ref();
        poi = NULL;
      }
    }
    void make_new() {
      clear();
      poi = new ga_rc_datahold< vector<T> >;
      poi->add_ref();
    }
    bool isdef() {
      if (poi==NULL) {
        return false;
      }
      return true;
    }
    vector<T> * p() { // potential segfault
      return &(poi->data);
    }
    bool sameref(const gav<T> & s) {
      if (poi==s.poi) {
        return true;
      }
      return false;
    }
    bool samevalue(const gav<T> & s) { // potential segfault
      if (poi->data==s.poi->data) {
        return true;
      }
      return false;
    }
    unsigned short refcount() { // potential segfault
      return poi->c;
    }
};

#endif
