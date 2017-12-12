#pragma once
#include "t.h"
#include <queue>
class Machine : public Task{
  int inBoundCount;
  bool bInstaller;
  bool bRemover;
  std::queue<Job> inputQ;
public:
  Machine() :inBoundCount(0) {}
  Machine(Task* t) :Task(*t), inBoundCount(0) {}
  bool isSink() { return getPass().empty(); }
  bool isSource() { return inBoundCount == 0; }
  bool isSingleton() { return isSink() && isSource(); }
  void incInBoundCount() { inBoundCount++; }
  void AddJob(Job j) { inputQ.push(move(j)); }

  size_t inptQSize() { return inputQ.size(); }
  Job getJob(){
    Job job = move(inputQ.front());
    inputQ.pop();
    return job;
  }

  void Classify(itemManager& im){
    bInstaller = false;
    bRemover = false;

    for (size_t i = 0; i < im.getSize(); i++)
    {
      if (getName() == im.getItem(i)->getInstaller())
      {
        bInstaller = true;
      }
      if (getName() == im.getItem(i)->getRemover())
      {
        bRemover = true;
      }
    }

    if (bInstaller)
    {
      cout << getName() << " is an installer\n";
    }
    if (bRemover)
    {
      cout << getName() << " is a remover\n";
    }

  }

  bool getInstaller() { return bInstaller; }
  bool getRemover() { return bRemover; }
};