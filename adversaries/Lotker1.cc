#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <math.h>
//#include <stdio.h>

#ifndef OMNETPP_H
#define OMNETPP_H
#include <omnetpp.h>
#endif

#include "../messages/AdversarialInjectionMessage_m.h"
#include "../messages/AdvSchedMess.h"
#include "AdvancedAdversary.h"

/**
 * Generates and schedules injections for the nodes in the network.
 */
class Lotker1 : public AdvancedAdversary
{
protected:
    void injectInitialPackets();
    void injectPhasePackets();
    void setLongPath(AdversarialInjectionMessage *message, int cur, bool lower);
    void singleEdgeConfinement(int targetGadget, double roundTime, SimTime baseTime);

    int curgadget;

    //lotker parameter
    const static int lengthn = 8;
    const static int lengthM = 10;
};

Define_Module(Lotker1);



void Lotker1::injectInitialPackets()
{
    timeSlots = &par("sendIaTime");
    bufferSize = par("frameCapacity");
    injectionRate = par("injectionRate");
    injectionCount = 0;
    timeSync = simTime(); // == 0 (as we init!)
    WATCH(injectionCount);

    //define adversarial injections
    int initialSetSize=1000; //in time steps (not simulationTime!!)
    AdvSchedMess * tmp;
    maxPhaseCounter=200;  //currently overall time fixed to simTime<=100s

    //learn initial queue length
    //before the queue length later on can be queried -> need to create the listener objects

    /*
     * restriction by omnet: module names must begin with letter
     * but routing based on integer addresses
     * hence work with integers and convert
     *
     * there are gadgets from a to j and 1 to 10
     * ascii(a is 97, j is 106)
     */
    cModule *targetModule;
    QueueLengthRequest *queueLenMsg;

    targetModule = getParentModule()->getSubmodule("a00")->getSubmodule("routing");
    queueLenMsg = new QueueLengthRequest("getGate");
    queueLenMsg->setModuleName("a00");
    queueLenMsg->setOutAddress(101);
    queueLenMsg->setKind(103);
    sendDirect(queueLenMsg, targetModule, "adversaryControl");

    for(int i=1; i<lengthM; i++)
    {
        char modn[3];
        sprintf (modn, "%c01", i+96);

        targetModule = getParentModule()->getSubmodule(modn)->getSubmodule("routing");
        queueLenMsg = new QueueLengthRequest("getGate");
        queueLenMsg->setModuleName(modn);
        queueLenMsg->setOutAddress(2+i*100);
        queueLenMsg->setKind(103);
        sendDirect(queueLenMsg, targetModule, "adversaryControl");
    }


    //  set where to start (upper left site of gadget)
    curgadget = 1;
    //now all gadget are addressed relative to that one

    //round 0 create C(S,F_1)
    // (1) + (2)
    // inject in each e_i (lower path) one packet and the remainder into the last one
    for (int i=1; i<lengthn; i++)
    {
        tmp = new AdvSchedMess;
        tmp->interInjectionTime=0;
        tmp->packetCount=1;
        tmp->message=new AdversarialInjectionMessage("(1) init");
        tmp->atNode=new char[3];
        sprintf(tmp->atNode,"%c2%d", curgadget+96, i);
        setLongPath(tmp->message,curgadget+1,true);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);
    }
    tmp = new AdvSchedMess;
    tmp->interInjectionTime=0;
    tmp->packetCount=initialSetSize-lengthn+1;
    tmp->message=new AdversarialInjectionMessage("(1) init");
    tmp->atNode=new char[3];
    sprintf(tmp->atNode,"%c2%d", curgadget+96,lengthn);
    setLongPath(tmp->message,curgadget+1,true);
    tmp->message->setKind(101);
    tmp->setSchedulingPriority(1);
    //schedule this at timesync as selfmessage
    scheduleAt(timeSync,tmp);


    // (3)
    tmp = new AdvSchedMess;
    tmp->interInjectionTime=0;
    tmp->packetCount=initialSetSize;
    tmp->message=new AdversarialInjectionMessage("(3) init");
    tmp->atNode=new char[3];
    sprintf(tmp->atNode,"%c01", curgadget+96);
    setLongPath(tmp->message,curgadget,false);
    tmp->message->setKind(101);
    tmp->setSchedulingPriority(1);
    //schedule this at timesync as selfmessage
    scheduleAt(timeSync,tmp);
}







