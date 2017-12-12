#include <iostream>
#include "string"
#include "fstream"
#include "vector"

#include "util.h"

using namespace std;

void trim(std::string& s) {
  while (!s.empty() && s[0] == ' ') {
    s.erase(0, 1); //delete 1st character
  }
  while (!s.empty() && s[s.size() - 1] == ' ') {
    s.erase(s.size() - 1, 1); //delete last character
  }
}

void csvRead(std::string& fileName, char separator, std::vector< std::vector<std::string> > &csvData) {

  std::fstream is(fileName, std::ios::in);
  if (!is.is_open())
    throw std::string("Cannot open file '") + fileName + "'";

  std::string line;


  while (getline(is, line)) {

    auto cr = line.find('\r');
    if (cr != std::string::npos)
      line.erase(cr);

    //cout << line << endl;

    std::string field;
    size_t index = 0;
    std::vector<std::string> fields;

    while (index < line.size()) {

      while (index < line.size() && line[index] != separator) {
        field += line[index];
        index++;
      }
      //cout << field << endl;
      index++;//skip separator

      trim(field); //get rid of spaces
      fields.push_back(move(field));
    }
    for (auto e : fields)
      std::cout << "[" << e << "] ";
    std::cout << std::endl;

    csvData.push_back(move(fields));
  }

  is.close();


}

void csvPrint(std::vector<std::vector<std::string>> &csvData) {
  for (auto row : csvData) {
    for (auto column : row) {
      std::cout << "<" << column << "> ";
    }
    std::cout << std::endl;
  }

  for (auto row = csvData.begin(); row != csvData.end(); row++) {
    for (auto column = row->begin(); column != row->end(); column++) {
      std::cout << "[" << *column << "] ";
    }
    std::cout << std::endl;
  }

  for (size_t row = 0; row < csvData.size(); row++) {
    for (size_t column = 0; column < csvData[row].size(); column++) {
      std::cout << "{" << csvData[row][column] << "} ";
    }
    std::cout << std::endl;
  }
}

bool validTaskName(std::string &s) {
  for (auto c : s)
    if (!(isalnum(c) || c == ' '))
      return false;
  return true;
}

bool validSlots(std::string &s) {
  for (auto c : s)
    if (!(isdigit(c)))
      return false;
  return true;
}

bool validItemName(std::string &s)
{
  for (auto c : s)
    if (!(isalnum(c)) || c == ' ') {
      return false;
    }
  return true;
}

bool validSequence(std::string &s)
{
  for (auto c : s)
    if (!(isdigit(c)))
      return false;
  return true;
}

bool validOrderName(std::string &s) {
  for (auto c : s)
    if (!(isalnum(c)) && c != ' ') {
      return false;
    }
  return true;

}
bool validCustomerName(std::string &s) {
  for (auto c : s)
    if (!(isalnum(c)) && c != ' ') {
      return false;
    }
  return true;

}