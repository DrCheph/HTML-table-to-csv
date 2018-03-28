//
//  HTMLparser.cpp
//  Excel converter
//
//  Created by Sam Ferguson on 2018-03-26.
//

#include "HTMLparser.hpp"
#include <fstream>

using namespace std;

namespace SKFhtml {
    
    //default constructor
    HTMLparser::HTMLparser():m_filename(""){
        
    }

    //constructor
    HTMLparser::HTMLparser(string filename) : m_filename(filename){
        
    }
    
    // get our relevent line from the file
    void HTMLparser::setLine(string searchString){
        
        //check to make sure there is something to search for
        if(m_filename == "" || searchString == "")
            return;
        
        //open the file
        ifstream input;
        
        input.open(m_filename);
        
        if(input.is_open()){
            //string to hold line
            string line;
            
            //loop to get each line of file
            while (!input.eof()){
                
                //gets lines of file, self advance with each output to line (second variable)
                getline(input, line);
                
                //cout line variable
                size_t Found = line.find(searchString);
                
                //if our search tearm is found in the string then extract our table
                if(Found != string::npos){
                    m_relLine = line;
                }
                
            }
            input.close();
        }
        else{
            cout<< "could not create file: " << m_filename << endl;
        }
    }
    
    //convert that line into a number of columns
    void HTMLparser::setCol(){
        
        //check to make sure we have somthing to work on
        if(m_relLine == "")
            return;
        
        size_t posStart = 1;
        size_t posEnd = 2;
        string SearchString = "<tr";
        
        
        while(true){
            
            //find the position of the begining of the row
            posStart = m_relLine.find(SearchString, posStart);
            
            //set the end position (the bigining of the next row or end of the table)
            if(m_relLine.find(SearchString, posStart+1) != string::npos){
                posEnd = m_relLine.find(SearchString, posStart+1);
            }
            else if(m_relLine.find(SearchString, posStart+1) == string::npos){
                posEnd = m_relLine.length();
            }
            else{
                cout<<"something screwed up"<<endl<<endl;
            }
            
            
            //find the lenght of the column to be copied
            size_t len = posEnd - posStart;
            
            char buffer[len];
            
            //copy the column to the buffer
            len = m_relLine.copy(buffer, len, posStart);
            
            //cap the buffer
            buffer[len] ='\0';
            
            //add column to vector
            m_Col.push_back(buffer);
            
            //check to see if there are any more columns to copy
            if(m_relLine.find(SearchString, posStart+1)== string::npos){
                m_ColNum++;
                break;
            }
            
            //set the position to the bigining of the next row and add one to the int holding our column number value
            posStart = posEnd;
            m_ColNum++;
        }
        
    }
    
    //extract cells from each columns
    void HTMLparser::parseCol(){
        
        //loop through all our columns
        for(int i=0; i<m_ColNum; i++){
            
            // make a vector to be pushed into our vector matrix
            vector<string> Coldata;
            size_t startPos = 0;
            size_t endPos = 0;
            
            //loop through all the cells in our column
            while(m_Col[i].find("<", endPos+1) != string::npos){
                
                size_t len = 0;
                
                //find the start and end of each cell
                endPos = m_Col[i].find("<", endPos + 1);
                startPos = m_Col[i].find(">", startPos + 1);
                
                if(startPos > endPos)
                    startPos = endPos - 1;
                
                //find the lenght of each cell
                len = (endPos - startPos);
                
                char buff[len];
                
                //copy the cell to our buffer and cap it
                if(len == 1){
                    buff[0] = '\0';
                }
                else{
                    len = m_Col[i].copy(buff, len-1, startPos+1);
                    buff[len] = '\0';
                }
                
                //add the buffer to the vector
                Coldata.push_back(buff);
                
            }
            //push the vector to our matrix
            m_ColData.push_back(Coldata);
            
        }
        
    }
    
