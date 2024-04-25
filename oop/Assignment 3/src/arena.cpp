/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */

#include "arena.h"

//destructor
// when the battle ends pokemon pointers won't be NULL, so we should delele these
Arena::~Arena(){
    if(this->electricPokemon != NULL)
        delete this->electricPokemon;
    if(this->psychicPokemon != NULL)
        delete this->psychicPokemon;
}
// sets the terrain type to "none"
void Arena::restartTerrain(){
    this->terrain_type="none";
}
// determines the terrain type 
// Electric and psychic terrains can be spawned with 20% chance each
// here is a 60% chance that the terrain type is none.
// if terrain_time is greater than 0, it means the arena is electric or psychic and we don't change anything
void Arena::spawnTerrain(){
    int random{};
    random=rand()%100; // the range is 0 to 99
    if(this->terrain_time == 0){
        if(random<20){
            this->terrain_type="electric";
            this->terrain_time=5;
        }
        else if(random>=20 && random <40){
            this->terrain_type="psychic";
            this->terrain_time=5;
        }   
        else if(random >=40)
            this->terrain_type="none";
    }
    
}

// prints pokemons HP's and who has more HP 
void Arena::printMatchResults() const{
    cout << this->electricPokemon->get_name() << " HP: "<< this->electricPokemon->get_HP() <<endl;
    cout << this->psychicPokemon->get_name()  << " HP: "<< this->psychicPokemon->get_HP()  <<endl;

    if(electricPokemon->get_HP() >= psychicPokemon->get_HP())
        cout<< this->electricPokemon->get_name() << " is in the lead! "<<endl;
    else 
        cout<< this->psychicPokemon->get_name() << " is in the lead! "<<endl;
    cout<<"\n \n";
}

// prints round count, current terrain type and which pokemon goes first
// it takes 2 input, round is round count and heads is the result of flipping coin
// if heads is 0 electric pokemon attacks first else psychic
void Arena::printRoundStats(int round,int heads) const{
    cout<<"Round: "<< round<<endl;
    cout<<"Current Terrain: "<<this->terrain_type<<endl;
    if(heads == 0)
        cout << this->electricPokemon->get_name() << " goes first"<<endl;
    else if (heads == 1)
        cout << this->psychicPokemon->get_name() << " goes first"<<endl;
}

// Add pokemons to the arena. "electricPokemon" and "psychicPokemon" are pointers. 
// If they aren't NULL we delete this pokemons if it is NULL we don't delete. After that we allocate memory
// Also prints the pokemon's properties
void Arena::addPokemon(char type,string name,int HP,int damage)
{
    if(type == 'e'){
        if(this->electricPokemon != NULL)
            delete this->electricPokemon;
        this->electricPokemon = new ElectricPokemon{name,HP,damage};

        cout << "Electric Pokemon " << name << " has entered the arena."<<endl;
        cout<< "Class: Electric"<<endl;
    }

    else if(type == 'p'){
        if(this->psychicPokemon != NULL)
            delete this->psychicPokemon;
        this->psychicPokemon = new PsychicPokemon{name,HP,damage};

        cout << "Psychic Pokemon " << name << " has entered the arena."<<endl;
        cout<< "Class: Psychic"<<endl;  
    }
    cout<< "\t Name: "<< name <<endl;
    cout<< "\t HP: "<< HP <<endl;
    cout<< "\t Damage: "<< damage <<endl;
    cout<<"\n \n";
}

