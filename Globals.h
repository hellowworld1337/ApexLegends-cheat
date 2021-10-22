//vs project settings
#pragma warning(disable : 4530)
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/MERGE:.pdata=.text")
extern "C" int _fltused = 0;

//main includes
#include <windows.h>
#include <winternl.h>
#include <intrin.h>
#include <stdio.h>
#include <stdint.h>
#include <d3d11.h>
#include <cfloat>
//#include <math.h>

//VMProtect SDK
#include "CheatSDK/Utils/VMP_SDK/VMProtectSDK.h"

//face sdk
#include "CheatSDK/Utils/EncStr.h"
#include "CheatSDK/Utils/Utils.h"
#include "CheatSDK/Utils/Math.h"

//global vars
namespace Global {
	Vector2 WndSize;
	Vector3 LP_HeadPos;
	Vector2 MouseDelta;
}

//face render & gui
#include "CheatSDK/GuiRender/D3D_Experimental/D3D_Render.h"
#include "CheatSDK/GuiRender/Gui.h"

//cheat settings
namespace AimBot
{
	bool Active = true;
	bool Silent = false;
	bool Nearest = true;
	bool WeaponFix = true;
	bool NoKnocked = true;
	bool PredictType = false;

	float FOV = 80.f;
	float Smooth = 4.f;
	float Distance = 250.f;
}

namespace Visuals
{
	bool ESP = true;
	bool GlowESP = true;
	bool LootESP = true;
	bool DrawCrossHair = true;
	bool DrawFOV = true;
	float DistanceESP = 300.f;
	float DistanceLoot = 100.f;
}

namespace Misc
{
	bool Bhop = true;
	//bool SpeedHack = false;
	//float SpeedHackMP = 10.f;
	//float SpeedHackOthMP = 4.f;
	//float SpeedHackChoke = 10.f;
}

//offsets
namespace Off
{
	//global vars
	DWORD64 NameList;
	DWORD64 ViewMatrix;
	DWORD64 ViewRender;
	DWORD64 EntityList;
	DWORD64 ClientState;
	DWORD64 LocalPlayer;

	//dinamic
	DWORD Flags;
	DWORD Health;
	DWORD TeamNum;
	DWORD PropName;
	DWORD Bleedout;
	DWORD LifeState;
	DWORD BoneClass;
	DWORD StudioHdr;
	DWORD ScriptInt;
	DWORD PunchAngle = 0x2450;
	DWORD AbsVelocity;
	DWORD ShieldHealth;
	DWORD ActiveWeapon;
	//DWORD NextShotTime;
	DWORD ShieldHealthMax;

	//obs
	DWORD ObsMode = 0x32a4;
	DWORD ObsTarget = 0x3398;

	//"lastVisibleTime (first down)"
	DWORD LastVisibleTime = 0x1ad4;

	//CInput (3 index)
	DWORD lkmState = 0x75028C0; //v2 |= 1u;
	DWORD rkmState = 0xBB3E9E8; //v2 |= 0x20000u;

	//"CameraPosition"
	DWORD CameraPos = 0x1EF0;

	//"GetAbsOrigin"
	DWORD AbsVecOrgin = 0x14C;

	//"Speed(%f) is greater than sv_maxvelocity(%f)"
	DWORD BulletSpeed = 0x1EE0;
	DWORD BulletGravity = BulletSpeed + 0x8;

	//"PlayerRunCommand_Prediction"
	DWORD CameraAng = 0x2538;
	DWORD DynamicAng = CameraAng - 0x10;

	//"GetSpread"
	//DWORD NoSpreadX = 0x15C0;
	//DWORD NoSpreadY = NoSpreadX - 0x10;
	//DWORD NoSpreadZ = NoSpreadY + 0x4;
}

namespace Fn {
	PVOID GetEntName;
	PVOID UpdateAbsOrigin;
	PVOID UpdateAbsVelocity;
}

namespace I {
	PVOID CHLClient;
	PVOID EngineTrace;
	//PVOID GlobalVars;
	IDXGISwapChain* SwapChain;
}

//IDA Vars
#define _BYTE BYTE
#define _DWORD DWORD
#define _QWORD DWORD64