#ifndef SERVICECHAIN_H   //prevents multiple inclusions of header file
#define	SERVICECHAIN_H
#include "VNF.h"
#include <string>
#include "TypeDef.h"
#include "ConstDef.h"
#include <list>
#include <vector>
//#include "MappedLinkList.h"
//#include "OchObject.h"


namespace NS_OCH {
//template<class Key, class T> class MappedLinkList; 	
class Connection;
class OXCNode;
class AbstractLink;

class ServiceChain {
public:
	//int id;
	double					 latency;		//in second
	double					 availability; //in Second
	int						Wavelength;	//wavelength associated  with this SC
	//int					 reqCapacity;
	//bool					 IsReq;			//if the SC is requested 
	int						 LengthOfSCs; //typcial length of service chain 
	VNF **					 VNFList;
	std::string				 VNFsName;	//name of vnfs that build the SC
	//MappedLinkList		<UINT, ServiceChain*> SCList; 
	int						NumofUsers;  //number of users requested this specific SC
	double					 HoldingTime;
	double					 StartTime;
	double					 BwReq;   //in Mbps
	//BandwidthGranularity	 bw;
	Connection				 *con;
	std::list<AbstractLink*> SCPath;
	std::vector <OXCNode*>	 SCnodes;
	OXCNode *				 LastNode;			//LA: last node on the chain that previous vnf on the SC is provisioned on it
	bool					local;				//LA:for mixed sc scenario where we serve some of the sc locally if possible
	
	ServiceType				 SCtype;
	
	ServiceChain();
	~ServiceChain();
	
	
	void SetSCType ();  //Based on a random number that is generated it sets a type of SC
	
	//MappedLinkList <UINT, ServiceChain*> SCnode;
	void SetNumberOfUsers();		//Sets number of users randomy in a range from 1000 to 4000
	std::string GetServiceChainName();

};

};

#endif