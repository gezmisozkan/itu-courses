/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef ARENA_H
#define ARENA_H

#include "pokemon.h"
#include "psychicPokemon.h"
#include "electricPokemon.h"

// the battles happen in the Arena class
class Arena{
    public:
        ~Arena();                                                   // destructor           
        void addPokemon(char, string, int, int);                    // add pokemons to arena
        void simulateBattle();                                      // simulates the battle
        void spawnTerrain();                                        // determines the terrain type  
        void restartTerrain();                                      // restart the terrain type
        void printRoundStats(int, int) const;                       // prints stats
        void printMatchResults() const;                             // prints results
    private:
        ElectricPokemon* electricPokemon{};                         // electricPokemon pointer
        PsychicPokemon*  psychicPokemon{};                          // psychicPokemon pointer
        string terrain_type{"none"};                                // terrain type
        int terrain_time{};                                         // the lifetime of terrain
        int round_count{};                                          // the round count
};



#endif