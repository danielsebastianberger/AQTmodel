
package adversarialqueueing.networks;

import adversarialqueueing.adversaries.AdvancedAdversary;
import adversarialqueueing.node.SourceRouting;
import adversarialqueueing.channelvariation.Randomization;
import adversarialqueueing.node.SourceRoutingNode;
import adversarialqueueing.channelvariation.VDrChannel;


//
// This is the basic omnet network description of a circular model for CE7. 
//
network VarCE71
{
    parameters:
        string adType;
        @display("bgb=688,735,white,,0");
    types:
        channel C extends VDrChannel
        {
        }
    submodules:

        adversary: <adType> like AdvancedAdversary {
            @display("p=352,300;i=block/blackboard,#AA0000;is=s");
        }

        n12: SourceRoutingNode {
            @display("p=29,50;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 112;
        }
        n91: SourceRoutingNode {
            @display("p=83,199;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 191;
        }
        n22: SourceRoutingNode {
            @display("p=145,99;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 122;
        }
        n92: SourceRoutingNode {
            @display("p=83,268;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 192;
        }
        n11: SourceRoutingNode {
            @display("p=650,50;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 111;
        }
        n32: SourceRoutingNode {
            @display("p=264,149;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 132;
        }
        n94: SourceRoutingNode {
            @display("p=196,268;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 194;
        }
        n95: SourceRoutingNode {
            @display("p=196,337;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 195;
        }
        n97: SourceRoutingNode {
            @display("p=196,416;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 197;
        }
        n93: SourceRoutingNode {
            @display("p=83,337;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 193;
        }
        n96: SourceRoutingNode {
            @display("p=83,416;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 196;
        }
        m11: SourceRoutingNode {
            @display("p=29,571;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 211;
        }
        m21: SourceRoutingNode {
            @display("p=145,520;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 221;
        }
        m31: SourceRoutingNode {
            @display("p=264,471;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 231;
        }
        n21: SourceRoutingNode {
            @display("p=541,99;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 121;
        }
        n31: SourceRoutingNode {
            @display("p=437,149;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 131;
        }
        m32: SourceRoutingNode {
            @display("p=437,471;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 232;
        }
        m22: SourceRoutingNode {
            @display("p=547,520;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 222;
        }
        m12: SourceRoutingNode {
            @display("p=650,571;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 212;
        }
        m91: SourceRoutingNode {
            @display("p=592,416;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 291;
        }
        m92: SourceRoutingNode {
            @display("p=592,343;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 292;
        }
        m93: SourceRoutingNode {
            @display("p=592,268;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 293;
        }
        m95: SourceRoutingNode {
            @display("p=494,268;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 295;
        }
        m97: SourceRoutingNode {
            @display("p=494,199;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 297;
        }
        m94: SourceRoutingNode {
            @display("p=494,343;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 294;
        }
        m96: SourceRoutingNode {
            @display("p=592,199;b=15,15,oval,#868686,,1;tt=q;i=misc/node2,#676767,100;is=vs");
            address = 296;
        }
    connections:

        n91.port++ <--> C <--> n22.port++;
        n12.port++ <--> C <--> n91.port++;
        n91.port++ <--> C <--> n92.port++;
        n32.port++ <--> C <--> n94.port++;
        n22.port++ <--> C <--> n94.port++;
        n94.port++ <--> C <--> n95.port++;
        n95.port++ <--> C <--> n92.port++;
        n92.port++ <--> C <--> n93.port++;
        n12.port++ <--> C <--> n94.port++;
        n32.port++ <--> C <--> n91.port++;
        n11.port++ <--> C <--> n12.port++;
        n21.port++ <--> C <--> n22.port++;
        n31.port++ <--> C <--> n32.port++;
        m31.port++ <--> C <--> m32.port++;
        m21.port++ <--> C <--> m22.port++;
        m11.port++ <--> C <--> m12.port++;
        m12.port++ <--> C <--> m91.port++;
        m12.port++ <--> C <--> m94.port++;
        m22.port++ <--> C <--> m91.port++;
        m22.port++ <--> C <--> m94.port++;
        m32.port++ <--> C <--> m91.port++;
        m32.port++ <--> C <--> m94.port++;
        m91.port++ <--> C <--> m92.port++;
        m92.port++ <--> C <--> m93.port++;
        m94.port++ <--> C <--> m95.port++;
        m95.port++ <--> C <--> m92.port++;
        m32.port++ <--> C <--> n31.port++;
        m22.port++ <--> C <--> n21.port++;
        m12.port++ <--> C <--> n11.port++;
        n32.port++ <--> C <--> m31.port++;
        n22.port++ <--> C <--> m21.port++;
        n12.port++ <--> C <--> m11.port++;
        n93.port++ <--> C <--> n96.port++;
        n96.port++ <--> C <--> m11.port++;
        n96.port++ <--> C <--> m21.port++;
        n96.port++ <--> C <--> m31.port++;
        m96.port++ <--> C <--> n31.port++;
        m96.port++ <--> C <--> n21.port++;
        m96.port++ <--> C <--> n11.port++;
        m93.port++ <--> C <--> m96.port++;
        n93.port++ <--> C <--> m93.port++;
        m95.port++ <--> C <--> m97.port++;
        m97.port++ <--> C <--> n31.port++;
        m97.port++ <--> C <--> n21.port++;
        m97.port++ <--> C <--> n11.port++;
        n95.port++ <--> C <--> n97.port++;
        n97.port++ <--> C <--> m11.port++;
        n97.port++ <--> C <--> m21.port++;
        n97.port++ <--> C <--> m31.port++;
        n96.port++ <--> C <--> n95.port++;
        m96.port++ <--> C <--> m95.port++;
}

