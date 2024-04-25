/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef ELECTRICPOKEMON_H
#define ELECTRICPOKEMON_H

#include "pokemon.h"

// electric pokemon is inherited from Pokemon class
class ElectricPokemon : public Pokemon{
    public:
        ElectricPokemon(string,int,int);        // constructor with parameters
        bool is_confused() const;               
        void set_num_of_confusion(int);  // set function
        int get_num_of_confusion() const;       // get function
    private:
        //members
        bool confusion {false};                 // default is false
        int  num_of_confusion{};                // the number of rounds required for the confusion to end
};

#endif