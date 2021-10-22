#include "Globals.h"

//game sdk
#include "GameSDK/NetvarMgr.h"
#include "GameSDK/GameSDK.h"

//cheat sdk
#include "CheatSDK/CheatSDK.h"
#include "CheatSDK/Hooks/GameMovement.h"
#include "CheatSDK/Hooks/ClientMode.h"
#include "CheatSDK/Hooks/DirectX.h"


void EnumVFT(PVOID Class)
{
	
	for (size_t i = 0; i < 0xFFFFFFFF; i += 8)
	{
		bool brk = true;

		auto addr1 = *(PVOID*)(*(ULONG64*)Class + i);

		MEMORY_BASIC_INFORMATION base{}; 
		hp(VirtualQueryEx(GetCurrentProcess(), addr1, &base, sizeof(base)));
		hp(base.Protect);
		

		if ((base.Protect & PAGE_EXECUTE) || (base.Protect & PAGE_EXECUTE_READ) || (base.Protect & PAGE_EXECUTE_READWRITE))
			brk = false;

		if (brk) 
		{
			dp(i);
			break;
		}
	}

		
}

BOOL DllMain(HMODULE ModBase, DWORD CallReason, PVOID Reserv)
{
	if (CallReason == DLL_PROCESS_ATTACH)
	{

		
		//base init
		FC::SetSpoofStub(FindPattern(E("48 83 C4 68 C3")), FindPattern(E("FF E3")));
		
		//auto ClientAuthInterfaceV001 = RVA(FindPattern("48 89 05 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10"), 7);
		//auto GameClientInterfaceV012 = RVA(FindPattern("48 8B 0D ? ? ? ? 48 85 C9 0F 84 ? ? ? ? 48 8B 01 4C"), 7);

		//hp(ClientAuthInterfaceV001);
		//EnumVFT((PVOID)ClientAuthInterfaceV001);

		//hp(GameClientInterfaceV012);
		///EnumVFT((PVOID)GameClientInterfaceV012);

		//return 1;


		//get game funcs
		//Fn::UpdateAbsOrigin = (PVOID)EPtr(RVA(FindPattern(E("E8 ? ? ? ? F3 0F 10 76 ? F3 0F 5C")), 5));
		/*if (!Fn::UpdateAbsOrigin || ((DWORD64)Reserv != (((DWORD64)GetModuleBase("secur32.dll") + 0x9F16B3) ^ (DWORD64)DllMain))) {
			while (true) SpoofCall(nullptr); goto FakeEnd;
		}*/

		//Fn::MuzzlePos = (PVOID)FindPattern(E("40 53 48 83 EC 30 48 8B DA 8B 91"));
		//Fn::RayInit = (PVOID)FindPattern(E("F3 41 0F 10 08 F3 0F 5C 0A F3 0F 11"));
		//Fn::GetEntName = (PVOID)EPtr(FindPattern(E("48 83 3D ? ? ? ? ? 74 08 8B")));
		//Fn::UpdateAbsVelocity = (PVOID)EPtr(FindPattern(E("40 57 48 83 EC 50 F7")));

		//get global vars
		Off::ClientState = EPtr(RVA(FindPattern(E("48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 58")), 7));
		Off::LocalPlayer = EPtr(RVA(FindPattern(E("48 8B 05 ? ? ? ? 48 0F 44 C7")), 7)/* + 8*/);
		Off::NameList = EPtr(RVA(FindPattern(E("48 8D 05 ? ? ? ? FF CA")), 7));
		Off::EntityList = EPtr(RVA(FindPattern(E("48 8D 05 ? ? ? ? 44 2B")), 7));
		MessageBoxA(NULL, "Hello world!", "Hello World!", NULL);
		//hp(EPtr(Off::NameList));
		//hp((DWORD64)EPtr(Off::EntityList) - (DWORD64)GetModuleBase(nullptr));

		

		//get interfaces
		//I::CHLClient = EPtr((PVOID)RVA(FindPattern(E("48 8D 0D ? ? ? ? 48 89 0D ? ? ? ? 48 8D 05")), 7));
		//PVOID ClientModeClass = (PVOID)RVA(FindPattern(E("48 89 05 ? ? ? ? 48 8D 3D ? ? ? ? C7")), 7);
		I::SwapChain = EPtr(*(IDXGISwapChain**)RVA(FindPattern(E("4C 8D 0D ? ? ? ? 2B 45")), 7));
		I::EngineTrace = EPtr(*(PVOID*)(RVA(FindPattern(E("48 8B 0D ? ? ? ? 89 44 24 50")), 7)));
		//PVOID CGameMoveClass = (PVOID)RVA(FindPattern(E("48 8D 0D ? ? ? ? 8B 83")), 7);

		//get offsets
		//Off::NextShotTime = GetOffset(E("DT_WeaponX_LocalWeaponData"), E("m_nextPrimaryAttackTime"));
		//Off::PunchAngle = *(DWORD*)(FindPattern(E("E8 ? ? ? ? F2 0F 10 93 ? ? ? ? 0F 28 D8")) + 9);
		Off::StudioHdr = *(DWORD*)(FindPattern(E("48 83 B9 ? ? ? ? ? 48 8B D9 75 36")) + 3);
		Off::ViewRender = RVA(FindPattern(E("48 8B 0D ? ? ? ? 0F 29 74 24 ? F2")), 7);
		Off::AbsVelocity = GetOffset(E("DT_LocalPlayerExclusive"), E("m_vecAbsVelocity"));
		Off::ViewMatrix = *(DWORD*)(FindPattern(E("48 89 AB ? ? ? ? 4C 89 9B")) + 3);
		Off::ShieldHealthMax = GetOffset(E("DT_BaseEntity"), E("m_shieldHealthMax"));
		Off::ActiveWeapon = GetOffset(E("DT_Player"), E("m_latestPrimaryWeapons"));
		Off::BoneClass = GetOffset(E("DT_BaseEntity"), E("m_nForceBone")) + 0x48;
		Off::ScriptInt = GetOffset(E("DT_PropSurvival"), E("m_customScriptInt"));
		Off::ShieldHealth = GetOffset(E("DT_BaseEntity"), E("m_shieldHealth"));
		Off::PropName = GetOffset(E("DT_BaseEntity"), E("m_iSignifierName"));
		Off::Bleedout = GetOffset(E("DT_Player"), E("m_bleedoutState"));
		Off::LifeState = GetOffset(E("DT_Player"), E("m_lifeState"));
		Off::TeamNum = GetOffset(E("DT_Player"), E("m_iTeamNum"));
		Off::Health = GetOffset(E("DT_Player"), E("m_iHealth"));
		Off::Flags = GetOffset(E("DT_Player"), E("m_fFlags"));

		

		//hp(Off::TeamNum);

		//GameMovement hook
		//VMT_Hook CGameMove(CGameMoveClass, 2);
		//CGameMove.SetupHook(&SilentEndOrg, SilentEnd, 5);
		//CGameMove.SetupHook(&SilentStartOrg, SilentStart, 4);

		//ClientMode hook
		/*PVOID ClientModeClass = (PVOID)RVA(FindPattern(E("48 8D 0D ? ? ? ? 40 0F B6 D7 FF D0 EB D0")), 7);
		VMT_Hook CCreateMove(ClientModeClass, 8);
		CCreateMove.SetupHook(&ClientModeOrg, ClientModeHk, 4);*/

		USHORT MagicOff = 0x130;//*(USHORT*)(FindPattern(E("C6 45 ? 01 41"), E("dxgi")) + 18); //RS1-19H2(1909)
		DWORD64 MagicClass = *(DWORD64*)((DWORD64)EPtr(I::SwapChain) + MagicOff);
		PresentOrg = *(PVOID*)(MagicClass + 0x108);
		*(PVOID*)(MagicClass + 0x108) = Present;

		//DXGI Present hook
		//USHORT MemeOffset = *(USHORT*)(FindPattern(E("48 8B 8B ? ? ? ? 48 8B 01 4C 8D 45"), E("dxgi")) + 3);

		//auto v29 = *(_QWORD*)((DWORD64)EPtr(I::SwapChain) + 0xB0);
		//auto v30 = (void(__fastcall***)(_QWORD))(v29 + 0xE0);

		//VMT_Hook D3D(v30/**(PVOID*)((DWORD64)EPtr(I::SwapChain) + MemeOffset)*/);
		//D3D.SetupHook(&PresentOrg, Present, 0);
	}

	//inject ok
	return true;
}