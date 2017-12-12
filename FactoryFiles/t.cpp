#include <iostream>
#include "string"
#include "fstream"
#include "vector"

using namespace std;

#include "util.h"
#include "t.h"

class Task;
class TaskManager;

  Task::Task() : taskSlots("1")
  {

  }

  Task::Task(std::vector<std::string> &row) : taskSlots("1")
  {
    switch (row.size()) {
    case 4:
      if (validTaskName(row[3]))
        taskFail = row[3];
      else
        throw string("Expected a fail task name, found '") + row[3] + "'";
    case 3:
      if (validTaskName(row[2]))
        taskPass = row[2];
      else
        throw string("Expected a fail task name, found '") + row[2] + "'";
    case 2:
      if (validSlots(row[1]))
        taskSlots = row[1];
      else
        throw string("Expected a slot, found '") + row[1] + "'";
    case 1:
      if (validTaskName(row[0]))
        taskName = row[0];
      else
        throw string("Expected a fail task name, found '") + row[0] + "'";
      break;
    default:
      throw string("expected 1,2,3, or 4 fields - found ") + to_string(row.size());
    }
  }

  Task::~Task() {}

  void Task::print()
  {
    cout << "task /name/slots/pass/fail/ = "
      << "/" << taskName
      << "/" << taskSlots
      << "/" << taskPass
      << "/" << taskFail
      << std::endl;
  }

  void Task::graph(fstream& os)
  {
    if (!taskFail.empty()) {
      os << '"' << taskName + "\" -> \"" + taskFail + "\"[color=red];\n";
    }
    if (!taskPass.empty()) {
      os << '"' << taskName + "\" -> \"" + taskPass + "\"[color=green];\n";
    }
    if (taskFail.empty() && taskPass.empty()) {		//isolated node
      os << '"' << taskName + "\";\n";
    }
  }



TaskManager::TaskManager(std::vector<std::vector<std::string>> &csvData)
  {
    for (auto row : csvData) {
      try {
        taskList.push_back(move(Task(row)));
      }
      catch (std::string& e) {
        cerr << e << std::endl;
      }
    }
  }

  void TaskManager::print()
  {
    for (auto t : taskList)
      t.print();
  }

  void TaskManager::graph(std::string& filename)
  {
    string dotFile = filename + ".gv";
    fstream os(dotFile, std::ios::out | std::ios::trunc);

    os << "digraph taskGraph{\n";

    for (auto t : taskList) {
      t.graph(os);
    }
    os << "}\n";

    os.close();

    string cmd = "dot -Tpng " + filename + ".gv > " + filename + ".gv.png";
    //cout << "running '" << cmd << "'\n";
    system(cmd.c_str());
  }