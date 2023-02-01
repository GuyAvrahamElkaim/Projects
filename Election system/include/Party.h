#pragma once
#include <string>

using std::string;

class JoinPolicy;
class Simulation;

enum State
{
    Waiting,
    CollectingOffers,
    Joined
};

class Party
{
public:
    Party(int id, string name, int mandates, JoinPolicy *); 
    //rule of 5
    ~Party();
    Party(const Party &other); //copy constructor
    Party(Party &&other); //move constructor 
    Party& operator=(const Party &other);//copy assignment operator
    Party& operator=(Party &&other);//move assignment operator

    State getState() const;
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;
    
    //added
    void increaseTimer();
    int getTimer() ;
    int getBestOffer();
    void takeNewOffer(int);
    JoinPolicy* getJP();//NEW
    int getPartymId();

    
private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;
    int bestOffer;//added, saves the the id of the agent with the best offer
    int mTimer; // added
};
