//
// Created by Caitlin on 8/06/2019.
//

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string>

namespace database
{
    struct movie {
        char title[50];
        char director[50];
        unsigned short year;
        unsigned long oscars;
        char country[50];
    };

    class DbTable {
    private:
        std::vector<movie> vmovies;
    public:
        //Get number of rows
        unsigned int rows() const;
        //Included so that can show a single row based on an input value
        bool show(unsigned int) const;
        //Included so that can add a movie to the database
        bool add(movie&);
        //Included so that can get a movie from the database based on a given input
        movie* get(unsigned int) const;
        //Load csv file into database
        bool loadCSV(const char *intn);
        //Show row but return as a string
        std::string showRow(unsigned int);
        //Show database, return a string
        std::string showAll();

    };
}
