#pragma once

#include <vector>
#include "Agent.h"
using std::vector;

class offersArray
{
    public:
    offersArray(int numOfAgent, int numOfParties, vector<Agent> &vec);//vetor length is as numOfAgents, each array is in the size of NumOfParties
    bool isOkToOffer(int agent, int party);//added
    void setPartyIsOffered(int agent, int party);//added
    //vector<vector<bool>> get2Dvector();// Amitia
    private:
     vector<vector<bool>> offersVector;//if is true isOK to offer, if false already offered
};