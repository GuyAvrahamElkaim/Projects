
#include "JoinPolicy.h"
#include "Simulation.h"
#include "Agent.h"
#include "Party.h"
JoinPolicy::JoinPolicy(string type): _type(type){};
MandatesJoinPolicy::MandatesJoinPolicy():JoinPolicy("mandate"){};
void MandatesJoinPolicy::join(Simulation &sim, Party &party,int agentId, int coalitionId)
{
//check which coalition has more mandates
        int currOfferMandates;
        
        if (party.getBestOffer() != -1) 
        {
                    //check for current best offer
        currOfferMandates = sim.coalitonsMandates[party.getBestOffer()]; // in the first time bestOffer is null or -1

        }
        else
        {
            currOfferMandates = 0;
            //check for the new offer mandates (the current agent)
        int newOfferMandates = sim.coalitonsMandates[coalitionId];
            
        if(newOfferMandates > currOfferMandates)
        {
            sim.getParty(party.getPartymId()).takeNewOffer(coalitionId);
        }
        // else if(newOfferMandates == currOfferMandates){ //smaller id wins
        //     if (agentId < party.getBestOffer())
        //         {
        //             sim.getParty(party.getPartymId()).takeNewOffer(coalitionId);
        //         }
                
        //     }
        }
        
}


LastOfferJoinPolicy::LastOfferJoinPolicy():JoinPolicy("lastoffer"){};
void LastOfferJoinPolicy::join(Simulation &sim, Party &party, int agentId, int coalitionId)
{
    sim.getParty(party.getPartymId()).takeNewOffer(coalitionId);//the party that the agent tried to offer accept the offer and saves the agent id
}