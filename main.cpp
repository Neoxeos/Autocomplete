#include <iostream>
#include "AutocompleteList.h";

using namespace std;

int t() {
  // AutocompleteList list1 = AutocompleteList(5); //works
  // cout << list1.getSize();
  // AutocompleteList list3;
  AutocompleteList list2 = AutocompleteList("tiny.txt");
  // list3 = list2;
  //cout << list2.getSize();
  // cout << list2[0].query << endl;
  // cout << list3[0].query << endl;

  // AutocompleteList list4 = AutocompleteList(list2);
  // cout << list4[0].query.substr(0,3);
  list2.print();
  cout << list2.numberOfMatches("spi") << endl;

  list2.allMatches("spi").print();
}