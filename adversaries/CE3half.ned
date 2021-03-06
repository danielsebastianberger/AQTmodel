//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package adversarialqueueing.adversaries;

//
// Application model to generate traffic for the network.
//
simple CE3Adversary
{
    parameters:
	double timeSlotLength @unit(s) = default(1ms); // time between generating packets
        int frameCapacity = default(1000); // L2Queue frame capacity (default value is not meaningful, but means infinite queue for L2Queue)
        int initialSetSize = default(100); // initial set size
        double injectionRate = default(1); // L2Queue frame capacity (default value is not meaningful, but means infinite queue for L2Queue)
	string gaussianInjTime; // set standard deviation of how much injections may deviate from scheduled time
        @signal[injectedPackets](type="long");
        @statistic[injectedPackets](title="injected Packets"; interpolationmode=none; record=count);
	@statistic[injectionTime](title="injectionTime";record=vector?;interpolationmode=sample-hold);
    gates:
        output adversaryControl @loose;
}


