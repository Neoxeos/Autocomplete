//
//  application.cpp
//  <Your header goes here.>
//

#include "AutocompleteList.h"
#include <iostream>

using namespace std;

int main() {
  string fileName;
  char type;
  string search;
  cout << "Enter filename: " << "Enter s-search or c-creative search: " << "Welcome!\n";
  cin >> fileName;
  // while (myList.getSize() == -1){h
  //   cout << "Wrong filename, Enter filename:\n";
  //   cin >> fileName;
  //   AutocompleteList myList2 = AutocompleteList(fileName);
  //   cout << myList2.getSize() << endl;
  // }
  // cout << "Enter s-search or c-creative: ";
  cin >> type;
  AutocompleteList myList = AutocompleteList(fileName);
  myList.print();
  cout << "Type your search below, press enter for autocomplete (# to stop).\n";
  while (true) {
    if (type == 's') {
      getline(cin, search);
      if (search == "#"){
        break;
      }
      myList.allMatches(search).print();
      cout << "Search: \n";
    }
   if (type == 'c'){
      getline(cin, search);
      if (search == "#"){
        break;
      }
      myList.allMatches(search).print();
      cout << "Search: \n";
   }
  }
  return 0;
}