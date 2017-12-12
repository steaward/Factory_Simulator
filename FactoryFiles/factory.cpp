#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h> 
#include "util.h"
#include "o.h"
#include "i.h"
#include "t.h"
#include "job.h"
#include "machine.h"

using namespace std;

class Factory {
  vector<Machine> machineList;
public:
  Factory(itemManager& im, OrderManager& om, TaskManager& tm)
  {

    srand(time(0)); 

    //step#1: load machines into the factory
    for (size_t t = 0; t < tm.size(); t++)
    {
      machineList.push_back(move(Machine(tm.getTask(t))));
    }

    //step#2: find source machine		
    auto MachineFind = [&](string name) -> Machine*
    {
      for (size_t i = 0; i < machineList.size(); i++)
      {
        if (machineList[i].getName() == name)
        {
          return &machineList[i];
        }

      }
      return nullptr;
    };

    for (auto m : machineList)
    {
      string pass = m.getPass();
      if (!pass.empty())
      {
        MachineFind(pass)->incInBoundCount();
      }

      string fail = m.getFail();
      if (!fail.empty())
      {
        MachineFind(fail)->incInBoundCount();
      }

    }

    int linenumber = 0;
    int source = -1;
    for (size_t m = 0; m < machineList.size(); m++)
    {
      linenumber++;

      cout << linenumber << ": " << machineList[m].getName();
      if (machineList[m].isSingleton())
      {
        cout << " SINGLETON";
      }

      else
      {
        if (machineList[m].isSource())
        {
          cout << " SOURCE";
          if (source == -1)//first source
          {
            source = m;
          }
          else
          {
            throw string("MULTIPLE SOURCES. FIX DATA\n");
          }
        }
        if (machineList[m].isSink())
        {
          cout << " SINK";
        }
      }

      cout << "\n";
    }

    //step3: load jobs into the input queue for the source machine
    cout << "Source machine is " << source << "\n";
    if (source == -1)
    {
      throw string("No source node. Fix data and re-submit\n");
    }

    for (size_t o = 0; o < om.getSize(); o++)
    {
      machineList[source].AddJob(Job(om.getOrder(o)));
    }

    linenumber = 0;
    for (auto m : machineList)
    {
      linenumber++;
      cout << linenumber << ": " << m.getName() << " has " << m.inptQSize() << " jobs\n";
    }

    for (auto &m : machineList)
    {
      m.Classify(im);
    }

    //step 4: time loop
    int time = 0;
    while (true)
    {
      cout << "TIME = " << time << "\n";
      //step 5: any machine have something in its input Q?
      bool AllDone = true;
      for (auto m : machineList)
      {
        if (m.inptQSize())
        {
          AllDone = false;
          break;
        }
      }

      if (AllDone)
      {
        cout << "All Done\n";
        break;//break out of time loop
      }

      cout << "Not all done\n";
      //machine loop for each machine
      for (auto &m : machineList)
      {
        if (m.inptQSize() == 0)
        {
          continue;
        }

        Job job = m.getJob();

        cout << "'" << m.getName() << "' will work on job "
          << job.getCustomer() << "/"
          << job.getProduct() << "\n";

        if (m.isSink())
        {
          cout << "Job arrived at '" << m.getName() << "' a sink - nothing to do.\n";
          cout << "Job existing system\n";
          continue;
        }

        bool didSomething = false;

        if (m.getInstaller())
        {
          cout << "'" << m.getName() << "' is an installer\n";
          for (size_t i = 0; i < job.size(); i++)
          {
            if (job.Installed(i))
            {
              continue;
            }
            string itemName = job.getItem(i);
            Item* itemPtr = im.find(itemName);
            if (m.getName() == itemPtr->getInstaller())
            {
              job.Installed(i, true);
              didSomething = true;
              time++;
              cout << "'" << m.getName() << "' installed '" << itemName << "'\n";
              break;
            }
          }
        }
        if (m.getRemover())
        {
          cout << "'" << m.getName() << "' is a remover\n";
          for (int i = job.size() - 1; i >= 0; i--)
          {
            if (!job.Installed(i))
            {
              continue;
            }
            string itemName = job.getItem(i);
            Item* itemPtr = im.find(itemName);
            if (m.getName() == itemPtr->getRemover())
            {
              job.Installed(i, false);
              didSomething = true;
              time++;
              cout << "'" << m.getName() << "' removed '" << itemName << "'\n";
              break;
            }
          }
        }

        auto MovePass = [&]
        {
          string pass = m.getPass();
          cout << "moving job to pass machine '" << pass << "'\n";
          for (auto &m2 : machineList)
          {
            if (m2.getName() == pass)
            {
              m2.AddJob(job);
              break;
            }
          }
        };
        auto MoveFail = [&]
        {
          string fail = m.getFail();
          cout << "moving job to fail machine '" << fail << "'\n";
          for (auto &m2 : machineList)
          {
            if (m2.getName() == fail)
            {
              m2.AddJob(job);
              break;
            }
          }
        };
        if (!didSomething)
        {
          MovePass();
          continue;//end of this machine
        }
        if (m.getFail().empty())
        {
          MovePass();
          continue;//end of this machine
        }
        if (rand() & 1)
        {
          //pass
          //move job to pass machine inputQ
          MovePass();
          continue;
        }
        else
        {
          //fail
          //move job to fail machine inputQ
          MoveFail();
          continue;//end of this machine
        }

      }//machine loop


    }//time loop

    cout << "SIMULATION OVER\n";
    cout << "TIME = " << time << "\n";
  }
};


int main(int argc, char**argv)
{
  try
  {
    if (argc != 5) {
      throw string("usage: ") + argv[0] + " itemfile.dat orderfile.dat taskfile.dat(all csv file) delimiter-char";
      return 2;
    }

    string filenameItem = argv[1];
    string filenameOrder = argv[2];
    string filenameTask = argv[3];
    char delim = argv[4][0];

    vector<vector<string> > csvTaskData;
    vector<vector<string> > csvOrderData;
    vector<vector<string> > csvItemData;

    csvRead(filenameItem, delim, csvItemData);
    csvRead(filenameOrder, delim, csvOrderData);
    csvRead(filenameTask, delim, csvTaskData);


    itemManager im(csvItemData);
    im.print();
    im.graph(filenameItem);

    OrderManager om(csvOrderData);
    om.print();
    om.graph(filenameOrder);

    TaskManager tm(csvTaskData);
    tm.print();
    tm.graph(filenameTask);

    bool GoodToGo = true;

    try
    {
      tm.validate();
    }
    catch (string& m)
    {
      GoodToGo = false;
      cerr << m << '\n';
    }
    try
    {
      im.validate(tm);
    }
    catch (string& m)
    {
      GoodToGo = false;
      cerr << m << '\n';
    }
    try
    {
      om.validate(im);
    }
    catch (string& m)
    {
      GoodToGo = false;
      cerr << m << '\n';
    }
    if (!GoodToGo)
    {
      cerr << "*********************************\n";
      cerr << "* DATA ERRORS: fix and resubmit *\n";
      cerr << "*********************************\n";
      exit(1);
    }
    cout << "*************************************\n";
    cout << "* NO DATA ERRORS: we are good to go *\n";
    cout << "*************************************\n";
    Factory factory(im, om, tm);
  }
  catch (const string& m)
  {
    cerr << m << '\n';
  }

  return 0;
}