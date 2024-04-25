/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */

#include "pokemon.h"
// constructor with parameters
Pokemon::Pokemon(string i_name,int i_HP,int i_damage,int i_powUp,int i_recharge,string type)
    :name{i_name},HP{i_HP},damage{i_damage},powerUpChance{i_powUp},recharge{i_recharge},pokemon_type{type}
{
}
// if HP is 0 function returns true
bool Pokemon::is_fainted() const {
    return this->HP==0;
}
bool Pokemon::is_in_powerUp() const{
    return this->powerUp;
}
// if the pokemon type and terrain type are equal function returns true
bool Pokemon::is_own_terrain(string terrain) const {
    return terrain==this->pokemon_type;
}

// setters
// if i_HP is less than 0 we should set HP to 0
void Pokemon::set_HP(int i_HP){
    if(i_HP >= 0)
        this->HP = i_HP;
    else 
        this->HP = 0;
}
void Pokemon::set_reattempt_time(int i_time){
    this->reattempt_time = i_time;
}
void Pokemon::set_powerUp(bool state){
    this->powerUp=state;
}

// getters
string Pokemon::get_name() const {
    return this->name;
}
int Pokemon::get_HP() const {
    return this->HP;
}
int Pokemon::get_damage() const {
    return this->damage;
}
int Pokemon::get_powerUpChance() const {
    return this->powerUpChance;
}
int Pokemon::get_recharge() const {
    return this->recharge;
}
int Pokemon::get_reattempt_time() const {
    return this->reattempt_time;
}