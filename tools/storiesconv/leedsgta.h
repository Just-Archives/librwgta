enum ModelInfoType
{
	MODELINFO_SIMPLE       = 1,
	MODELINFO_MLO          = 2,	// unused
	MODELINFO_TIME         = 3,
	MODELINFO_WEAPON       = 4,
	MODELINFO_ELEMENTGROUP = 5,
	MODELINFO_VEHICLE      = 6,
	MODELINFO_PED          = 7,
	MODELINFO_XTRACOMP     = 8,	// unused
};

enum VehicleType
{
	VEHICLETYPE_CAR,
	VEHICLETYPE_BOAT,
#ifdef VCS
	VEHICLETYPE_JETSKI,
#endif
	VEHICLETYPE_TRAIN,
	VEHICLETYPE_HELI,
	VEHICLETYPE_PLANE,
	VEHICLETYPE_BIKE,
	VEHICLETYPE_FERRY,
};

struct CVector
{
	float x, y, z;
};

struct CVector4d	// or VuVector?
{
	float x, y, z, w;
};

struct sChunkHeader
{
	uint32   ident;
	bool32   type;
	uint32   fileSize;
	uint32   dataSize;
	uint32   relocTab;
	uint32   numRelocs;
	uint32   globalTab;
	uint16   numClasses;
	uint16   numFuncs;
};

struct TexListDef
{
	RslTexList *texlist;
	int32 refCount;
	char name[20];
};

struct PedStats
{
#ifdef LCS
	int32 index;
	char name[24];
	float fleeDistance;
	float headingChangeRate;
	uint8 fear;
	uint8 temper;
	uint8 lawfulness;
	uint8 sexiness;
	float attackStrength;
	float defendWeakness;
	uint16 flags;
#else
	int32 index;
	float fleeDistance;
	float headingChangeRate;
	float attackStrength;
	float defendWeakness;
	uint16 flags;
	uint8 fear;
	uint8 temper;
	uint8 lawfulness;
	uint8 sexiness;
	char name[24];
#endif
};

struct CBaseModelInfo
{
	int32	field0;
	const char *name;	// we'll use field4 for the name for now
//	int32	field4;
	uint32	hashKey;
	void	*mdlFile;	// points to chunk header of mdl file at runtime
	uint8	type;
	int8	num2dFx;
	bool	ownsColModel;
	int8	field15;	// alpha?
	void	*colModel;
	int16	_2dfxIndex;
	int16	objectIndex;
	int16	refCount;
	int16	txdSlot;
#ifdef VCS
	int16	unknownIndex;
#endif
	void	*vtable;
};
#ifdef LCS
static_assert(sizeof(CBaseModelInfo) == 0x24, "CBaseModelInfo: error");
#else
static_assert(sizeof(CBaseModelInfo) == 0x28, "CBaseModelInfo: error");
#endif


struct CSimpleModelInfo : public CBaseModelInfo
{
	RslObject **objects;
	float	drawDistances[3];
	uint8	numObjects;
	uint16	flags;
	CSimpleModelInfo *relatedObject;
};

struct CTimeModelInfo : public CSimpleModelInfo
{
	int32	timeOn;
	int32	timeOff;
	int16	otherModel;
	// int16 pad
};

struct CWeaponModelInfo : public CSimpleModelInfo
{
	union {
		char *animFile;
		int32 animFileIndex;
	};

	int32 GetWeaponType(void) { return *(int32*)&this->relatedObject; }
	void SetWeaponType(int32 type) { *(int32*)&this->relatedObject = type; }
};

struct CElementGroupModelInfo : public CBaseModelInfo
{
	RslElementGroup *elementGroup;
	union {
		char *animFile;
		int32 animFileIndex;
	};
};

