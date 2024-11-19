#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
using namespace std;

class Player
{
private:
    string _name;
    int _strength, _stamina, _provisions, _bugs_points, _age;
    int _board;
    int _idx, _lastIdx;
    char _meeple;
    /// @brief checks if value is between two bounds. If outside of bounds, returns closest bound; else, returns the value
    /// @param v the value to check
    /// @param tb the top bound
    /// @param bb the bottom bound
    /// @return either v, tb, or bb based on given conditions
    int boundVal(int v, int tb, int bb)
    {
        if (v > tb)
        {
            return tb;
        }
        if (v < bb)
        {
            return bb;
        }
        return v;
    }

public:
    // Creates a new instance of Player by setting _name to an empty string, and all numerical values to their minimum accepted value.
    Player()
    {
        _name = "";
        _strength = 100;
        _stamina = 100;
        _provisions = 100;
        _bugs_points = 0;
        _age = 1;
        _idx = 0;
        _meeple = '@';
    }
    // Creates a new instance of Player with _name, _strength, _stamina and _provisions coming from the parameters. If the parameters are outside of the accepted range of values, use the minimum accepted value instead. _bugs_points should default to 0, and _age should default to 1.
    Player(string name, int strength, int stamina, int provisions, char rep)
    {
        _name = name;
        _strength = ((strength <= 1000) && (strength >= 100)) ? strength : 100;
        _stamina = ((stamina <= 1000) && (stamina >= 100)) ? stamina : 100;
        _provisions = ((provisions <= 1000) && (provisions >= 100)) ? provisions : 100;
        _bugs_points = 0;
        _age = 1;
        _idx = 0;
        _meeple = rep;
    }

    // Getters
    // return the _name data member.
    string getName() { return _name; }
    // return the _strength data member.
    int getStrength() { return _strength; }
    // return the _stamina data member.
    int getStamina() { return _stamina; }
    // return the _provisions data member.
    int getProvisions() { return _provisions; }
    // return the _bugs_points data member.
    int getBugsPoints() { return _bugs_points; }
    // return the _age data member.
    int getAge() { return _age; }
    // return board position
    void getPos(int position[2]){
        position[0] = _board;
        position[1] = _idx;
    }
    char getMeeple(){
        return _meeple;
    }
    // Adders
    // update the stored name for your Player
    void setName(string name) { _name = name; }

    // update the stored strength
    void addStrength(int strength)
    {
        _strength = boundVal(_strength + strength, 1000, 100);
    }
    // update the stored stamina
    void addStamina(int stamina)
    {
        _stamina = boundVal(_stamina + stamina, 1000, 100);
    }
    // update the stored provisions
    void addProvisions(int provisions)
    {
        _provisions = boundVal(_provisions + provisions, 1000, 100);
    }
    // update the stored pride points
    void addBugsPoints(int bugs_points)
    {
        _bugs_points += bugs_points;
    }
    // update the stored age
    void addAge(int age)
    {
        _age += age;
        if (_age > 20)
        {
            _age = 20;
        }
    }

    // Other
    // This function can be called with a player chooses to train their cub at the start of the game instead of going straight to the pride lands. This should increase the cub’s strength, stamina, and provisions by the parameters given. It should also decrease the pride points by -5,000.
    void rememberToPack(int strength, int stamina, int provisions)
    {
        addStrength(strength);
        addStamina(stamina);
        addProvisions(provisions);
        addBugsPoints(-5000);
        _board = 1;
    }
    // This function can be called when a player chooses to go straight to the pride lands at the start of the game and skip training. This should give the player an immediate boost of +5,000 Pride Points, but come at the cost of reduced Strength (-2,000), Provisions (-2,000), and Stamina (-1,000).
    void chargeForth()
    {
        addStrength(-2000);
        addStamina(-1000);
        addProvisions(-2000);
        addBugsPoints(5000);
        _board = 0;
    }
    // This function should print all of the data members in an organized stats message.
    void printStats();
};

#endif