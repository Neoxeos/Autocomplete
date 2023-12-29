// AutocompleteList.h
//
// <Insert your header here>
//
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct Term {
    string query;
    long long int weight;
    // need this for autograding
    bool operator==(const Term& term) const {
        return (query == term.query && weight == term.weight);
    }
    // defining < operator for Term query
    bool operator<(const Term& term2){
      return (this->query < term2.query);
    }
};

//
// AutocompleteList
//
class AutocompleteList {
 private:
    //
    // Private member variables for the abstraction.
    // This implementation uses a low-level C array, terms, to store a list of
    // Terms.  As a result, you must also keep track of the number of elements
    // stored (size).
    //
    Term* terms;  // pointer to a C-style array
    int size;  // number of elements in the array
    string sortType;  // records how list is sorted

    Term* getTerms(){
      return terms;
    }

    /**File reader function
    * description: Opens the file to read in data to build N-gram map of
    
    * @Input: String: FileName.
    
    * @Output: Int: indicating if file opened successfully.
                    0 -> file not opened succesffuly
                    1 -> file opened successfully
    **/
    int readFile(string& fileName){
      ifstream fileIS;
    
      // attempt to open file
      fileIS.open(fileName);
    
      if (!fileIS.is_open()) {
        return 0; // indicates error
      }
    
    
      // Once our file is opened successfully, we return 1
      fileIS.close();
      return 1;
    }
    
    
    //
    // Private member function.
    // Returns the index of the first word in terms[] that contains the prefix,
    // or -1 if there are no words containing the prefix. This search should be
    // implemented using a variation of the binary search algorithm.
    // Performance requirement: O(logn), n is size.
    //
    
    int firstIndexOf(string prefix) {
       int mid = 0;
       int low = 0;
       int len = prefix.length(); //we want to search up to len of prefix
       int high = size - 1;
       int firstIndex = -1;
       while (high >= low) {
          mid = (high + low) / 2;
          if (terms[mid].query.substr(0, len) < prefix) { // this is to only search words up until the prefix
             low = mid + 1;
          }
          else if (terms[mid].query.substr(0, len) > prefix) {
             high = mid - 1;
          }
          else{// found
             firstIndex = mid;
             high = mid - 1;
          }
       }
      return firstIndex;
    }
    
    //
    // Private member function.
    // Returns the index of the last word in terms[] that contains the prefix,
    // or -1 if there are no words containing the prefix. This search should be
    // implemented using a variation of the binary search algorithm.
    // Performance requirement: O(logn), n is size.
    //
    int lastIndexOf(string prefix) {
       int mid = 0;
       int low = 0;
       int len = prefix.length();
       int high = size - 1;
       int lastIndex = -1;
       while (high >= low) {
          mid = (high + low) / 2;
          if (terms[mid].query.substr(0, len) < prefix) {
             low = mid + 1;
          }
          else if (terms[mid].query.substr(0, len) > prefix) {
             high = mid - 1;
          }
          else {// found
             lastIndex = mid;
             low = mid + 1;
          }
       }
      return lastIndex;
    }
    
    //
    // Private member function.
    // This function sorts the terms array by lexicographic order of the query
    // string.
    // Set sortType to "lex".
    // Performance requirement: O(nlogn), n is size.
    //
    void sortByLexOrder() {
      sortType = "lex";
      sort(terms, terms + size, 
        [](const Term &term1, const Term &term2){ //Lambda function
          return term1.query < term2.query; // sorted in ascending order
        });
    }

    //
    // Private member function.
    // This function sorts the terms array by decreasing weight order.
    // Set the sortType to "weight".
    // Performance requirement: O(nlogn), n is size.
    //
    void sortByWeightOrder() { 
      sortType = "weight";
      sort(terms, terms + size,
        [](const Term& term1, const Term& term2){
          return term1.weight > term2.weight; // sorted in decreasing order
        });
    }
    
 public:
    //
    // default constructor:
    //
    // Called automatically by C++ to create a AutocompleteList.
    // When this is called, intialize terms to a nullptr and set size to 0.
    // Set sortType to "none".
    // Performance requirement: O(1)
    //
    AutocompleteList() {
        size = 0;
        sortType = "none";
        terms = NULL;
        this->sortByLexOrder();
    }
    
    //
    // a second constructor:
    //
    // Parameter passed in determines size of terms.
    // When this is called, allocate memory for n Terms and
    // set size accordingly.
    // Set sortType to "none".
    // Performance requirement: O(1)
    //
    AutocompleteList(int n) {
        sortType = "none";
        size = n;
        terms = new Term[n]; //heap allocated here
        this->sortByLexOrder();
    }
    
