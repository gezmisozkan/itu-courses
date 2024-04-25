/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef POKEMON_H
#define POKEMON_H

#include <iostream> 
using std::string;
using std::cout;
using std::endl;

// this is base Pokemon class 
class Pokemon{
    public:
        Pokemon(string,int,int,int,int,string); // constructor with parameters
        bool is_fainted() const;                // is_fainted function
        bool is_in_powerUp() const;             // is_in_powerUp function
        bool is_own_terrain(string) const;      // is_own_terrain function
    
        // setters
        void set_HP(int);                               
        void set_reattempt_time(int);    
        void set_powerUp(bool);                     

        // getters, getter functions are all const 
        string get_name() const;
        int get_HP() const;
        int get_damage() const;
        int get_powerUpChance() const ;
        int get_recharge() const;
        int get_reattempt_time() const ;

    private:
        // members
        const string name{};          // name of the pokemon
        const string pokemon_type{};  // type of the pokemon
        int HP{};                     // HP is int
        const int damage{};           // damage is constant
        const int powerUpChance{};    // power up chance is constant  
        const int recharge{};         // the number of rounds that must pass until a Pokemon has the chance to attempt to PowerUp again
        bool powerUp{false};          // it is true or false
        int reattempt_time{};         // rounds time remaining for reattempt
};

#endif