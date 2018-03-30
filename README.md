# HTML-table-to-csv
A simple class that converts HTML table's to csv files


How to use:

Set the name of the htm file you want to parse in the constructor like:

HTMLparser myhtmlParser("filename");

Set the tables from the file
myhtmlParser.setTables();

Then to extract the relevent line from the with setLine and adding your search term

myhtmlParser.setLine("searchterm");

Once you have your relevent line turn your html table into a vector of columns with setCol

myhtmlParser.setCol();

Then parse through the columns and extract all the data in the cells with parseCol

myhtmlParser.parseCol();

Print the cells out to a csv file with printFile and add the output file name, a pointer to an int array containing the cells you want to print, an int denoting the size of that array and an int with what column you want to start with.

myhtmlParser.printFile("outputfilename.csv", pointertointarray, sizeofarray, startprintingatcolumn);

Optionaly you can convert the date to: number of weeks since September 5th 2017, number of months since the bigining of the year, day of that month. With convertDate adding the cell number that contains the date.

myhtmlParser.convertDate(cellnumber);

Get the data in the cells after it has been set to a vector matrix

vector<vector<string> > cellMatrix;
cellMatrix = myhtmlParser.getColData();
