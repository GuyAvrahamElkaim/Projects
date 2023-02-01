#include "offersArray.h"
#include <vector>


offersArray::offersArray(int numOfAgent, int numOfParties, vector<Agent> &vec):offersVector()
{
   offersVector.resize(numOfAgent,vector<bool>(numOfParties));
    for(int i=0; i<numOfAgent; i++)
    {
        for(int j=0; j<numOfParties; j++)
        {
            offersVector[i][j]=true;
        }
    }
    for(Agent agent: vec)
    {
        offersVector[agent.getId()][agent.getPartyId()]=false;
    }
}

bool offersArray::isOkToOffer(int agent, int party)
{
   return offersVector[agent][party]; 
}

void offersArray::setPartyIsOffered(int agent, int party)
{
    offersVector[agent][party] = false;//false=offered, true=not offered yet   
}
