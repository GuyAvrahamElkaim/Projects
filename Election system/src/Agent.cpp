#include "Agent.h"
#include "Simulation.h"
#include "Graph.h" 
#include "SelectionPolicy.h"

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
    //
}

// Destructour
Agent:: ~Agent() {if (mSelectionPolicy) delete (mSelectionPolicy);}

// Copy constructor
Agent::Agent(const Agent &other):
  mAgentId(other.mAgentId), mPartyId(other.mPartyId), mSelectionPolicy(nullptr)
  {
    if(other.mSelectionPolicy->_type == "mandate"){
        mSelectionPolicy = new MandatesSelectionPolicy();
    }
    else{
        mSelectionPolicy = new EdgeWeightSelectionPolicy();
    }
  }

 // Move constructor
 Agent::Agent(Agent &&other):mAgentId(other.getId()), mPartyId(other.getPartyId()), mSelectionPolicy(other.getSP())
 {
    other.mSelectionPolicy = nullptr;
 }   

// Copy assignmnet operator
 Agent & Agent::operator=(const Agent &other)
 {
    if(this != &other){
        mAgentId = other.mAgentId;
        mPartyId = other.mPartyId;
        if (mSelectionPolicy)
        {
            delete mSelectionPolicy;
        }
        
        if(other.mSelectionPolicy->_type == "mandate"){
            mSelectionPolicy = new MandatesSelectionPolicy();
        }
        else{
            mSelectionPolicy = new EdgeWeightSelectionPolicy();
        }
    }
    return *this;
 }

 // Move assignment operator 
;
 Agent & Agent::operator=(Agent &&other){
    if(mSelectionPolicy) delete (other.mSelectionPolicy);
    
    mAgentId = other.mAgentId;
    mPartyId = other.mAgentId;
    mSelectionPolicy = other.mSelectionPolicy;
    other.mSelectionPolicy = nullptr;
    return *this;
 }

int Agent::getId() const
{
    return mAgentId;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

int Agent::getPartyId() //overload
{
    return mPartyId;
}
    
void Agent::step(Simulation &sim)
{
    //step will find the best party and will offer her to join using the selection policy algo
    mSelectionPolicy->select(sim,mAgentId,mPartyId);
}

SelectionPolicy* Agent::getSP()
{
    return mSelectionPolicy;
}
