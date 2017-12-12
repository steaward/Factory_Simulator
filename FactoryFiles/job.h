#pragma once
#include "o.h"
class Job : public Order
{
  vector<bool> installed;
public:
  Job() {}
  Job(Order* o) :Order(*o){
    installed.resize(size()); //allocate space
    for (size_t i = 0; i < size(); i++)
    {
      installed[i] = false; //initialize to false(not installed)
    }
  }
  ~Job() {}
  bool Installed(size_t i) { return installed[i]; }
  void Installed(size_t i, bool value) { installed[i] = value; }
};