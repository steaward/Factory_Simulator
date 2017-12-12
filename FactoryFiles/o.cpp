#include <iostream>
#include "string"
#include "fstream"
#include "vector"
#include "util.h"
#include "i.h"
#include "o.h"
#include "t.h"

using namespace std;


  Order::Order()
  {

  }

  Order::Order(std::vector<std::string> &row)
  {
    if (row.size() < 3)
      throw string("Expected at least 3 field - found ") + to_string(row.size());

    if (validCustomerName(row[0]))
      orderCustomer = row[0];
    else
      throw string("Expected a customer name - found '") + row[0] + "'";

    if (validOrderName(row[1]))
      orderProduct = row[1];
    else
      throw string("Expected a product name - found '") + row[1] + "'";

    for (size_t i = 2; i < row.size(); i++)
      if (validItemName(row[i]))
        itemList.push_back(row[i]);
  }

  Order::~Order() {}

  void  Order::print()
  {
    cout << "order/customer/product/items.../ = "
      << "/" << orderCustomer
      << "/" << orderProduct;

    for (auto item : itemList)
      cout << "/" << item;

    cout << endl;
  }

  void  Order::graph(fstream& os)
  {
    string q("\"");
    for (auto item : itemList)
      os << q + orderCustomer + ":" + orderProduct + q + " -> " + q + item + q + " [color=blue];\n";

  }


  OrderManager::OrderManager(std::vector<std::vector<std::string>> &csvData)
  {
    for (auto row : csvData) {
      try {
        orderList.push_back(move(Order(row)));
      }
      catch (std::string& e) {
        cerr << e << std::endl;
      }
    }
  }

  void OrderManager::print()
  {
    for (auto t : orderList)
      t.print();
  }

  void OrderManager::graph(std::string& filename)
  {
    string dotFile = filename + ".gv";
    fstream os(dotFile, std::ios::out | std::ios::trunc);

    os << "digraph orderGraph{\n";

    for (auto t : orderList) {
      t.graph(os);
    }
    os << "}\n";

    os.close();

    string cmd = "dot -Tpng " + filename + ".gv > " + filename + ".gv.png";
    //cout << "running '" << cmd << "'\n";
    system(cmd.c_str());
  }

