#include "Simulation.h"
#include "Graph.h" // added
#include "Party.h"  // added
#include "offersArray.h"//addaed!!

Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents), offers(mAgents.size(),graph.getNumVertices(), mAgents)
{
    
    coalitions.resize(getAgents().size(),std::vector<int>(0));
    for(Agent agent : mAgents){
        coalitions[agent.getId()].push_back(agent.getPartyId()); 
    } 


    // coalitonsMandates =  number of mandates for each coalition.
    coalitonsMandates.resize(mAgents.size()); // Initialize the number of mandates in each coalition.
    
    for(Agent &agent: mAgents){
        coalitonsMandates[agent.getId()] = graph.getParty(agent.getPartyId()).getMandates();
    }

}

void Simulation::step()
{
    // TODO: implement this method
    for (int i = 0; i < mGraph.getNumVertices(); i++)
    {
        mGraph.getParty(i).step(*this);
    }

    for(Agent& agent: mAgents){
        agent.step(*this);
    }
    
}

bool Simulation::shouldTerminate() const
{
    // TODO implement this method
    bool allPartiesJoined = false;
    
    for(int mandates: coalitonsMandates){
        if(mandates >= 61){
            allPartiesJoined = true;
        }
    }

    if (!allPartiesJoined)
    {
        allPartiesJoined = true;
        for (int i = 0; i < mGraph.getNumVertices() && allPartiesJoined; i++)
        {
            if(getParty(i).getState() != State::Joined){
                allPartiesJoined = false;
            }
        }
    }
    return allPartiesJoined;
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

Party &Simulation::getParty(int partyId) //overload NEW ADDED
{
    return mGraph.getParty(partyId);
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    // TODO: you MUST implement this method for getting proper output, read the documentation above.  
    return coalitions;
}

//added
bool Simulation::isOkToOffer(int agent, int party)//added
{
    return offers.isOkToOffer(agent, party);
}

 void Simulation::setPartyIsOffered(int agent, int party)//added
 {
    offers.setPartyIsOffered(agent, party);
 }

 //amitai new add
 void Simulation::addPartyToCoalition(int coalitionId, int partyId){
    coalitions[coalitionId].push_back(partyId); //add party to coalition
    coalitonsMandates[coalitionId] += getParty(partyId).getMandates(); // add number of mandates to the coalition.
    getParty(partyId).setState(Joined); // set state of party to Joined.
    
   
}

 void Simulation::cloneAgent(int agentId, int partyId, SelectionPolicy *SP){
   int newAgentID = mAgents.size();
    mAgents.push_back(Agent(newAgentID,partyId, SP));
 }