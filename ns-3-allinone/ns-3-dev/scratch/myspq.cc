#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include<typeinfo>
#include "ns3/output-stream-wrapper.h"
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/myspq.h"
#include "ns3/netanim-module.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/stats-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MYSPQ");

int main (int argc, char *argv[])
{
	Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("60000p"));
	//LogComponentEnable ("MYSPQ", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpServer", LOG_LEVEL_ALL);
	
	CommandLine cmd;
	int inputCount = 1;
	int outputCount = 1;
	cmd.AddValue ("inputCount","the number of inputServer(s)",inputCount);
	cmd.AddValue ("outputCount","the number of outputServer(s)", outputCount);
	cmd.Parse (argc, argv);
	
	//ports=9 10 11 12 13 14 15 16
	
	NS_LOG_INFO ("Install nodes");
	NodeContainer inputServers;
	inputServers.Create(inputCount);
	NodeContainer outputServers;
	outputServers.Create(outputCount);
	NodeContainer router;
	router.Create(1);
	
	NS_LOG_INFO ("Install channels");
	NodeContainer in_router[inputCount];
	NodeContainer router_out[outputCount];
	PointToPointHelper p2p_in[inputCount];
	PointToPointHelper p2p_out[outputCount];
	
	//no spq test
	/*
	NetDeviceContainer d_in[inputCount];
	for(int n=0;n<inputCount;n++){
		p2p_in[n].SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
		in_router[n]=NodeContainer(inputServers.Get(n),router.Get(0));
		d_in[n] = p2p_in[n].Install (in_router[n]);
	}
	NetDeviceContainer d_out[outputCount];
	for(int m=0;m<outputCount;m++)
	{
		p2p_out[m].SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
		router_out[m]=NodeContainer(router.Get(0),outputServers.Get(m));
		d_out[m] = p2p_out[m].Install (router_out[m]);
	}*/
	
	int numQueues = 8;
	std::string queueLevels = "01234567";
	
	NetDeviceContainer d_in[inputCount];			   
	for(int n=0;n<inputCount;n++)
	{
		p2p_in[n].SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
		p2p_in[n].SetQueue ("ns3::MYSPQ",        
					 "Mode", StringValue ("QUEUE_MODE_PACKETS"),
                     "NumberOfQueues", IntegerValue (numQueues),
                     "QueueLevels", StringValue (queueLevels),
                     "Setup", IntegerValue (1)); 
		in_router[n]=NodeContainer(inputServers.Get(n),router.Get(0));
		d_in[n] = p2p_in[n].Install (in_router[n]);
	}
	
	NetDeviceContainer d_out[outputCount];
	for(int m=0;m<outputCount;m++)
	{
		p2p_out[m].SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
		p2p_out[m].SetQueue ("ns3::MYSPQ",        
					 "Mode", StringValue ("QUEUE_MODE_PACKETS"),
                     "NumberOfQueues", IntegerValue (numQueues),
                     "QueueLevels", StringValue (queueLevels),
                     "Setup", IntegerValue (1)); 
		router_out[m]=NodeContainer(router.Get(0),outputServers.Get(m));
		d_out[m] = p2p_out[m].Install (router_out[m]);
	}
	
	NS_LOG_INFO ("Assign addresses");
	InternetStackHelper stack;
	stack.Install(inputServers);
	stack.Install(outputServers);
	stack.Install(router);
	
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");

	Ipv4InterfaceContainer ii[inputCount];
	for(int n=0;n<inputCount;n++)
	{
		ii[n]=address.Assign(d_in[n]);
	}
	
	Ipv4InterfaceContainer ii2[outputCount];
	for(int m=0;m<outputCount;m++)
	{
		string ip = "10.1." + std::to_string(m+2) +".0";
		address.SetBase (ns3::Ipv4Address(ip.c_str()), "255.255.255.0");
		ii2[m]=address.Assign(d_out[m]);
	}
	
	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
	
	Ipv4StaticRoutingHelper routingHelper;
	for(int n=0;n<inputCount;n++){
		Ptr<Ipv4> ipv4 = inputServers.Get(n)->GetObject<Ipv4>();
		Ptr<Ipv4StaticRouting> routing = routingHelper.GetStaticRouting(ipv4);
		for(int m=0;m<outputCount;m++){
			routing->AddHostRouteTo(ii2[m].GetAddress(1),ii[n].GetAddress(1),1);
		}
	}
		
	Packet::EnablePrinting ();
	double start = 1.0;
	double stop = 50.0;   
	
	NS_LOG_INFO ("Install Server App");
	uint16_t port1 = 9;
    uint16_t port2 = 10;
    uint16_t port3 = 11;
    uint16_t port4 = 12;
    uint16_t port5 = 13;
    uint16_t port6 = 14;
    uint16_t port7 = 15;
    uint16_t port8 = 16;
	UdpServerHelper server1(port1);
    UdpServerHelper server2(port2);
    UdpServerHelper server3(port3);
    UdpServerHelper server4(port4);
    UdpServerHelper server5(port5);
    UdpServerHelper server6(port6);
    UdpServerHelper server7(port7);
    UdpServerHelper server8(port8);
	for(int m=0;m<outputCount;m++){
	  ApplicationContainer apps = server1.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server2.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server3.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server4.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server5.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server6.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server7.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));

	  apps = server8.Install (outputServers.Get(m));
	  apps.Start (Seconds (start));
	  apps.Stop (Seconds (stop));	
	}	  
		
