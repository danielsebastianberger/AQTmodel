#include <L2Queue.h>

L2Queue::L2Queue()
{
    endTransmissionEvent = NULL;
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
}

void L2Queue::initialize()
{
    queue.setName("queue");
    endTransmissionEvent = new cMessage("endTxEvent");

    frameCapacity = par("frameCapacity");

    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    dropSignal = registerSignal("drop");
    txBytesSignal = registerSignal("txBytes");
    rxBytesSignal = registerSignal("rxBytes");

    emit(qlenSignal, queue.length());
    emit(busySignal, 0);
}

void L2Queue::startTransmitting(cMessage *msg)
{
    if (ev.isGUI()) displayStatus(true);

    //EV << "Starting transmission of " << msg << endl;
    int64 numBytes = check_and_cast<cPacket *>(msg)->getByteLength();
    send(msg, "line$o");

    emit(txBytesSignal, (long)numBytes);

    // Schedule an event for the time when last bit will leave the gate.
    simtime_t endTransmission = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission, endTransmissionEvent);
}

void L2Queue::handleMessage(cMessage *msg)
{
    if (msg==endTransmissionEvent)
    {
        // Transmission finished, we can start next one.
        //EV << "Transmission finished.\n";
        if (ev.isGUI()) displayStatus(false);
        if (queue.empty())
        {
            emit(busySignal, 0);
        }
        else
        {
            msg = (cMessage *) queue.pop();
            emit(queueingTimeSignal, simTime() - msg->getTimestamp());
            emit(qlenSignal, queue.length());
            startTransmitting(msg);
        }
    }
    else if (msg->arrivedOn("line$i"))
    {
        // pass up
        emit(rxBytesSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
        send(msg,"out");
    }
    else // arrived on gate "in"
    {
        if (endTransmissionEvent->isScheduled())
        {
            // We are currently busy, so just queue up the packet.
            if (frameCapacity && queue.length()>=frameCapacity)
            {
                EV << "LOSS     Rcvd " << msg << " but transmitter busy and queue full: discarding\n";
                emit(dropSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
                delete msg;
            }
            else
            {
                EV << "ENQUEUE     Rcvd " << msg << " but transmitter busy: queueing up\n";
                msg->setTimestamp();
                queue.insert(msg);
                emit(qlenSignal, queue.length());
            }
        }
        else
        {
            // We are idle, so we can start transmitting right away.
            EV << "DIRECT " << msg << "queue idle -> transmit"<< endl;
            emit(queueingTimeSignal, 0.0);
            startTransmitting(msg);
            emit(busySignal, 1);
        }
    }
}

void L2Queue::displayStatus(bool isBusy)
{
    getDisplayString().setTagArg("t",0, isBusy ? "transmitting" : "idle");
    getDisplayString().setTagArg("i",1, isBusy ? (queue.length()>=3 ? "red" : "yellow") : "");
}

void L2Queue::dispose()
{
    while(queue.length()>0){
        cMessage *msg = (cMessage *) queue.pop();
        cancelAndDelete(msg);
    }
    emit(qlenSignal, queue.length());

}
