//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/SourceRoutingPacket.msg.
//

#ifndef _SOURCEROUTINGPACKET_M_H_
#define _SOURCEROUTINGPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0405
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/SourceRoutingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet SourceRoutingPacket {
 *     int srcAddr;							
 *     int destAddr[]; 					
 *     int hopCount;							
 *     int recordRoute[];
 * }
 * </pre>
 */
class SourceRoutingPacket : public ::cPacket
{
  protected:
    int srcAddr_var;
    int *destAddr_var; // array ptr
    unsigned int destAddr_arraysize;
    int hopCount_var;
    int *recordRoute_var; // array ptr
    unsigned int recordRoute_arraysize;

  private:
    void copy(const SourceRoutingPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SourceRoutingPacket&);

  public:
    SourceRoutingPacket(const char *name=NULL, int kind=0);
    SourceRoutingPacket(const SourceRoutingPacket& other);
    virtual ~SourceRoutingPacket();
    SourceRoutingPacket& operator=(const SourceRoutingPacket& other);
    virtual SourceRoutingPacket *dup() const {return new SourceRoutingPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getSrcAddr() const;
    virtual void setSrcAddr(int srcAddr);
    virtual void setDestAddrArraySize(unsigned int size);
    virtual unsigned int getDestAddrArraySize() const;
    virtual int getDestAddr(unsigned int k) const;
    virtual void setDestAddr(unsigned int k, int destAddr);
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
    virtual void setRecordRouteArraySize(unsigned int size);
    virtual unsigned int getRecordRouteArraySize() const;
    virtual int getRecordRoute(unsigned int k) const;
    virtual void setRecordRoute(unsigned int k, int recordRoute);
};

inline void doPacking(cCommBuffer *b, SourceRoutingPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SourceRoutingPacket& obj) {obj.parsimUnpack(b);}


#endif // _SOURCEROUTINGPACKET_M_H_
