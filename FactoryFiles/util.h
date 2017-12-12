#pragma once

#include <iostream>
#include "string"
#include "fstream"
#include "vector"

//using namespace std;

void trim(std::string& s);

void csvRead(std::string& fileName, char separator, std::vector< std::vector<std::string> > &csvData);

void csvPrint(std::vector<std::vector<std::string>> &csvData);

bool validTaskName(std::string &s);

bool validSlots(std::string &s);

bool validItemName(std::string &s);

bool validSequence(std::string &s);

bool validOrderName(std::string &s);

bool validCustomerName(std::string &s);