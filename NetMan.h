#ifndef NETMAN_H
#define NETMAN_H

#include <vector>
#include <list>
#include <fstream>
#include "Stat.h"
namespace NS_OCH {

class WDMNetwork;
class Graph;
class Connection;
class LightpathDB;
class ConnectionDB;
class Circuit;
class Log;
class Event;
class EventList;
class VNF;
class ServiceChain;

class NetMan: public OchObject {
friend class WDMNetwork;
	NetMan(const NetMan&);
	const NetMan& operator=(const NetMan&);
public:
	typedef enum {
		PT_PAL_DPP = 0,	// dedicated-path protection (1+1 or 1:1)-fabio 8 dic:la uso io..
		PT_PAL_SPP,		// shared-path protection
		PT_PAC_DPP,
		PT_PAC_SPP,
		PT_SPAC_SPP,
		PT_PAL2_SP,

        PT_SEG_SP_L_NO_HOP, // link protection --teo
        PT_SEG_SP_AGBSP, //auxiliary graph based seg.protection

		PT_SEG_SP_NO_HOP,	// seg. protection w/o hop-count constraint
		PT_SEG_SP_B_HOP,	// seg. protection w/ backup hop-count constraint
		PT_SEG_SP_PB_HOP,	// seg. protection w/ (p + b) hop-count constraint
		PT_SEG_SPP,			// shared-path protection, link/node disjoint
		PT_SEG_SPP_B_HOP,
		PT_SEG_SPP_PB_HOP,
		PT_UNPROTECTED,//FABIO 8 dic: introduco caso non protetto	
		PT_UNPROTECTED_GREEN,
		PT_wpSEG_SPP,		//FABIO 7 genn: nuove definizioni per casi wp
		PT_wpUNPROTECTED,
		PT_wpPAL_DPP,		//..(ma quanto è lunga sta lista !!!) DEDICATA
		PT_SPPBw,			//FABIO 2 Mar: algoritmo di Barcellona
		PT_SPPCh,

		PT_BBU,				//-B = 21
		PT_DVNF				//LA: provisioning type for dynamic_VNF
	} ProvisionType;
	
	typedef enum {
		GP_MinTHP = 0,
		GP_MinNTHP,
		GP_MinTHV,
	} GroomingPolicy;
    
	// I can di this to clarify the case in WDMNET.cpp 
	//	typedef enum {
	//		NoTimeInf = 0,
	//		AggrTimeInf,
	//		ComplTimeInf,
	//  } TimePolicy;

	//FABIO 11 ottobre: nuovo typedef per identificare il tipo di conflitto
	//    quando verifico se la nuova connnessione sia effettivamente possibile
	typedef enum {
		NO_Conflict = 0,
		PRIMARY_Conflict,
		BACKUP_Conflict,
		BOTH_Conflict
	} ConflictType;

	typedef enum {
		PBLOCK = 0,
		WL,
		TX,
	} StatActive;//ANDREA

	UINT connReached; //FABIO
	double pc;	//num of primary conflicts
	double bc;	//num of backup conflicts
	double fc;	//num of fake conflicts
	double bothc;
	double pcPer;
	double bcPer;
	double fcPer;
	double bothcPer;
	double pcTot;
	double bcTot;
	double fcTot;
	double bothcTot;
	double pathTotCost;
	double n;
	double PTot;
	double PTransport;
	double PProc;
	double PProc_parz;
	double PEDFA;
	double PEDFA_parz;
	double PGreen;
	double PBrown;
	double carico_rete;
	LINK_COST hCost;
	LINK_COST transport_cost;
	bool isLinkDisjointActive;//FABIO 24 Genn:

public:
	void logPeriodical(SimulationTime);					//-B: Log some info about lightpaths, count num of backup channels and other logs
	UINT computeNetworkCost();
	void logFinal();									//-B: it calls different log methods -> see these method's description
	//-B: builder modified to set specific parameters
	NetMan(); //-B: (from var pc to var n all set to 0 + stop_perc=5, conf=0.95, isLinkDisjointActive=true; m_nNumberOfPaths to look for =1, TxThreshold = 1, m_bWithOptimization = false)
	~NetMan();
	virtual void dump(ostream&);					//-B: it should print NetMan object status (useful for debugging purpose; to be tried)
														//	it also dump LightpathDB and Graph objects
    virtual void WDMNetdump(ostream&) const; //----t	//-B: it should print the WDMNetwork object status (useful for debugging purpose; to be tried)
	void dumpAbsLinkList(const list<AbstractLink*>&) const;

