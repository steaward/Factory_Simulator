#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void csvPrint( vector< vector<string> > & csvData)
{
  cout << "range-based for:\n";
  for(auto line : csvData) {
    for(auto field : line)
      cout << "(" << field << ")";
    cout << "\n";
  }
  cout << "\n";

  cout << "for with iterators:\n";
  for(auto line = csvData.begin(); line != csvData.end(); line++) {
    for(auto field = line->begin(); field != line->end(); field++)
      cout << "[" << *field << "]";
    cout << "\n";
  }
  cout << "\n";

  cout << "for with indices:\n";
  for(size_t line = 0; line < csvData.size(); line++) {
    for(size_t field = 0; field < csvData[line].size(); field++)
      cout << "<" << csvData [line] [field] << ">";
    cout << "\n";
  }
  cout << "\n";

}

void TrimSpaces(string& s)
{
  while(! s.empty() && s[0] == ' ')
    s.erase(0, 1);

  while(! s.empty() && s[s.size()-1] == ' ')
    s.erase(s.size()-1, 1);
}

void csvRead(string& filename, char delimiter, vector< vector<string> > & csvData)
{

  cout << "filename='"<< filename <<"', delimiter=<"<<delimiter<<">\n";

  fstream in(filename, ios::in);
  if(in.is_open()) {

    string line;
    while(getline(in,line)) {
      auto cr = line.find( '\r' );
      if(cr != string::npos)
        line.erase( cr );

      string field;
      size_t index = 0;
      vector< string > fields;
      while(index < line.size()) {
        if(line[index] == delimiter) {
          TrimSpaces( field );
          fields.push_back( move(field) );
        } else {
          field += line[index];
        }
        index++;
      }
      TrimSpaces( field );
      fields.push_back( move(field) );

      csvData.push_back( move(fields) );
    }

    in.close();
  } else {
    throw string("Cannot open file '") + filename + "'";
  }
}

int main(int argc, char* argv[])
{
  if(argc != 3) {
    cerr << "Usage: " << argv[0] << " csv-file csv-delimiter\n";
    return 1;
  }

  string filename  = argv[1];
  char   delimiter = argv[2] [0];

  try {
    vector< vector<string> > csvData;
    csvRead(filename, delimiter, csvData);
    csvPrint(csvData);
  } catch(const string& e) {
    cerr << "It threw! -->" << e << "\n";
    return 2;
  }

}
