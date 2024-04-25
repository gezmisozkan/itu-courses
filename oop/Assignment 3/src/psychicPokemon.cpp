/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */

#include "psychicPokemon.h"

// constructor, it takes name, HP and damage as a parameter. In the initializer list we call Pokemon's constructor
// recharge is 5, powerUpChange is 30 and pokemon_type is psychic for psychic pokemon
PsychicPokemon::PsychicPokemon(string i_name,int i_HP,int i_damage)
    :Pokemon{i_name,i_HP,i_damage,30,5,"psychic"}
{
}