    //convert dates to week-month-day format only works if date is in format like "Mar 4, 2018"
    void HTMLparser::convertDate(const int elNumb){
        
        string month;
        string day;
        string week;
        string monthNum;
        int iyear;
        int iday;
        int imonth;
        int totalDays;
        size_t daylen;
        int yearmult;
        
        //runn through all the columns
        
        for(int i = 0; i<m_ColData.size(); i++){
            
            //set the date string
            string date = m_ColData[i][elNumb];
            
            //make sure the element is not empty
            if(date != ""){

                //get the year info from string
                char buffyear[4];
                
                size_t yearLen = date.copy(buffyear, 4, date.find(",") + 2);
                
                buffyear[yearLen] = '\0';
                
                sscanf(buffyear, "%d", &iyear);
                
                //get the month info
                char buffmonth[3];
                
                size_t monthLen = date.copy(buffmonth, 3, 0);
                
                buffmonth[monthLen] = '\0';
                
                month = buffmonth;
                
                //get the day info
                char buffday[2];
                
                daylen = date.find(",") - date.find(" ") - 1;
                
                daylen = date.copy(buffday, daylen, date.find(",") - daylen );
                
                buffday[daylen] = '\0';
                
                day = buffday;
  
                sscanf(buffday, "%d", &iday);
                
                //how many years since 2017
                yearmult = iyear - 2018;
                
                //check for leap year
                if(iyear%4 == 0){
                    
                    //multiply yearmult by number of days in year
                    yearmult *= 366;
                    
                    //turn the month string into a number(still string for string vector output) and get number of day's since begining of year
                    if(month == "Jan"){
                        imonth = 0;
                        monthNum = "1";
                    }
                    else if (month == "Feb"){
                        imonth = 31;
                        monthNum = "2";
                    }
                    else if (month == "Mar"){
                        imonth = 60;
                        monthNum = "3";
                    }
                    else if (month == "Apr"){
                        imonth = 91;
                        monthNum = "4";
                    }
                    else if (month == "May"){
                        imonth = 121;
                        monthNum = "5";
                    }
                    else if (month == "Jun"){
                        imonth = 152;
                        monthNum = "6";
                    }
                    else if (month == "Jul"){
                        imonth = 182;
                        monthNum = "7";
                    }
                    else if (month == "Aug"){
                        imonth = 213;
                        monthNum = "8";
                    }
                    else if (month == "Sep"){
                        imonth = 244;
                        monthNum = "9";
                    }
                    else if (month == "Oct"){
                        imonth = 274;
                        monthNum = "10";
                    }
                    else if (month == "Nov"){
                        imonth = 305;
                        monthNum = "11";
                    }
                    else{
                        imonth = 335;
                        monthNum = "12";
                    }
                }
                else{
                    //multiply yearmult by number of days in year
                    yearmult *= 365;
                    
                    //turn the month string into a number(still string for string vector output) and get number of day's since begining of year
                    if(month == "Jan"){
                        imonth = 0;
                        monthNum = "1";
                    }
                    else if (month == "Feb"){
                        imonth = 31;
                        monthNum = "2";
                    }
                    else if (month == "Mar"){
                        imonth = 59;
                        monthNum = "3";
                    }
                    else if (month == "Apr"){
                        imonth = 90;
                        monthNum = "4";
                    }
                    else if (month == "May"){
                        imonth = 120;
                        monthNum = "5";
                    }
                    else if (month == "Jun"){
                        imonth = 151;
                        monthNum = "6";
                    }
                    else if (month == "Jul"){
                        imonth = 181;
                        monthNum = "7";
                    }
                    else if (month == "Aug"){
                        imonth = 212;
                        monthNum = "8";
                    }
                    else if (month == "Sep"){
                        imonth = 243;
                        monthNum = "9";
                    }
                    else if (month == "Oct"){
                        imonth = 273;
                        monthNum = "10";
                    }
                    else if (month == "Nov"){
                        imonth = 304;
                        monthNum = "11";
                    }
                    else{
                        imonth = 334;
                        monthNum = "12";
                    }
                }
                
                //calculate the total number of days since  september 5th 2017
                totalDays = (16 * 8) - 1 + yearmult + imonth + iday;
                
                //translate that into number of weeks
                int TotalWeeks = totalDays / 7;
                
                week = to_string(TotalWeeks);
                
                //insert data to front of our vector matrix
                m_ColData[i].insert(m_ColData[i].begin(), day);
                m_ColData[i].insert(m_ColData[i].begin(), monthNum);
                m_ColData[i].insert(m_ColData[i].begin(), week);
                
            }
        }
        
    }

    //return all the data extracted in a vector of string vectors
    vector<vector<string> > HTMLparser::getColData() const{
        vector<vector<string> > colData;
        
        colData = m_ColData;
        
        return colData;
    }
    
    //output our csv file
    void HTMLparser::printFile(const string outFileName, const int arr[], const int size, const int start){
        
        if(outFileName == "")
            return;
        
        //test code
        //            for(int i = start; i<m_ColData.size(); i++){
        //                for(int j = 0; j < size; j++){
        //
        //
        //                    if(j == size - 1){
        //                        cout<<m_ColData[i][arr[j]];
        //                    }
        //                    else{
        //                        cout<<m_ColData[i][arr[j]]<<",";
        //                    }
        //
        //                }
        //                cout<<endl;
        //            }
        //
        
        //create the file
        ofstream outFile;
        
        
        outFile.open(outFileName);
        
        if(outFile.is_open()){
            //loop through our matrix
            for(int i = start; i<m_ColData.size(); i++){
                for(int j = 0; j < size; j++){
                    
                    //check for ","'s inside the elements
                    if (m_ColData[i][arr[j]].find(",") != string::npos){
                        m_ColData[i][arr[j]].erase(m_ColData[i][arr[j]].find(","), 1);
                    }
                    
                    //add's element/"," or just element if last item in vector
                    
                    if(j == size - 1){
                        outFile<<m_ColData[i][arr[j]];
                    }
                    else{
                        outFile<<m_ColData[i][arr[j]]<<",";
                    }
                    
                }
                outFile<<endl;
            }
            outFile.close();
        }
        else{
            cout<< "could not create file: " << outFileName << endl;
        }
        
        
    }
    
} //namespace