void Lotker1::injectPhasePackets()
{
    AdvSchedMess * tmp;
    //we assume we are indeed subscribed to the right queue! - no further consistency check!
    //  10 in qlarray is a00: needed for wrap-around phase
    long roundTime=qlarray[curgadget]->queuelength + 1; //because one transmitted right away
    ev << "QL: "<< roundTime << endl;
    timeSync = simTime(); //offset for first round = 0

    if(curgadget>0)
    {

        // (single-edge confinement)
        // not to be called when already in last gadget see Lemmata 3.14/3.16
        singleEdgeConfinement(curgadget+1,roundTime, timeSync);


        //actual new injections:    new (3)
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount= floor(roundTime*injectionRate);
        tmp->message=new AdversarialInjectionMessage("(3) long way");
        tmp->atNode=new char[3];
        sprintf(tmp->atNode,"%c01", curgadget+96);
        setLongPath(tmp->message,curgadget,false);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);




        timeSync += (timeSlots->doubleValue())*(roundTime+lengthn+1);

        //actual new injections: X   new (4)
        double Rn = (1-injectionRate)/(1-pow(injectionRate,lengthn));
        int X = floor(2*roundTime*(1-Rn)-injectionRate*roundTime+lengthn);
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount= X;
        tmp->message=new AdversarialInjectionMessage("X, (4)");
        tmp->atNode=new char[3];
        sprintf(tmp->atNode,"%c01", curgadget+1+96);
        setLongPath(tmp->message,curgadget+1,false);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);


        //each phase's duration: 2S+n
        //already (roundTime+lengthn+1)
        timeSync += (timeSlots->doubleValue()*roundTime);

        //stop before even last gadget
        //TODO stop where?
        if (++curgadget > 9)
        {
            //reset
            curgadget = 0;

            //as stated below proof of Lemma 3.14 (that is, in the proof of Lemma 3.13 on page 10)
            // "no injection are done in the interval [t,t+S+n]"
            timeSync += (timeSlots->doubleValue()*(roundTime + lengthn));
            //wait for another buffer for appropriate timing
            timeSync += (timeSlots->doubleValue()*roundTime);
        }

    }
    else
    {
        //Lemma 3.16

        //wrap around step (1) called a0,a1,a2 in paper here: a00,a01,a02
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount= floor(roundTime*injectionRate);
        tmp->message=new AdversarialInjectionMessage("wrap 1");
        tmp->atNode=new char[3];
        strcpy (tmp->atNode,"a00");
        tmp->message->setPathArraySize(1);
        tmp->message->setPath(0,102);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);

        //wrap around step (2) called a2 in paper here: a02
        timeSync += (timeSlots->doubleValue()*roundTime);
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount= floor(roundTime*injectionRate*injectionRate);
        tmp->message=new AdversarialInjectionMessage("wrap 2");
        tmp->atNode=new char[3];
        strcpy (tmp->atNode,"a01");
        tmp->message->setPathArraySize(1);
        tmp->message->setPath(0,102);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);

        //wrap around step (3) called a2 in paper here: a02
        timeSync += (timeSlots->doubleValue()*roundTime*injectionRate);
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount= floor(roundTime*injectionRate*injectionRate*injectionRate);
        tmp->message=new AdversarialInjectionMessage("wrap 3");
        tmp->atNode=new char[3];
        strcpy (tmp->atNode,"a01");
        setLongPath(tmp->message,1,true);
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync,tmp);


        //Lemma 3.15

        timeSync += (timeSlots->doubleValue()*roundTime*injectionRate*injectionRate);
        //single-edge confinement in gadget number 1
        singleEdgeConfinement(1,roundTime,timeSync);

         // (3) n packets of length 1
         tmp = new AdvSchedMess;
         tmp->interInjectionTime=0;
         tmp->packetCount=lengthn;
         tmp->message=new AdversarialInjectionMessage("wrap 4");
         tmp->atNode=new char[3];
         strcpy (tmp->atNode,"a01");
         tmp->message->setPathArraySize(1);
         tmp->message->setPath(0,102);
         tmp->message->setKind(101);
         tmp->setSchedulingPriority(1);
         //schedule this at timesync as selfmessage
         scheduleAt(timeSync,tmp);

         // (3) S'=2S(1-Rn) packets full path
         timeSync += (timeSlots->doubleValue()*lengthn);
         double Rn = (1-injectionRate)/(1-pow(injectionRate,lengthn));
         tmp = new AdvSchedMess;
         tmp->interInjectionTime=0;
         tmp->packetCount= floor(2*roundTime*(1-Rn));
         tmp->message=new AdversarialInjectionMessage("wrap 5");
         tmp->atNode=new char[3];
         strcpy (tmp->atNode,"a01");
         setLongPath(tmp->message,curgadget,false);
         tmp->message->setKind(101);
         tmp->setSchedulingPriority(1);
         //schedule this at timesync as selfmessage
         scheduleAt(timeSync,tmp);

         timeSync += (timeSlots->doubleValue()*floor(2*roundTime*(1-Rn))/injectionRate);
    }




    //schedule next round!
    cMessage *selfNote = new cMessage("Start of Phase");
    selfNote->setKind(102); //this means that the first entry of the injection struct shall be started by this message
    tmp->setSchedulingPriority(7); //higher means lower priority, normal packets get 4 (initial injection 1, other injection 2)
    //the round number 5 of this phase is the first round of the next phase
    scheduleAt(timeSync, selfNote);

}






