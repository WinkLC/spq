/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// - Tracing of queues and packet receptions to file "cda.tr"

#include <fstream>
#include <string>
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "src/json/json.h"
#include "ns3/spq.h"
#include "ns3/myspq.h"
#include "ns3/netanim-module.h"
#include "ns3/udp-client-server-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MYSPQ");

int
main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("60000p"));
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
  // LogComponentEnable ("SPQ", LOG_LEVEL_INFO);
  // LogComponentEnable ("CdaServerApplication", LOG_LEVEL_ALL);
  // LogComponentEnable ("SPQ", LOG_LEVEL_ALL);
  // LogComponentEnable ("TrafficClass", LOG_LEVEL_ALL);
  // LogComponentEnable ("DropTailQueue", LOG_LEVEL_ALL);
  // LogComponentEnable ("Filter", LOG_LEVEL_ALL);

  CommandLine cmd;

  std::string inputFile = "";

  cmd.AddValue ("i", "inputFile for SPQ Application", inputFile);
  cmd.Parse (argc, argv);

  std::cout << "InputFile: " << inputFile << std::endl;

  // Read the json config file
  Json::Value root;
  Json::CharReaderBuilder rbuilder;
  std::string errs;
  std::ifstream config_doc (inputFile, std::ifstream::binary);
  config_doc >> root;
  Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  Json::StreamWriterBuilder wbuilder;
  // Get number of queues
  const Json::Value outputNumQueues = root["numberOfQueues"];
  std::string numQueuesStr = Json::writeString (wbuilder, outputNumQueues);

  int numQueues = stoi (numQueuesStr);
  std::string queueLevels;
  int i;
  for (i = 0; i < numQueues; i++)
    {
      std::string q = "q" + to_string (i);
      const Json::Value outputQ0 = root[q];
      std::string q0Str = Json::writeString (wbuilder, outputQ0);
      // int qVal = stoi (q0Str);
      queueLevels = queueLevels + q0Str;
    }
  // End json parsing
  cout << "numQueues:" << numQueues << endl;
  cout << "queueLevels:" << queueLevels << endl;
  cout << queueLevels.compare("01234567") << endl;
  // Explicitly create the nodes required by the topology.
  NodeContainer n;
  n.Create (3);

  NodeContainer n0n1 = NodeContainer (n.Get (0), n.Get (1));
  NodeContainer n1n2 = NodeContainer (n.Get (1), n.Get (2));;

  PointToPointHelper p0p1;
  p0p1.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));

  NetDeviceContainer c0c1 = p0p1.Install (n0n1);

  PointToPointHelper p1p2;
  p1p2.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p1p2.SetQueue ("ns3::MYSPQ",
                 "Mode", StringValue ("QUEUE_MODE_PACKETS"),
                 "NumberOfQueues", IntegerValue (numQueues),
                 "QueueLevels", StringValue (queueLevels),
                 "Setup", IntegerValue (1));

  NetDeviceContainer c1c2 = p1p2.Install (n1n2);

  InternetStackHelper stack;
  stack.Install (n);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer i0i1 = address.Assign (c0c1);
  Ipv4InterfaceContainer i1i2 = address.Assign (c1c2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //
  // Create a CdaServer application on node 2.
  //

  Packet::EnablePrinting ();
  double start = 1.0;
  double stop = 300.0;
  uint16_t port1 = 9;
  uint16_t port2 = 10;
  uint16_t port3 = 11;
  uint16_t port4 = 12;
  uint16_t port5 = 13;
  uint16_t port6 = 14;
  uint16_t port7 = 15;
  uint16_t port8 = 16;

 // CdaServerHelper server1 (port1);
 // CdaServerHelper server2 (port2);
  UdpServerHelper server1(port1);
  UdpServerHelper server2(port2);
  UdpServerHelper server3(port3);
  UdpServerHelper server4(port4);
  UdpServerHelper server5(port5);
  UdpServerHelper server6(port6);
  UdpServerHelper server7(port7);
  UdpServerHelper server8(port8);

  ApplicationContainer apps = server1.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server2.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server3.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server4.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server5.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server6.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server7.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server8.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  //
  // Create a SPQ application to send UDP datagrams from node zero to
  // node two.
  //
  uint32_t packetSize1 = 1000;
  uint32_t maxPacketCount1 = 15000;
  Time interPacketInterval1 = MicroSeconds (10);

  uint32_t packetSize2 = 1000;
  uint32_t maxPacketCount2 = 5000;
  Time interPacketInterval2 = MicroSeconds (10);

  UdpClientHelper client1 (i1i2.GetAddress(1),port1);
//  CdaClientHelper client1 (i1i2.GetAddress (1), port1);
  client1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client1.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client1.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client1.Install (n.Get (0));
  apps.Start (Seconds (start + 1));
  apps.Stop (Seconds (stop));

  UdpClientHelper client2(i1i2.GetAddress(1),port2);
 // CdaClientHelper client2 (i1i2.GetAddress (1), port2);
  client2.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount2));
  client2.SetAttribute ("Interval", TimeValue (interPacketInterval2));
  client2.SetAttribute ("PacketSize", UintegerValue (packetSize2));

  apps = client2.Install (n.Get (0));
  apps.Start (Seconds (start + 1.1));
  apps.Stop (Seconds (stop));

  UdpClientHelper client3(i1i2.GetAddress(1),port3);
  client3.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client3.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client3.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client3.Install (n.Get (0));
  apps.Start (Seconds (start + 1.2));
  apps.Stop (Seconds (stop));

  UdpClientHelper client4(i1i2.GetAddress(1),port4);
  client4.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount2));
  client4.SetAttribute ("Interval", TimeValue (interPacketInterval2));
  client4.SetAttribute ("PacketSize", UintegerValue (packetSize2));

  apps = client4.Install (n.Get (0));
  apps.Start (Seconds (start + 1.3));
  apps.Stop (Seconds (stop));

  UdpClientHelper client5(i1i2.GetAddress(1),port5);
  client5.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client5.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client5.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client5.Install (n.Get (0));
  apps.Start (Seconds (start + 1.4));
  apps.Stop (Seconds (stop));

  UdpClientHelper client6(i1i2.GetAddress(1),port6);
  client6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount2));
  client6.SetAttribute ("Interval", TimeValue (interPacketInterval2));
  client6.SetAttribute ("PacketSize", UintegerValue (packetSize2));

  apps = client6.Install (n.Get (0));
  apps.Start (Seconds (start + 1.5));
  apps.Stop (Seconds (stop));

  UdpClientHelper client7(i1i2.GetAddress(1),port7);
  client7.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client7.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client7.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client7.Install (n.Get (0));
  apps.Start (Seconds (start + 1.6));
  apps.Stop (Seconds (stop));

  UdpClientHelper client8(i1i2.GetAddress(1),port8);
  client8.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client8.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client8.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client8.Install (n.Get (0));
  apps.Start (Seconds (start + 1.7));
  apps.Stop (Seconds (stop));

  AsciiTraceHelper ascii;

  std::string fileName = "spq-";
  p0p1.EnablePcapAll (fileName, false);
  p1p2.EnablePcapAll (fileName, false);

  //
  // Now, do the actual simulation.
  //
  AnimationInterface anim ("spq.xml");
  Ipv4StaticRoutingHelper routingHelper;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>("route2.tr",std::ios::out);
  routingHelper.PrintRoutingTableAt(Seconds(3.0),n.Get(1),routingStream);

  Simulator::Run ();
  Simulator::Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}