/*	
    //循环添加udpserverclient
	uint16_t port[numQueues]={9,10,11,12,13,14,15,16};
	std::vector<UdpServerHelper *> servers;	
	for(int i=0;i<numQueues;i++){
		servers.push_back (new UdpServerHelper(port[i]));
	}

	ApplicationContainer apps;
	for(int m=0;m<outputCount;m++){
		for(int i=0;i<numQueues;i++){
			UdpServerHelper* u = servers.at(i);
			apps = u->Install(outputServers.Get(m));
			apps.Start (Seconds (start));
			apps.Stop (Seconds (stop));
		}
	}
*/
	
	//
	// Create MYSPQ application to send UDP datagrams from inputservers to outputservers
	//
	NS_LOG_INFO ("Install Client App");
	uint32_t packetSize = 1000;
	uint32_t maxPacketCount = 50;
	Time interPacketInterval = MicroSeconds (1000);
	double t = start+1;
	for(int n=0;n<inputCount;n++){
		for(int m=0;m<outputCount;m++){
			  UdpClientHelper client1 (ii2[m].GetAddress(1),port1);
			  client1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client1.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client1.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  ApplicationContainer apps = client1.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;
			  
			  UdpClientHelper client2(ii2[m].GetAddress(1),port2);
			  client2.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client2.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client2.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client2.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client3(ii2[m].GetAddress(1),port3);
			  client3.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client3.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client3.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client3.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client4(ii2[m].GetAddress(1),port4);
			  client4.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client4.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client4.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client4.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client5(ii2[m].GetAddress(1),port5);
			  client5.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client5.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client5.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client5.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client6(ii2[m].GetAddress(1),port6);
			  client6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client6.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client6.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client6.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client7(ii2[m].GetAddress(1),port7);
			  client7.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client7.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client7.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client7.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;

			  UdpClientHelper client8(ii2[m].GetAddress(1),port8);
			  client8.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
			  client8.SetAttribute ("Interval", TimeValue (interPacketInterval));
			  client8.SetAttribute ("PacketSize", UintegerValue (packetSize));

			  apps = client8.Install (inputServers.Get(n));
			  apps.Start (Seconds (t));
			  apps.Stop (Seconds (stop));
			  t = t + 0.1;
		}
	}
			
	/*循环添加udpclienthelper
	double t = 0.0;
	for(int n = 0;n < inputCount; n++){
		for(int m = 0;m < outputCount; m++){
			for(int i = 0;i<numQueues;i++){
				UdpClientHelper* client = new UdpClientHelper(ii2[m].GetAddress(1),port[i]);
				client->SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
				client->SetAttribute ("Interval", TimeValue (interPacketInterval));
				client->SetAttribute ("PacketSize", UintegerValue (packetSize));
				apps = client->Install (inputServers.Get (n));
				cout << "inputServer:" <<n<<":"<<ii[n].GetAddress(0)<<"install app to node:"<<m<<":"<<ii2[m].GetAddress(1)<<"port:"<<port[i]<< endl;
				apps.Start (Seconds (start + 2 + t));
				apps.Stop (Seconds (stop));
				t = t + 0.1;
			}	
		}	
	}*/
	
	AsciiTraceHelper ascii;
	std::string fileName ="result/myspq-";
	for(int n=0;n<inputCount;n++){
		p2p_in[n].EnablePcapAll(fileName,false);
	}
	
	for(int m=0;m<outputCount;m++){
		p2p_out[m].EnablePcapAll(fileName,false);
	}

    //生成netanim可视化文件
	for(int n=0;n<inputCount;n++){
		AnimationInterface::SetConstantPosition(inputServers.Get (n), 10 , (n+1)*10);
    }
    AnimationInterface::SetConstantPosition(router.Get (0), 20 , 20);
    for(int m=0;m<outputCount;m++){
		AnimationInterface::SetConstantPosition(outputServers.Get (m), 30,(m+1)*10);
    }
    AnimationInterface anim ("result/myspq.xml");
	anim.EnablePacketMetadata(true);
	
	//数据收集
	
	std::string probeType = "ns3::Ipv4PacketProbe";
	std::string tracePath = "/NodeList/*/$ns3::Ipv4L3Protocol/Tx";
	/*
	GnuplotHelper plotHelper;
    plotHelper.ConfigurePlot ("myspq-packet-byte-count","Packet Byte Count vs. Time","Time (Seconds)",
    plotHelper.PlotProbe (probeType,tracePath,"OutputBytes","Packet Byte Count", GnuplotAggregator::KEY_BELOW);
	
	FileHelper fileHelper;
	fileHelper.ConfigureFile ("result/myspq-packet-byte-count",FileAggregator::FORMATTED);
	fileHelper.Set2dFormat ("%.3e\t%.0f");
    //fileHelper.Set2dFormat ("Time (Seconds) = %.3e\tPacket Byte Count = %.0f");
	fileHelper.WriteProbe (probeType,
                           tracePath,
                           "OutputBytes");	
	*/
	
	
	//
	// Now, do the actual simulation.
    //	
	
	NS_LOG_INFO ("Start!");
	Simulator::Run ();	
	Simulator::Simulator::Destroy ();
	NS_LOG_INFO ("Done.");
	 
	return 0;
}



	

		
	
	
		
		
		
		
	
	
	
	
	
	
	