// this function simulates the battle
void Arena::simulateBattle(){
    this->restartTerrain();     // firstly we should reset the terrain type and clear round count
    this->round_count=0;        

    // while pokemons' HPs are both greater than 0, I mean they both live, the match continues 
    while( !this->electricPokemon->is_fainted() && !this->psychicPokemon->is_fainted() ){
        this->spawnTerrain();       // determine the terrain
        if(this->terrain_time != 0) // if terrain type isn't "none" then terrain time is greater than 0 and we should decrease it each round
            terrain_time--;
        
        this->round_count ++;       // each round we should increase round count
        int attack_turn= rand()%2; // if it is 0 electric pokemon attacks first, else psychic pokemon attacks first
        this->printRoundStats(round_count,attack_turn); // prints round stats

        int damage_of_electric{this->electricPokemon->get_damage()}; // to not write getter functions all the time and change the damage amount when it needs I create new local variable
        int damage_of_psychic{this->psychicPokemon->get_damage()};

        int randomForE=rand()%100;  // there is a 2 different random number
        int randomForP=rand()%100;

        // if the pokemon's recharge is completed and random number is less than the powerUpChance pokemon will power Up
        // For electric pokemon powerUp Chance is 20, so the probability of power Up is 20%
        // If a pokemon powers up we set the reattempt time
        if(this->electricPokemon->get_reattempt_time()==0 && (randomForE < this->electricPokemon->get_powerUpChance())){
            this->electricPokemon->set_powerUp(true);
            cout<< this->electricPokemon->get_name() << " has initiated a power up. "<<endl;
            this->electricPokemon->set_reattempt_time( this->electricPokemon->get_recharge() );
            damage_of_electric *= 3; // if electric pokemon powers Up it generate 300% of it regular damage 
        }
        // if a pokemon isn't in a powerUp state we should set state to false
        else 
            this->electricPokemon->set_powerUp(false);
        if(this->psychicPokemon->get_reattempt_time()==0 && (randomForP < this->electricPokemon->get_powerUpChance())){
            this->psychicPokemon->set_powerUp(true);
            cout<< this->psychicPokemon->get_name() << " has initiated a power up. "<<endl;
            this->psychicPokemon->set_reattempt_time( this->psychicPokemon->get_recharge() );
        }
        else 
            this->psychicPokemon->set_powerUp(false);

        // if terrain type is electric we should clear number of confusion since electric pokemon is immune to the confusion in electric terrain
        if(this->electricPokemon->is_own_terrain(terrain_type)){
            this->electricPokemon->set_num_of_confusion(0);
        }
        // if terrain type is psychic then psychic pokemon can attack twice in one round which means its damage will be doubled
        else if(this->psychicPokemon->is_own_terrain(terrain_type)){
            damage_of_psychic *= 2;
        }
        

        // battle time
        // according to the attack_turn result, pokemons will attack in order 
        // if it is 0 electric pokemon attacks first, else psychic pokemon attacks first
        // if electric pokemon isn't confused and isn't fainted it can attack
        // if psychic pokemon isn't fainted it can attack too
        if(attack_turn == 0){
            if(!this->electricPokemon->is_confused())
                this->psychicPokemon->set_HP( this->psychicPokemon->get_HP() - damage_of_electric );
            if( !this->psychicPokemon->is_fainted() ){
                this->electricPokemon->set_HP( this->electricPokemon->get_HP() - damage_of_psychic);
                if(this->psychicPokemon->is_in_powerUp())
                    this->electricPokemon->set_num_of_confusion(4); // if psychic pokemon is in power Up state it confuses its opponent for 3 rounds
                    // but since electric pokemon attacks first and after the battle we decrease the confusion number we should set num_of_confusion to 4 so that electric pokemon will stay confused for 3 rounds
            }
        }
        else if(attack_turn == 1){
            if(this->psychicPokemon->is_in_powerUp())
                this->electricPokemon->set_num_of_confusion(3); // if psychic pokemon is in power Up state it confuses its opponent for 3 rounds
            this->electricPokemon->set_HP( this->electricPokemon->get_HP() - damage_of_psychic);
            if( !this->electricPokemon->is_fainted() && !this->electricPokemon->is_confused())
                this->psychicPokemon->set_HP( this->psychicPokemon->get_HP() - damage_of_electric );
        }
        // battle ends

        // if reattempt_time of pokemon is greater than 0 we should decrease it 
        if(this->electricPokemon->get_reattempt_time() > 0)
            this->electricPokemon->set_reattempt_time( this->electricPokemon->get_reattempt_time() - 1);
        if(this->psychicPokemon->get_reattempt_time() > 0)
            this->psychicPokemon->set_reattempt_time( this->psychicPokemon->get_reattempt_time() -1 );

        // if number of confusion is greater than 1 we should subtract 1 from it, otherwise set_num_of_confusion function don't change the value    
        this->electricPokemon->set_num_of_confusion(this->electricPokemon->get_num_of_confusion() - 1); 
        this->printMatchResults();  // prints match result
    }


    string winner;
    // if battle is end there should be fainted pokemon, and the other pokemon wins
    if( !electricPokemon->is_fainted())
        winner = electricPokemon->get_name();
    else if( !psychicPokemon->is_fainted())
        winner = psychicPokemon->get_name();

    // printing winner
    cout<<"Match Results: "<<endl;
    cout<< winner <<" has won the match! \n \n"<<endl;
    
}