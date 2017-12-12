#pragma once

#include <iostream>
#include "string"
#include "fstream"
#include "vector"

using namespace std;

#include "util.h"
#include "i.h"
//#include "o.h"
//#include "t.h"

class Order {
  string orderCustomer, orderProduct;
  vector<string> itemList;
public:
  Order();

  Order(std::vector<std::string> &row);

  ~Order();

  void print();

  void graph(fstream& os);

  std::string& getCustomer() { return orderCustomer; }
  std::string& getProduct() { return orderProduct; }
  size_t size() { return itemList.size(); }
  std::string& getItemNumber(int index) { return itemList[index]; }

}; //class order

class OrderManager {
  std::vector<Order> orderList;

public:
  OrderManager(std::vector<std::vector<std::string>> &csvData);

  void print();

  void graph(std::string& filename);

  bool validate(itemManager& im) {
  
    int errors = 0;

    for (auto o : orderList) { //for each order
      for (size_t i = 0; i < o.size(); i++) { //for each item in that order
        if (im.find(o.getItemNumber(i)) == nullptr) {
          std::cerr << o.getCustomer() << ": " << o.getProduct()
            << "cannot find item number '" << o.getItemNumber(i) << "'\n";
          errors++;
        }
      }
    }

    return errors == 0;
  }
 

}; //class orderManager