/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef PSYCHICPOKEMON_H
#define PSYCHICPOKEMON_H

#include "pokemon.h"

// psychic pokemon is inherited from Pokemon class
class PsychicPokemon : public Pokemon{
    public:
        PsychicPokemon(string,int,int);      // constructor with parameters
};

#endif