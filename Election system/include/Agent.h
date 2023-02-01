#pragma once

#include <vector>
#include "Graph.h"

class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);

    int getPartyId() const;
    int getId() const;
    void step(Simulation &);
    SelectionPolicy* getSP();
    
    // Rule of 5
    Agent(const Agent &other); 
    ~Agent();
    Agent(Agent &&other);
    Agent& operator=(const Agent &other);
    Agent& operator=(Agent &&other);

    // new add
    int getPartyId(); //overload
private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
};
