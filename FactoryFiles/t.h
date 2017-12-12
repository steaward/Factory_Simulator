
#include <iostream>
#include "string"
#include "fstream"
#include "vector"

using namespace std;

#include "util.h"

class Task;
class TaskManager;

class Task {
  string taskName, taskSlots, taskPass, taskFail;
public:
  Task();

  Task(std::vector<std::string> &row);
 
  string& getName() { return taskName; }
  string& getPass() { return taskPass; }
  string& getFail() { return taskFail; }

  ~Task();

  void print();

  void graph(fstream& os);
}; //class Task

class TaskManager {
  std::vector<Task> taskList;

public:
  TaskManager(std::vector<std::vector<std::string>> &csvData);

  Task* find(std::string name) { 
    for (size_t i = 0; i < taskList.size(); i++) {
      if (name == taskList[i].getName())
        return &taskList[i];
    }

    return nullptr;
  }

  void print();

  void graph(std::string& filename);

  bool validate() {  
    int errors = 0;

    for (auto task : taskList) {
      if (find(task.getPass()) == nullptr) {
        errors++;
        cerr << "Task " << task.getName() << " references unknown task " << task.getPass() << endl;
      }

      if (find(task.getFail()) == nullptr) {
        errors++;
        cerr << "Task " << task.getName() << " references unknown fail task " << task.getFail() << endl;
      }
    }

    return errors == 0;
  }

}; //class TaskManager