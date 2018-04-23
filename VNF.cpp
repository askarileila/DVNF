#include "VNF.h"
#include <iostream>
//#include "OchObject.h"

//#include "OchObject.h"
using namespace NS_OCH;
using namespace std;



VNF::VNF(string vnfname)
{
	this->NumSC=0;
	if (vnfname=="NAT"){
		//setcpu usage 
		CpuUsage=0.00092;
		VNFName="NAT";

		//set traffic rate to handle 
				
		//set ram usage
		//this->RAMUsage=2;
	}
	else
		if(vnfname=="FW"){
			CpuUsage=0.0009;
			//this->RAMUsage=4;
			VNFName="FW";
	}
		else
 
		if(vnfname=="TM"){
			CpuUsage=0.0133;
			//this->RAMUsage=2;
			VNFName="TM";
		}
		else
			if(vnfname=="WOC"){
				CpuUsage=0.0054;
				//this->RAMUsage=2;
				VNFName="WOC";
			
		} else
				if(vnfname=="IDPS"){
				CpuUsage=0.0107;
				VNFName="IDPS";
				//this->RAMUsage=2;
			
		}else
				if(vnfname=="VOC"){
				CpuUsage=0.0054;
				VNFName="VOC";
				}
				
				else{
					CpuUsage=0;
					cerr<<"The requested VNF was not found";
				}
}

/*bool VNF::IsActivated()
{
}*/

VNF::~VNF(void)
{
}



