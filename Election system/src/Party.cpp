#include "Party.h"
#include "JoinPolicy.h"//added
#include "SelectionPolicy.h"//new

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting) , bestOffer(-1), mTimer(0)
{
    // You can change the implementation of the constructor, but not the signature!
}

//destructor
Party:: ~Party()
{
    if(mJoinPolicy) delete (mJoinPolicy);
}

//copy constructor
Party:: Party(const Party &other):
    mId(other.mId), mName(other.getName()), mMandates(other.getMandates()), mJoinPolicy(nullptr), mState(other.getState()) , bestOffer(other.bestOffer), mTimer(other.mTimer)
{
    if(other.mJoinPolicy->_type=="mandate"){
        mJoinPolicy = new MandatesJoinPolicy();
    }
    else{
        mJoinPolicy = new LastOfferJoinPolicy();
    }
}

//move constructor
Party::Party(Party&& other): mId(other.mId), mName(other.getName()), mMandates(other.getMandates()), mJoinPolicy(nullptr), mState(other.getState()) , bestOffer(other.getBestOffer()), mTimer(other.getTimer())
{
    this->mJoinPolicy=other.mJoinPolicy;
    other.mJoinPolicy = nullptr;
}

//copy assignment operator
Party & Party::operator=(const Party &other)
{
    if (this != &other)
    {
        mId = other.mId;
        mName = other.mName;
        mMandates = other.mMandates;
        mState = other.mState;
        mTimer = other.mTimer;
        if (mJoinPolicy)
        {
            delete mJoinPolicy;
        }
        if(other.mJoinPolicy->_type=="mandate"){
            mJoinPolicy = new MandatesJoinPolicy();
        }
        else{
            mJoinPolicy = new LastOfferJoinPolicy();
         }
    }
    return *this;
}

//move assignment operator
Party & Party:: operator=(Party &&other)
{
    if(mJoinPolicy) delete mJoinPolicy;
    
    mId = other.mId;
    mName = other.mName;
    mMandates = other.mMandates;
    mState = other.mState;
    mTimer = other.mTimer;

    mJoinPolicy = other.mJoinPolicy;
    other.mJoinPolicy = nullptr;
    
    return *this;
}

State Party::getState() const
{
    return mState;
}

void Party::setState(State state)
{
    mState = state;
}

int Party::getMandates() const
{
    return mMandates;
}

const string & Party::getName() const
{
    return mName;
}

void Party::step(Simulation &s)
{
    // TODO: implement this method
    State state = getState();
    if (state == State::CollectingOffers)
    {
        if (getTimer() >= 2 )
        {
            //join coalition
            s.addPartyToCoalition(this->getBestOffer(),mId); 
            SelectionPolicy *SP; 
            Agent a = s.getAgents()[bestOffer];
            if(a.getSP()->_type == "mandate")
            {
                 SP = new MandatesSelectionPolicy();
            }
            else
            {
                SP = new EdgeWeightSelectionPolicy();
            }
            s.cloneAgent(bestOffer, mId, SP); // clone Agent
        }
        else{
            increaseTimer();
        }
        
    }
    
    
}

//added to timer
void Party::increaseTimer(){
    mTimer += 1;
}

int Party::getTimer()
{
    return mTimer;
}

//added
int Party:: getBestOffer()
{
    return bestOffer;
}

void Party:: takeNewOffer(int newOffer)
{
    bestOffer = newOffer;
}

JoinPolicy* Party::getJP()
{
    return mJoinPolicy;
}

int Party::getPartymId()
{
    return mId;
}