    //
    // a third constructor:
    //
    // Parameter passed in is name of file to read in.
    // This constructor will build Autocomplete list from file.
    // Assume file format is:
    // line 1: "<size>"
    // lines 2 thru size+1: "weight query"
    // The list should be sorted lexicographic order by the query string.
    // Set sortType to "lex".
    // Performance requirement: O(nlogn), n is size.
    //
    AutocompleteList(string filename) {
        int sizeTerms;
        int count = 0;
        Term input;
        string result;
        if (readFile(filename) != 1){ // file error check
          this->size = -1;
          return; // did not work
        } else {
          // build our list
          ifstream fileIS;
          fileIS.open(filename);
          fileIS >> size; // first line
          terms = new Term[size];
        
          while (!fileIS.eof()){
            fileIS >> input.weight;
            if (!fileIS.fail()){
              fileIS >> ws; // remove leading whitespaces
              getline(fileIS, input.query);            
              terms[count] = input;
              count++;
            }
          }
  
          // sort our list
          sortType = "lex";
          this->sortByLexOrder();
  
          // close our file
          fileIS.close();
      }
    }
    
    //
    // copy constructor:
    //
    // Called automatically by C++ to create an AutocompleteList that contains
    // a copy of an existing AutocompleteList.  Example: this occurs when
    // passing AutocompleteList as a parameter by value.
    // Performance requirement: O(n), n is size.
    //
    AutocompleteList(const AutocompleteList& other) {
        this->size = other.size;
        this->terms = new Term[this->size];
        for (unsigned int i =0; i < other.size; i++){
          this->terms[i] = other.terms[i];
        }
        this->sortType = other.sortType;  
        this->sortByLexOrder();
    }
    
    //
    // copy operator=
    //
    // Called when you assign one AutocompleteList into another,
    // i.e. this = other;
    // Performance requirement: O(n), n is size.
    //
    AutocompleteList& operator=(const AutocompleteList& other) {
        // alternative way
        // *this = AutocompleteList(other);

        // chosen way (independent from copy constructor better design)
        this->size = other.size;
        this->terms = new Term[this->size];
        this->sortType = other.sortType;
        for (unsigned int i =0; i < other.size; i++){
          this->terms[i] = other.terms[i];
        }
        return *this;  
    }
    
    //
    // destructor:
    //
    // Called automatically by C++ to free the memory associated by the Term.
    //
    virtual ~AutocompleteList() {
        // TODO: Write this destructor (required).
        delete []terms; 
        
    }
    
    //
    // Public member function.
    // Returns the size of the AutocompleteList.
    // Performance requirement: O(1).
    //
    int getSize() {
        return size;
    }
    
    //
    // Public member function.
    // Returns Term element in AutocompleteList.
    // This gives public access to Terms stored in the AutocompleteList.
    // If i is out of bounds, throw an out_of_range error message:
    // "AutocompleteList: i out of bounds"
    // Note:  This public function does not necessarily fit the design of this
    // abstraction but we are having you write it for testing purposes.
    // Performance requirement: O(1).
    //
    Term& operator[](int i) {
        return *(terms + i);
    }


    // Public member function.
    // Returns an AutocompleteList which stores a list of all Terms that
    // start with the prefix.  The AutocompleteList returned should be sorted
    // in descending order by weight.  Set the returned AutocompleteList's
    // sortType to "weight".
    // If prefix is empty string, return an empty AutocompleteList;
    // If there are no words in the list the start with the prefix, return an
    // empty AutocompleteList.
    // If *this* AutocompleteList's sortType does not equal "lex", then return
    // an empty AutocompleteList.  In other words, allMatches should only be
    // called on an AutocompleteList that is sorted in lexicographic order by
    // the query string.
    // Performance requirement: O(mlogm+logn), n is size and
    // where m is the number of matching terms.
    //
    AutocompleteList allMatches(string prefix) {
        AutocompleteList result = AutocompleteList(numberOfMatches(prefix));
        int count = 0;
        if (prefix.empty()){
          return result;
        }
        if (this->sortType != "lex"){
          return result;
        }
        int low = firstIndexOf(prefix);
        int high = lastIndexOf(prefix);
        if (low == -1 || high == -1){ // word not found
          return result;
        }
      
        for (int i = low; i < high + 1; ++i){
          result.terms[count] = this->terms[i];
          count++;
        }
        result.sortType = "weight";
        result.sortByWeightOrder();
        return result; 
    }



    // Public member function.
    // Returns the number of Terms that start with the given prefix.
    // If prefix is empty, return 0.
    // If *this* AutocompleteList's sortType does not equal "lex",
    // then return 0.
    // Performance requirement: O(logn), n is size.
    //
    int numberOfMatches(string prefix) {
        if (prefix.empty()) {
          return 0;
        }
        if (this->sortType != "lex"){
          return 0;
        }
      
        int low = firstIndexOf(prefix);
        int high = lastIndexOf(prefix);
        if (low == -1 || high == -1){
          return 0;
        }
        return high - low + 1;
    }



    //
    // Public member function.
    // Prints the AutocompleteList.
    // Pad the front of the print with 8 spaces for nicely formatted search:
    // print: "        query, weight".
    // NOTE:  This is also useful for debugging purposes.
    // Performance requirement: O(n), n is size
    //
    void print() {
      for (int i = 0; i < size; ++i){
        // cout << "        " << "in position: " << i << " " << terms[i].query << ", " << terms[i].weight << endl;

          cout << "        "  << terms[i].query << ", " << terms[i].weight << endl;
      }
    }
};
