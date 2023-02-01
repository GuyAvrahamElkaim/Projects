#pragma once

#include <vector>

#include "Graph.h"
#include "Agent.h"
#include "offersArray.h"

using std::string;
using std::vector;

class Simulation
{
public:
    Simulation(Graph g, vector<Agent> agents);

    void step();
    bool shouldTerminate() const;

    const Graph &getGraph() const;
    const vector<Agent> &getAgents() const;
    const Party &getParty(int partyId) const;
    const vector<vector<int>> getPartiesByCoalitions() const;
    bool isOkToOffer(int agent, int party);//added
    void setPartyIsOffered(int agent, int party);//added

    //amitai
      //added
    void addPartyToCoalition(int coalitionId, int partyId);
    int getCoalitioinMandates(int coalitioinId) const;
    std::vector<std::vector<int>> coalitions{};
    std::vector<int> coalitonsMandates{};

    Party &getParty(int partyId); //overload of getParty. for fun addPartyToCoalition
    void cloneAgent(int agentId, int partyId, SelectionPolicy *SP);
    

private:
    Graph mGraph;
    vector<Agent> mAgents;
    offersArray offers;
};
