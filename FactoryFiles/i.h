#pragma once

#include <iostream>
#include "string"
#include "fstream"
#include "vector"

using namespace std;

//#include "i.h"
//#include "o.h"
#include "t.h"
#include "util.h"

class Item {
  string itemName, itemInstaller, itemRemover, itemSequence, itemDescription;
public:
  Item();

  Item(std::vector<std::string> &row);

  string& getName() { return itemName; }
  string& getInstaller() { return itemInstaller; }
  string& getRemover() { return itemRemover; }

  ~Item();

  void print();

  void graph(fstream& os);
};

class itemManager {
  std::vector<Item> itemList;

public:
  itemManager(std::vector<std::vector<std::string>> &csvData);

  void print();

  void graph(std::string& filename);

  Item* find(std::string& name) {
    for (size_t i = 0; i < itemList.size(); i++) {
      if (name == itemList[i].getName())
        return &itemList[i];
    }

    return nullptr;
  }


  bool validate(TaskManager& tm) {
    int errors = 0;

    for (auto item : itemList) {
      if ( tm.find(item.getInstaller()) == nullptr ) {
        errors++;
        cerr << "Item " << item.getName() << " references unknown installer task " << item.getInstaller() << endl;
      }

      if ( tm.find(item.getRemover()) == nullptr ) {
        errors++;
        cerr << "Item " << item.getName() << " references unknown remover task " << item.getRemover() << endl;
      }
    }

    return errors == 0;
  }
}; //class itemManager