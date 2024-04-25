/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */

#include "electricPokemon.h"

// constructor, it takes name, HP and damage as a parameter. In the initializer list we call Pokemon's constructor
// recharge is 3, powerUpChange is 20 and pokemon_type is electric for electric pokemon
ElectricPokemon::ElectricPokemon(string i_name,int i_HP,int i_damage)
    :Pokemon{i_name,i_HP,i_damage,20,3,"electric"}
{
}
// return the confusion
bool ElectricPokemon::is_confused() const{
    return this->confusion;
}

// if number is greater than or equal to zero we set num_of_confusion variable and change the status of confusion
// if number is negative we don't change the num_of_confusion variable and set the confusion to false
void ElectricPokemon::set_num_of_confusion(int number){
    if(number >= 0)
        this->num_of_confusion = number;
    if(number > 0)
        this->confusion=true;
    else
        this->confusion=false;
}

// return num_of_confusion variable
int ElectricPokemon::get_num_of_confusion() const {
    return this->num_of_confusion;
}
