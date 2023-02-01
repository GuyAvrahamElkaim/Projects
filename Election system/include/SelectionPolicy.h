#pragma once
#include "Simulation.h"
//#include "Agent.h"

class SelectionPolicy { 
    public:
        virtual void select(Simulation &sim, int agentId, int agentPartyId)=0;
        SelectionPolicy(string type);
        virtual ~SelectionPolicy()= default;
        string _type;
    private:
};

class MandatesSelectionPolicy: public SelectionPolicy{ //Select by mandates.
    public:
        MandatesSelectionPolicy();
        virtual void select(Simulation &sim, int agentId, int agentPartyId);
    private:
};

class EdgeWeightSelectionPolicy: public SelectionPolicy{ //Select by edge weight
    public:
        EdgeWeightSelectionPolicy();
        virtual void select(Simulation &sim, int agentId, int agentPartyId);        
    private:
 };