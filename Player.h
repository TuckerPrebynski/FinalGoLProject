#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
struct Stats{
    string name;
    int points;
    string display;
};
class Player
{
private:
    string _name;
    string _playerName;
    vector <string> _companion;
    int _strength, _stamina, _provisions, _bugs_points, _age;
    int _board;
    int _idx, _lastIdx;
    bool isSkipped = false;
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
        _idx = -1;
        _board = 0;
        _meeple = '@';
        _companion = {"None","",""};
    }
    // Creates a new instance of Player with _name, _strength, _stamina and _provisions coming from the parameters. If the parameters are outside of the accepted range of values, use the minimum accepted value instead. _bugs_points should default to 0, and _age should default to 1.
    Player(string name, int age, int strength, int stamina, int provisions, int bugs, char rep)
    {
        _name = name;
        _strength = ((strength <= 1000) && (strength >= 100)) ? strength : 100;
        _stamina = ((stamina <= 1000) && (stamina >= 100)) ? stamina : 100;
        _provisions = ((provisions <= 1000) && (provisions >= 100)) ? provisions : 100;
        _bugs_points = bugs;
        _age = age;
        _idx = -1;
        _board = 0;
        _meeple = rep;
        _companion = {"None","",""};
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

    vector <string> getCompanion(){return _companion;}
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
    void setCompanion(vector <string> companion){_companion = companion;}
    void changePos(int change){
        _idx += change;
        if(_idx < 0){
            _idx = 0;
        }
        if(_idx > 51){
            _idx = 51;
        }
    }
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
    void skip()
    {
        isSkipped = true;
    }
    bool skipCheck()
    {
        if(isSkipped){
            isSkipped = false;
            return true;
        }else{
            return false;
        }
    }
    // Other
    // This function can be called with a player chooses to train their cub at the start of the game instead of going straight to the pride lands. This should increase the cub’s strength, stamina, and provisions by the parameters given. It should also decrease the pride points by -5,000.
    void rememberToPack(int strength, int stamina, int provisions)
    {
        addStrength(strength);
        addStamina(stamina);
        addProvisions(provisions);
        addBugsPoints(-500);
        _board = 1;
        _idx = 0;
    }
    // This function can be called when a player chooses to go straight to the pride lands at the start of the game and skip training. This should give the player an immediate boost of +5,000 Pride Points, but come at the cost of reduced Strength (-2,000), Provisions (-2,000), and Stamina (-1,000).
    void chargeForth()
    {
        addStrength(-200);
        addStamina(-100);
        addProvisions(-200);
        addBugsPoints(500);
        _board = 0;
        _idx = 0;
    }
    // This function should print all of the data members in an organized stats message.
    Stats printStats(){
        Stats pStat;
        pStat.name = _name;
        if(_name == ""){
            pStat.points = -1;
        }else{
        pStat.points = _bugs_points; 
        }
        stringstream statsOut; 
        statsOut <<"|"<<_age<<"|"<<_strength<<"|"<<_stamina<<"|"<<_provisions<<"|"<<_bugs_points;

        statsOut >> pStat.display;
        return pStat;
    }
    Stats printFinalStats(){
        Stats pStat;
        
        if(_name == ""){
            pStat.points = -1;
        }else{
        pStat.points = _bugs_points + 100*(_strength + _stamina + _provisions); 
        }
        stringstream nameStr(_name);
        nameStr << " Final Points: " << pStat.points;
        nameStr >> pStat.name;
        stringstream statsOut; 
        statsOut <<"|"<<_age<<"|"<<_strength<<"|"<<_stamina<<"|"<<_provisions;

        statsOut >> pStat.display;
        return pStat;
    }
};

#endif