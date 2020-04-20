/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include <iostream>
using namespace std;

#include "myspq.h"
#include "ns3/filter.h"
#include "ns3/trafficclass.h"
#include "ns3/string.h"
#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"

namespace ns3 {

  NS_OBJECT_ENSURE_REGISTERED (MYSPQ);

  // NS_LOG_COMPONENT_DEFINE ("MYSPQ");
/* ... */ 


  TypeId
  MYSPQ::GetTypeId (void) {
    static TypeId tid = TypeId ("ns3::MYSPQ<Packet>")
      .SetParent<DiffServ<Packet>> ()
      .SetGroupName("Network")
      .template AddConstructor<MYSPQ> ()
      .AddAttribute ("Mode",
                     "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                     EnumValue(QUEUE_MODE_PACKETS),
                     MakeEnumAccessor(&MYSPQ::SetMode,&MYSPQ::GetMode),
                     MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
      .AddAttribute ("NumberOfQueues", "The number of queues to use in MYSPQ", IntegerValue (0),
                     MakeIntegerAccessor (&MYSPQ::SetNumberOfQueues),
                     MakeIntegerChecker<int> (INT64_MIN, INT64_MAX))
      .AddAttribute ("QueueLevels",
                     "A string that represents the level for each queue. Number of chars must "
                     "match queue levels.",
                     StringValue (""), MakeStringAccessor (&MYSPQ::SetQueueLevels),
                     MakeStringChecker ())
      .AddAttribute ("Setup", "Initiates setup for MYSPQ queue.", IntegerValue (1),
                     MakeIntegerAccessor (&MYSPQ::Setup),
                     MakeIntegerChecker<int> (INT64_MIN, INT64_MAX));
    ;
    return tid;
  }

  // template <typename Item>
  MYSPQ::MYSPQ () : DiffServ (), NS_LOG_TEMPLATE_DEFINE ("MYSPQ")
  {
    NS_LOG_FUNCTION (this);
  }

  // template <typename Item>
  MYSPQ::~MYSPQ () {}

  void
  MYSPQ::Setup (int s)
  {
    NS_LOG_FUNCTION (this);
	int i;
    int port = 9;
    for (i = 0; i < m_numberOfQueues; i++){
        // New traffic class
        q_class.push_back (new TrafficClass ());
        q_class[i]->SetPriorityLevel (m_queueLevels[i]);
        q_class[i]->SetMode (0);
        q_class[i]->SetMaxPackets (20000);
        // New filter elements vec
        vector<FilterElements *> fe;
        fe.push_back (new Destination_Port_Number (port));
        port = port + 1;
        // Add to filters
        vector<Filter *> f;
        f.push_back (new Filter ());
        f[0]->set (fe);
        q_class[i]->SetFilters (f);
	}
  }

  // template <typename Item>
  Ptr<Packet>
  MYSPQ::Schedule (void) {
    NS_LOG_FUNCTION (this);
   //check from high queues
    Ptr<Packet> p;
/*    for(int i = m_numberOfQueues-1;i >= 0;i--){
	    if(!q_class[i]->isEmpty()){
		p = q_class[i]->Dequeue();
		return p;
	    }
    }
    return 0;*/
	if(!q_class[7]->isEmpty()){
		p = q_class[7]->Dequeue();
		return p;
    }
	else if(!q_class[6]->isEmpty()){
		p = q_class[6]->Dequeue();
		return p;
	}
	else if(!q_class[6]->isEmpty()){
		p = q_class[6]->Dequeue();
		return p;
	}	
	else if(!q_class[5]->isEmpty()){
		p = q_class[5]->Dequeue();
		return p;
	}
	else if(!q_class[4]->isEmpty()){
		p = q_class[4]->Dequeue();
		return p;
	}
	else if(!q_class[3]->isEmpty()){
		p = q_class[3]->Dequeue();
		return p;
	}
	else if(!q_class[2]->isEmpty()){
		p = q_class[2]->Dequeue();
		return p;
	}
	else if(!q_class[1]->isEmpty()){
		p = q_class[1]->Dequeue();
		return p;
	}
	else if(!q_class[0]->isEmpty()){
		p = q_class[0]->Dequeue();
		return p;
	}
    else{
        return 0;
	}		
  }

  // template <typename Item>
  uint32_t
  MYSPQ::Classify (Ptr<Packet> p) {
	 NS_LOG_FUNCTION (this);
	 NS_LOG_FUNCTION ("Destination_Port_Number");

	 NS_LOG_FUNCTION (p);
	 Ptr<Packet> copy = p->Copy ();
	 PppHeader ph;
	 Ipv4Header iph;
	 UdpHeader uh;
	 copy->RemoveHeader(ph);
	 copy->RemoveHeader (iph);
	 copy->RemoveHeader (uh);
	 uint16_t port = uh.GetDestinationPort();
	 NS_LOG_FUNCTION (port);
	 int level = port - 9;
	 if(level>7){
		 return 7;
	 }
	 if(level<0){
		 return 0;
	 }
	 return level;
	 
  }

  // template <typename Item>
  bool
  MYSPQ::Enqueue(Ptr<Packet> p) {
    NS_LOG_FUNCTION (this);
    //call Classify
    bool res = false;
    uint32_t priorityLevel = Classify(p);
    res = q_class[priorityLevel]->Enqueue(p);
    return res;
  }

  // template <typename Item>
  Ptr<Packet>
  MYSPQ::Dequeue (void) {
    NS_LOG_FUNCTION (this);
    //call Schedule
    Ptr<Packet> p = Schedule();
    // printf("dequeue packet size: %d\n", p->GetSize ());
    return p;
  }

/**
* Peek the front item in the queue without removing it.
  Retun 0 if queue(s) is(are) empty.
*/
  // template <typename Item>
  Ptr<const Packet>
  MYSPQ::Peek (void) const {
    NS_LOG_FUNCTION (this);
    Ptr<Packet> p;
/*    for(int i = m_numberOfQueues-1;i >= 0;i--){
	if(!q_class[i]->isEmpty()){
		p = q_class[i]->m_queue.front();
		return p;
	}
    }
    return 0;*/
    if(!q_class[7]->isEmpty()){
		p = q_class[7]->m_queue.front();
		return p;
	} 
	else if(!q_class[6]->isEmpty()){
        p = q_class[6]->m_queue.front();
        return p;
    }
	else if(!q_class[6]->isEmpty()){
        p = q_class[6]->m_queue.front();
        return p;
    } 
	else if(!q_class[5]->isEmpty()){
        p = q_class[5]->m_queue.front();
        return p;
    }
	else if(!q_class[4]->isEmpty()){
        p = q_class[4]->m_queue.front();
        return p;
    } 
	else if(!q_class[3]->isEmpty()){
        p = q_class[3]->m_queue.front();
        return p;
    }
	else if(!q_class[2]->isEmpty()){
        p = q_class[2]->m_queue.front();
        return p;
    }
	else if(!q_class[1]->isEmpty()){
        p = q_class[1]->m_queue.front();
        return p;
    }
	else if(!q_class[0]->isEmpty()){
        p = q_class[0]->m_queue.front();
        return p;
    } 	
	else {
        return 0;
    }
  }


  /**
   Pull the item to drop from the queue
  */
  // template <typename Item>
  Ptr<Packet>
  MYSPQ::Remove (void) {
    NS_LOG_FUNCTION (this);
    Ptr<Packet> p;
/*    for(int i = m_numberOfQueues-1;i >= 0;i--){
	if(!q_class[i]->isEmpty()){
		p = q_class[i]->Dequeue();
		return p;
	}
	if(i == 0 && q_class[0]->isEmpty()){
		return 0;
	}
    }*/
	if(!q_class[7]->isEmpty()){
		p = q_class[7]->Dequeue();
		return p;
    }
	else if(!q_class[6]->isEmpty()){
		p = q_class[6]->Dequeue();
		return p;
	}
	else if(!q_class[6]->isEmpty()){
		p = q_class[6]->Dequeue();
		return p;
	}	
	else if(!q_class[5]->isEmpty()){
		p = q_class[5]->Dequeue();
		return p;
	}
	else if(!q_class[4]->isEmpty()){
		p = q_class[4]->Dequeue();
		return p;
	}
	else if(!q_class[3]->isEmpty()){
		p = q_class[3]->Dequeue();
		return p;
	}
	else if(!q_class[2]->isEmpty()){
		p = q_class[2]->Dequeue();
		return p;
	}
	else if(!q_class[1]->isEmpty()){
		p = q_class[1]->Dequeue();
		return p;
	}
	else if(!q_class[0]->isEmpty()){
		p = q_class[0]->Dequeue();
		return p;
	}
    else{
        return 0;
	}
    DropAfterDequeue(p);
    return p;
  }


  // template <typename Item>
  void
  MYSPQ::SetMode (MYSPQ::QueueMode mode) {
    m_mode = mode;
  }

  //template <typename Item>
  typename MYSPQ::QueueMode
  MYSPQ::GetMode (void) const {
    return m_mode;
  }

  void
  MYSPQ::SetNumberOfQueues (int numberOfQueues)
  {
    NS_LOG_FUNCTION (this << numberOfQueues);
    m_numberOfQueues = numberOfQueues;
  }

  void
  MYSPQ::SetQueueLevels (std::string queueLevels)
  {
    NS_LOG_FUNCTION (this << queueLevels);
    m_queueLevels = queueLevels;
  }

  // NS_OBJECT_TEMPLATE_CLASS_DEFINE (MYSPQ,Packet);
  // NS_OBJECT_TEMPLATE_CLASS_DEFINE (MYSPQ,QueueDiscItem);
}
