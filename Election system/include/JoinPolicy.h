#pragma once
#include "Agent.h"
#include "Party.h"
#include "Simulation.h"

class JoinPolicy {
    public:
    virtual void join(Simulation &sim, Party &party, int agentId, int coalitionId)=0;
    JoinPolicy(string type);
    string _type;
    virtual ~JoinPolicy()=default;
};

class MandatesJoinPolicy : public JoinPolicy {
    public:
    MandatesJoinPolicy();
    virtual void join(Simulation &sim, Party &party, int agentId, int coalitionId);
};

class LastOfferJoinPolicy : public JoinPolicy {
    public:
    LastOfferJoinPolicy();
    virtual void join(Simulation &sim, Party &party, int agentId,  int coalitionId);
};