#pragma once
#include"Overlay11.hpp"
#define PI 3.1415926535897932384626433832795f

// Generated using ReClass 2016

class Mesh;
class Instance;
class AUObject;
class AUSkeleton;
class AVULodMeshInstance;
class AVCSCAnimModel;
class PlayerPawn;
class BasicWeapon;
class AVUWeaponFiringComponent;
class HoldWeapon;
class AVUPlayerInventory;
class Inventory;
class PlayerInventoryGroupSpace;
class AVUHealthSystem;

class AVAECamera;
class ViewAngle;

class Mesh
{
public:
	fVec4 normalizePos; //0x0000 
	fVec3 pos; //0x0010 
	float w; //0x001C 
	fVec4 noIdea; //0x0020 

}; //Size=0x0030

class Instance
{
public:
	AUObject* object; //0x0000 

}; //Size=0x0004

class AUObject
{
public:
	char pad_0x0000[0x20]; //0x0000
	PlayerPawn* playerPawn; //0x0020 
	char pad_0x0024[0xD8]; //0x0024
	AUSkeleton* skeleton; //0x00FC 

}; //Size=0x0100

class AUSkeleton
{
public:
	char pad_0x0000[0x2C]; //0x0000
	AVULodMeshInstance* meshInstance; //0x002C 
	char pad_0x0030[0xBC]; //0x0030
	__int32 meshLen; //0x00EC 

}; //Size=0x00F0

class AVULodMeshInstance
{
public:
	char pad_0x0000[0xC0]; //0x0000
	AVCSCAnimModel* model; //0x00C0 

}; //Size=0x00C4

class AVCSCAnimModel
{
public:
	char pad_0x0000[0xAC8]; //0x0000
	Mesh* pMesh; //0x0AC8 
	char pad_0x0ACC[0x34]; //0x0ACC
	char NameModel[20]; //0x6A5888 

}; //Size=0x0B14

class PlayerPawn
{
public:
	char pad_0x0000[0x9C]; //0x0000
	fVec3 oriPos; //0x009C 
	char pad_0x00A8[0x410]; //0x00A8
	BasicWeapon* basicWeapon; //0x04B8 
	char pad_0x04BC[0x20]; //0x04BC
	AVUHealthSystem* pHealthSystem; //0x04DC 
	char pad_0x04E0[0xF30]; //0x04E0
	AVUPlayerInventory* playerInventory; //0x1410 

}; //Size=0x1414

class BasicWeapon
{
public:
	char pad_0x0000[0x9C]; //0x0000
	fVec3 pos; //0x009C 
	char pad_0x00A8[0x6A0]; //0x00A8
	AVUWeaponFiringComponent* weaponComponent; //0x0748 

}; //Size=0x074C

class AVUWeaponFiringComponent
{
public:
	char pad_0x0000[0x154]; //0x0000
	HoldWeapon* holdWeapon; //0x0154 

}; //Size=0x0158

class HoldWeapon
{
public:
	__int32 id; //0x0000 
	__int32 ammoLeft; //0x0004 
	__int32 reloadAmmoLeft; //0x0008 

}; //Size=0x000C

class AVUPlayerInventory
{
public:
	char pad_0x0000[0x38]; //0x0000
	Inventory* pInventory; //0x0038 
	__int32 maxPlayerInv; //0x003C 

}; //Size=0x0040

class Inventory
{
public:
	PlayerInventoryGroupSpace* pPlayerInventoryGroupSpace; //0x0000 

}; //Size=0x0004

class PlayerInventoryGroupSpace
{
public:
	char pad_0x0000[0x30]; //0x0000
	__int32 slotLeft; //0x0030 
	__int32 maxCap; //0x0034 

}; //Size=0x0038

class AVUHealthSystem
{
public:
	char pad_0x0000[0x68]; //0x0000
	float maxHealth; //0x0068 
	float currentHealth; //0x006C 
}; //Size=0x0084

class globalCamera
{
public:
	AVAECamera* pAVAECamera; //0x0000 
}; //Size=0x0004

class AVAECamera
{
public:
	char pad_0x0000[0x9C]; //0x0000
	fVec3 eyePos; //0x009C 
	char pad_0x00A8[0x574]; //0x00A8
	ViewAngle* pViewAngle; //0x061C 

}; //Size=0x0620

class ViewAngle
{
public:
	char pad_0x0000[0x1530]; //0x0000
	vec2<int> angle; //0x1530 

}; //Size=0x1538


class BlackList : public Overlay11
{
public:
	void Init();
	void Clean();

	void MainLoop(IDXGISwapChain* pSwapChain);

	void DrawESPBone();
	void AimBot();
	void AddOjbect();

	hook_t hViewMatrix;
	hook_t hAddOjbect;
	hook_t hcamera;
	ViewAngle* camera =0;
	AUObject* object = 0;
	globalCamera* pCam = 0;
	float* viewMatrix = 0;
private:
	void DrawDog(Mesh* mesh);
	void DrawMan(Mesh* mesh);
	void DrawDrone(Mesh* mesh);
	uintptr_t base = 0;
	uintptr_t offsetObject = 0x63590E;
	uintptr_t offsetViewMatrix = 0x1394560;
	uintptr_t offsetViewAngle = 0x96D839;
	uintptr_t offsetGlobalCamera = 0x2F48048;
	AUObject* oList[100] = { 0 };
	size_t len = 0;
};
extern BlackList bList;