	// for PAL2_SP
	Lightpath* lookUpLightpathOfMinCost(OXCNode*, OXCNode*, 
			double) const;
      
	void PAL2_SP_SetUpLightpath(Lightpath&);
	void PAL2_SP_ReleaseLightpath(Lightpath&, bool&, const Circuit&);
	void PAL2_SetSlack(LINK_COST);
	LINK_COST PAL2_GetSlack() const;

	void appendLightpath(Lightpath *, double eBW);
	void removeLightpath(Lightpath *);
	
	//-B: return true if the provisioning of the connection passed as parameter has been done correctly
	//	it calls the provisionConnectionHelper method, which calls a different method CASENAME_Provision
	//	depending on the provisioning type of the connection. Also, log the provisioning of the connection
	//	passed as parameter and add it to the Connection DB (a Connection list)
    bool provisionConnection(Connection*);
	bool provisionConnectionxTime(Connection*, list<Event*>&,EventList&);//-M
	void deprovisionConnection(Connection*);

	UINT getNumberOfOXCNodes() const;
	UINT getNumberOfUniFibers() const;
	UINT getNumberOfAltPaths() const;
	UINT getTimePolicy() const;

	WDMNetwork* getWDMNetwork();

	/// bool initialize(const char*, const char*, int, double);
	bool initialize(const char*, const char*, int, double, int,const char*); // M ANDREA
	void resetAll();					//-B: reset attributes/info about connections, nodes (also links???)
	void deprovisionAll();				//-B: delete everything (?) about connections
										//-> before resetting everything, it seems to update the stats of each lighpath
	UINT countFreeChannels() const;

	//FABIO20Sett
	ConflictType verifyCircuit(Circuit*); //verifica se il circuito puo essere effettivamente instradato
	void RouteConverter(Circuit*);
	void RouteConverterUNPR(Circuit*);
	ConflictType verifyCircuitUNPR(Circuit*);//9 dic
	ConflictType wpverifyCircuit(Circuit*);
	ConflictType wpverifyCircuitUNPR(Circuit*);
	ConflictType wpverifyCircuitDED(Circuit*);

protected:
	bool provisionConnectionHelper(Connection*);
	//bool provisionConnectionHelperxTime(Connection*, list<Event*>&);//-M
	void genInitialStateGraph();
	void consumeLightpathBandwidthHelper(const Lightpath*, int);
	// Protection At Connection (PAC) level: dedicated-path protection
	bool PAC_DPP_Provision(Connection*);
	bool PAC_DPP_ProvisionHelper(Connection*, Circuit&, Circuit&, 
				bool, Vertex*, Vertex*);
	bool PAC_DPP_CutSet_HandleBackHaulLink(const list<AbstractLink*>&);
	bool PAC_DPP_CutSet_HandleBackHaulPhyicalLink(const map<UINT, OXCNode*>&,
				const UniFiber*) const;
	void PAC_DPP_Deprovision(Connection*);
	
	bool PAC_DPP_Provision_OCLightpath(Connection*);
	void PAC_DPP_Provision_OCLightpath_Helper(list<AbstractLink*>&, 
				const AbstractPath&);
	void PAC_InvalidatePath(Graph&, list<AbstractLink*>&);

	// MPAC: mixed protection at connection level
	void MPAC_Optimize(list<AbstractLink*>&, list<AbstractLink*>&, 
			LINK_COST&, LINK_COST&, const Connection&, Vertex*, Vertex*);
	void MPAC_Opt_NewBCircuit(Circuit&, Graph&, const list<AbstractLink*>&);
	void MPAC_Opt_UpdateLinkCost_Primary(const Circuit&);
	void MPAC_Opt_UpdateLinkCost_Primary_Lightpath(SimplexLink&, const Circuit&);