// hash: c8807962
struct CPedModelInfo : CElementGroupModelInfo
{
	int32 animGroup;
	int32 pedType;
	int32 pedStatType;
	uint32 carsDriveMask;
	void *hitColModel;
	int8 radio1, radio2;
#ifdef VCS
	// colour indices? maybe bytes?
	short unknown0[55];
	// editable materials it would seem, but how do they work?
	struct {
		RslMaterial *mat;	// filled at runtime
		int32 numX;
	} materialthing[6];
	char someName[16];	// 16 is just a wild guess, possibly wrong
	uint32 unknown1;	// always 0x7ffe9f0, even at runtime
	uint32 unknown2;	// awlays 0
#endif
};
#ifdef LCS
static_assert(sizeof(CPedModelInfo) == 0x44, "ResourceImage: error");
#else
static_assert(sizeof(CPedModelInfo) == 0xFC, "ResourceImage: error");
#endif

struct CVehicleModelInfo__inst
{
	void *unused;	// probably ms_pEnvironmentMaps
	RGBA m_vehicleColourTable[256];
	char m_compsUsed[2];
	char ms_compsToUse[2];
};

#ifdef LCS
#define NUMPRIM 25
#define NUMSEC 25
#else
#define NUMPRIM 30
#define NUMSEC 25
#endif


// hash: 51964ff
struct CVehicleModelInfo : CElementGroupModelInfo
{
#ifdef LCS
	uint8 m_lastColours[2];
	char m_gameName[10];
	uint32 m_vehicleType;
	float m_wheelScale;
	int16 m_wheelId;
	int16 m_handlingId;
	uint8 m_numDoors;
	int8 m_vehicleClass;
	uint8 m_level;
	uint8 m_numExtras;
	uint16 m_frequency;
	int32 unk0;	// probably pad for CVector4d
	CVector4d m_dummyPos[5];
	uint32 m_compRules;
	float m_bikeSteerAngle;
	RslMaterial *m_materialsPrimary[NUMPRIM];
	RslMaterial *m_materialsSecondary[NUMSEC];
	uint8 m_colours[2][8];	// from carcols.dat
	uint8 m_numColours;
	uint8 m_lastColour;
	int8 m_currentColours[2];
	float m_normalSplay;	// "amount the normals are splayed by to improve the environment mapping on PSP"
	RslElement **m_extras;
	int32 m_animFileIndex;
#else
/*
	still looking for:
	uint8 m_numExtras;
*/
	int unk0[2];
	void *m_handling;
	int unk1[5];
	float m_normalSplay;	// "amount the normals are splayed by to improve the environment mapping on PSP"
	uint32 m_vehicleType;
	float m_wheelScale;
	float m_wheelScaleRear;
	CVector4d m_dummyPos[15];
	uint32 m_compRules;
	float m_bikeSteerAngle;
	char m_gameName[8];

	uint8 unk2;	// m_lastColour ?
	uint8 m_numColours;
	uint8 unk3;	// 0/1?
	uint8 m_colours[8][2];	// from carcols.dat
	char unk4[25];
	RslMaterial *m_materialsPrimary[NUMPRIM];
	RslMaterial *m_materialsSecondary[NUMSEC];

	RslElement **m_extras;
	int32 m_animFileIndex;
	int16 m_wheelId;
	uint16 m_frequency;
	uint8 m_numDoors;
	int8 m_vehicleClass;
	uint8 m_level;
	char unk5[0x29];
#endif
};
#ifdef LCS
static_assert(sizeof(CVehicleModelInfo) == 0x190, "CVehicleModelInfo: error");
#else
static_assert(offsetof(CVehicleModelInfo, m_dummyPos)-sizeof(CElementGroupModelInfo) == 0x30, "CVehicleModelInfo: error");
static_assert(offsetof(CVehicleModelInfo, m_gameName)-sizeof(CElementGroupModelInfo) == 0x128, "CVehicleModelInfo: error");
static_assert(sizeof(CVehicleModelInfo) == 0x2a0, "CVehicleModelInfo: error");
#endif


// might be nicer to have this as proper templates
struct CPool_generic
{
	void *items;
	void *flags;
	int  size;
	int  allocPtr;
	char name[16];
};

