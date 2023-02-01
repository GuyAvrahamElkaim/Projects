#include "SelectionPolicy.h"
#include "JoinPolicy.h"
#include "Agent.h"


SelectionPolicy::SelectionPolicy(string type):_type(type){};
MandatesSelectionPolicy::MandatesSelectionPolicy():SelectionPolicy("mandate"){};
EdgeWeightSelectionPolicy::EdgeWeightSelectionPolicy():SelectionPolicy("edge"){};

void MandatesSelectionPolicy::select(Simulation &sim, int agentId, int agentPartyId)
{

    Graph g = sim.getGraph();
    int numOfParties=g.getNumVertices();
    int bestPartyToOffer = -1;
    int coalitionID=0;
    int count = 0;
    for(vector<int> vec: sim.coalitions)
    {
        for(int partyInVec: vec)
        {
            if(partyInVec==agentPartyId)
            {
                coalitionID = count;
            }
        }
        count++;
    }



    for(int i = 0; i < numOfParties; i++)//i=currParty for loop decide which party to offer
        {
            if(sim.isOkToOffer(coalitionID, i) && g.getEdgeWeight(i,agentPartyId)!=0)//checking if there is an edge in the graph, and that we havent offered yet to the party
            {
                if(sim.getParty(i).getState()!= State::Joined)//check if the party accept offers
                {
                    
                    if(bestPartyToOffer == -1 || g.getParty(i).getMandates()>g.getParty(bestPartyToOffer).getMandates())//check if currOffer is better then best offer
                    {
                        bestPartyToOffer = i;
                        
                    }
                    else if (g.getParty(i).getMandates() == g.getParty(bestPartyToOffer).getMandates()) // if number of mandates are tie the lower id wins.
                    {
                        if (i < bestPartyToOffer) // lower id wins.
                        {
                            bestPartyToOffer = i;
                            
                        }
                        
                    }
                    
                }
            }
            
        }
        //offer the party
            if(bestPartyToOffer != -1) 
            {
                  Party party = sim.getParty(bestPartyToOffer);
                  party.getJP()->join(sim,party,agentId, coalitionID);
                  sim.setPartyIsOffered(coalitionID,bestPartyToOffer); // new add Amitai

                  if (sim.getParty(bestPartyToOffer).getState() == State::Waiting) // if state == Wating change to CollectingOffers. Add NEW
                    {
                        sim.getParty(bestPartyToOffer).setState(CollectingOffers);
                    }
                  
            }
}

void EdgeWeightSelectionPolicy::select(Simulation &sim, int agentId, int agentPartyId)
{
     Graph g = sim.getGraph();
    int numOfParties=g.getNumVertices();
    int bestPartyToOffer = -1;
    int coalitionID=0;
    int count = 0;
    for(vector<int> vec: sim.coalitions)
    {
        for(int partyInVec: vec)
        {
            if(partyInVec==agentPartyId)
            {
                coalitionID = count;
            }
        }
        count++;
    }

    for(int i=0; i<numOfParties; i++)
        {
            if(sim.isOkToOffer(coalitionID, i) && g.getEdgeWeight(i,agentPartyId) != 0)//checking if there is an edge in the graph, and that we havent offered yet to the party
            {
                if(sim.getParty(i).getState()!= State::Joined)//check if the party accept offers
                {
                    
                    if(bestPartyToOffer == -1 || g.getEdgeWeight(i,agentPartyId) > g.getEdgeWeight(bestPartyToOffer,agentPartyId))//check if currOffer is better then best offer
                    {
                        bestPartyToOffer = i;
                        
                    }
                    else if (g.getEdgeWeight(i,agentPartyId) == g.getEdgeWeight(bestPartyToOffer,agentPartyId)) // if number of mandates are tie the lower id wins.
                    {
                        if (i < bestPartyToOffer) // lower id wins.
                        {
                            bestPartyToOffer = i;
                            
                        }
                        
                    }

                    
                }
            }
            //offer the party
        }
        if(bestPartyToOffer != -1)
            {
                  Party party = sim.getParty(bestPartyToOffer);
                  party.getJP()->join(sim, party, agentId,coalitionID);
                  //add to the not offered party array
                  sim.setPartyIsOffered(coalitionID,bestPartyToOffer); 

                  if (sim.getParty(bestPartyToOffer).getState() == State::Waiting) // if state == Wating change to CollectingOffers.
                    {
                        sim.getParty(bestPartyToOffer).setState(CollectingOffers);
                    }
            }
        
}