	bool PAC_SPP_Provision(Connection *);
	bool PAC_SPP_ProvisionHelper(Connection*, Circuit&, Circuit&,
				Vertex*, Vertex*);
	void PAC_SPP_NewCircuit(Circuit&, const list<AbstractLink*>&);
	void PAC_SPP_UpdateLinkCost_Backup(Graph&, const Circuit&);
	void PAC_SPP_UpdateLinkCost_Backup_Lightpath(const Circuit&, SimplexLink&);
	void PAC_SPP_NewBackupCircuit(Circuit&, const Circuit&, Graph&, 
			Connection *, const list<AbstractLink*>&);
	void PAC_SPP_Deprovision(Connection*);

	// SPAC: segragated protection at connection level
	void SPAC_Optimize(list<AbstractLink*>&, list<AbstractLink*>&, 
			LINK_COST&, LINK_COST&, const Connection&, Vertex*, Vertex*);
	void SPAC_Opt_UpdateLinkCost_Primary(const list<AbstractLink*>&, 
			double);
	void SPAC_Opt_UpdateLinkCost_Primary_Lightpath(SimplexLink&, 
			const list<AbstractLink*>&, double);

	bool SPAC_SPP_Provision(Connection *);
	bool SPAC_SPP_Helper(Connection*, Circuit&, Vertex *, Vertex *);
	void SPAC_SPP_NewCircuit(Circuit&, const list<AbstractLink*>&);
	void SPAC_SPP_UpdateLinkCost_Backup(Graph&, Circuit&,
			double, bool bIgnoreTx=false);
	void SPAC_SPP_Deprovision(Connection *);
	void SPAC_InvalidatePath(Graph&, Circuit&);

	// Protection At Lightpath (PAL) level: dedicated-path protection
	bool PAL_DPP_MinTHP_Provision(Connection*);
	void PAL_DPP_MinTHP_GenAuxGraph(AbstractGraph&, vector<Lightpath*>&, 
			const Connection*);
	bool PAL_DPP_MinTHP_NewCircuit(Circuit&, const AbstractGraph&, 
			const vector<Lightpath*>&, const list<AbstractLink*>&);
	void PAL_DPP_MinTHP_NewCircuit_Helper(int*, const AbstractPath&);
	void PAL_DPP_MinTHP_Deprovision(Connection*);
	bool PAL_DPP_Provision_OCLightpath(Connection*);

	// PAL_SP:
	void PAL_SPP_Deprovision(Connection*);
	bool PAL_SPP_Provision(Connection *);
	bool PAL_SPP_Helper(Connection*, Circuit&, Vertex*, Vertex*);
	void PAL_SPP_New_Circuit(Circuit&, list<Lightpath*>&, 
			const list<AbstractLink*>&);
	void PAL_SPP_UpdateLinkCost_Backup(Graph&, const list<UniFiber*>&);

	// PAL_SP: heuristic 2
	void PAL2_SP_Deprovision(Connection*);
	bool PAL2_SP_Provision(Connection *);

	// SEG_SP
	void SEG_SP_NO_HOP_Deprovision(Connection*);
	bool SEG_SP_NO_HOP_Provision(Connection *);

    // link  SEG_SP_L
	void SEG_SP_L_NO_HOP_Deprovision(Connection*);//-t
	bool SEG_SP_L_NO_HOP_Provision(Connection *);//-t

	// SEG_SP_AGBSP
    bool SEG_SP_AGBSP_Provision(Connection *);//-t
    void SEG_SP_AGBSP_Deprovision(Connection*);//-t

	// SEG_SPP
	void SEG_SPP_Deprovision(Connection*);
	bool SEG_SPP_Provision(Connection *);

	// SEG_SPP_B_HOP
	void SEG_SPP_B_HOP_Deprovision(Connection*);
	bool SEG_SPP_B_HOP_Provision(Connection *);

	// SEG_SP_B_HOP
	void SEG_SP_B_HOP_Deprovision(Connection*);
	bool SEG_SP_B_HOP_Provision(Connection *);

	// SEG_SP_PB_HOP
	void SEG_SP_PB_HOP_Deprovision(Connection*);
	bool SEG_SP_PB_HOP_Provision(Connection *);

	// SEG_SPP_PB_HOP
	void SEG_SPP_PB_HOP_Deprovision(Connection*);
	bool SEG_SPP_PB_HOP_Provision(Connection *);

	//UNPROTECTED 8 dic
	bool UNPROTECTED_Provision(Connection*);
	void UNPROTECTED_Deprovision(Connection*);
	bool UNPROTECTED_ProvisionGreen(Connection*);
	//void EstimateGreenPower(int nDCFlag, double SimulationTime);  //Mirko