struct CPool_txd
{
	TexListDef *items;
	void *flags;
	int  size;
	int  allocPtr;
	char name[16];
};

#include "animation.h"

struct ResourceImage {
	void *paths;
	CPool_generic *buildingPool;
	CPool_generic *treadablePool;
	CPool_generic *dummyPool;
	CPool_generic *entryInfoNodePool;
	CPool_generic *ptrNodePool;
	int32 numModelInfos;
	CBaseModelInfo **modelInfoPtrs;
	void *carArrays;
	void *totalNumOfCarsOfRating;
	void *theZones;	// gta3.zon
	void *sectors;
	void *bigBuildingList;
	void *_2deffectStore;
	void *_2deffects;
	int16 *modelIndices;
	CPool_txd *texlistPool;
	RslTexList *storedTexList;
	CPool_generic *colPool;
	int32 colOnlyBB;
	void *tempColModels;
	void *objectInfo;	// object.dat
#ifdef LCS
	CVehicleModelInfo__inst *vehicleModelInfo_Info;	// carcols.dat
#else
	void *unknown0;
#endif
	void *streaming_Inst;	// gta3.dir

	CAnimManager *animManagerInst;
	void *fightMoves;		// fistfite.dat

#ifdef LCS
	void *pedAnimInfo;
#endif
	void *pedType;		// ped.dat
	PedStats **pedStats;		// pedstats.dat [42]

#ifdef VCS
	uint8 *colourTable;	// pedcols.dat + carcols.dat
	void *unknown1;
#endif

	int32 numAttributeZones;	// cull.ipl
	void *attributeZones;		// cull.ipl
	int32 numOccludersOnMap;
	void *occluders;
	void *waterLevelInst;		// waterpro.dat
#ifdef LCS
	void *handlingManager;		// handling.dat
#else
	void *weatherTypeList;
#endif
	void *adhesiveLimitTable;		// surface.dat
	void *timecycle;			// timecyc.dat
	void *pedGroups;			// pedgrp.dat
	void *particleSystemManager;	// particle.dat
	void *weaponTables;		// weapon.dat
#ifdef VCS
	int unknown2;
#endif
	RslElementGroup **markers;	// [32] in LCS
	int cutsceneDir;

#ifdef LCS
	void *ferryInst;	// ferry.dat
	void *trainInst;	// tracks(2).dat
	void *planeInst;	// flight.dat
//	int UNUSED;	// only on mobile, which doesn't write a game.dtz/dat to begin with
#else
	// one probably flight.dat
	void *unknown3;
	void *unknown4;	// 0
	void *unknown5;
	void *unknown6;	// 0
#endif
	void *menuCompressedTextures;
#ifdef VCS
	void *unknown7;
	void *unknown8;
	void *unknown9;
	int32 unknown10;
	void *unknown11;
#endif
	int fontTexListSize;
	void *fontCompressedTexList;
#ifdef VCS
	void *radarTextures;
	// some more
#endif
};
#ifdef VCS
static_assert(offsetof(ResourceImage, animManagerInst)+0x20 == 0x80, "ResourceImage: error");
static_assert(offsetof(ResourceImage, weatherTypeList) == 0x8C, "ResourceImage: error");
static_assert(offsetof(ResourceImage, timecycle) == 0x94, "ResourceImage: error");
static_assert(offsetof(ResourceImage, markers)+0x20 == 0xC8, "ResourceImage: error");
static_assert(offsetof(ResourceImage, menuCompressedTextures)+0x20 == 0xE0, "ResourceImage: error");
static_assert(offsetof(ResourceImage, fontTexListSize)+0x20 == 0xF8, "ResourceImage: error");
#endif
#ifdef LCS
static_assert(offsetof(ResourceImage, animManagerInst)+0x20 == 0x80, "ResourceImage: error");
static_assert(offsetof(ResourceImage, markers)+0x20 == 0xC0, "ResourceImage: error");
static_assert(offsetof(ResourceImage, ferryInst)+0x20 == 0xC8, "ResourceImage: error");
#endif