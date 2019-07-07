#include "database.hh"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;
using namespace database;

//Get the number of rows in the database
unsigned int DbTable::rows() const{
    return vmovies.size();
}

//Add a new movie to the database
bool DbTable::add(movie &m){
    vmovies.push_back(m);
    return true;
}

//Show a movie from the database, using an int for row number
bool DbTable::show(unsigned int rowToShow) const {
    if(rowToShow < rows()) {
        unsigned int i = rowToShow;
        printf("Row %d:\n", i);
        printf("Title : %s\n",vmovies[i].title);
        printf("Director : %s\n", vmovies[i].director );
        printf("Year of Release : %d\n", vmovies[i].year );
        printf("Oscars : %lu\n", vmovies[i].oscars );
        printf("Country : %s\n", vmovies[i].country );

        return true;
    } else {
        return false;
    }
}

//Get a movie from the database
movie *DbTable::get(unsigned int i) const{
    if(i<rows()) {
        return (&const_cast<movie &>(vmovies[i]));
    } else {
        return nullptr;
    }
}

//Show a row, and return a string with the information for that row in it
string DbTable::showRow(unsigned int row) {
    ostringstream out;
    if(row>=rows()) {
        out << "Error: Invalid Row \n";
        return out.str();
    }
    out <<"Title: "<< vmovies[row].title << "\n";
    out <<"Director: "<< vmovies[row].director<< "\n";
    out << "Year: " <<vmovies[row].year<< "\n";
    out <<"Oscars: " << vmovies[row].oscars<< "\n";
    out << "Country: " <<vmovies[row].country<< "\n";
    return out.str();
}

//Show the whole database, and return information as a string
string DbTable::showAll() {
    ostringstream out;
    for(movie m : vmovies) {
        out <<"Title: "<< m.title << "\n";
        out <<"Director: "<< m.director<< "\n";
        out << "Year: " <<m.year<< "\n";
        out <<"Oscars: " << m.oscars<< "\n";
        out << "Country: " <<m.country<< "\n";
        out << "\n";
    }
    return out.str();
}


//Save database to a csv file
bool DbTable::loadCSV(const char*intn) {
    string title,year,director,country,oscars;
    ifstream input(intn);
    if(!input) {
        cout <<"Can't find file" <<endl;
        return false;
    }
    while(!input.eof()) {
        if(!getline(input,title,',')){
            break;
        }
        if(!getline(input,director,',')){
            break;
        }
        if(!getline(input,year,',')){
            break;
        }
        if(!getline(input,oscars,',')){
            break;
        }
        if(!getline(input,country,'\n')){
            break;
        }
        movie m={};
        //Input values that were read into database
        strcpy(m.title,title.c_str());
        m.year = (unsigned short) stoul(year);
        strcpy(m.director,director.c_str());
        m.oscars = (unsigned long) stoul(oscars);
        strcpy(m.country,country.c_str());
        add(m);
    }
    input.close();
    return true;

}