	//DEDICATA 8 dic
	bool PAL_DPP_Provision(Connection*);
	void PAL_DPP_Deprovision(Connection*);
	bool wpPAL_DPP_Provision(Connection*);
	void wpPAL_DPP_deprovision(Connection*);
	//SEGMENTED WP 7 genn
	bool wpSEG_SPP_Provision(Connection*);
	void wpSEG_SPP_Deprovision(Connection*);
	//UNPROTECTED WP  12 genn
	bool wpUNPROTECTED_Provision(Connection*);
	void wpUNPROTECTED_Deprovision(Connection*);
	//ALGORITMI BARCELLONA
	bool SPPBw_Provision(Connection*);
	void SPPBw_Deprovision(Connection*);

	bool SPPCh_Provision(Connection*);
	void SPPCh_Deprovision(Connection*);

	//-B: add provision and deprovision for BBU hotelling
	void BBU_Deprovision(Connection*);
	bool WP_BBU_Provision(Connection*);
	bool BBU_ProvisionNew(Connection * pCon);
	bool BBU_ProvisionHelper_Unprotected(Connection * pCon, Circuit & hPCircuit, Vertex * pSrc, Vertex * pDst);
	void updateGraphValidityAndCosts(OXCNode * pOXCSrc, double bwd);
	void BBU_NewCircuit(Circuit & hCircuit, const list<AbstractLink*>& hLinkList, UINT, Connection*pCon);
	bool BBU_Provision_OCLightpath(Connection * pCon, Circuit & hPCircuit, Vertex * pSrc, Vertex * pDst);
	UINT BBU_ProvisionHelper_ConvertPath(list<AbstractLink*>& hPath, const AbsPath&hAbsPath);
	bool WP_BBU_ProvisionHelper_Unprotected(Connection * pCon, Circuit * pCircuit, OXCNode*pSrc, OXCNode*pDst);
	//-B
	float calculateLatency(list<AbstractLink*>& hLinkList); //-B: routing time
	void genAuxBBUsList(vector<OXCNode*>& auxBBUsList);
	void genAuxBBUsList_BBUPooling(vector<OXCNode*>& auxBBUsList, double backBwd);
	bool isAlreadyActive(OXCNode * pOXCsrc);
	bool checkAvailabilityHotelNode(OXCNode * pOXCsrc);
	bool checkResourcesPoolNode(OXCNode * pOXCsrc, double backBwd);
	UINT placeBBUHigh(UINT, vector<OXCNode*>&);
	UINT placeBBUClose(UINT src, vector<OXCNode*>& BBUsList);
	UINT placeBBU_Metric(UINT src, vector<OXCNode*>& BBUsList);
	void updateCostsForBestFit();
	void buildHotelsList(vector<OXCNode*>& otherHotels);
	void buildNotActiveBBUsList(vector<OXCNode*>&);
	void buildNotActivePoolsList(vector<OXCNode*>&);
	void linkCapacityDump();	//-B
	void simplexLinkDump();	//-B
	void checkSrcDstSimplexLinkLp(SimplexLink*, Lightpath*);
	LINK_CAPACITY getUniFiberFreeCap(UniFiber*fiber);	//-B
	LINK_CAPACITY getUniFiberCap(UniFiber*fiber);	//-B

	

	
public:
	//LA:DVNF
	
