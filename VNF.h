#ifndef VNF_H
#define VNF_H
#include <string>

namespace NS_OCH{
class OXCNode;
//#pragma message ("World object is defined")
class VNF 
{

public:
	//double vnfdelay;
	double CpuUsage;
	int MemUsage;
	//int RAMUsage;
	//bool IsActivated;
	double CompFactor;
	double ExpanFactor;
	int NumSC;				//LA:number of SCs using this specific VNF
	
	
	std::string VNFName;
	//vector <OXCNode*> VNFNode;
	VNF(std::string);
	VNF();
	~VNF(void);
	VNF GetVNftype();
	//bool IsActivated();
	
};
};

#endif
