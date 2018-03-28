//
//  HTMLparser.hpp
//  Excel converter
//
//  Created by Sam Ferguson on 2018-03-26.
//

#ifndef HTMLparser_hpp
#define HTMLparser_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

namespace SKFhtml {

class HTMLparser{
private:
    string m_filename{""};
    string m_outFilename{""};
    string m_relLine{""};
    int m_ColNum{0};
    vector<string> m_Col;
    vector<vector<string> > m_ColData;
    
public:
    //default constructor
    HTMLparser();
    
    //constructor
    HTMLparser(string filename);
    
    // get our relevent line from the file takes a search term to find what that line is
    void setLine(string searchString);
    
    //convert that line into a number of columns
    void setCol();
    
    //extract cells from each columns
    void parseCol();
    
    //convert dates to week-month-day format only works if date is in format like "Mar 4, 2018" takes an int pointing to the cell number the date can be found in
    void convertDate(const int elNumb);
    
    //return all the data extracted in a vector of string vectors
    vector<vector<string> > getColData() const;
    
    //output our csv file takes a name for the file, a pointer to an int array cointaining the cell numbers we want printed, the size of that array and what column to start printing at
    void printFile(const string outFileName, const int arr[], const int size, const int start);
    
};

    
}


#endif /* HTMLparser_hpp */