	UINT					NumNAT; //LA: number of active NAT vnf instances
	UINT					NumFW;  //LA:num of active FW vnf instances
	UINT					NumTM;	//LA:num of active TM
	UINT					NumWOC; //LA: num of active WOC
	UINT					NumIDPS; 
	UINT					NumVOC;
	vector <OXCNode*>		VNFNodes;
	UINT					m_NumPVNF;		//LA: number of Provisioned VNF instances 
	UINT					m_NumActVNF;	//LA:number of activated VNF instances
	//LA:moved from private to public to be accessible in simulator class
	Graph					m_hGraph;				// representing network state
	UINT					VNFCounter;
	std::ofstream			outfile;				//LA:to save sc chain path in a file
	float					TotSimTime;
	void PAC_DPP_NewCircuit(Circuit&, const list<AbstractLink*>&); //LA:moved to be accessible in simulator class
	ConnectionDB	m_hConnectionDB;        // Connection info //LA:moved from private to public to be accessible in simulator
	bool DVNF_ProvisionVNF(VNF* , Connection*);
	bool DVNF_ActivateNewInst(VNF* ,Connection *);
	vector <OXCNode*> DVNF_CreateVNFNodes (string );
	UINT DVNF_computeNetworkCost();
	void DVNF_Deprovision(Connection*);
	void DVNF_Deprovision_Helper(Connection*);
	void DVNF_NewCircuit(Circuit & , const list<AbstractLink*>& hLinkList, UINT, Connection*pCon); //LA:moved from protected
	void DVNF_NewCircuit(Circuit & , const list<AbstractLink*>& hLinkList, Connection*pCon); //LA:to support lighter version
	void DVNF_Dump_SC(ServiceChain *);
	bool DVNF_ProvisionSC(Connection * , Circuit *);
	bool DVNF_ProvisionSCHelper(Connection *);
	bool DVNF_GroupingVNF(ServiceChain *,Connection*);				//LA:for the case where latency requirements is not satisfied
	vector<int> DVNF_BuildLocalNodes(ServiceChain*);						//LA:when SC can be served locally builds list of nodes that can be dst 
	double DVNF_Count_VNF_SC_Ratio();									//LA:to caculate SC per VNF ratio for all active VNFs
	int DVNF_Count_Active_Nodes();
	void DVNF_Recover_AftGrouping( vector <OXCNode*>,list<AbstractLink*>,Connection* pCon);
	void NetMan::DVNF_EnableVNF( OXCNode* ,VNF*, ServiceChain*);

	void consumeLightpathBandwidth(const Lightpath*, UINT);
	void releaseLightpathBandwidth(const Lightpath*, UINT);
	void consumeTx(OXCNode*, int);
	void consumeRx(OXCNode*, int);
	void setTxThreshold(double dTxThreshold);
	Lightpath* lookUpLightpathOfMinChannels(UINT, UINT, UINT) const;
	void showLog(ostream&) const;
	Log				m_hLog;
	//---------- Statistic -------------------------
	Log 			m_runLog;		// Add by ANDREA
	double			stop_perc;		// Add by ANDREA
	double			conf;			// Add by ANDREA
	Sstat			*p_block;		// Add by ANDREA
	
	void printStat(ostream&) const;		// Add by ANDREA
	bool updateStat(UINT, UINT);		// Add by ANDREA		//-B: update stats about blocking prob and conflict's stats
	
//////////////////////////// -B: ADDED BY ME
	void printInputData(int numArg, char *arg[]);	//-B
	bool BBUinitialize(const char * pTopoFile, const char * pProtectionType, int nNumberOfPaths, double dEpsilon, int nTimePolicy, const char * UnAv); //-B
	LightpathDB& getLightpathDB(); //-B
	ConnectionDB & getConnectionDB();
	void updateLinkCapacity(Lightpath*, int, UniFiber*); //-B
	bool checkFreedomSimplexLink(UINT, UINT, int); //-B
	bool checkAvailabilitySimplexLink(UINT srcId, UINT dstId, int w, UINT nBW);
	void printChannelReference(); //-B: for WDMNet
	void printChannelReferencePast(); //-B: for WDMNetPast
	void printChannelReference(UniFiber * pUniFiber); //-B
	void printChannelLightpath(); //-B
	void releaseLinkBandwidth(Lightpath*, UINT); //-B
	UINT findBestBBUHotel(UINT src, double&);	//-B: for BBUSTACKING
	UINT findBestBBUPool_Soft(UINT src, double bwd); //-B: for INTRA_BBUPOOLING
	UINT findBestBBUPool_Evolved(UINT src, double bwd, double backhaulBwd); //-B: for INTER_BBUPOOLING

	void connDBDump(); //-B
	LINK_COST calculateCost(list<AbstractLink*> hPrimaryPath); //-B
	void addConnection(Connection * pCon); //-B: add pCon to connectionDB

	void printChannelLightpathNetPast();

	void invalidateSimplexLinkDueToFreeStatus();

	void invalidateSimplexLinkDueToCapOrStatus(UINT bwd);

	void invalidateSimplexLinkDueToFreeStatus(double); //-B

	void invalidateSimplexLinkDueToOccupancy(); //-B
	AbstractLink * lookUpSimplexLink(AbstractNode * pSrc, AbstractNode * pDst, int channel);

