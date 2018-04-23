#include "ServiceChain.h"
//#include "OchObject.h"
#include <random>
#include <iostream>
#include <string>
//#include "ConstDef.h"
//#include "TypeDef.h"
//#include "MacroDef.h"
using namespace NS_OCH;
using namespace std;



ServiceChain::ServiceChain():LengthOfSCs(5), HoldingTime(1),Wavelength(-1)
{
	     
	//LA:moved to each SC separetly 
	//VNFList = new VNF *  [LengthOfSCs] ;	
	 
}


ServiceChain::~ServiceChain()
{
	/*for (int i=0; i < this->LengthOfSCs; i++)
		delete this->VNFList [i];*/
	
	delete [] this->VNFList;
	
}

void ServiceChain::SetSCType ()
{
	 
	int randnum;
	//Generate a random between 1 and 5
	std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 6);
		randnum = dis(gen);
	//	cout<<"this is the random number:"<<randnum;

	if (randnum==1){
		//SCtype=WebService;
		SCtype=MIoT;
		local=true;
		BwReq=100;
		//BwReq=4; 
		//latency=0.5; //0.005
		latency=0.005;
		LengthOfSCs=3;
		VNFList = new VNF *  [LengthOfSCs];
		std::string SCVNF []= {"NAT","FW","IDPS"};
		for (int i=0;i<this->LengthOfSCs;i++)
		{
		#ifdef DEBUGLA
			cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
		#endif	
			this->VNFList[i]= new VNF (SCVNF[i]);
		}
	}
	else
		if(randnum==2){
			//SCtype=VideoStreaming;
			SCtype=AugmentedReality;
			local=true;
			//BwReq=4;
			BwReq=100;
			//latency=0.1; //LA:0.1
			latency=0.001;
			VNFList = new VNF *  [LengthOfSCs];
			std::string SCVNF []= {"NAT","FW","TM","VOC","IDPS"};
			for (int i=0;i<this->LengthOfSCs;i++)
				{
		#ifdef DEBUGLA
					cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
		#endif	
					this->VNFList[i]= new VNF (SCVNF[i]);
				}
			
		}
		else
			if(randnum==3){
				SCtype=Voip;
				local=false;
				//BwReq=4;
				BwReq=0.064;
				latency=0.1;  //LA:0.1
				VNFList = new VNF *  [LengthOfSCs];
				std::string SCVNF []= {"NAT","FW","TM","FW","NAT"};
			for (int i=0;i<this->LengthOfSCs;i++)
			{
	#ifdef DEBUGLA
			cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
	#endif
				this->VNFList[i]= new VNF (SCVNF[i]);
			}
			}
			else
			if (randnum==4){
				//SCtype=OnlineGaming;
				SCtype=CloudGaming;
				local=false;
				BwReq=4;
				//BwReq=0.05;
				latency=0.08; //LA:previous value: latency=0.06;
				VNFList = new VNF *  [LengthOfSCs];
				std::string SCVNF []= {"NAT","FW","VOC","WOC","IDPS"};
				for (int i=0;i<this->LengthOfSCs;i++)
				{
					#ifdef DEBUGLA
						cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
					#endif
					this->VNFList[i]= new VNF (SCVNF[i]);
				}
			}	
			if (randnum==5){
				SCtype=SmartFactory;
				local=true;
				BwReq=100;
				latency=0.001; 
				LengthOfSCs=2;
				VNFList = new VNF *  [LengthOfSCs];
				std::string SCVNF []= {"NAT","FW"};
				for (int i=0;i<this->LengthOfSCs;i++)
				{
					#ifdef DEBUGLA
						cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
					#endif
					this->VNFList[i]= new VNF (SCVNF[i]);
				}
			}
			if(randnum==6){
			SCtype=VideoStreaming;
			local=false;
			BwReq=4;
			latency=0.1; //LA:0.1
			VNFList = new VNF *  [LengthOfSCs];
			std::string SCVNF []= {"NAT","FW","TM","VOC","IDPS"};
			for (int i=0;i<this->LengthOfSCs;i++)
				{
		#ifdef DEBUGLA
					cout<<"\nCreating a VNF of type:"<<SCVNF[i]<<endl;
		#endif	
					this->VNFList[i]= new VNF (SCVNF[i]);
				}
			
		}
		else{
			#ifdef DEBUGLA
				cout<<"\nThe requested service chain is not found"<<endl;
			#endif
		}
}



void ServiceChain ::SetNumberOfUsers ()
{
	int randnum;
	//Generate a random between 500 to 1000
	std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(500, 1000);
		randnum = dis(gen);

	//this->NumofUsers= randnum;
	this->NumofUsers=20;

}
std::string ServiceChain :: GetServiceChainName()
{
	/*if(SCtype==WebService)
		return "WebService";*/
	if(SCtype==MIoT)
		return "MIoT";
	else 
		if(SCtype==Voip)
			return"VOIP";
		else
			/*if(SCtype==OnlineGaming)
				return "OnlineGaming";*/
			if(SCtype==AugmentedReality)
				return "AugmentedReality";
			else 
				if(SCtype==CloudGaming)
					return "CloudGaming";

				else	if(SCtype==VideoStreaming)
					return "VideoStreaming";
				else 
					if(SCtype==SmartFactory)
						return "SmartFactory";
}			