void Lotker1::setLongPath(AdversarialInjectionMessage *message, int cur, bool lower)
{
    //cur might be different that global counter curGadget

    // set path starting from firstgadget to last
    //if lower always user lower ones
    //if !lower start with one upper and then lower ones
    // see [Lotker04]

    //always use 4 waypoints per gadget (static shortest-path routing!)
    // we don't always need the full chain - start from current gadget cur
    int dc = 4*(lengthM-cur+1);
    message->setPathArraySize(dc);



    for(int i=cur;i<=lengthM;i++)
    {
        message->setPath(--dc,(i*100)+02);
        //if lower or after the first gadget choose lower path x2y, else upper x1y
        message->setPath(--dc,(i*100)+01+(lower || i!=cur ? 20 : 10));
        //chose the last one in some gadget
        message->setPath(--dc,(i*100)+lengthn+(lower || i!=cur ? 20 : 10));
        //if next one is after last one: wrap around counter
        message->setPath(--dc,(((i+1)>10?(i+1)%lengthM:i+1)*100)+01);
    }

    /*
     * resulting path should look like (if c=cur, n=lengthn):
     *   if lower: c(i*100)+02);02,c21,c2n,(c+1)01, (c+1)02,(c+1)21,(c+1)2n,(c+1)01,...
     *   else: c02,c11,c1n,(c+1)01, (c+1)02,(c+1)21,(c+1)2n,(c+1)01,...
     */

}


void Lotker1::singleEdgeConfinement(int targetGadget, double roundTime, SimTime baseTime)
{
    for (int i=1; i<=lengthn; i++)
    {
        AdvSchedMess * tmp;

        double Ri = (1-injectionRate)/(1-pow(injectionRate,i));
        double ti = (2*roundTime)/(injectionRate + Ri);
        //for this node, inject in [i,i+ti]
        //hence scheduleAt timeSync + i*timeslot and send ceil(ti) packets at rate r
        tmp = new AdvSchedMess;
        tmp->interInjectionTime = (timeSlots->doubleValue())/injectionRate;
        tmp->packetCount=floor(ti*injectionRate);
        tmp->message=new AdversarialInjectionMessage("confinement");
        tmp->atNode=new char[3];
        sprintf(tmp->atNode,"%c2%d", targetGadget+96, i);
        tmp->message->setPathArraySize(1);
        if (i==lengthn)
        {
            int targetAddr = targetGadget +1;
            targetAddr = ((targetAddr>lengthM?targetAddr%lengthM:targetAddr)*100) + 1;
            tmp->message->setPath(0,targetAddr);
        }
        else
            tmp->message->setPath(0,(targetGadget*100)+20+i+1); //send towards second gadget (seen from targetGadget)
        tmp->message->setKind(101);
        tmp->setSchedulingPriority(1);
        //schedule this at timesync as selfmessage
        scheduleAt(timeSync+(timeSlots->doubleValue())*i,tmp);
    }
}