	int getSimplexLinkGraphSize();

	void removeSimplexLinkFromGraph(Lightpath * pLightpath);

	void removeEmptyLinkFromGraph();

	void releaseVirtualLinkBandwidth(UINT nBWToRelease, Lightpath * pLightpath);

	void checkSrcDstLightpath(Lightpath * pLightpath);

	void checkSimplexLinkRemoval(Lightpath * pLightpath);
	
	void buildBestHotelsList();

	float computeLatency(list<AbstractLink*>& path);

	void clearPrecomputedPath();

	void setPrecomputedPath(Circuit*);

	void consumeBandwOnSimplexLink(Lightpath * pLightpath, UINT BWToBeAllocated);

	UINT checkEmptyLightpaths();

	void invalidateGrooming();

	void invalidateSimplexLinkGrooming();

	void validateGroomingHotelNode();

	UINT countGroomingNodes();

	UINT countLinksInGraph();

	void getGroomNodesList();

	void buildGroomNodeList(Circuit * pCircuit);

	void logNetCostPeriodical(SimulationTime hTimeSpan);

	void logActiveLpPeriodical(SimulationTime hTimeSpan);

	void logActiveHotelsPeriodical(SimulationTime hTimeSpan);

	void logActiveBBUs(SimulationTime hTimeSpan);

	void logActiveSmallCells(SimulationTime hTimeSpan);

	void reduceHotelsForConsolidation(UINT);

	void checkGrooming(Connection*);

	void computeAvgLatency(Event * pEvent);

	bool isLastLinkFull(list<AbstractLink*>&pPath);

	void genSmallCellCluster();

	void checkLpToSLinkEquality();

	void buildHotelsListForNode();

	bool BBU_ProvisionHelper_Unprotected_BIG(Connection * pCon, double bwd);
	//LA: to see how many nfv enable nodes we have-neede in simulator class
	UINT getNumberOfNFVNodes();
	UINT getNumberOfEdgeNodes();
	vector<OXCNode*> BuildNFVNodesList();
	vector<OXCNode*> BuildEdgeNodesList();

    list<Event*>            m_hDepaNet;//-M
    SimulationTime          m_hHoldingTimeIncoming;//-M
    SimulationTime          m_hArrivalTimeIncoming;//-M
    bool					m_UnAv;//-t
	
	// WDMNetwork		m_hServWDMNet;	    
	StatActive	m_eStatActive;			// Add by ANDREA
	//FABIO
	ProvisionType	m_eProvisionType;
	EventList* evList;			//Lo faccio puntare all' eventList di Simulator

private:
	WDMNetwork		m_hServWDMNet;	        // for time in CI
	LightpathDB		m_hLightpathDB;	        // Lightpath info
	
		
	AbstractGraph	hWLGraph;				//-B: auxiliary graph
	Graph			auxGraph;				//-B: auxiliary graph
	GroomingPolicy	m_eGroomingPolicy;		//-B: used only for PAL_DPP case
	UINT			m_nTxReqPerLightpath;	// Tx requirement per lightpath
											// for PAL: 2, for PAC: 1
	double			m_dTxThreshold;
	UINT			m_nNumberOfPaths;		// # of paths to enumerate (-B: I guess, when we look for the first k paths)
	LINK_COST		m_hCostSlack;
	bool			m_bWithOptimization;
    UINT            m_nTimePolicy;			// 0 No Time - 1 CI - 2 PI --M	//LA: just use 0- 0=backup path calc without considering con duration,1=keep in mind duration of connection
	list<AbstractLink*> precomputedPath;	//-B: needed for placeBBUX methods
	LINK_COST		precomputedCost;		//-B: needed for placeBBUX methods

	//map a lightpath to the corresponding SimplexLink
	typedef pair<UINT, SimplexLink*> LP2SimplexLinkPair;
	typedef map<UINT, SimplexLink*> LP2SimplexLinkMap;
	LP2SimplexLinkMap m_hLP2SLinkMap;

public:
		WDMNetwork		m_hWDMNet;		// topo & channel usage :reso pubblico
		WDMNetwork		m_hWDMNetPast;
		bool			m_bHotelNotFoundBecauseOfLatency;

		list<Lightpath*>	auxLightpathsList;
		// FABIO:
		
		


};
};

#endif
