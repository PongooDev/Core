#include "pch.h"

#include "../Public/Finder.h"

#include "../Public/Version.h"
#include "../Public/Offsets.h"
#include "../Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"

uintptr_t Finder::FindGUObjectArray() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::GUObjectArray)
		return ServerOffsets::GUObjectArray;

	static std::vector<std::string> Signatures = {
		"48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1",
		"48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1",
		"48 8B 05 ? ? ? ? 48 8D 14 C8 EB 03 49 8B D6 8B 42 08 C1 E8 1D A8 01 0F 85 ? ? ? ? F7 86 ? ? ? ? ? ? ? ?",
		"48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30"
	};

	for (const auto& Sig : Signatures)
	{
		Addr = Memcury::Scanner::FindPattern(Sig.c_str(), false).RelativeOffset(3).Get();
		if (Addr)
			break;
	}

	if (Addr)
	{
		ServerOffsets::GUObjectArray = Addr - ImageBase;
	}

	Log("GUObjectArray found at: 0x" + std::format("{:X}", ServerOffsets::GUObjectArray));
	return ServerOffsets::GUObjectArray;
}

uintptr_t Finder::FindGEngine()
{
	static uintptr_t Addr = 0;

	if (ServerOffsets::GEngine)
		return ServerOffsets::GEngine;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Initializing Game Engine...").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x05) {
				int32_t disp = *reinterpret_cast<int32_t*>(Ptr + 3);
				uintptr_t absAddr = (uintptr_t)(Ptr + 7) + disp;
				Addr = absAddr;
				break;
			}
		}
	}

	if (Addr)
	{
		ServerOffsets::GEngine = Addr - ImageBase;
	}

	Log("GEngine found at: 0x" + std::format("{:X}", ServerOffsets::GEngine));
	return ServerOffsets::GEngine;
}

uintptr_t Finder::FindGWorld() {
	static uintptr_t Addr = 0;

	if (ServerOffsets::GWorld)
		return ServerOffsets::GWorld;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Please restart your client. Error %d").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0x05) {
				int32_t disp = *reinterpret_cast<int32_t*>(Ptr + 3);
				uintptr_t absAddr = (uintptr_t)(Ptr + 7) + disp;
				Addr = absAddr;
				break;
			}
		}
	}

	if (Addr)
	{
		ServerOffsets::GWorld = Addr - ImageBase;
	}

	Log("GWorld found at: 0x" + std::format("{:X}", ServerOffsets::GWorld));
	return ServerOffsets::GWorld;
}

uintptr_t Finder::FindProcessEventVFT() {
	if (ServerOffsets::ProcessEventVFT)
		return ServerOffsets::ProcessEventVFT;

	uintptr_t Addr = 0;

	if (Version::Fortnite_Version < 14.00)
		Addr = Memcury::Scanner::FindStringRef(L"AccessNoneNoContext").ScanFor({ 0x40, 0x55 }, true, 0).Get();
	else if (floor(Version::Fortnite_Version) == 27)
		Addr = Memcury::Scanner::FindPattern(
			"40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 45 33 E4 4C 89 45 ? 4D 8B F8")
		.Get();
	else if (Version::Engine_Version == 5.2 || (std::floor(Version::Fortnite_Version) == 24 && Version::Fortnite_Version >= 24.30))
		Addr = Memcury::Scanner::FindPattern(
			"40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 45 33 F6")
		.Get();
	else if (Version::Fortnite_Version >= 23.00)
	{
		Addr = Memcury::Scanner::FindPattern("48 85 C9 0F 85 ? ? ? ? F7 87 ? ? ? ? ? ? ? ? ? 8B ?").ScanFor({ 0x40, 0x55 }, false).Get();
		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("41 FF 92 ? ? ? ? E9 ? ? ? ? 49 8B C8").ScanFor({ 0x40, 0x55 }, false).Get();
	}
	else
		Addr = Memcury::Scanner::FindStringRef(L"UMeshNetworkComponent::ProcessEvent: Invalid mesh network node type: %s", true, 0, Version::Fortnite_Version >= 19.00)
		.ScanFor({ 0xE8 }, true, Version::Fortnite_Version < 19.00 ? 1 : 3)
		.RelativeOffset(1)
		.Get();

	auto DefaultObj = UObject::StaticClass()->GetDefaultObj();

	if (DefaultObj) {
		for (int i = 0; i < 0x100; i++) {
			if (__int64(DefaultObj->VTable[i]) == Addr)
			{
				ServerOffsets::ProcessEventVFT = i;
				break;
			}
		}
	}

	Log("ProcessEvent VFT found at: 0x" + std::format("{:X}", ServerOffsets::ProcessEventVFT));
	return ServerOffsets::ProcessEventVFT;
}

uintptr_t Finder::FindProcessEvent() {
	if (ServerOffsets::ProcessEvent)
		return ServerOffsets::ProcessEvent;

	ServerOffsets::ProcessEvent = ((uintptr_t)UObject::StaticClass()->GetDefaultObject()->VTable[FindProcessEventVFT()]) - ImageBase;

	Log("ProcessEvent found at: 0x" + std::format("{:X}", ServerOffsets::ProcessEvent));
	return ServerOffsets::ProcessEvent;
}

uintptr_t Finder::FindGIsClient() {
	if (ServerOffsets::GIsClient)
		return ServerOffsets::GIsClient;
	auto Addr = Memcury::Scanner::FindStringRef(L"AllowCommandletRendering", false);

	std::vector<std::vector<uint8_t>> BytesArray = { {0xC6, 0x05}, {0x88, 0x1D}, {0x44, 0x88} };

	int Skip = 2;

	uint64_t Addy = 0;
	int PickedByte = 0;

	for (int i = 0; i < 50; i++)
	{
		auto CurrentByte = *(Memcury::ASM::MNEMONIC*)(Addr.Get() - i);

		bool ShouldBreak = false;

		for (auto& Bytes : BytesArray)
		{
			if (CurrentByte == Bytes[0])
			{
				bool Found = true;
				for (int j = 1; j < Bytes.size(); j++)
				{
					if (*(Memcury::ASM::MNEMONIC*)(Addr.Get() - i + j) != Bytes[j])
					{
						Found = false;
						break;
					}
				}
				if (Found)
				{
					bool bIsScuffedByte = Bytes[0] == 0x44;
					int Relative = bIsScuffedByte ? 3 : 2;
					auto current = Memcury::Scanner(Addr.Get() - i);

					if (bIsScuffedByte)
					{
						if (*(Memcury::ASM::MNEMONIC*)(Addr.Get() - i + 2) == 0x74)
						{
							continue;
						}
					}

					if (!PickedByte)
					{
						PickedByte = Bytes[0];
					}
					else if (PickedByte != Bytes[0])
						continue;

					if (Skip > 0)
					{
						Skip--;
						continue;
					}

					Addy = Bytes[0] == 0xC6
						? current.RelativeOffset(Relative, 1).Get()
						: current.RelativeOffset(Relative).Get();
					ShouldBreak = true;
					break;
				}
			}
		}

		if (ShouldBreak)
			break;

	}

	if (Addy)
	{
		ServerOffsets::GIsClient = Addy - ImageBase;
	}

	Log("GIsClient found at: 0x" + std::format("{:X}", ServerOffsets::GIsClient));
	return ServerOffsets::GIsClient;
}

uintptr_t Finder::FindGIsServer() {
	if (!Finder::FindGIsClient())
		return 0;

	ServerOffsets::GIsServer = ServerOffsets::GIsClient + 1;
	Log("GIsServer found at: 0x" + std::format("{:X}", ServerOffsets::GIsServer));
	return ServerOffsets::GIsServer;
}

uintptr_t Finder::FindStaticFindObject() {
	uint64_t Addr = 0;
	if (ServerOffsets::StaticFindObject)
		return ServerOffsets::StaticFindObject;

	if (Version::Engine_Version == 5.00)
	{

		Addr = Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 4C 8B E1 45 0F B6 E9 49 8B F8 41 8B C6").Get();

		if (!Addr)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 4C 89 64 24 ? 55 41 55 41 57 48 8B EC 48 83 EC 50 4C 8B E9").Get(); // 19.40
		}

	}
	else if (Version::Engine_Version >= 4.27)
	{
		if (Version::Fortnite_Version < 18.00)
		{
			if (Version::Fortnite_Version == 16.50)
			{
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60 45 33 ED 45 8A F9 44 38 2D ? ? ? ? 49 8B F8 48 8B F2 4C 8B E1").Get();
			}

			else
			{
				Addr = Memcury::Scanner::FindPattern("40 55 53 57 41 54 41 55 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85").Get();
			}
		}
		else
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60 45 33 ED 45 8A F9 44 38 2D ? ? ? ? 49 8B F8 48 8B").Get();
		}
	}
	else if (!Addr)
	{
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ?? 48 89 74 24 ?? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ?? 80 3D ?? ?? ?? ?? ?? 45 0F B6 F1").Get();
	}

	else if (!Addr)
	{
		auto String = Memcury::Scanner::FindStringRef(L"Illegal call to StaticFindObject() while serializing object data!", true, 1, Version::Engine_Version >= 4.27);
		auto addr = Memcury::Scanner::FindBytes(String, { 0x48, 0x89, 0x5C }, 1024, 0, true, 0, false);
	}

	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("4C 8B DC 57 48 81 EC ? ? ? ? 80 3D").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 45 33 ED 4D 8B F9").Get();
	}

	if (Addr)
	{
		ServerOffsets::StaticFindObject = Addr - ImageBase;
	}

	Log("StaticFindObject found at: 0x" + std::format("{:X}", ServerOffsets::StaticFindObject));
	return ServerOffsets::StaticFindObject;
}

uintptr_t Finder::FindStaticLoadObject() {
	if (ServerOffsets::StaticLoadObject)
		return ServerOffsets::StaticLoadObject;
	auto Addr = Memcury::Scanner::FindStringRef(L"STAT_LoadObject", false).Get();

	if (!Addr)
	{
		auto String = Memcury::Scanner::FindStringRef(L"Calling StaticLoadObject during PostLoad may result in hitches during streaming.");
		Addr = Memcury::Scanner::FindBytes(String, { 0x40, 0x55 }, 1000, 0, true);
	}

	for (int i = 0; i < 400 && Addr; i++)
	{
		if ((*(uint8_t*)(Addr - i) == 0x4C && *(uint8_t*)(Addr - i + 1) == 0x89 && *(uint8_t*)(Addr - i + 2) == 0x4C) ||
			(*(uint8_t*)(Addr - i) == 0x48 && *(uint8_t*)(Addr - i + 1) == 0x8B && *(uint8_t*)(Addr - i + 2) == 0xC4))
		{
			Addr = Addr - i;
			break;
		}
	}

	if (!Addr)
	{
		Addr = Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 8B 85 ? ? ? ? 33 F6 4C 8B BD").Get();
	}

	if (Addr)
	{
		ServerOffsets::StaticLoadObject = Addr - ImageBase;
	}

	Log("StaticLoadObject found at: 0x" + std::format("{:X}", ServerOffsets::StaticLoadObject));
	return ServerOffsets::StaticLoadObject;
}

uintptr_t Finder::FindUEngine_CreateNetDriver() {
	if (ServerOffsets::UEngine_CreateNetDriver)
		return ServerOffsets::UEngine_CreateNetDriver;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ?? 57 48 83 EC ?? 49 8B D8 48 8B F9 E8 ?? ?? ?? ?? 48 8B D0").Get();

	if (!Addr)
	{
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 49 8B D8 48 8B F9 E8 ? ? ? ? 48 8B D0").Get();
	}

	if (Version::Fortnite_Version >= 16.00)
	{
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 4C 89 44 24 ? 56 57 41 56 48 83 EC ? 48 63 81").Get();
	}

	if (Addr)
	{
		ServerOffsets::UEngine_CreateNetDriver = Addr - ImageBase;
	}

	Log("UEngine::CreateNetDriver found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CreateNetDriver));
	return ServerOffsets::UEngine_CreateNetDriver;
}

uintptr_t Finder::FindUNetDriver_SetWorldVFT() {
	if (ServerOffsets::UNetDriver_SetWorldVFT)
		return ServerOffsets::UNetDriver_SetWorldVFT;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::UNetDriver_SetWorldVFT;

	auto stringAddr = Memcury::Scanner::FindStringRef(L"All Beacon Requests Paused.").Get();

	for (int i = 0; i < 300; i++)
	{
		if (*(uint8*)(stringAddr + i) == 0xFF && *(uint8*)(stringAddr + i + 1) == 0x90)
		{
			ServerOffsets::UNetDriver_SetWorldVFT = (*(uint32*)(stringAddr + i + 2)) / 8;
			break;
		}
	}

	bInitialized = true;
	Log("UNetDriver::SetWorld VFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_SetWorldVFT));
	return ServerOffsets::UNetDriver_SetWorldVFT;
}

uintptr_t Finder::FindUNetDriver_SetWorld() {
	if (ServerOffsets::UNetDriver_SetWorld)
		return ServerOffsets::UNetDriver_SetWorld;

	auto stringAddr = Memcury::Scanner::FindStringRef(L"All Beacon Requests Resumed.").Get();

	int skipped = 0;

	for (int i = 0; i < 150; i++)
	{
		if (*(uint8*)(stringAddr + i) == 0xE8)
		{
			if (skipped == 1)
			{
				ServerOffsets::UNetDriver_SetWorld = Utils::GetCallDestination(stringAddr + i) - ImageBase;
				break;
			}
			else
			{
				skipped++;
			}
		}
	}

	Log("UNetDriver::SetWorld found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_SetWorld));
	return ServerOffsets::UNetDriver_SetWorld;
}

uintptr_t Finder::FindUNetDriver_InitListen() {
	if (ServerOffsets::UNetDriver_InitListen)
		return ServerOffsets::UNetDriver_InitListen;
	uintptr_t Addr = 0;

	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::UNetDriver_InitListen;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Fortnite_Version == 1.91) {
			Addr = ImageBase + 0x3A5D200;
		}
		else if (Version::Engine_Version >= 5.0)
		{
			Addr = Memcury::Scanner::FindPattern("4D 8B C8 4C 8B C2 33 D2 FF 90 ? ? ? ? 84 C0 75 ? 80 3D").ScanFor({ 0x4C, 0x8B, 0xDC }, false).Get();
			if (!Addr)
			{
				Addr = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B ? 49 89 73 ? 57 48 83 EC ? 48 8B BC 24").Get();

				if (!Addr)
					Addr = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B 08 49 89 73 10 57 48 83 EC 40 48 8B 7C 24 ? 49 8B F0 48 8B 01 48 8B D9 49 89 7B E0 45").Get();
			}
		}
		else if (Version::Engine_Version >= 4.27) {
			Addr = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B 08 49 89 73 10 57 48 83 EC 50 48 8B BC 24 ? ? ? ? 49 8B F0 48 8B 01 48 8B").Get();
		}
		else
		{
			auto sRef = Memcury::Scanner::FindStringRef(L"%s IpNetDriver listening on port %i").Get();
			int skip = 1;
			for (int i = 0; i < 2000; i++)
			{
				auto Ptr = (uint8_t*)(sRef - i);

				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c)
				{
					if (skip > 0)
					{
						skip--;
						continue;
					}
					Addr = uint64_t(Ptr);
					break;
				}
			}
		}
	}

	if (Addr)
	{
		ServerOffsets::UNetDriver_InitListen = Addr - ImageBase;
	}

	Log("UNetDriver::InitListen found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_InitListen));
	return ServerOffsets::UNetDriver_InitListen;
}

uintptr_t Finder::FindUNetDriver_InitListenVFT() {
	if (ServerOffsets::UNetDriver_InitListenVFT)
		return ServerOffsets::UNetDriver_InitListenVFT;

	static void** NetDriverVFT = ((UClass*)FUObjectArray::FindObjectFast("IpNetDriver"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1000; i++)
	{
		if ((uintptr_t)NetDriverVFT[i] == (ImageBase + FindUNetDriver_InitListen()))
		{
			ServerOffsets::UNetDriver_InitListenVFT = i;
			break;
		}
	}

	Log("UNetDriver::InitListen VFT Index found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_InitListenVFT));
	return ServerOffsets::UNetDriver_InitListenVFT;
}

uintptr_t Finder::FindChangeGameSessionId() {
	if (ServerOffsets::ChangeGameSessionId)
		return ServerOffsets::ChangeGameSessionId;
	uintptr_t Addr = 0;

	if (Version::Engine_Version >= 427)
	{
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 4C 8B FA 4C").Get();
	}

	if (Version::Engine_Version == 500)
	{
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 50 4C 8B FA 48 8B F1 E8").Get();
	}

	else if (!Addr)
	{
		auto String = Memcury::Scanner::FindStringRef(L"Changing GameSessionId from '%s' to '%s'");

		Addr = Memcury::Scanner::FindBytes(String, { 0x40, 0x55 }, 2000, 0, true);
	}

	if (Addr)
	{
		ServerOffsets::ChangeGameSessionId = Addr - ImageBase;
	}

	Log("ChangeGameSessionId found at: 0x" + std::format("{:X}", ServerOffsets::ChangeGameSessionId));
	return ServerOffsets::ChangeGameSessionId;
}

uintptr_t Finder::FindInternalServerTryActivateAbilityVFT() {
	if (ServerOffsets::InternalServerTryActivateAbilityVFT)
		return ServerOffsets::InternalServerTryActivateAbilityVFT;
	uintptr_t VTableIndex = 0x0;

	UObject* DefaultObj = UAbilitySystemComponent::GetDefaultObj();

	if (Version::Engine_Version > 4.20)
	{
		auto OnRep_ReplicatedAnimMontage = DefaultObj->FindFunction("OnRep_ReplicatedAnimMontage");
		VTableIndex = OnRep_ReplicatedAnimMontage->GetVTableIndex() - 1;
	}
	else
	{
		auto ServerTryActivateAbilityWithEventData = DefaultObj->FindFunction("ServerTryActivateAbilityWithEventData");
		auto ServerTryActivateAbilityWithEventDataNativeAddr = __int64(DefaultObj->VTable[ServerTryActivateAbilityWithEventData->GetVTableIndex()]);

		for (int i = 0; i < 400; i++)
		{
			if ((*(uint8_t*)(ServerTryActivateAbilityWithEventDataNativeAddr + i) == 0xFF && *(uint8_t*)(ServerTryActivateAbilityWithEventDataNativeAddr + i + 1) == 0x90)
				|| (*(uint8_t*)(ServerTryActivateAbilityWithEventDataNativeAddr + i) == 0xFF && *(uint8_t*)(ServerTryActivateAbilityWithEventDataNativeAddr + i + 1) == 0x93))
			{
				VTableIndex = *(uint32*)(ServerTryActivateAbilityWithEventDataNativeAddr + i + 2) / 8;
				break;
			}
		}
	}

	ServerOffsets::InternalServerTryActivateAbilityVFT = VTableIndex;
	Log("InternalServerTryActivateAbility VFT Index found at: 0x" + std::format("{:X}", ServerOffsets::InternalServerTryActivateAbilityVFT));
	return ServerOffsets::InternalServerTryActivateAbilityVFT;
}

uintptr_t Finder::FindUAbilitySystemComponent_GiveAbility() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UAbilitySystemComponent_GiveAbility)
		return ServerOffsets::UAbilitySystemComponent_GiveAbility;

	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 83 B9 ? ? ? ? ? 49 8B F0").Get();
	}

	if (Addr)
	{
		ServerOffsets::UAbilitySystemComponent_GiveAbility = Addr - ImageBase;
	}

	Log("UAbilitySystemComponent::GiveAbility found at: 0x" + std::format("{:X}", ServerOffsets::UAbilitySystemComponent_GiveAbility));
	return ServerOffsets::UAbilitySystemComponent_GiveAbility;
}

uintptr_t Finder::FindGHandle() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::GHandle)
		return ServerOffsets::GHandle;
	static bool bInitialized = false;
	if (bInitialized) return ServerOffsets::GHandle;

	auto match = Memcury::Scanner::FindPattern(
		"8B 05 ? ? ? ? 89 41 0C FF C0 89 05 ? ? ? ?"
	);

	if (match.IsValid()) {
		uintptr_t movLoadInstr = match.Get();
		uintptr_t Addr = Utils::ResolveRipRelative(movLoadInstr, 2, 6);
	}

	if (Addr) {
		ServerOffsets::GHandle = Addr - ImageBase;
	}

	if (!bInitialized) bInitialized = true;
	Log("GHandle found at: 0x" + std::format("{:X}", ServerOffsets::GHandle));
	return ServerOffsets::GHandle;
}

uintptr_t Finder::FindAbilitySpecDefaultConstructor() {
	if (ServerOffsets::AbilitySpecDefaultConstructor)
		return ServerOffsets::AbilitySpecDefaultConstructor;
	static uintptr_t Addr = 0;
	if (Addr) return Addr;

	auto match = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 33 FF C6 01 ? 48 89 79 ? 48 8B D9 89 79");

	if (!match.IsValid())
		return 0;

	uintptr_t fnVa = match.Get();

	for (size_t i = 0; i < 0x60; ++i)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(fnVa + i);

		if (p[0] == 0xE8)
		{
			uintptr_t callVa = fnVa + i;
			uintptr_t targetVa = Utils::ResolveRipRelative(callVa, 1, 5);

			ServerOffsets::AbilitySpecDefaultConstructor = targetVa - ImageBase;
		}
	}

	Log("AbilitySpecDefaultConstructor found at: 0x" + std::format("{:X}", ServerOffsets::AbilitySpecDefaultConstructor));
	return ServerOffsets::AbilitySpecDefaultConstructor;
}

uintptr_t Finder::FindAbilitySpecClassConstructor() {
	if (ServerOffsets::AbilitySpecClassConstructor)
		return ServerOffsets::AbilitySpecClassConstructor;
	uintptr_t Addr = 0;

	// 12.41 sig
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 45 33 F6 48 C7 01").Get();

	if (Addr)
	{
		ServerOffsets::AbilitySpecClassConstructor = Addr - ImageBase;
	}

	Log("AbilitySpecClassConstructor found at: 0x" + std::format("{:X}", ServerOffsets::AbilitySpecClassConstructor));
	return ServerOffsets::AbilitySpecClassConstructor;
}

uintptr_t Finder::FindAbilitySpecCDOConstructor() {
	if (ServerOffsets::AbilitySpecCDOConstructor)
		return ServerOffsets::AbilitySpecCDOConstructor;
	static uintptr_t Addr = 0;

	static bool bInitialized = false;
	if (bInitialized) return ServerOffsets::AbilitySpecCDOConstructor;

	if (!bInitialized)
	{
		if (Version::Engine_Version >= 4.20 && Version::Engine_Version <= 4.24) {
			Addr = Memcury::Scanner::FindPattern("80 61 29 F8 48 8B 44 24 ?").Get();
		}
		else if (Version::Engine_Version == 4.25)
		{
			Addr = Memcury::Scanner::FindPattern("48 8B 44 24 ? 80 61 29 F8 80 61 31 FE 48 89 41 20 33 C0 89 41", false).Get();

			if (!Addr) {
				Addr = Memcury::Scanner::FindPattern("80 61 29 F8 48 8B 44 24 ?").Get();
			}
		}
		else if (Version::Engine_Version == 4.26)
		{
			Addr = Memcury::Scanner::FindPattern("80 61 31 FE 0F 57 C0 80 61 29 F0 48 8B 44 24 ? 48").Get();

			if (!Addr) {
				Addr = Memcury::Scanner::FindPattern("48 8B 44 24 ? 80 61").Get();
			}
		}
		else if (Version::Engine_Version == 4.27) {
			Addr = Memcury::Scanner::FindPattern("80 61 31 FE 41 83 C9 FF 80 61 29 F0 48 8B 44 24 ? 48 89 41").Get();
		}
		else if (Version::Engine_Version == 5.0) {
			Addr = Memcury::Scanner::FindPattern("4C 8B C9 48 8B 44 24 ? 83 C9 FF 41 80 61 ? ? 41 80 61 ? ? 49 89 41 20 33 C0 41 88 41 30 49 89 41").Get();
		}
		else if (Version::Engine_Version >= 5.4)
		{
			Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 41 83 CE ? 33 ED 40 38 2D ? ? ? ? 41 8B F8").Get();

			if (!Addr) {
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 41 83 CE").Get();
			}
		}
		else if (Version::Engine_Version >= 5.1)
		{
			Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 48 8B 44 24 ? 41 83 CE").Get();

			if (!Addr) {
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 44 89 48 ? 57 48 83 EC ? 48 8B 44 24").Get();
			}
		}
	}

	if (Addr)
	{
		ServerOffsets::AbilitySpecCDOConstructor = Addr - ImageBase;
	}

	bInitialized = true;
	Log("AbilitySpecCDOConstructor found at: 0x" + std::format("{:X}", ServerOffsets::AbilitySpecCDOConstructor));
	return ServerOffsets::AbilitySpecCDOConstructor;
}

uintptr_t Finder::FindUAbilitySystemComponent_InternalTryActivateAbility() {
	uintptr_t Addr = 0;
	if (ServerOffsets::UAbilitySystemComponent_InternalTryActivateAbility)
		return ServerOffsets::UAbilitySystemComponent_InternalTryActivateAbility;

	auto sRef = Memcury::Scanner::FindStringRef(L"InternalTryActivateAbility called with invalid Handle! ASC: %s. AvatarActor: %s", true, 0, Version::Fortnite_Version >= 16).Get();

	for (int i = 0; i < 1000; i++)
	{
		if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4)
			Addr = sRef - i;

		else if (*(uint8_t*)(sRef - i) == 0x4C && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x4C)
			Addr = sRef - i;

		else if (Version::Fortnite_Version >= 25 && *(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C)
			Addr = sRef - i;
	}

	if (Addr)
	{
		ServerOffsets::UAbilitySystemComponent_InternalTryActivateAbility = Addr - ImageBase;
		Log("UAbilitySystemComponent::InternalTryActivateAbility found at: 0x" + std::format("{:X}", ServerOffsets::UAbilitySystemComponent_InternalTryActivateAbility));
		return ServerOffsets::UAbilitySystemComponent_InternalTryActivateAbility;
	}

	return 0;
}

uintptr_t Finder::FindABuildingActor_OnDamageServer() {
	uintptr_t Addr = 0;
	if (ServerOffsets::ABuildingActor_OnDamageServer)
		return ServerOffsets::ABuildingActor_OnDamageServer;

	Addr = FindFunctionCall(L"OnDamageServer", Version::Engine_Version == 4.16 ? std::vector<uint8_t>{ 0x4C, 0x89, 0x4C } : Version::Engine_Version == 4.19 || Version::Engine_Version >= 4.27 ? std::vector<uint8_t>{ 0x48, 0x8B, 0xC4 } : std::vector<uint8_t>{ 0x40, 0x55 });

	if (Addr)
	{
		ServerOffsets::ABuildingActor_OnDamageServer = Addr - ImageBase;
	}

	Log("ABuildingActor_OnDamageServer found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingActor_OnDamageServer));
	return ServerOffsets::ABuildingActor_OnDamageServer;
}

uintptr_t Finder::FindAFortPlayerControllerZone_ClientOnPawnDied() {
	uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerControllerZone_ClientOnPawnDied)
		return ServerOffsets::AFortPlayerControllerZone_ClientOnPawnDied;

	Addr = FindFunctionCall(L"ClientOnPawnDied", Version::Engine_Version == 4.16 ? std::vector<uint8_t> { 0x48, 0x89, 0x54 } : (Version::Fortnite_Version >= 24 && Version::Fortnite_Version < 25 ? std::vector<uint8_t> { 0x48, 0x8B, 0xC4 } : std::vector<uint8_t>{ 0x48, 0x89, 0x5C }));

	if (Addr)
	{
		ServerOffsets::AFortPlayerControllerZone_ClientOnPawnDied = Addr - ImageBase;
	}

	Log("AFortPlayerControllerZone_ClientOnPawnDied found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerControllerZone_ClientOnPawnDied));
	return ServerOffsets::AFortPlayerControllerZone_ClientOnPawnDied;
}

uintptr_t Finder::FindSpawnActor() {
	if (ServerOffsets::SpawnActor)
		return ServerOffsets::SpawnActor;
	static uintptr_t Addr = 0;

	if (Version::Engine_Version >= 4.27)
	{
		auto stat = Memcury::Scanner::FindStringRef(L"STAT_SpawnActorTime").Get();

		for (int i = 0; i < 0x1000; i++)
		{
			if (*(uint8_t*)(stat - i) == 0x40 && *(uint8_t*)(stat - i + 1) == 0x55)
			{
				Addr = stat - i;
				break;
			}
			else if (*(uint8_t*)(stat - i) == 0x48 && *(uint8_t*)(stat - i + 1) == 0x8B && *(uint8_t*)(stat - i + 2) == 0xC4)
			{
				Addr = stat - i;
				break;
			}
		}
	}
	else
	{
		auto sRef = Memcury::Scanner::FindStringRef(L"SpawnActor failed because no class was specified");

		if (Version::Fortnite_Version <= 3.3)
			Addr = sRef.ScanFor({ 0x40, 0x55 }, false, 0).Get();
		else
			Addr = sRef.ScanFor({ 0x4C, 0x8B, 0xDC }, false, 0).Get();
	}

	if (Addr)
	{
		ServerOffsets::SpawnActor = Addr - ImageBase;
	}

	Log("SpawnActor found at: 0x" + std::format("{:X}", ServerOffsets::SpawnActor));
	return ServerOffsets::SpawnActor;
}

uintptr_t Finder::FindStep() {
	if (ServerOffsets::Step)
		return ServerOffsets::Step;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 41 20 4C 8B D2 48 8B D1 44 0F B6 08 48 FF").Get();
	if (!Addr)
		Addr = Memcury::Scanner::FindPattern("48 8B 41 ? 4C 8B DA 44 0F B6 08").Get();

	if (Addr) {
		ServerOffsets::Step = Addr - ImageBase;
	}

	Log("Step found at: 0x" + std::format("{:X}", ServerOffsets::Step));
	return ServerOffsets::Step;
}

uintptr_t Finder::FindStepExplicitProperty() {
	if (ServerOffsets::StepExplicitProperty)
		return ServerOffsets::StepExplicitProperty;
	static uintptr_t Addr = 0;

	if (Version::Engine_Version >= 5.4 || Version::Engine_Version == 5.2)
		Addr = Memcury::Scanner::FindPattern("41 8B 40 ? 4D 8B C8 48 0F BA E0").Get();
	else if (Version::Engine_Version == 5.3)
		Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 54 41 56 41 57 48 83 EC ? 41 8B 40 ? 49 8B D8 48 8B F2").Get();
	else if (Version::Fortnite_Version >= 20.20)
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 41 8B 40 ? 49 8B D8 48 8B F2").Get();
	else
		Addr = Memcury::Scanner::FindPattern("41 8B 40 ? 4D 8B C8").Get();

	if (Addr) {
		ServerOffsets::StepExplicitProperty = Addr - ImageBase;
	}

	Log("StepExplicitProperty found at: 0x" + std::format("{:X}", ServerOffsets::StepExplicitProperty));
	return ServerOffsets::StepExplicitProperty;
}

uintptr_t Finder::FindFMemory_Free() {
	if (ServerOffsets::FMemory_Free)
		return ServerOffsets::FMemory_Free;

	int skipped = 0;

	auto straddr = Memcury::Scanner::FindStringRef(L"GiveAbilityAndActivateOnce called on ability %s on the client, not allowed!").Get();

	for (int i = 0; i < 100; i++)
	{
		uintptr_t Addy = straddr + i;
		if (*(uint8*)(Addy) == 0xE8)
		{
			if (skipped == 1)
			{
				ServerOffsets::FMemory_Free = Utils::GetCallDestination(Addy) - ImageBase;
				break;
			}
			skipped++;
		}
	}

	Log("FMemory::Free found at: 0x" + std::format("{:X}", ServerOffsets::FMemory_Free));
	return ServerOffsets::FMemory_Free;
}

uintptr_t Finder::FindFMemory_Realloc() {
	if (ServerOffsets::FMemory_Realloc)
		return ServerOffsets::FMemory_Realloc;
	static uintptr_t Addr = 0;

	// 12.41 sig
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D ? ? ? ?").Get();

	if (Addr) {
		ServerOffsets::FMemory_Realloc = Addr - ImageBase;
	}

	Log("FMemory::Realloc found at: 0x" + std::format("{:X}", ServerOffsets::FMemory_Realloc));
	return ServerOffsets::FMemory_Realloc;
}

uintptr_t Finder::FindFName_AppendString() {
	if (ServerOffsets::FName_AppendString)
		return ServerOffsets::FName_AppendString;
	static uintptr_t Addr = 0;

	auto SRef = Memcury::Scanner::FindStringRef("ForwardShadingQuality_");
	constexpr std::array<const char*, 5> sigs =
	{
		"48 8D ? ? 48 8D ? ? E8",
		"48 8D ? ? ? 48 8D ? ? E8",
		"48 8D ? ? 49 8B ? E8",
		"48 8D ? ? ? 49 8B ? E8",
		"48 8D ? ? 48 8B ? E8"
	};

	for (auto& sig : sigs)
	{
		auto Scanner = SRef;
		Scanner.ScanFor(sig, true, 0, 1, Version::Engine_Version == 5.0 ? 0x100 : 0x50);

		if (Scanner.Get() != SRef.Get())
		{
			auto p2b = Memcury::ASM::pattern2bytes(sig);

			Addr = Scanner.RelativeOffset((uint32_t)p2b.size()).Get();
			break;
		}
	}

	if (Addr) {
		ServerOffsets::FName_AppendString = Addr - ImageBase;
	}

	Log("FName::AppendString found at: 0x" + std::format("{:X}", ServerOffsets::FName_AppendString));
	return ServerOffsets::FName_AppendString;
}

uintptr_t Finder::FindFName_ToStringOut() {
	if (ServerOffsets::FName_ToStringOut)
		return ServerOffsets::FName_ToStringOut;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::FName_ToStringOut;

	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B E9 E8 ? ? ? ? 48 8B F0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 ED 48 8B FA 4C 89 2A").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 33 ED 48 8B FA 48 89 2A 48 89 6A ? 8B 19").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 01 48 8B DA").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B 01").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B DA 4C 8B F1").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B E9 E8 ? ? ? ? 48 8B C8").Get();
	}

	if (Addr) {
		ServerOffsets::FName_ToStringOut = Addr - ImageBase;
	}

	if (!bInitialized) bInitialized = true;
	Log("FName::ToStringOut found at: 0x" + std::format("{:X}", ServerOffsets::FName_ToStringOut));
	return ServerOffsets::FName_ToStringOut;
}

uintptr_t Finder::FindFName_Compare() {
	if (ServerOffsets::FName_Compare)
		return ServerOffsets::FName_Compare;
	static uintptr_t Addr = 0;

	constexpr std::array<const char*, 5> sigs =
	{
		"48 89 5C 24 ? 57 48 83 EC ? 8B 02 48 8B DA 48 8B F9 39 01 75 ? 8B 41",
		"8B 02 39 01 75 ? 8B 41"
	};

	for (auto& sig : sigs)
	{
		Addr = Memcury::Scanner::FindPattern(sig).Get();
		if (Addr) {
			break;
		}
	}

	if (Addr) {
		ServerOffsets::FName_Compare = Addr - ImageBase;
	}

	Log("FName::Compare found at: 0x" + std::format("{:X}", ServerOffsets::FName_Compare));
	return ServerOffsets::FName_Compare;
}

uintptr_t Finder::FindUObjectBase_AddObject() {
	if (ServerOffsets::UObjectBase_AddObject)
		return ServerOffsets::UObjectBase_AddObject;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 89 51 ? 41 8B F0").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_AddObject = Addr - ImageBase;
	}

	Log("UObjectBase::AddObject found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_AddObject));
	return ServerOffsets::UObjectBase_AddObject;
}

uintptr_t Finder::FindUObjectBase_AtomicallyClearFlags() {
	if (ServerOffsets::UObjectBase_AtomicallyClearFlags)
		return ServerOffsets::UObjectBase_AtomicallyClearFlags;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("F7 D2 0F 0D 49").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_AtomicallyClearFlags = Addr - ImageBase;
	}

	Log("UObjectBase::AtomicallyClearFlags found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_AtomicallyClearFlags));
	return ServerOffsets::UObjectBase_AtomicallyClearFlags;
}

uintptr_t Finder::FindUObjectBase_DeferredRegister() {
	if (ServerOffsets::UObjectBase_DeferredRegister)
		return ServerOffsets::UObjectBase_DeferredRegister;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B F1 49 8B D0").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_DeferredRegister = Addr - ImageBase;
	}

	Log("UObjectBase::DeferredRegister found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_DeferredRegister));
	return ServerOffsets::UObjectBase_DeferredRegister;
}

uintptr_t Finder::FindUObjectBase_EmitBaseReferences() {
	if (ServerOffsets::UObjectBase_EmitBaseReferences)
		return ServerOffsets::UObjectBase_EmitBaseReferences;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 65 48 8B 04 25 ? ? ? ? 48 8B F9 8B 15 ? ? ? ? B9 ? ? ? ? 48 8B 1C D0 48 03 D9 8B 03 39 05 ? ? ? ? 7F").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_EmitBaseReferences = Addr - ImageBase;
	}

	Log("UObjectBase::EmitBaseReferences found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_EmitBaseReferences));
	return ServerOffsets::UObjectBase_EmitBaseReferences;
}

uintptr_t Finder::FindUObjectBase_IsValidLowLevel() {
	if (ServerOffsets::UObjectBase_IsValidLowLevel)
		return ServerOffsets::UObjectBase_IsValidLowLevel;
	static uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"NULL object").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Object is not registered").Get();
	}
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x83 && *(Ptr + 2) == 0xEC)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UObjectBase_IsValidLowLevel = Addr - ImageBase;
	}

	Log("UObjectBase::IsValidLowLevel found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_IsValidLowLevel));
	return ServerOffsets::UObjectBase_IsValidLowLevel;
}

uintptr_t Finder::FindUObjectBase_IsValidLowLevelFast() {
	if (ServerOffsets::UObjectBase_IsValidLowLevelFast)
		return ServerOffsets::UObjectBase_IsValidLowLevelFast;
	static uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"'this' pointer is misaligned.").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Virtual functions table is invalid.").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Object flags are invalid or either Class or Outer is misaligned").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Class object failed IsValidLowLevelFast test.").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Object array index or name index is invalid.").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Class pointer is invalid or CDO is invalid.").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"'this' pointer is invalid.").Get();
	}
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x53)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UObjectBase_IsValidLowLevelFast = Addr - ImageBase;
	}

	Log("UObjectBase::IsValidLowLevelFast found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_IsValidLowLevelFast));
	return ServerOffsets::UObjectBase_IsValidLowLevelFast;
}

uintptr_t Finder::FindUObjectBase_LowLevelRename() {
	if (ServerOffsets::UObjectBase_LowLevelRename)
		return ServerOffsets::UObjectBase_LowLevelRename;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B DA 48 8B F9 E8 ? ? ? ? 48 89 5F").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_LowLevelRename = Addr - ImageBase;
	}

	Log("UObjectBase::LowLevelRename found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_LowLevelRename));
	return ServerOffsets::UObjectBase_LowLevelRename;
}

uintptr_t Finder::FindUObjectBase_Register() {
	if (ServerOffsets::UObjectBase_Register)
		return ServerOffsets::UObjectBase_Register;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 49 8B F0 48 8B EA 48 8B F9 E8 ? ? ? ? B9").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_Register = Addr - ImageBase;
	}

	Log("UObjectBase::Register found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_Register));
	return ServerOffsets::UObjectBase_Register;
}

uintptr_t Finder::FindUObjectBase_Destructor() {
	if (ServerOffsets::UObjectBase_Destructor)
		return ServerOffsets::UObjectBase_Destructor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B F9 48 89 01 80 3D").Get();

	if (Addr) {
		ServerOffsets::UObjectBase_Destructor = Addr - ImageBase;
	}

	Log("UObjectBase::Destructor found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBase_Destructor));
	return ServerOffsets::UObjectBase_Destructor;
}

uintptr_t Finder::FindUObjectBaseUtility_AddToCluster() {
	if (ServerOffsets::UObjectBaseUtility_AddToCluster)
		return ServerOffsets::UObjectBaseUtility_AddToCluster;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 55 53 57 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F9").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_AddToCluster = Addr - ImageBase;
	}

	Log("UObjectBaseUtility_AddToCluster found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_AddToCluster));
	return ServerOffsets::UObjectBaseUtility_AddToCluster;
}

uintptr_t Finder::FindUObjectBaseUtility_CanBeInCluster() {
	if (ServerOffsets::UObjectBaseUtility_CanBeInCluster)
		return ServerOffsets::UObjectBaseUtility_CanBeInCluster;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 49 ? 48 85 C9 74 ? 48 8B 01 48 FF 60 ? B0").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_CanBeInCluster = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::CanBeInCluster found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_CanBeInCluster));
	return ServerOffsets::UObjectBaseUtility_CanBeInCluster;
}

uintptr_t Finder::FindUObjectBaseUtility_GetFullName() {
	if (ServerOffsets::UObjectBaseUtility_GetFullName)
		return ServerOffsets::UObjectBaseUtility_GetFullName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 48 8B DA 45 33 F6").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetFullName = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetFullName found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetFullName));
	return ServerOffsets::UObjectBaseUtility_GetFullName;
}

uintptr_t Finder::FindUObjectBaseUtility_GetFullName_1() {
	if (ServerOffsets::UObjectBaseUtility_GetFullName_1)
		return ServerOffsets::UObjectBaseUtility_GetFullName_1;
	static uintptr_t Addr = 0;
	
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 41 56 48 83 EC ? 45 33 F6").Get();
	
	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetFullName_1 = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetFullName_1 found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetFullName_1));
	return ServerOffsets::UObjectBaseUtility_GetFullName_1;
}

uintptr_t Finder::FindUObjectBaseUtility_GetInterfaceAddress() {
	if (ServerOffsets::UObjectBaseUtility_GetInterfaceAddress)
		return ServerOffsets::UObjectBaseUtility_GetInterfaceAddress;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 FF 48 8B DA 48 8B F1 48 85 D2 0F 84").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetInterfaceAddress = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetInterfaceAddress found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetInterfaceAddress));
	return ServerOffsets::UObjectBaseUtility_GetInterfaceAddress;
}

uintptr_t Finder::FindUObjectBaseUtility_GetLinker() {
	if (ServerOffsets::UObjectBaseUtility_GetLinker)
		return ServerOffsets::UObjectBaseUtility_GetLinker;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 59 ? 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 8B C3 99 0F B7 D2 03 C2 44 8B C0 0F B7 C0 2B C2 41 C1 F8 ? 44 3B 05 ? ? ? ? 7D ? 48 8B 15 ? ? ? ? 4D 63 C0 4D 03 C0 4E 8B 4C C2 ? 4D 85 C9 74 ? 48 98 48 8D 0D ? ? ? ? 48 03 C0 41 0F 10 04 C1 0F 11 44 24 ? FF 15 ? ? ? ? 48 8B 44 24").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetLinker = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetLinker found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetLinker));
	return ServerOffsets::UObjectBaseUtility_GetLinker;
}

uintptr_t Finder::FindUObjectBaseUtility_GetLinkerIndex() {
	if (ServerOffsets::UObjectBaseUtility_GetLinkerIndex)
		return ServerOffsets::UObjectBaseUtility_GetLinkerIndex;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 59 ? 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 8B C3 99 0F B7 D2 03 C2 44 8B C0 0F B7 C0 2B C2 41 C1 F8 ? 44 3B 05 ? ? ? ? 7D ? 48 8B 15 ? ? ? ? 4D 63 C0 4D 03 C0 4E 8B 4C C2 ? 4D 85 C9 74 ? 48 98 48 8D 0D ? ? ? ? 48 03 C0 41 0F 10 04 C1 0F 11 44 24 ? FF 15 ? ? ? ? 0F 10 44 24").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetLinkerIndex = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetLinkerIndex found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetLinkerIndex));
	return ServerOffsets::UObjectBaseUtility_GetLinkerIndex;
}

uintptr_t Finder::FindUObjectBaseUtility_GetNativeInterfaceAddress() {
	if (ServerOffsets::UObjectBaseUtility_GetNativeInterfaceAddress)
		return ServerOffsets::UObjectBaseUtility_GetNativeInterfaceAddress;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 4C 8B 49 ? 4C 8B DA").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetNativeInterfaceAddress = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetNativeInterfaceAddress found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetNativeInterfaceAddress));
	return ServerOffsets::UObjectBaseUtility_GetNativeInterfaceAddress;
}

uintptr_t Finder::FindUObjectBaseUtility_GetOutermost() {
	if (ServerOffsets::UObjectBaseUtility_GetOutermost)
		return ServerOffsets::UObjectBaseUtility_GetOutermost;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 41 ? 48 85 C0 74 ? 0F 1F 80 ? ? ? ? 48 8B C8").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetOutermost = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetOutermost found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetOutermost));
	return ServerOffsets::UObjectBaseUtility_GetOutermost;
}

uintptr_t Finder::FindUObjectBaseUtility_GetPathName() {
	if (ServerOffsets::UObjectBaseUtility_GetPathName)
		return ServerOffsets::UObjectBaseUtility_GetPathName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC ? 49 8B F8").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetPathName = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetPathName found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetPathName));
	return ServerOffsets::UObjectBaseUtility_GetPathName;
}

uintptr_t Finder::FindUObjectBaseUtility_GetTypedOuter() {
	if (ServerOffsets::UObjectBaseUtility_GetTypedOuter)
		return ServerOffsets::UObjectBaseUtility_GetTypedOuter;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 49 ? 4C 8B D2 33 C0").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_GetTypedOuter = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::GetTypedOuter found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_GetTypedOuter));
	return ServerOffsets::UObjectBaseUtility_GetTypedOuter;
}

uintptr_t Finder::FindUObjectBaseUtility_IsDefaultSubobject() {
	if (ServerOffsets::UObjectBaseUtility_IsDefaultSubobject)
		return ServerOffsets::UObjectBaseUtility_IsDefaultSubobject;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 41 ? 48 85 C0 74 ? 8B 40").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_IsDefaultSubobject = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::IsDefaultSubobject found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_IsDefaultSubobject));
	return ServerOffsets::UObjectBaseUtility_IsDefaultSubobject;
}

uintptr_t Finder::FindUObjectBaseUtility_IsIn() {
	if (ServerOffsets::UObjectBaseUtility_IsIn)
		return ServerOffsets::UObjectBaseUtility_IsIn;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 41 ? 48 85 C0 74 ? 0F 1F 80 ? ? ? ? 48 3B C2").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_IsIn = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::IsIn found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_IsIn));
	return ServerOffsets::UObjectBaseUtility_IsIn;
}

uintptr_t Finder::FindUObjectBaseUtility_IsInA() {
	if (ServerOffsets::UObjectBaseUtility_IsInA)
		return ServerOffsets::UObjectBaseUtility_IsInA;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 C9 74 ? 4C 63 42").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_IsInA = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::IsInA found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_IsInA));
	return ServerOffsets::UObjectBaseUtility_IsInA;
}

uintptr_t Finder::FindUObjectBaseUtility_IsTemplate() {
	if (ServerOffsets::UObjectBaseUtility_IsTemplate)
		return ServerOffsets::UObjectBaseUtility_IsTemplate;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 C9 74 ? 85 51").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_IsTemplate = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::IsTemplate found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_IsTemplate));
	return ServerOffsets::UObjectBaseUtility_IsTemplate;
}

uintptr_t Finder::FindUObjectBaseUtility_MarkPackageDirty() {
	if (ServerOffsets::UObjectBaseUtility_MarkPackageDirty)
		return ServerOffsets::UObjectBaseUtility_MarkPackageDirty;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 56 48 83 EC ? 8B 41 ? 48 8B F1 C1 E8 ? A8 ? 0F 85 ? ? ? ? 48 8B 41").Get();

	if (Addr) {
		ServerOffsets::UObjectBaseUtility_MarkPackageDirty = Addr - ImageBase;
	}

	Log("UObjectBaseUtility::MarkPackageDirty found at: 0x" + std::format("{:X}", ServerOffsets::UObjectBaseUtility_MarkPackageDirty));
	return ServerOffsets::UObjectBaseUtility_MarkPackageDirty;
}

uintptr_t Finder::FindUObject_BeginDestroy() {
	if (ServerOffsets::UObject_BeginDestroy)
		return ServerOffsets::UObject_BeginDestroy;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 41 ? 48 8B D9 C1 E8 ? A8 ? 75 ? 48 8B 41").Get();

	if (Addr) {
		ServerOffsets::UObject_BeginDestroy = Addr - ImageBase;
	}

	Log("UObject::BeginDestroy found at: 0x" + std::format("{:X}", ServerOffsets::UObject_BeginDestroy));
	return ServerOffsets::UObject_BeginDestroy;
}

uintptr_t Finder::FindUObject_FinishDestroy() {
	if (ServerOffsets::UObject_FinishDestroy)
		return ServerOffsets::UObject_FinishDestroy;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? F6 41 ? ? 48 8B D9 75 ? 48 8B 41").Get();

	if (Addr) {
		ServerOffsets::UObject_FinishDestroy = Addr - ImageBase;
	}

	Log("UObject::FinishDestroy found at: 0x" + std::format("{:X}", ServerOffsets::UObject_FinishDestroy));
	return ServerOffsets::UObject_FinishDestroy;
}

uintptr_t Finder::FindUObject_DefaultConstructor() {
	if (ServerOffsets::UObject_DefaultConstructor)
		return ServerOffsets::UObject_DefaultConstructor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B F9 48 89 01 E8 ? ? ? ? 48 8D 44 24").Get();

	if (Addr) {
		ServerOffsets::UObject_DefaultConstructor = Addr - ImageBase;
	}

	Log("UObject::DefaultConstructor found at: 0x" + std::format("{:X}", ServerOffsets::UObject_DefaultConstructor));
	return ServerOffsets::UObject_DefaultConstructor;
}

uintptr_t Finder::FindUObject_GetDefaultSubobjects() {
	if (ServerOffsets::UObject_GetDefaultSubobjects)
		return ServerOffsets::UObject_GetDefaultSubobjects;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B 42 ? 48 8B DA C7 42 ? ? ? ? ? 48 8B F9 85 C0 79 ? 33 D2 48 8B CB E8 ? ? ? ? 48 8D 44 24").Get();

	if (Addr) {
		ServerOffsets::UObject_GetDefaultSubobjects = Addr - ImageBase;
	}

	Log("UObject::GetDefaultSubobjects found at: 0x" + std::format("{:X}", ServerOffsets::UObject_GetDefaultSubobjects));
	return ServerOffsets::UObject_GetDefaultSubobjects;
}

uintptr_t Finder::FindUObject_GetDefaultSubobjectByName() {
	if (ServerOffsets::UObject_GetDefaultSubobjectByName)
		return ServerOffsets::UObject_GetDefaultSubobjectByName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 54 24 ? 57 48 83 EC ? 33 FF 48 8B DA 40 38 3D").Get();

	if (Addr) {
		ServerOffsets::UObject_GetDefaultSubobjectByName = Addr - ImageBase;
	}

	Log("UObject::GetDefaultSubobjectByName found at: 0x" + std::format("{:X}", ServerOffsets::UObject_GetDefaultSubobjectByName));
	return ServerOffsets::UObject_GetDefaultSubobjectByName;
}

uintptr_t Finder::FindUObject_PostInitProperties() {
	if (ServerOffsets::UObject_PostInitProperties)
		return ServerOffsets::UObject_PostInitProperties;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B D1 48 C7 44 24").Get();

	if (Addr) {
		ServerOffsets::UObject_PostInitProperties = Addr - ImageBase;
	}

	Log("UObject::PostInitProperties found at: 0x" + std::format("{:X}", ServerOffsets::UObject_PostInitProperties));
	return ServerOffsets::UObject_PostInitProperties;
}

uintptr_t Finder::FindUObject_PostLoad() {
	if (ServerOffsets::UObject_PostLoad)
		return ServerOffsets::UObject_PostLoad;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 41 ? 33 FF 48 8B D9 8B 90").Get();

	if (Addr) {
		ServerOffsets::UObject_PostLoad = Addr - ImageBase;
	}

	Log("UObject::PostLoad found at: 0x" + std::format("{:X}", ServerOffsets::UObject_PostLoad));
	return ServerOffsets::UObject_PostLoad;
}

uintptr_t Finder::FindUObject_FindFunctionChecked() {
	if (ServerOffsets::UObject_FindFunctionChecked)
		return ServerOffsets::UObject_FindFunctionChecked;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 54 24 ? 57 48 83 EC ? 48 8B F9 41 B8").Get();

	if (Addr) {
		ServerOffsets::UObject_FindFunctionChecked = Addr - ImageBase;
	}

	Log("UObject::FindFunctionChecked found at: 0x" + std::format("{:X}", ServerOffsets::UObject_FindFunctionChecked));
	return ServerOffsets::UObject_FindFunctionChecked;
}

uintptr_t Finder::FindUField_GetOwnerClass() {
	if (ServerOffsets::UField_GetOwnerClass)
		return ServerOffsets::UField_GetOwnerClass;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 33 C0 48 8B D9 48 85 C9").Get();

	if (Addr) {
		ServerOffsets::UField_GetOwnerClass = Addr - ImageBase;
	}

	Log("UField::GetOwnerClass found at: 0x" + std::format("{:X}", ServerOffsets::UField_GetOwnerClass));
	return ServerOffsets::UField_GetOwnerClass;
}

uintptr_t Finder::FindUField_GetOwnerStruct() {
	if (ServerOffsets::UField_GetOwnerStruct)
		return ServerOffsets::UField_GetOwnerStruct;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 85 C9 74 ? E8 ? ? ? ? 48 8B 53 ? 4C 8D 40 ? 48 63 40 ? 3B 42 ? 7F ? 48 8B C8 48 8B 42 ? 4C 39 04 C8 74 ? 48 8B 5B").Get();

	if (Addr) {
		ServerOffsets::UField_GetOwnerStruct = Addr - ImageBase;
	}

	Log("UField::GetOwnerStruct found at: 0x" + std::format("{:X}", ServerOffsets::UField_GetOwnerStruct));
	return ServerOffsets::UField_GetOwnerStruct;
}

uintptr_t Finder::FindUStruct_FindPropertyByName() {
	if (ServerOffsets::UStruct_FindPropertyByName)
		return ServerOffsets::UStruct_FindPropertyByName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 49 ? 48 85 C9 74 ? 4C 8B CA").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 8B 41 ? 48 85 C0 74 ? 0F 1F 80 ? ? ? ? 48 39 50").Get();
	}

	if (Addr) {
		ServerOffsets::UStruct_FindPropertyByName = Addr - ImageBase;
	}

	Log("UStruct_FindPropertyByName found at: 0x" + std::format("{:X}", ServerOffsets::UStruct_FindPropertyByName));
	return ServerOffsets::UStruct_FindPropertyByName;
}

uintptr_t Finder::FindUClass_FindFunctionByName() {
	if (ServerOffsets::UClass_FindFunctionByName)
		return ServerOffsets::UClass_FindFunctionByName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 55 56 57 41 55 48 83 EC ? 41 8B F0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 53 56 48 83 EC ? 8B 81").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 53 57 48 83 EC ? 8B 81 ? ? ? ? 41 8B D8").Get();
	}

	if (Addr) {
		ServerOffsets::UClass_FindFunctionByName = Addr - ImageBase;
	}

	Log("UClass_FindFunctionByName found at: 0x" + std::format("{:X}", ServerOffsets::UClass_FindFunctionByName));
	return ServerOffsets::UClass_FindFunctionByName;
}

uintptr_t Finder::FindFName_GetPlainNameString1() {
	if (ServerOffsets::FName_GetPlainNameString1)
		return ServerOffsets::FName_GetPlainNameString1;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B 01 48 8B FA 8B D8 0F B7 C8 C1 EB ? 80 3D ? ? ? ? ? 89 5C 24 ? 89 4C 24 ? 74 ? 4C 8D 05").Get();

	if (Addr) {
		ServerOffsets::FName_GetPlainNameString1 = Addr - ImageBase;
	}

	Log("FName_GetPlainNameString1 found at: 0x" + std::format("{:X}", ServerOffsets::FName_GetPlainNameString1));
	return ServerOffsets::FName_GetPlainNameString1;
}

uintptr_t Finder::FindFName_GetPlainNameString2() {
	if (ServerOffsets::FName_GetPlainNameString2)
		return ServerOffsets::FName_GetPlainNameString2;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B 01 48 8B FA 8B D8 0F B7 C8 C1 EB ? 80 3D ? ? ? ? ? 89 5C 24 ? 89 4C 24 ? 74 ? 48 8D 15 ? ? ? ? EB ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B D0 C6 05 ? ? ? ? ? 48 8B 44 24 ? 8B CB 48 C1 E8 ? 8D 1C 00 48 03 5C CA ? 48 8B D7").Get();

	if (Addr) {
		ServerOffsets::FName_GetPlainNameString2 = Addr - ImageBase;
	}

	Log("FName_GetPlainNameString2 found at: 0x" + std::format("{:X}", ServerOffsets::FName_GetPlainNameString2));
	return ServerOffsets::FName_GetPlainNameString2;
}

uintptr_t Finder::FindUEngine_BroadcastNetworkFailure() {
	if (ServerOffsets::UEngine_BroadcastNetworkFailure)
		return ServerOffsets::UEngine_BroadcastNetworkFailure;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 56 48 81 EC ? ? ? ? 49 89 5B ? 33 F6").Get();

	if (Addr) {
		ServerOffsets::UEngine_BroadcastNetworkFailure = Addr - ImageBase;
	}

	Log("UEngine::BroadcastNetworkFailure found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_BroadcastNetworkFailure));
	return ServerOffsets::UEngine_BroadcastNetworkFailure;
}

uintptr_t Finder::FindUEngine_BroadcastNetworkLagStateChanged() {
	if (ServerOffsets::UEngine_BroadcastNetworkLagStateChanged)
		return ServerOffsets::UEngine_BroadcastNetworkLagStateChanged;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 56 48 83 EC ? 48 89 6C 24 ? 48 8D B1 ? ? ? ? FF 46 ? 40 32 ED 8B 46 ? 8B 4E ? 83 E8 ? 48 89 7C 24 ? 4C 89 64 24 ? 4C 8B E2 4C 89 74 24 ? 45 8B F1").Get();

	if (Addr) {
		ServerOffsets::UEngine_BroadcastNetworkLagStateChanged = Addr - ImageBase;
	}

	Log("UEngine::BroadcastNetworkLagStateChanged found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_BroadcastNetworkLagStateChanged));
	return ServerOffsets::UEngine_BroadcastNetworkLagStateChanged;
}

uintptr_t Finder::FindUEngine_CancelAllPending() {
	if (ServerOffsets::UEngine_CancelAllPending)
		return ServerOffsets::UEngine_CancelAllPending;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 33 FF 48 8B D9 39 B9 ? ? ? ? 7E ? 48 89 74 24 ? 8B F7 66 90 48 8B 93").Get();

	if (Addr) {
		ServerOffsets::UEngine_CancelAllPending = Addr - ImageBase;
	}

	Log("UEngine::CancelAllPending found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CancelAllPending));
	return ServerOffsets::UEngine_CancelAllPending;
}

uintptr_t Finder::FindUEngine_CreateGameUserSettings() {
	if (ServerOffsets::UEngine_CreateGameUserSettings)
		return ServerOffsets::UEngine_CreateGameUserSettings;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B E9 33 C9").Get();

	if (Addr) {
		ServerOffsets::UEngine_CreateGameUserSettings = Addr - ImageBase;
	}

	Log("UEngine::CreateGameUserSettings found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CreateGameUserSettings));
	return ServerOffsets::UEngine_CreateGameUserSettings;
}

uintptr_t Finder::FindUEngine_GetFirstGamePlayer() {
	if (ServerOffsets::UEngine_GetFirstGamePlayer)
		return ServerOffsets::UEngine_GetFirstGamePlayer;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? E8 ? ? ? ? 83 78 ? ? 74 ? 48 8B 00").Get();

	if (Addr) {
		ServerOffsets::UEngine_GetFirstGamePlayer = Addr - ImageBase;
	}

	Log("UEngine::GetFirstGamePlayer found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetFirstGamePlayer));
	return ServerOffsets::UEngine_GetFirstGamePlayer;
}

uintptr_t Finder::FindUEngine_GetNetMode() {
	if (ServerOffsets::UEngine_GetNetMode)
		return ServerOffsets::UEngine_GetNetMode;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 74 ? 48 8B CA E9 ? ? ? ? 33 C0").Get();

	if (Addr) {
		ServerOffsets::UEngine_GetNetMode = Addr - ImageBase;
	}

	Log("UEngine::GetNetMode found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetNetMode));
	return ServerOffsets::UEngine_GetNetMode;
}

uintptr_t Finder::FindUEngine_GetMaxTickRate() {
	if (ServerOffsets::UEngine_GetMaxTickRate)
		return ServerOffsets::UEngine_GetMaxTickRate;
	static uintptr_t Addr = 0;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Engine_Version >= 5.0)
		{
			Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 55 41 56 48 83 EC ? 0F 29 70 ? 48 8B D9 0F 29").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC 50 0F 29 74 24 ? 48 8B D9 0F 29 7C 24 ? 0F 28 F9 44 0F 29").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 56 48 83 EC ? 0F 29 70 ? 48 8B D9").Get();
		}
		else if (Version::Engine_Version >= 4.27) {
			Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC 60 0F 29 74 24 ? 48 8B D9 0F 29 7C 24 ? 0F 28").Get();
		}
		else {
			auto sRef = Memcury::Scanner::FindStringRef(L"Hitching by request!").Get();

			if (!sRef)
				return 0;

			for (int i = 0; i < 400; i++)
			{
				if (*(uint8_t*)(sRef - i) == 0x40 && *(uint8_t*)(sRef - i + 1) == 0x53)
				{
					Addr = sRef - i;
					break;
				}
				else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C)
				{
					Addr = sRef - i;
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UEngine_GetMaxTickRate = Addr - ImageBase;
	}

	Log("UEngine::GetMaxTickRate found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetMaxTickRate));
	return ServerOffsets::UEngine_GetMaxTickRate;
}

uintptr_t Finder::FindUEngine_GetMaxTickRateVFT() {
	if (ServerOffsets::UEngine_GetMaxTickRateVFT)
		return ServerOffsets::UEngine_GetMaxTickRateVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.Engine"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindUEngine_GetMaxTickRate() + ImageBase))
		{
			ServerOffsets::UEngine_GetMaxTickRateVFT = i;
			break;
		}
	}

	Log("UEngine::GetMaxTickRateVFT found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetMaxTickRateVFT));
	return ServerOffsets::UEngine_GetMaxTickRateVFT;
}

uintptr_t Finder::FindUWorld_AddController() {
	if (ServerOffsets::UWorld_AddController)
		return ServerOffsets::UWorld_AddController;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 4C 8B F9 48 8B F2 48 8D 4C 24").Get();

	if (Addr) {
		ServerOffsets::UWorld_AddController = Addr - ImageBase;
	}

	Log("UWorld_AddController found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AddController));
	return ServerOffsets::UWorld_AddController;
}

uintptr_t Finder::FindUWorld_AddLevel() {
	if (ServerOffsets::UWorld_AddLevel)
		return ServerOffsets::UWorld_AddLevel;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 53 48 83 EC ? 48 8B D9 48 85 D2").Get();

	if (Addr) {
		ServerOffsets::UWorld_AddLevel = Addr - ImageBase;
	}

	Log("UWorld_AddLevel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AddLevel));
	return ServerOffsets::UWorld_AddLevel;
}

uintptr_t Finder::FindUWorld_AddNetworkActor() {
	if (ServerOffsets::UWorld_AddNetworkActor)
		return ServerOffsets::UWorld_AddNetworkActor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 56 48 83 EC ? F6 42 ? ? 48 8B F2").Get();

	if (Addr) {
		ServerOffsets::UWorld_AddNetworkActor = Addr - ImageBase;
	}

	Log("UWorld_AddNetworkActor found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AddNetworkActor));
	return ServerOffsets::UWorld_AddNetworkActor;
}

uintptr_t Finder::FindUWorld_AddStreamingLevel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UWorld_AddStreamingLevel)
		return ServerOffsets::UWorld_AddStreamingLevel;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 56 48 83 EC ? 48 8B F1").Get();

	if (Addr) {
		ServerOffsets::UWorld_AddStreamingLevel = Addr - ImageBase;
	}

	Log("UWorld_AddStreamingLevel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AddStreamingLevel));
	return ServerOffsets::UWorld_AddStreamingLevel;
}

uintptr_t Finder::FindUWorld_AddToWorld() {
	if (ServerOffsets::UWorld_AddToWorld)
		return ServerOffsets::UWorld_AddToWorld;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 50 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 58 ? 33 DB").Get();

	if (Addr) {
		ServerOffsets::UWorld_AddToWorld = Addr - ImageBase;
	}

	Log("UWorld_AddToWorld found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AddToWorld));
	return ServerOffsets::UWorld_AddToWorld;
}

uintptr_t Finder::FindUWorld_AllowLevelLoadRequests() {
	if (ServerOffsets::UWorld_AllowLevelLoadRequests)
		return ServerOffsets::UWorld_AllowLevelLoadRequests;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 0F B6 81 ? ? ? ? 48 8B D9 3C").Get();

	if (Addr) {
		ServerOffsets::UWorld_AllowLevelLoadRequests = Addr - ImageBase;
	}

	Log("UWorld_AllowLevelLoadRequests found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AllowLevelLoadRequests));
	return ServerOffsets::UWorld_AllowLevelLoadRequests;
}

uintptr_t Finder::FindUWorld_AreActorsInitialized() {
	if (ServerOffsets::UWorld_AreActorsInitialized)
		return ServerOffsets::UWorld_AreActorsInitialized;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("80 B9 ? ? ? ? ? 7D ? 48 8B 41").Get();

	if (Addr) {
		ServerOffsets::UWorld_AreActorsInitialized = Addr - ImageBase;
	}

	Log("UWorld_AreActorsInitialized found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AreActorsInitialized));
	return ServerOffsets::UWorld_AreActorsInitialized;
}

uintptr_t Finder::FindUWorld_AreAlwaysLoadedLevelsLoaded() {
	if (ServerOffsets::UWorld_AreAlwaysLoadedLevelsLoaded)
		return ServerOffsets::UWorld_AreAlwaysLoadedLevelsLoaded;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B B9 ? ? ? ? 48 63 81 ? ? ? ? 48 8D 2C C7").Get();

	if (Addr) {
		ServerOffsets::UWorld_AreAlwaysLoadedLevelsLoaded = Addr - ImageBase;
	}

	Log("UWorld_AreAlwaysLoadedLevelsLoaded found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_AreAlwaysLoadedLevelsLoaded));
	return ServerOffsets::UWorld_AreAlwaysLoadedLevelsLoaded;
}

uintptr_t Finder::FindUWorld_BeginPlay() {
	if (ServerOffsets::UWorld_BeginPlay)
		return ServerOffsets::UWorld_BeginPlay;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 FF 90 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9").Get();

	if (Addr) {
		ServerOffsets::UWorld_BeginPlay = Addr - ImageBase;
	}

	Log("UWorld_BeginPlay found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_BeginPlay));
	return ServerOffsets::UWorld_BeginPlay;
}

uintptr_t Finder::FindUWorld_CommitMapChange() {
	if (ServerOffsets::UWorld_CommitMapChange)
		return ServerOffsets::UWorld_CommitMapChange;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 1D ? ? ? ? 48 8B F9 48 8B D1").Get();

	if (Addr) {
		ServerOffsets::UWorld_CommitMapChange = Addr - ImageBase;
	}

	Log("UWorld_CommitMapChange found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_CommitMapChange));
	return ServerOffsets::UWorld_CommitMapChange;
}

uintptr_t Finder::FindUWorld_ConditionallyCreateDefaultLevelCollections() {
	if (ServerOffsets::UWorld_ConditionallyCreateDefaultLevelCollections)
		return ServerOffsets::UWorld_ConditionallyCreateDefaultLevelCollections;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 49 89 5B ? 48 8B F9 48 8D 99").Get();

	if (Addr) {
		ServerOffsets::UWorld_ConditionallyCreateDefaultLevelCollections = Addr - ImageBase;
	}

	Log("UWorld_ConditionallyCreateDefaultLevelCollections found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_ConditionallyCreateDefaultLevelCollections));
	return ServerOffsets::UWorld_ConditionallyCreateDefaultLevelCollections;
}

uintptr_t Finder::FindUWorld_ContainsActor() {
	if (ServerOffsets::UWorld_ContainsActor)
		return ServerOffsets::UWorld_ContainsActor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 85 D2 74 ? 48 8B 02 48 8B CA FF 90 ? ? ? ? 48 3B C3").Get();

	if (Addr) {
		ServerOffsets::UWorld_ContainsActor = Addr - ImageBase;
	}

	Log("UWorld_ContainsActor found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_ContainsActor));
	return ServerOffsets::UWorld_ContainsActor;
}

uintptr_t Finder::FindUWorld_ContainsLevel() {
	if (ServerOffsets::UWorld_ContainsLevel)
		return ServerOffsets::UWorld_ContainsLevel;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B 89 ? ? ? ? 48 63 89 ? ? ? ? 49 8B C1 4D 8D 04 C9 4D 3B C8 74 ? 66 0F 1F 44 00 ? 48 39 10 74 ? 48 83 C0 ? 49 3B C0 75 ? B8 ? ? ? ? 3B C0").Get();

	if (Addr) {
		ServerOffsets::UWorld_ContainsLevel = Addr - ImageBase;
	}

	Log("UWorld_ContainsLevel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_ContainsLevel));
	return ServerOffsets::UWorld_ContainsLevel;
}

uintptr_t Finder::FindUWorld_CreateAISystem() {
	if (ServerOffsets::UWorld_CreateAISystem)
		return ServerOffsets::UWorld_CreateAISystem;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 83 B9 ? ? ? ? ? 48 8B F9 0F 85 ? ? ? ? E8 ? ? ? ? 8B C8").Get();

	if (Addr) {
		ServerOffsets::UWorld_CreateAISystem = Addr - ImageBase;
	}

	Log("UWorld_CreateAISystem found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_CreateAISystem));
	return ServerOffsets::UWorld_CreateAISystem;
}

uintptr_t Finder::FindUWorld_GetActiveLevelCollection() {
	if (ServerOffsets::UWorld_GetActiveLevelCollection)
		return ServerOffsets::UWorld_GetActiveLevelCollection;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 63 81 ? ? ? ? 85 C0 78 ? 3B 81 ? ? ? ? 7D ? 48 6B C0 ? 48 03 81 ? ? ? ? C3").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetActiveLevelCollection = Addr - ImageBase;
	}

	Log("UWorld_GetActiveLevelCollection found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetActiveLevelCollection));
	return ServerOffsets::UWorld_GetActiveLevelCollection;
}

uintptr_t Finder::FindUWorld_GetActorCount() {
	if (ServerOffsets::UWorld_GetActorCount)
		return ServerOffsets::UWorld_GetActorCount;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 4C 63 91 ? ? ? ? 45 33 C0").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetActorCount = Addr - ImageBase;
	}

	Log("UWorld_GetActorCount found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetActorCount));
	return ServerOffsets::UWorld_GetActorCount;
}

uintptr_t Finder::FindUWorld_GetAddressURL() {
	if (ServerOffsets::UWorld_GetAddressURL)
		return ServerOffsets::UWorld_GetAddressURL;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B DA 48 81 C1 ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 83 78").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetAddressURL = Addr - ImageBase;
	}

	Log("UWorld_GetAddressURL found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetAddressURL));
	return ServerOffsets::UWorld_GetAddressURL;
}

uintptr_t Finder::FindUWorld_GetDefaultGravityZ() {
	if (ServerOffsets::UWorld_GetDefaultGravityZ)
		return ServerOffsets::UWorld_GetDefaultGravityZ;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? E8 ? ? ? ? 48 8B D8 48 8B 88").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetDefaultGravityZ = Addr - ImageBase;
	}

	Log("UWorld_GetDefaultGravityZ found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetDefaultGravityZ));
	return ServerOffsets::UWorld_GetDefaultGravityZ;
}

uintptr_t Finder::FindUWorld_GetFirstLocalPlayerFromController() {
	if (ServerOffsets::UWorld_GetFirstLocalPlayerFromController)
		return ServerOffsets::UWorld_GetFirstLocalPlayerFromController;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? 48 8B F1 48 89 7C 24 ? 33 DB 85 DB 78 ? 3B 9E ? ? ? ? 7D ? B0").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetFirstLocalPlayerFromController = Addr - ImageBase;
	}

	Log("UWorld_GetFirstLocalPlayerFromController found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetFirstLocalPlayerFromController));
	return ServerOffsets::UWorld_GetFirstLocalPlayerFromController;
}

uintptr_t Finder::FindUWorld_GetFirstPlayerController() {
	if (ServerOffsets::UWorld_GetFirstPlayerController)
		return ServerOffsets::UWorld_GetFirstPlayerController;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("83 B9 ? ? ? ? ? 7E ? 48 8B 89 ? ? ? ? E9").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetFirstPlayerController = Addr - ImageBase;
	}

	Log("UWorld_GetFirstPlayerController found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetFirstPlayerController));
	return ServerOffsets::UWorld_GetFirstPlayerController;
}

uintptr_t Finder::FindUWorld_GetGameViewport() {
	if (ServerOffsets::UWorld_GetGameViewport)
		return ServerOffsets::UWorld_GetGameViewport;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B D1 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetGameViewport = Addr - ImageBase;
	}

	Log("UWorld_GetGameViewport found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetGameViewport));
	return ServerOffsets::UWorld_GetGameViewport;
}

uintptr_t Finder::FindUWorld_GetGravityZ() {
	if (ServerOffsets::UWorld_GetGravityZ)
		return ServerOffsets::UWorld_GetGravityZ;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 49 ? 48 85 C9 74 ? B2").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetGravityZ = Addr - ImageBase;
	}

	Log("UWorld_GetGravityZ found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetGravityZ));
	return ServerOffsets::UWorld_GetGravityZ;
}

uintptr_t Finder::FindUWorld_GetMapName() {
	if (ServerOffsets::UWorld_GetMapName)
		return ServerOffsets::UWorld_GetMapName;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 89 6C 24 ? 48 8B DA 48 89 7C 24 ? 4C 89 74 24").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetMapName = Addr - ImageBase;
	}

	Log("UWorld_GetMapName found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetMapName));
	return ServerOffsets::UWorld_GetMapName;
}

uintptr_t Finder::FindUWorld_GetTimerManager() {
	if (ServerOffsets::UWorld_GetTimerManager)
		return ServerOffsets::UWorld_GetTimerManager;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 81 ? ? ? ? 48 85 C0 74 ? 48 8B 80 ? ? ? ? C3 48 8B 81").Get();

	if (Addr) {
		ServerOffsets::UWorld_GetTimerManager = Addr - ImageBase;
	}

	Log("UWorld_GetTimerManager found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetTimerManager));
	return ServerOffsets::UWorld_GetTimerManager;
}

uintptr_t Finder::FindUWorld_GetWorldSettings() {
	if (ServerOffsets::UWorld_GetWorldSettings)
		return ServerOffsets::UWorld_GetWorldSettings;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 41 0F B6 F0 48 8B 49").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 57 48 83 EC ? 48 8B F9 0F B6 F2 48 8B 49 ? 48 85 C9").Get();
	}

	if (Addr) {
		ServerOffsets::UWorld_GetWorldSettings = Addr - ImageBase;
	}

	Log("UWorld_GetWorldSettings found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_GetWorldSettings));
	return ServerOffsets::UWorld_GetWorldSettings;
}

uintptr_t Finder::FindUWorld_K2_GetWorldSettings() {
	if (ServerOffsets::UWorld_K2_GetWorldSettings)
		return ServerOffsets::UWorld_K2_GetWorldSettings;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B 49 ? 48 85 C9 74 ? 33 D2 E9").Get();

	if (Addr) {
		ServerOffsets::UWorld_K2_GetWorldSettings = Addr - ImageBase;
	}

	Log("UWorld_K2_GetWorldSettings found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_K2_GetWorldSettings));
	return ServerOffsets::UWorld_K2_GetWorldSettings;
}

uintptr_t Finder::FindUWorld_HasBegunPlay() {
	if (ServerOffsets::UWorld_HasBegunPlay)
		return ServerOffsets::UWorld_HasBegunPlay;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("F6 81 ? ? ? ? ? 74 ? 48 8B 41 ? 48 85 C0").Get();

	if (Addr) {
		ServerOffsets::UWorld_HasBegunPlay = Addr - ImageBase;
	}

	Log("UWorld_HasBegunPlay found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_HasBegunPlay));
	return ServerOffsets::UWorld_HasBegunPlay;
}

uintptr_t Finder::FindUWorld_InternalGetNetMode() {
	if (ServerOffsets::UWorld_InternalGetNetMode)
		return ServerOffsets::UWorld_InternalGetNetMode;
	static uintptr_t Addr = 0;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (floor(Version::Fortnite_Version) == 18)
			Addr = Memcury::Scanner::FindPattern("48 83 EC 28 48 83 79 ? ? 75 20 48 8B 91 ? ? ? ? 48 85 D2 74 1E 48 8B 02 48 8B CA FF 90").Get();
		else
		{
			auto sRef = Memcury::Scanner::FindStringRef(L"PREPHYSBONES").Get();

			for (int i = 0; i < 1000; i++)
			{
				auto Ptr = (uint8_t*)(sRef - i);

				if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}

			for (int i = 0; i < 400; i++)
			{
				auto Ptr = (uint8_t*)(Addr + i);

				if (*Ptr == 0xe8 && *(Ptr - 1) != 0x8b && *(Ptr - 1) != 0xe7 && *(Ptr - 1) != 0x24)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}

			Addr = Memcury::Scanner(Addr).RelativeOffset(1).Get();
		}
	}

	if (Addr) {
		ServerOffsets::UWorld_InternalGetNetMode = Addr - ImageBase;
	}

	Log("UWorld_InternalGetNetMode found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_InternalGetNetMode));
	return ServerOffsets::UWorld_InternalGetNetMode;
}

uintptr_t Finder::FindUWorld_IsGameWorld() {
	if (ServerOffsets::UWorld_IsGameWorld)
		return ServerOffsets::UWorld_IsGameWorld;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("0F B6 81 ? ? ? ? 3C ? 77 ? B9").Get();

	if (Addr) {
		ServerOffsets::UWorld_IsGameWorld = Addr - ImageBase;
	}

	Log("UWorld_IsGameWorld found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_IsGameWorld));
	return ServerOffsets::UWorld_IsGameWorld;
}

uintptr_t Finder::FindUWorld_IsInSeamlessTravel() {
	if (ServerOffsets::UWorld_IsInSeamlessTravel)
		return ServerOffsets::UWorld_IsInSeamlessTravel;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B D1 48 8B 0D ? ? ? ? E8 ? ? ? ? 0F B6 80").Get();

	if (Addr) {
		ServerOffsets::UWorld_IsInSeamlessTravel = Addr - ImageBase;
	}

	Log("UWorld_IsInSeamlessTravel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_IsInSeamlessTravel));
	return ServerOffsets::UWorld_IsInSeamlessTravel;
}

uintptr_t Finder::FindUWorld_IsServer() {
	if (ServerOffsets::UWorld_IsServer)
		return ServerOffsets::UWorld_IsServer;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B C1 48 8B 49 ? 48 85 C9 74 ? 48 8B 01 48 FF A0").Get();

	if (Addr) {
		ServerOffsets::UWorld_IsServer = Addr - ImageBase;
	}

	Log("UWorld_IsServer found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_IsServer));
	return ServerOffsets::UWorld_IsServer;
}

uintptr_t Finder::FindUWorld_MarkObjectsPendingKill() {
	if (ServerOffsets::UWorld_MarkObjectsPendingKill)
		return ServerOffsets::UWorld_MarkObjectsPendingKill;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8D 44 24 ? C7 44 24").Get();

	if (Addr) {
		ServerOffsets::UWorld_MarkObjectsPendingKill = Addr - ImageBase;
	}

	Log("UWorld_MarkObjectsPendingKill found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_MarkObjectsPendingKill));
	return ServerOffsets::UWorld_MarkObjectsPendingKill;
}

uintptr_t Finder::FindUWorld_RemoveActor() {
	if (ServerOffsets::UWorld_RemoveActor)
		return ServerOffsets::UWorld_RemoveActor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? 48 8B D9 48 8B F2 48 8B CA E8 ? ? ? ? 48 8B 88").Get();

	if (Addr) {
		ServerOffsets::UWorld_RemoveActor = Addr - ImageBase;
	}

	Log("UWorld_RemoveActor found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_RemoveActor));
	return ServerOffsets::UWorld_RemoveActor;
}

uintptr_t Finder::FindUWorld_RemoveController() {
	if (ServerOffsets::UWorld_RemoveController)
		return ServerOffsets::UWorld_RemoveController;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 57 41 55 41 56 48 83 EC ? 4C 8B F1").Get();

	if (Addr) {
		ServerOffsets::UWorld_RemoveController = Addr - ImageBase;
	}

	Log("UWorld_RemoveController found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_RemoveController));
	return ServerOffsets::UWorld_RemoveController;
}

uintptr_t Finder::FindUWorld_RemoveNetworkActor() {
	if (ServerOffsets::UWorld_RemoveNetworkActor)
		return ServerOffsets::UWorld_RemoveNetworkActor;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 56 48 83 EC ? 48 8B F2 48 8B D1 48 85 F6").Get();

	if (Addr) {
		ServerOffsets::UWorld_RemoveNetworkActor = Addr - ImageBase;
	}

	Log("UWorld_RemoveNetworkActor found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_RemoveNetworkActor));
	return ServerOffsets::UWorld_RemoveNetworkActor;
}

uintptr_t Finder::FindUWorld_SeamlessTravel() {
	if (ServerOffsets::UWorld_SeamlessTravel)
		return ServerOffsets::UWorld_SeamlessTravel;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 83 7A ? ? 49 89 5B ? 49 89 73 ? 48 8B F2 49 89 7B ? 4D 89 73").Get();

	if (Addr) {
		ServerOffsets::UWorld_SeamlessTravel = Addr - ImageBase;
	}

	Log("UWorld_SeamlessTravel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_SeamlessTravel));
	return ServerOffsets::UWorld_SeamlessTravel;
}

uintptr_t Finder::FindUWorld_ServerTravel() {
	if (ServerOffsets::UWorld_ServerTravel)
		return ServerOffsets::UWorld_ServerTravel;
	static uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef("?Restart").Get();
	if (StringAddr) {
		int CallCount = 0;

		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0xE8)
			{
				CallCount++;

				if (CallCount == 2)
				{
					int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
					Addr = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UWorld_ServerTravel = Addr - ImageBase;
	}

	Log("UWorld_ServerTravel found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_ServerTravel));
	return ServerOffsets::UWorld_ServerTravel;
}

uintptr_t Finder::FindUWorld_SetGameMode() {
	if (ServerOffsets::UWorld_SetGameMode)
		return ServerOffsets::UWorld_SetGameMode;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 56 48 81 EC ? ? ? ? 48 8B F1 48 8B EA 48 8B 49").Get();

	if (Addr) {
		ServerOffsets::UWorld_SetGameMode = Addr - ImageBase;
	}

	Log("UWorld_SetGameMode found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_SetGameMode));
	return ServerOffsets::UWorld_SetGameMode;
}

uintptr_t Finder::FindUWorld_SetGameState() {
	if (ServerOffsets::UWorld_SetGameState)
		return ServerOffsets::UWorld_SetGameState;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? 48 8B DA 48 8B F1 48 3B 91").Get();

	if (Addr) {
		ServerOffsets::UWorld_SetGameState = Addr - ImageBase;
	}

	Log("UWorld_SetGameState found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_SetGameState));
	return ServerOffsets::UWorld_SetGameState;
}

uintptr_t Finder::FindUWorld_SetNavigationSystem() {
	if (ServerOffsets::UWorld_SetNavigationSystem)
		return ServerOffsets::UWorld_SetNavigationSystem;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 3B CA").Get();

	if (Addr) {
		ServerOffsets::UWorld_SetNavigationSystem = Addr - ImageBase;
	}

	Log("UWorld_SetNavigationSystem found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_SetNavigationSystem));
	return ServerOffsets::UWorld_SetNavigationSystem;
}

uintptr_t Finder::FindUWorld_WelcomePlayer() {
	if (ServerOffsets::UWorld_WelcomePlayer)
		return ServerOffsets::UWorld_WelcomePlayer;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 58 ? 48 8B 59 ? 48 89 78 ? 48 8B FA").Get();

	if (Addr) {
		ServerOffsets::UWorld_WelcomePlayer = Addr - ImageBase;
	}

	Log("UWorld_WelcomePlayer found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_WelcomePlayer));
	return ServerOffsets::UWorld_WelcomePlayer;
}

uintptr_t Finder::FindFGuid_ToString() {
	if (ServerOffsets::FGuid_ToString)
		return ServerOffsets::FGuid_ToString;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 41 FF C8").Get();

	if (Addr) {
		ServerOffsets::FGuid_ToString = Addr - ImageBase;
	}

	Log("FGuid_ToString found at: 0x" + std::format("{:X}", ServerOffsets::FGuid_ToString));
	return ServerOffsets::FGuid_ToString;
}

uintptr_t Finder::FindUGameInstance_StartGameInstance() {
	if (ServerOffsets::UGameInstance_StartGameInstance)
		return ServerOffsets::UGameInstance_StartGameInstance;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 41 ? 33 D2").Get();

	if (Addr) {
		ServerOffsets::UGameInstance_StartGameInstance = Addr - ImageBase;
	}

	Log("UGameInstance_StartGameInstance found at: 0x" + std::format("{:X}", ServerOffsets::UGameInstance_StartGameInstance));
	return ServerOffsets::UGameInstance_StartGameInstance;
}

uintptr_t Finder::FindUGameInstance_Shutdown() {
	if (ServerOffsets::UGameInstance_Shutdown)
		return ServerOffsets::UGameInstance_Shutdown;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B 4B").Get();

	if (Addr) {
		ServerOffsets::UGameInstance_Shutdown = Addr - ImageBase;
	}

	Log("UGameInstance_Shutdown found at: 0x" + std::format("{:X}", ServerOffsets::UGameInstance_Shutdown));
	return ServerOffsets::UGameInstance_Shutdown;
}

uintptr_t Finder::FindUGameInstance_GetFirstLocalPlayerController() {
	if (ServerOffsets::UGameInstance_GetFirstLocalPlayerController)
		return ServerOffsets::UGameInstance_GetFirstLocalPlayerController;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B C2 48 85 D2 75 ? 48 8B 51").Get();

	if (Addr) {
		ServerOffsets::UGameInstance_GetFirstLocalPlayerController = Addr - ImageBase;
	}

	Log("UGameInstance_GetFirstLocalPlayerController found at: 0x" + std::format("{:X}", ServerOffsets::UGameInstance_GetFirstLocalPlayerController));
	return ServerOffsets::UGameInstance_GetFirstLocalPlayerController;
}

uintptr_t Finder::FindUGameInstance_ClientTravelToSession() {
	if (ServerOffsets::UGameInstance_ClientTravelToSession)
		return ServerOffsets::UGameInstance_ClientTravelToSession;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 4C 89 44 24 ? 57 48 83 EC ? 48 8B 59").Get();

	if (Addr) {
		ServerOffsets::UGameInstance_ClientTravelToSession = Addr - ImageBase;
	}

	Log("UGameInstance_ClientTravelToSession found at: 0x" + std::format("{:X}", ServerOffsets::UGameInstance_ClientTravelToSession));
	return ServerOffsets::UGameInstance_ClientTravelToSession;
}

uintptr_t Finder::FindUGameInstance_CleanupGameViewport() {
	if (ServerOffsets::UGameInstance_CleanupGameViewport)
		return ServerOffsets::UGameInstance_CleanupGameViewport;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B 41 ? 48 8B F9 83 E8 ? 48 63 D8 78 ? 0F 1F 84 00 ? ? ? ? 48 8B 47 ? 48 8B 14 D8").Get();

	if (Addr) {
		ServerOffsets::UGameInstance_CleanupGameViewport = Addr - ImageBase;
	}

	Log("UGameInstance_CleanupGameViewport found at: 0x" + std::format("{:X}", ServerOffsets::UGameInstance_CleanupGameViewport));
	return ServerOffsets::UGameInstance_CleanupGameViewport;
}

uintptr_t Finder::FindAActor_ActorHasTag() {
	if (ServerOffsets::AActor_ActorHasTag)
		return ServerOffsets::AActor_ActorHasTag;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 54 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 8B CB 33 D2 E8 ? ? ? ? 44 8B 4C 24 ? 84 C0 0F 94 C2 45 85 C9 0F 95 C0 0A D0 74 ? 48 8B 87").Get();

	if (Addr) {
		ServerOffsets::AActor_ActorHasTag = Addr - ImageBase;
	}

	Log("AActor_ActorHasTag found at: 0x" + std::format("{:X}", ServerOffsets::AActor_ActorHasTag));
	return ServerOffsets::AActor_ActorHasTag;
}

uintptr_t Finder::FindAActor_CallPreReplication() {
	if (ServerOffsets::AActor_CallPreReplication)
		return ServerOffsets::AActor_CallPreReplication;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 41 56 48 83 EC ? 4C 8B F2").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 8B C4 55 57 41 57").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 8B C4 55 57 41 54").Get();
	}

	if (Addr) {
		ServerOffsets::AActor_CallPreReplication = Addr - ImageBase;
	}

	Log("AActor_CallPreReplication found at: 0x" + std::format("{:X}", ServerOffsets::AActor_CallPreReplication));
	return ServerOffsets::AActor_CallPreReplication;
}

uintptr_t Finder::FindAActor_CheckStillInWorld() {
	if (ServerOffsets::AActor_CheckStillInWorld)
		return ServerOffsets::AActor_CheckStillInWorld;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 41 ? 48 8B D9 3B 05 ? ? ? ? 7D ? 99 0F B7 D2 03 C2 44 8B C0 0F B7 C0 2B C2 48 98 41 C1 F8 ? 49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB ? 33 C0 8B 40").Get();
	if (Addr) {
		ServerOffsets::AActor_CheckStillInWorld = Addr - ImageBase;
	}

	Log("AActor_CheckStillInWorld found at: 0x" + std::format("{:X}", ServerOffsets::AActor_CheckStillInWorld));
	return ServerOffsets::AActor_CheckStillInWorld;
}

uintptr_t Finder::FindAActor_ClearCrossLevelReferences() {
	if (ServerOffsets::AActor_ClearCrossLevelReferences)
		return ServerOffsets::AActor_ClearCrossLevelReferences;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B F9 48 85 C0 74 ? 48 8B 88").Get();

	if (Addr) {
		ServerOffsets::AActor_ClearCrossLevelReferences = Addr - ImageBase;
	}
	Log("AActor_ClearCrossLevelReferences found at: 0x" + std::format("{:X}", ServerOffsets::AActor_ClearCrossLevelReferences));
	return ServerOffsets::AActor_ClearCrossLevelReferences;
}

uintptr_t Finder::FindAActor_CopyRemoteRoleFrom() {
	if (ServerOffsets::AActor_CopyRemoteRoleFrom)
		return ServerOffsets::AActor_CopyRemoteRoleFrom;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 0F B6 42 ? 48 8B D9 88 41").Get();

	if (Addr) {
		ServerOffsets::AActor_CopyRemoteRoleFrom = Addr - ImageBase;
	}
	Log("AActor_CopyRemoteRoleFrom found at: 0x" + std::format("{:X}", ServerOffsets::AActor_CopyRemoteRoleFrom));
	return ServerOffsets::AActor_CopyRemoteRoleFrom;
}

uintptr_t Finder::FindAActor_Destroy() {
	if (ServerOffsets::AActor_Destroy)
		return ServerOffsets::AActor_Destroy;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 FF 41 0F B6 F0").Get();

	if (Addr) {
		ServerOffsets::AActor_Destroy = Addr - ImageBase;
	}
	Log("AActor_Destroy found at: 0x" + std::format("{:X}", ServerOffsets::AActor_Destroy));
	return ServerOffsets::AActor_Destroy;
}

uintptr_t Finder::FindAActor_DisableInput() {
	if (ServerOffsets::AActor_DisableInput)
		return ServerOffsets::AActor_DisableInput;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 4C 8B C2 48 8B D9 48 8B 91").Get();

	if (Addr) {
		ServerOffsets::AActor_DisableInput = Addr - ImageBase;
	}
	Log("AActor_DisableInput found at: 0x" + std::format("{:X}", ServerOffsets::AActor_DisableInput));
	return ServerOffsets::AActor_DisableInput;
}

uintptr_t Finder::FindAActor_EnableInput() {
	if (ServerOffsets::AActor_EnableInput)
		return ServerOffsets::AActor_EnableInput;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 48 8B 91").Get();

	if (Addr) {
		ServerOffsets::AActor_EnableInput = Addr - ImageBase;
	}
	Log("AActor_EnableInput found at: 0x" + std::format("{:X}", ServerOffsets::AActor_EnableInput));
	return ServerOffsets::AActor_EnableInput;
}

uintptr_t Finder::FindAActor_ExchangeNetRoles() {
	if (ServerOffsets::AActor_ExchangeNetRoles)
		return ServerOffsets::AActor_ExchangeNetRoles;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("0F B6 41 ? 4C 8B C1 84 C0").Get();

	if (Addr) {
		ServerOffsets::AActor_ExchangeNetRoles = Addr - ImageBase;
	}
	Log("AActor_ExchangeNetRoles found at: 0x" + std::format("{:X}", ServerOffsets::AActor_ExchangeNetRoles));
	return ServerOffsets::AActor_ExchangeNetRoles;
}

uintptr_t Finder::FindAActor_FindOrAddNetworkObjectInfo() {
	if (ServerOffsets::AActor_FindOrAddNetworkObjectInfo)
		return ServerOffsets::AActor_FindOrAddNetworkObjectInfo;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 85 C0 74 ? 48 8B 48").Get();

	if (Addr) {
		ServerOffsets::AActor_FindOrAddNetworkObjectInfo = Addr - ImageBase;
	}
	Log("AActor_FindOrAddNetworkObjectInfo found at: 0x" + std::format("{:X}", ServerOffsets::AActor_FindOrAddNetworkObjectInfo));
	return ServerOffsets::AActor_FindOrAddNetworkObjectInfo;
}

uintptr_t Finder::FindAActor_InternalGetNetMode() {
	if (ServerOffsets::AActor_InternalGetNetMode)
		return ServerOffsets::AActor_InternalGetNetMode;
	static uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ClientPlayerJoined %s %s.").Get();
	if (StringAddr) {
		int CallCount = 0;

		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0xE8)
			{
				CallCount++;

				if (CallCount == 3)
				{
					int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
					Addr = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::AActor_InternalGetNetMode = Addr - ImageBase;
	}

	Log("AActor_InternalGetNetMode found at: 0x" + std::format("{:X}", ServerOffsets::AActor_InternalGetNetMode));
	return ServerOffsets::AActor_InternalGetNetMode;
}

uintptr_t Finder::FindAGameSession_ApproveLogin() {
	if (ServerOffsets::AGameSession_ApproveLogin)
		return ServerOffsets::AGameSession_ApproveLogin;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B 01 4D 8B E8").Get();

	if (Addr) {
		ServerOffsets::AGameSession_ApproveLogin = Addr - ImageBase;
	}
	Log("AGameSession_ApproveLogin found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_ApproveLogin));
	return ServerOffsets::AGameSession_ApproveLogin;
}

uintptr_t Finder::FindAGameSession_AtCapacity() {
	if (ServerOffsets::AGameSession_AtCapacity)
		return ServerOffsets::AGameSession_AtCapacity;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 0F B6 DA 48 8B F9 E8 ? ? ? ? 85 C0").Get();

	if (Addr) {
		ServerOffsets::AGameSession_AtCapacity = Addr - ImageBase;
	}
	Log("AGameSession_AtCapacity found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_AtCapacity));
	return ServerOffsets::AGameSession_AtCapacity;
}

uintptr_t Finder::FindAGameSession_GetSessionJoinability() {
	if (ServerOffsets::AGameSession_GetSessionJoinability)
		return ServerOffsets::AGameSession_GetSessionJoinability;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 49 8B E8 48 8B DA 48 8B F9 FF 90 ? ? ? ? 8B 97").Get();

	if (Addr) {
		ServerOffsets::AGameSession_GetSessionJoinability = Addr - ImageBase;
	}

	Log("AGameSession_GetSessionJoinability found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_GetSessionJoinability));
	return ServerOffsets::AGameSession_GetSessionJoinability;
}

uintptr_t Finder::FindAGameSession_KickPlayer() {
	if (ServerOffsets::AGameSession_KickPlayer)
		return ServerOffsets::AGameSession_KickPlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B DA 48 85 D2").Get();

	if (Addr) {
		ServerOffsets::AGameSession_KickPlayer = Addr - ImageBase;
	}
	Log("AGameSession_KickPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_KickPlayer));
	return ServerOffsets::AGameSession_KickPlayer;
}

uintptr_t Finder::FindAGameSession_KickPlayerVFT() {
	if (ServerOffsets::AGameSession_KickPlayerVFT)
		return ServerOffsets::AGameSession_KickPlayerVFT;
	static uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0xC9;
	}
	
	if (Addr) {
		ServerOffsets::AGameSession_KickPlayerVFT = Addr;
	}

	Log("AGameSession_KickPlayerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_KickPlayerVFT));
	return ServerOffsets::AGameSession_KickPlayerVFT;
}

uintptr_t Finder::FindAGameSession_ReturnToMainMenuHost() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameSession_ReturnToMainMenuHost)
		return ServerOffsets::AGameSession_ReturnToMainMenuHost;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 4C 8D 0D ? ? ? ? 48 8D 4C 24").Get();

	if (Addr) {
		ServerOffsets::AGameSession_ReturnToMainMenuHost = Addr - ImageBase;
	}
	Log("AGameSession_ReturnToMainMenuHost found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_ReturnToMainMenuHost));
	return ServerOffsets::AGameSession_ReturnToMainMenuHost;
}

uintptr_t Finder::FindAGameSession_UnregisterPlayer() {
	if (ServerOffsets::AGameSession_UnregisterPlayer)
		return ServerOffsets::AGameSession_UnregisterPlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 E8 ? ? ? ? 85 C0 74").Get();

	if (Addr) {
		ServerOffsets::AGameSession_UnregisterPlayer = Addr - ImageBase;
	}
	Log("AGameSession_UnregisterPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_UnregisterPlayer));
	return ServerOffsets::AGameSession_UnregisterPlayer;
}

uintptr_t Finder::FindAGameSession_UpdateSessionJoinability() {
	if (ServerOffsets::AGameSession_UpdateSessionJoinability)
		return ServerOffsets::AGameSession_UpdateSessionJoinability;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 41 56 41 57 48 83 EC ? 41 0F B6 E9 45 0F B6 F8").Get();

	if (Addr) {
		ServerOffsets::AGameSession_UpdateSessionJoinability = Addr - ImageBase;
	}
	Log("AGameSession_UpdateSessionJoinability found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_UpdateSessionJoinability));
	return ServerOffsets::AGameSession_UpdateSessionJoinability;
}

uintptr_t Finder::FindAGameSession_InitOptions() {
	if (ServerOffsets::AGameSession_InitOptions)
		return ServerOffsets::AGameSession_InitOptions;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B 01 4C 8B EA").Get();

	if (Addr) {
		ServerOffsets::AGameSession_InitOptions = Addr - ImageBase;
	}
	Log("AGameSession_InitOptions found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_InitOptions));
	return ServerOffsets::AGameSession_InitOptions;
}

uintptr_t Finder::FindAGameSession_RegisterPlayer() {
	if (ServerOffsets::AGameSession_RegisterPlayer)
		return ServerOffsets::AGameSession_RegisterPlayer;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 74 ? 48 89 5C 24 ? 57 48 83 EC ? 8B 05").Get();

	if (Addr) {
		ServerOffsets::AGameSession_RegisterPlayer = Addr - ImageBase;
	}

	Log("AGameSession_RegisterPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_RegisterPlayer));
	return ServerOffsets::AGameSession_RegisterPlayer;
}

uintptr_t Finder::FindAGameSession__NextPlayerID() {
	if (Finder::FindAGameSession_RegisterPlayer() == 0) {
		return 0;
	}
	if (ServerOffsets::AGameSession__NextPlayerID)
		return ServerOffsets::AGameSession__NextPlayerID;
	uintptr_t func = ImageBase + Finder::FindAGameSession_RegisterPlayer();

	const size_t scanSize = 0x300;

	uintptr_t readInstr = 0;
	uintptr_t writeInstr = 0;

	for (size_t i = 0; i + 6 <= scanSize; i++)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(func + i);

		if (p[0] == 0x8B && p[1] == 0x15)
		{
			readInstr = func + i;
			break;
		}
	}

	if (!readInstr)
		return 0;

	for (size_t i = (readInstr - func); i + 6 <= scanSize; i++)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(func + i);

		if (p[0] == 0x89 && p[1] == 0x05)
		{
			writeInstr = func + i;
			break;
		}
	}

	if (!writeInstr)
		return 0;

	uintptr_t globalAddr = Utils::ResolveRipRelative(writeInstr, 2, 6);
	if (globalAddr) {
		ServerOffsets::AGameSession__NextPlayerID = globalAddr - ImageBase;
	}

	Log("AGameSession__NextPlayerID found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession__NextPlayerID));
	return ServerOffsets::AGameSession__NextPlayerID;
}

uintptr_t Finder::FindAGameSession_ProcessAutoLogin() {
	if (ServerOffsets::AGameSession_ProcessAutoLogin)
		return ServerOffsets::AGameSession_ProcessAutoLogin;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 48 8B F9 FF 90 ? ? ? ? 45 33 C0").Get();

	if (Addr) {
		ServerOffsets::AGameSession_ProcessAutoLogin = Addr - ImageBase;
	}
	Log("AGameSession_ProcessAutoLogin found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_ProcessAutoLogin));
	return ServerOffsets::AGameSession_ProcessAutoLogin;
}

uintptr_t Finder::FindAGameSession_OnAutoLoginComplete() {
	if (ServerOffsets::AGameSession_OnAutoLoginComplete)
		return ServerOffsets::AGameSession_OnAutoLoginComplete;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 8B FA 48 8B F1").Get();

	if (Addr) {
		ServerOffsets::AGameSession_OnAutoLoginComplete = Addr - ImageBase;
	}
	Log("AGameSession_OnAutoLoginComplete found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_OnAutoLoginComplete));
	return ServerOffsets::AGameSession_OnAutoLoginComplete;
}

uintptr_t Finder::FindAGameSession_UnregisterPlayer_1() {
	if (ServerOffsets::AGameSession_UnregisterPlayer_1)
		return ServerOffsets::AGameSession_UnregisterPlayer_1;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 49 8B F0 48 8B DA 48 8B F9 FF 90 ? ? ? ? 48 8B CF 48 8B E8 E8 ? ? ? ? 85 C0 74 ? 48 8B 4E").Get();

	if (Addr) {
		ServerOffsets::AGameSession_UnregisterPlayer_1 = Addr - ImageBase;
	}
	Log("AGameSession_UnregisterPlayer_1 found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_UnregisterPlayer_1));
	return ServerOffsets::AGameSession_UnregisterPlayer_1;
}

uintptr_t Finder::FindAGameSession_UnregisterPlayers() {
	if (ServerOffsets::AGameSession_UnregisterPlayers)
		return ServerOffsets::AGameSession_UnregisterPlayers;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 49 8B F0 48 8B DA 48 8B F9 FF 90 ? ? ? ? 48 8B CF 48 8B E8 E8 ? ? ? ? 85 C0 74 ? 83 7E").Get();

	if (Addr) {
		ServerOffsets::AGameSession_UnregisterPlayers = Addr - ImageBase;
	}
	Log("AGameSession_UnregisterPlayers found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_UnregisterPlayers));
	return ServerOffsets::AGameSession_UnregisterPlayers;
}

uintptr_t Finder::FindAGameSession_GetPlayerControllerFromNetId() {
	if (ServerOffsets::AGameSession_GetPlayerControllerFromNetId)
		return ServerOffsets::AGameSession_GetPlayerControllerFromNetId;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 02 48 8B D9 48 8B CA 48 8B FA FF 50 ? 84 C0 0F 84 ? ? ? ? 48 8D 54 24 ? 48 8B CB E8 ? ? ? ? 8B 44 24 ? 85 C0").Get();

	if (Addr) {
		ServerOffsets::AGameSession_GetPlayerControllerFromNetId = Addr - ImageBase;
	}
	Log("AGameSession_GetPlayerControllerFromNetId found at: 0x" + std::format("{:X}", ServerOffsets::AGameSession_GetPlayerControllerFromNetId));
	return ServerOffsets::AGameSession_GetPlayerControllerFromNetId;
}

uintptr_t Finder::FindAGameModeBase_InitGame() {
	if (ServerOffsets::AGameModeBase_InitGame)
		return ServerOffsets::AGameModeBase_InitGame;
	static uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? ? ? ? 49 8B E8 48 8B D9").Get();
	}

	if (Addr) {
		ServerOffsets::AGameModeBase_InitGame = Addr - ImageBase;
	}

	Log("AGameModeBase_InitGame found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_InitGame));
	return ServerOffsets::AGameModeBase_InitGame;
}

uintptr_t Finder::FindAGameModeBase_InitGameVFT() {
	if (ServerOffsets::AGameModeBase_InitGameVFT)
		return ServerOffsets::AGameModeBase_InitGameVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.GameModeBase"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAGameModeBase_InitGame() + ImageBase))
		{
			ServerOffsets::AGameModeBase_InitGameVFT = i;
			break;
		}
	}

	Log("AGameModeBase_InitGameVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_InitGameVFT));
	return ServerOffsets::AGameModeBase_InitGameVFT;
}

uintptr_t Finder::FindAGameModeBase_InitGameState() {
	if (ServerOffsets::AGameModeBase_InitGameState)
		return ServerOffsets::AGameModeBase_InitGameState;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 41 ? 48 8B D9 48 8B 91").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ?? 48 8B 41 ?? 48 8B D9 48 8B 91").Get();
	}

	if (Addr) {
		ServerOffsets::AGameModeBase_InitGameState = Addr - ImageBase;
	}

	Log("AGameModeBase_InitGameState found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_InitGameState));
	return ServerOffsets::AGameModeBase_InitGameState;
}

uintptr_t Finder::FindAGameModeBase_CanServerTravel() {
	if (ServerOffsets::AGameModeBase_CanServerTravel)
		return ServerOffsets::AGameModeBase_CanServerTravel;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 01 48 8B DA").Get();

	if (Addr) {
		ServerOffsets::AGameModeBase_CanServerTravel = Addr - ImageBase;
	}
	Log("AGameModeBase_CanServerTravel found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_CanServerTravel));
	return ServerOffsets::AGameModeBase_CanServerTravel;
}

uintptr_t Finder::FindUKismetStringLibrary_Conv_StringToName() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UKismetStringLibrary_Conv_StringToName)
		return ServerOffsets::UKismetStringLibrary_Conv_StringToName;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 83 7A ? ? 48 8B D9 74 ? 48 8B 12 EB ? 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8").Get();

	if (Addr) {
		ServerOffsets::UKismetStringLibrary_Conv_StringToName = Addr - ImageBase;
		Log("UKismetStringLibrary_Conv_StringToName found at: 0x" + std::format("{:X}", ServerOffsets::UKismetStringLibrary_Conv_StringToName));
		return ServerOffsets::UKismetStringLibrary_Conv_StringToName;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_AddInactivePlayer() {
	if (ServerOffsets::AGameMode_AddInactivePlayer)
		return ServerOffsets::AGameMode_AddInactivePlayer;
	static uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 56 57 41 54 48 83 EC ? ? ? ? 49 8B F0").Get();

	if (Addr) {
		ServerOffsets::AGameMode_AddInactivePlayer = Addr - ImageBase;
	}

	Log("AGameMode_AddInactivePlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_AddInactivePlayer));
	return ServerOffsets::AGameMode_AddInactivePlayer;
}

uintptr_t Finder::FindAGameMode_AddInactivePlayerVFT() {
	if (ServerOffsets::AGameMode_AddInactivePlayerVFT)
		return ServerOffsets::AGameMode_AddInactivePlayerVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.GameMode"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAGameMode_AddInactivePlayer() + ImageBase))
		{
			ServerOffsets::AGameMode_AddInactivePlayerVFT = i;
			break;
		}
	}

	Log("AGameMode_AddInactivePlayerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_AddInactivePlayerVFT));
	return ServerOffsets::AGameMode_AddInactivePlayerVFT;
}

uintptr_t Finder::FindAGameMode_Broadcast() {
	if (ServerOffsets::AGameMode_Broadcast)
		return ServerOffsets::AGameMode_Broadcast;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 33 F6 0F 29 74 24").Get();

	if (Addr) {
		ServerOffsets::AGameMode_Broadcast = Addr - ImageBase;
	}
	Log("AGameMode_Broadcast found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_Broadcast));
	return ServerOffsets::AGameMode_Broadcast;
}

uintptr_t Finder::FindAGameMode_BroadcastLocalized() {
	if (ServerOffsets::AGameMode_BroadcastLocalized)
		return ServerOffsets::AGameMode_BroadcastLocalized;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 01 41 8B F9").Get();

	if (Addr) {
		ServerOffsets::AGameMode_BroadcastLocalized = Addr - ImageBase;
	}
	Log("AGameMode_BroadcastLocalized found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_BroadcastLocalized));
	return ServerOffsets::AGameMode_BroadcastLocalized;
}

uintptr_t Finder::FindAGameMode_EndMatch() {
	if (ServerOffsets::AGameMode_EndMatch)
		return ServerOffsets::AGameMode_EndMatch;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 74 ? 48 8B 03 48 8B CB 48 8B 15").Get();

	if (Addr) {
		ServerOffsets::AGameMode_EndMatch = Addr - ImageBase;
	}
	Log("AGameMode_EndMatch found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_EndMatch));
	return ServerOffsets::AGameMode_EndMatch;
}

uintptr_t Finder::FindAGameMode_FindInactivePlayer() {
	if (ServerOffsets::AGameMode_FindInactivePlayer)
		return ServerOffsets::AGameMode_FindInactivePlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 41 55 41 56 41 57 48 81 EC ? ? ? ? 45 33 FF").Get();

	if (Addr) {
		ServerOffsets::AGameMode_FindInactivePlayer = Addr - ImageBase;
	}
	Log("AGameMode_FindInactivePlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_FindInactivePlayer));
	return ServerOffsets::AGameMode_FindInactivePlayer;
}

uintptr_t Finder::FindAGameMode_GetNetworkNumber() {
	if (ServerOffsets::AGameMode_GetNetworkNumber)
		return ServerOffsets::AGameMode_GetNetworkNumber;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 33 F6 48 8B DA 89 74 24").Get();

	if (Addr) {
		ServerOffsets::AGameMode_GetNetworkNumber = Addr - ImageBase;
	}
	Log("AGameMode_GetNetworkNumber found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_GetNetworkNumber));
	return ServerOffsets::AGameMode_GetNetworkNumber;
}

uintptr_t Finder::FindAGameMode_HandleMatchHasEnded() {
	if (ServerOffsets::AGameMode_HandleMatchHasEnded)
		return ServerOffsets::AGameMode_HandleMatchHasEnded;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 03").Get();

	if (Addr) {
		ServerOffsets::AGameMode_HandleMatchHasEnded = Addr - ImageBase;
	}
	Log("AGameMode_HandleMatchHasEnded found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_HandleMatchHasEnded));
	return ServerOffsets::AGameMode_HandleMatchHasEnded;
}

uintptr_t Finder::FindAGameMode_HandleMatchHasStarted() {
	if (ServerOffsets::AGameMode_HandleMatchHasStarted)
		return ServerOffsets::AGameMode_HandleMatchHasStarted;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 48 8B EC 48 81 EC ? ? ? ? 48 89 58 ? 48 89 78").Get();

	if (Addr) {
		ServerOffsets::AGameMode_HandleMatchHasStarted = Addr - ImageBase;
	}
	Log("AGameMode_HandleMatchHasStarted found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_HandleMatchHasStarted));
	return ServerOffsets::AGameMode_HandleMatchHasStarted;
}

uintptr_t Finder::FindAGameMode_HandleMatchIsWaitingToStart() {
	if (ServerOffsets::AGameMode_HandleMatchIsWaitingToStart)
		return ServerOffsets::AGameMode_HandleMatchIsWaitingToStart;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 FF 90 ? ? ? ? 48 8B CB").Get();

	if (Addr) {
		ServerOffsets::AGameMode_HandleMatchIsWaitingToStart = Addr - ImageBase;
	}
	Log("AGameMode_HandleMatchIsWaitingToStart found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_HandleMatchIsWaitingToStart));
	return ServerOffsets::AGameMode_HandleMatchIsWaitingToStart;
}

uintptr_t Finder::FindAGameMode_HandleSeamlessTravelPlayer() {
	if (ServerOffsets::AGameMode_HandleSeamlessTravelPlayer)
		return ServerOffsets::AGameMode_HandleSeamlessTravelPlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 56 41 55 41 56 41 57 48 83 EC").Get();

	if (Addr) {
		ServerOffsets::AGameMode_HandleSeamlessTravelPlayer = Addr - ImageBase;
	}
	Log("AGameMode_HandleSeamlessTravelPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_HandleSeamlessTravelPlayer));
	return ServerOffsets::AGameMode_HandleSeamlessTravelPlayer;
}

uintptr_t Finder::FindAGameMode_HandleStartingNewPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_HandleStartingNewPlayer)
		return ServerOffsets::AGameMode_HandleStartingNewPlayer;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? F6 81 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? E8 ? ? ? ? 84 C0 75 ? 48 8B 03").Get();

	if (Addr) {
		ServerOffsets::AGameMode_HandleStartingNewPlayer = Addr - ImageBase;
	}

	Log("AGameMode_HandleStartingNewPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_HandleStartingNewPlayer));
	return ServerOffsets::AGameMode_HandleStartingNewPlayer;
}

uintptr_t Finder::FindAGameMode_InitSeamlessTravelPlayer() {
	if (ServerOffsets::AGameMode_InitSeamlessTravelPlayer)
		return ServerOffsets::AGameMode_InitSeamlessTravelPlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 48 85 FF 74 ? E8 ? ? ? ? 48 8B 57 ? 4C 8D 40 ? 48 63 40 ? 3B 42 ? 7F ? 48 8B C8 48 8B 42 ? 4C 39 04 C8 75 ? 48 8B 03").Get();

	if (Addr) {
		ServerOffsets::AGameMode_InitSeamlessTravelPlayer = Addr - ImageBase;
	}
	Log("AGameMode_InitSeamlessTravelPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_InitSeamlessTravelPlayer));
	return ServerOffsets::AGameMode_InitSeamlessTravelPlayer;
}

uintptr_t Finder::FindAGameMode_IsHandlingReplays() {
	if (ServerOffsets::AGameMode_IsHandlingReplays)
		return ServerOffsets::AGameMode_IsHandlingReplays;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 85 C0 74 ? 48 8B 03 48 8B CB FF 90 ? ? ? ? 48 8B C8 E8").Get();

	if (Addr) {
		ServerOffsets::AGameMode_IsHandlingReplays = Addr - ImageBase;
	}
	Log("AGameMode_IsHandlingReplays found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_IsHandlingReplays));
	return ServerOffsets::AGameMode_IsHandlingReplays;
}

uintptr_t Finder::FindAGameMode_OnMatchStateSet() {
	if (ServerOffsets::AGameMode_OnMatchStateSet)
		return ServerOffsets::AGameMode_OnMatchStateSet;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("41 56 48 83 EC ? 8B 15 ? ? ? ? 4C 8B F1").Get();

	if (Addr) {
		ServerOffsets::AGameMode_OnMatchStateSet = Addr - ImageBase;
	}
	Log("AGameMode_OnMatchStateSet found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_OnMatchStateSet));
	return ServerOffsets::AGameMode_OnMatchStateSet;
}

uintptr_t Finder::FindAGameMode_PlayerCanRestart() {
	if (ServerOffsets::AGameMode_PlayerCanRestart)
		return ServerOffsets::AGameMode_PlayerCanRestart;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 01 48 8B DA FF 90 ? ? ? ? 84 C0 74 ? 48 85 DB").Get();

	if (Addr) {
		ServerOffsets::AGameMode_PlayerCanRestart = Addr - ImageBase;
	}
	Log("AGameMode_PlayerCanRestart found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_PlayerCanRestart));
	return ServerOffsets::AGameMode_PlayerCanRestart;
}

uintptr_t Finder::FindAGameMode_PostSeamlessTravel() {
	if (ServerOffsets::AGameMode_PostSeamlessTravel)
		return ServerOffsets::AGameMode_PostSeamlessTravel;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("41 56 48 83 EC ? 48 89 5C 24 ? 48 8B D9").Get();

	if (Addr) {
		ServerOffsets::AGameMode_PostSeamlessTravel = Addr - ImageBase;
	}
	Log("AGameMode_PostSeamlessTravel found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_PostSeamlessTravel));
	return ServerOffsets::AGameMode_PostSeamlessTravel;
}

uintptr_t Finder::FindAGameMode_ReadyToStartMatch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_ReadyToStartMatch)
		return ServerOffsets::AGameMode_ReadyToStartMatch;
	Addr = Memcury::Scanner::FindPattern("F6 81 ? ? ? ? ? 75 ? 4C 8B 81").Get();

	if (Addr) {
		ServerOffsets::AGameMode_ReadyToStartMatch = Addr - ImageBase;
	}
	Log("AGameMode_ReadyToStartMatch found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_ReadyToStartMatch));
	return ServerOffsets::AGameMode_ReadyToStartMatch;
}

uintptr_t Finder::FindAGameMode_RestartGame() {
	if (ServerOffsets::AGameMode_RestartGame)
		return ServerOffsets::AGameMode_RestartGame;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8B F9 48 8B 89 ? ? ? ? 48 8B 01 FF 90").Get();

	if (Addr) {
		ServerOffsets::AGameMode_RestartGame = Addr - ImageBase;
	}
	Log("AGameMode_RestartGame found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_RestartGame));
	return ServerOffsets::AGameMode_RestartGame;
}

uintptr_t Finder::FindAGameMode_Say() {
	if (ServerOffsets::AGameMode_Say)
		return ServerOffsets::AGameMode_Say;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 33 C0 4C 8B C2 48 89 44 24").Get();

	if (Addr) {
		ServerOffsets::AGameMode_Say = Addr - ImageBase;
	}
	Log("AGameMode_Say found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_Say));
	return ServerOffsets::AGameMode_Say;
}

uintptr_t Finder::FindAGameMode_SendPlayer() {
	if (ServerOffsets::AGameMode_SendPlayer)
		return ServerOffsets::AGameMode_SendPlayer;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 33 C9 49 8B C0 48 89 4C 24").Get();

	if (Addr) {
		ServerOffsets::AGameMode_SendPlayer = Addr - ImageBase;
	}
	Log("AGameMode_SendPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_SendPlayer));
	return ServerOffsets::AGameMode_SendPlayer;
}

uintptr_t Finder::FindAGameMode_SetMatchState() {
	if (ServerOffsets::AGameMode_SetMatchState)
		return ServerOffsets::AGameMode_SetMatchState;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 4C 8B DC 53").Get();

	if (Addr) {
		ServerOffsets::AGameMode_SetMatchState = Addr - ImageBase;
	}
	Log("AGameMode_SetMatchState found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_SetMatchState));
	return ServerOffsets::AGameMode_SetMatchState;
}

uintptr_t Finder::FindAGameMode_SetSeamlessTravelViewTarget() {
	if (ServerOffsets::AGameMode_SetSeamlessTravelViewTarget)
		return ServerOffsets::AGameMode_SetSeamlessTravelViewTarget;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 48 83 EC ? 83 60 ? ? 4C 8D 44 24 ? C6 40 ? ? 0F 57 C0 F3 0F 10 48 ? 48 8B CA").Get();

	if (Addr) {
		ServerOffsets::AGameMode_SetSeamlessTravelViewTarget = Addr - ImageBase;
	}
	Log("AGameMode_SetSeamlessTravelViewTarget found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_SetSeamlessTravelViewTarget));
	return ServerOffsets::AGameMode_SetSeamlessTravelViewTarget;
}

uintptr_t Finder::FindAGameMode_StartMatch() {
	if (ServerOffsets::AGameMode_StartMatch)
		return ServerOffsets::AGameMode_StartMatch;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0").Get();

	if (Addr) {
		ServerOffsets::AGameMode_StartMatch = Addr - ImageBase;
	}
	Log("AGameMode_StartMatch found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_StartMatch));
	return ServerOffsets::AGameMode_StartMatch;
}

uintptr_t Finder::FindAGameMode_StartPlay() {
	if (ServerOffsets::AGameMode_StartPlay)
		return ServerOffsets::AGameMode_StartPlay;
	static uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 05 ? ? ? ? 48 8B D9 39 81 ? ? ? ? 44 8B 81").Get();

	if (Addr) {
		ServerOffsets::AGameMode_StartPlay = Addr - ImageBase;
	}
	Log("AGameMode_StartPlay found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_StartPlay));
	return ServerOffsets::AGameMode_StartPlay;
}

uintptr_t Finder::FindAPlayerController_HasClientLoadedCurrentWorld() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::APlayerController_HasClientLoadedCurrentWorld)
		return ServerOffsets::APlayerController_HasClientLoadedCurrentWorld;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 99 ? ? ? ? 48 8B F9 48 85 DB 74 ? E8 ? ? ? ? 48 8B 53 ? 4C 8D 40 ? 48 63 40 ? 3B 42 ? 7F ? 48 8B C8 48 8B 42 ? 4C 39 04 C8 74 ? 48 8B 1D").Get();

	if (Addr) {
		ServerOffsets::APlayerController_HasClientLoadedCurrentWorld = Addr - ImageBase;
	}
	Log("APlayerController_HasClientLoadedCurrentWorld found at: 0x" + std::format("{:X}", ServerOffsets::APlayerController_HasClientLoadedCurrentWorld));
	return ServerOffsets::APlayerController_HasClientLoadedCurrentWorld;
}

uintptr_t Finder::FindUGameMapsSettings_GetGameModeForName() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UGameMapsSettings_GetGameModeForName)
		return ServerOffsets::UGameMapsSettings_GetGameModeForName;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 48 8B EC 48 83 EC ? 48 8B F2").Get();

	if (Addr) {
		ServerOffsets::UGameMapsSettings_GetGameModeForName = Addr - ImageBase;
		Log("UGameMapsSettings_GetGameModeForName found at: 0x" + std::format("{:X}", ServerOffsets::UGameMapsSettings_GetGameModeForName));
		return ServerOffsets::UGameMapsSettings_GetGameModeForName;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_OverridePlayerState() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_OverridePlayerState)
		return ServerOffsets::AGameMode_OverridePlayerState;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0").Get();

	if (Addr) {
		ServerOffsets::AGameMode_OverridePlayerState = Addr - ImageBase;
		Log("AGameMode_OverridePlayerState found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_OverridePlayerState));
		return ServerOffsets::AGameMode_OverridePlayerState;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_PostLogin() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_PostLogin)
		return ServerOffsets::AGameMode_PostLogin;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 48 8B EC 48 81 EC ? ? ? ? 48 89 58 ? 48 8B D9 48 89 78 ? 4C 89 60").Get();

	if (Addr) {
		ServerOffsets::AGameMode_PostLogin = Addr - ImageBase;
		Log("AGameMode_PostLogin found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_PostLogin));
		return ServerOffsets::AGameMode_PostLogin;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_Logout() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_Logout)
		return ServerOffsets::AGameMode_Logout;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 85 D2 0F 84 ? ? ? ? E8 ? ? ? ? 48 8B 4F ? 4C 8D 40 ? 48 63 40 ? 3B 41 ? 7F").Get();

	if (Addr) {
		ServerOffsets::AGameMode_Logout = Addr - ImageBase;
		Log("AGameMode_Logout found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_Logout));
		return ServerOffsets::AGameMode_Logout;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_CanServerTravel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_CanServerTravel)
		return ServerOffsets::AGameMode_CanServerTravel;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 48 8B 93 ? ? ? ? 48 8B C2").Get();

	if (Addr) {
		ServerOffsets::AGameMode_CanServerTravel = Addr - ImageBase;
		Log("AGameMode_CanServerTravel found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_CanServerTravel));
		return ServerOffsets::AGameMode_CanServerTravel;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_PostLoad() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_PostLoad)
		return ServerOffsets::ULevelStreaming_PostLoad;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? BA ? ? ? ? 48 8B CB E8 ? ? ? ? 84 C0 74").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_PostLoad = Addr - ImageBase;
		Log("ULevelStreaming_PostLoad found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_PostLoad));
		return ServerOffsets::ULevelStreaming_PostLoad;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_DetermineTargetState() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_DetermineTargetState)
		return ServerOffsets::ULevelStreaming_DetermineTargetState;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 F6 48 8B C1").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_DetermineTargetState = Addr - ImageBase;
		Log("ULevelStreaming_DetermineTargetState found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_DetermineTargetState));
		return ServerOffsets::ULevelStreaming_DetermineTargetState;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_OnLevelAdded() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_OnLevelAdded)
		return ServerOffsets::ULevelStreaming_OnLevelAdded;
	Addr = Memcury::Scanner::FindPattern("48 8B 81 ? ? ? ? 48 85 C0 74 ? 0F B6 80 ? ? ? ? 24 ? 0C").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_OnLevelAdded = Addr - ImageBase;
		Log("ULevelStreaming_OnLevelAdded found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_OnLevelAdded));
		return ServerOffsets::ULevelStreaming_OnLevelAdded;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_IsDesiredLevelLoaded() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_IsDesiredLevelLoaded)
		return ServerOffsets::ULevelStreaming_IsDesiredLevelLoaded;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 83 B9 ? ? ? ? ? 48 8B D9 0F 84 ? ? ? ? 48 89 74 24 ? BA").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_IsDesiredLevelLoaded = Addr - ImageBase;
		Log("ULevelStreaming_IsDesiredLevelLoaded found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_IsDesiredLevelLoaded));
		return ServerOffsets::ULevelStreaming_IsDesiredLevelLoaded;
	}
	return 0;
}

uintptr_t Finder::FindUWorld_UpdateStreamingLevelShouldBeConsidered() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UWorld_UpdateStreamingLevelShouldBeConsidered)
		return ServerOffsets::UWorld_UpdateStreamingLevelShouldBeConsidered;
	Addr = Memcury::Scanner::FindPattern("48 85 D2 74 ? 48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 48 8B CA E8 ? ? ? ? 48 3B C7").Get();

	if (Addr) {
		ServerOffsets::UWorld_UpdateStreamingLevelShouldBeConsidered = Addr - ImageBase;
		Log("UWorld_UpdateStreamingLevelShouldBeConsidered found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_UpdateStreamingLevelShouldBeConsidered));
		return ServerOffsets::UWorld_UpdateStreamingLevelShouldBeConsidered;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_SetLevelLODIndex() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_SetLevelLODIndex)
		return ServerOffsets::ULevelStreaming_SetLevelLODIndex;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 3B 91 ? ? ? ? 74 ? 80 B9").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_SetLevelLODIndex = Addr - ImageBase;
		Log("ULevelStreaming_SetLevelLODIndex found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_SetLevelLODIndex));
		return ServerOffsets::ULevelStreaming_SetLevelLODIndex;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_SetPriority() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_SetPriority)
		return ServerOffsets::ULevelStreaming_SetPriority;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 3B 91 ? ? ? ? 74 ? F6 81").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_SetPriority = Addr - ImageBase;
		Log("ULevelStreaming_SetPriority found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_SetPriority));
		return ServerOffsets::ULevelStreaming_SetPriority;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_SetWorldAsset() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_SetWorldAsset)
		return ServerOffsets::ULevelStreaming_SetWorldAsset;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 83 C1").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_SetWorldAsset = Addr - ImageBase;
		Log("ULevelStreaming_SetWorldAsset found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_SetWorldAsset));
		return ServerOffsets::ULevelStreaming_SetWorldAsset;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_GetWorldAssetPackageName() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_GetWorldAssetPackageName)
		return ServerOffsets::ULevelStreaming_GetWorldAssetPackageName;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? F6 81 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? 48 83 C1 ? 48 8D 54 24 ? E8 ? ? ? ? 48 8D 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 83 78 ? ? 74 ? 48 8B 10 EB ? 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8B 08 48 89 8B ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 80 8B ? ? ? ? ? 48 8B 83 ? ? ? ? 48 8D 4C 24").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_GetWorldAssetPackageName = Addr - ImageBase;
		Log("ULevelStreaming_GetWorldAssetPackageName found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_GetWorldAssetPackageName));
		return ServerOffsets::ULevelStreaming_GetWorldAssetPackageName;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_GetWorldAssetPackageFName() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_GetWorldAssetPackageFName)
		return ServerOffsets::ULevelStreaming_GetWorldAssetPackageFName;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? F6 81 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? 48 83 C1 ? 48 8D 54 24 ? E8 ? ? ? ? 48 8D 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 83 78 ? ? 74 ? 48 8B 10 EB ? 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8B 08 48 89 8B ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 80 8B ? ? ? ? ? 48 8B 83 ? ? ? ? 48 8B 5C 24").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_GetWorldAssetPackageFName = Addr - ImageBase;
		Log("ULevelStreaming_GetWorldAssetPackageFName found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_GetWorldAssetPackageFName));
		return ServerOffsets::ULevelStreaming_GetWorldAssetPackageFName;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_RenameForPIE() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_RenameForPIE)
		return ServerOffsets::ULevelStreaming_RenameForPIE;
	Addr = Memcury::Scanner::FindPattern("40 55 41 56 41 57 48 83 EC ? 44 8B FA").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_RenameForPIE = Addr - ImageBase;
		Log("ULevelStreaming_RenameForPIE found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_RenameForPIE));
		return ServerOffsets::ULevelStreaming_RenameForPIE;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_ShouldBeVisible() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_ShouldBeVisible)
		return ServerOffsets::ULevelStreaming_ShouldBeVisible;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? BA ? ? ? ? 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 33 C9").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_ShouldBeVisible = Addr - ImageBase;
		Log("ULevelStreaming_ShouldBeVisible found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_ShouldBeVisible));
		return ServerOffsets::ULevelStreaming_ShouldBeVisible;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_IsStreamingStatePending() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_IsStreamingStatePending)
		return ServerOffsets::ULevelStreaming_IsStreamingStatePending;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? BA ? ? ? ? 48 8B F9").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_IsStreamingStatePending = Addr - ImageBase;
		Log("ULevelStreaming_IsStreamingStatePending found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_IsStreamingStatePending));
		return ServerOffsets::ULevelStreaming_IsStreamingStatePending;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_CreateInstance() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_CreateInstance)
		return ServerOffsets::ULevelStreaming_CreateInstance;
	Addr = Memcury::Scanner::FindPattern("40 55 53 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B DA").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_CreateInstance = Addr - ImageBase;
		Log("ULevelStreaming_CreateInstance found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_CreateInstance));
		return ServerOffsets::ULevelStreaming_CreateInstance;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_RequestLevel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_RequestLevel)
		return ServerOffsets::ULevelStreaming_RequestLevel;
	Addr = Memcury::Scanner::FindPattern("44 89 4C 24 ? 44 88 44 24 ? 55").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_RequestLevel = Addr - ImageBase;
		Log("ULevelStreaming_RequestLevel found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_RequestLevel));
		return ServerOffsets::ULevelStreaming_RequestLevel;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_SetLoadedLevel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_SetLoadedLevel)
		return ServerOffsets::ULevelStreaming_SetLoadedLevel;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B D9 80 A1").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_SetLoadedLevel = Addr - ImageBase;
		Log("ULevelStreaming_SetLoadedLevel found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_SetLoadedLevel));
		return ServerOffsets::ULevelStreaming_SetLoadedLevel;
	}
	return 0;
}

uintptr_t Finder::FindULevelStreaming_DiscardPendingUnloadLevel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreaming_DiscardPendingUnloadLevel)
		return ServerOffsets::ULevelStreaming_DiscardPendingUnloadLevel;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 8B C2 48 8B 89").Get();

	if (Addr) {
		ServerOffsets::ULevelStreaming_DiscardPendingUnloadLevel = Addr - ImageBase;
		Log("ULevelStreaming_DiscardPendingUnloadLevel found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreaming_DiscardPendingUnloadLevel));
		return ServerOffsets::ULevelStreaming_DiscardPendingUnloadLevel;
	}
	return 0;
}

uintptr_t Finder::FindULevel_SortActorList() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_SortActorList)
		return ServerOffsets::ULevel_SortActorList;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 48 8B EC 48 83 EC ? 4D 89 6B").Get();

	if (Addr) {
		ServerOffsets::ULevel_SortActorList = Addr - ImageBase;
		Log("ULevel_SortActorList found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_SortActorList));
		return ServerOffsets::ULevel_SortActorList;
	}
	return 0;
}

uintptr_t Finder::FindULevel_InitializeNetworkActors() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_InitializeNetworkActors)
		return ServerOffsets::ULevel_InitializeNetworkActors;
	Addr = Memcury::Scanner::FindPattern("40 55 41 54 41 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B E9 48 8B 89").Get();

	if (Addr) {
		ServerOffsets::ULevel_InitializeNetworkActors = Addr - ImageBase;
		Log("ULevel_InitializeNetworkActors found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_InitializeNetworkActors));
		return ServerOffsets::ULevel_InitializeNetworkActors;
	}
	return 0;
}

uintptr_t Finder::FindULevel_ClearActorsSeamlessTraveledFlag() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_ClearActorsSeamlessTraveledFlag)
		return ServerOffsets::ULevel_ClearActorsSeamlessTraveledFlag;
	Addr = Memcury::Scanner::FindPattern("48 8B 81 ? ? ? ? 48 63 91 ? ? ? ? 4C 8D 04 D0").Get();

	if (Addr) {
		ServerOffsets::ULevel_ClearActorsSeamlessTraveledFlag = Addr - ImageBase;
		Log("ULevel_ClearActorsSeamlessTraveledFlag found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_ClearActorsSeamlessTraveledFlag));
		return ServerOffsets::ULevel_ClearActorsSeamlessTraveledFlag;
	}
	return 0;
}

uintptr_t Finder::FindULevel_SetWorldSettings() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_SetWorldSettings)
		return ServerOffsets::ULevel_SetWorldSettings;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 8B D9 E8 ? ? ? ? 48 85 C0").Get();

	if (Addr) {
		ServerOffsets::ULevel_SetWorldSettings = Addr - ImageBase;
		Log("ULevel_SetWorldSettings found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_SetWorldSettings));
		return ServerOffsets::ULevel_SetWorldSettings;
	}
	return 0;
}

uintptr_t Finder::FindULevel_IsNetActor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_IsNetActor)
		return ServerOffsets::ULevel_IsNetActor;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 48 85 C9 75 ? 32 C0").Get();

	if (Addr) {
		ServerOffsets::ULevel_IsNetActor = Addr - ImageBase;
		Log("ULevel_IsNetActor found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_IsNetActor));
		return ServerOffsets::ULevel_IsNetActor;
	}
	return 0;
}

uintptr_t Finder::FindULevel_PostInitProperties() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_PostInitProperties)
		return ServerOffsets::ULevel_PostInitProperties;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 0F 10 00").Get();

	if (Addr) {
		ServerOffsets::ULevel_PostInitProperties = Addr - ImageBase;
		Log("ULevel_PostInitProperties found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_PostInitProperties));
		return ServerOffsets::ULevel_PostInitProperties;
	}
	return 0;
}

uintptr_t Finder::FindULevel_BeginDestroy() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_BeginDestroy)
		return ServerOffsets::ULevel_BeginDestroy;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 84 C0 75 ? E8 ? ? ? ? 48 8B D3").Get();

	if (Addr) {
		ServerOffsets::ULevel_BeginDestroy = Addr - ImageBase;
		Log("ULevel_BeginDestroy found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_BeginDestroy));
		return ServerOffsets::ULevel_BeginDestroy;
	}
	return 0;
}

uintptr_t Finder::FindULevel_FinishDestroy() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_FinishDestroy)
		return ServerOffsets::ULevel_FinishDestroy;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B B9 ? ? ? ? 48 8B D9 48 85 FF 74 ? 48 8B CF E8 ? ? ? ? BA ? ? ? ? 48 8B CF E8 ? ? ? ? 48 8B BB").Get();

	if (Addr) {
		ServerOffsets::ULevel_FinishDestroy = Addr - ImageBase;
		Log("ULevel_FinishDestroy found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_FinishDestroy));
		return ServerOffsets::ULevel_FinishDestroy;
	}
	return 0;
}

uintptr_t Finder::FindULevel_PostLoad() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_PostLoad)
		return ServerOffsets::ULevel_PostLoad;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8D 54 24").Get();

	if (Addr) {
		ServerOffsets::ULevel_PostLoad = Addr - ImageBase;
		Log("ULevel_PostLoad found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_PostLoad));
		return ServerOffsets::ULevel_PostLoad;
	}
	return 0;
}

uintptr_t Finder::FindULevel_CreateCluster() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_CreateCluster)
		return ServerOffsets::ULevel_CreateCluster;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 48 81 EC ? ? ? ? 83 3D").Get();

	if (Addr) {
		ServerOffsets::ULevel_CreateCluster = Addr - ImageBase;
		Log("ULevel_CreateCluster found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_CreateCluster));
		return ServerOffsets::ULevel_CreateCluster;
	}
	return 0;
}

uintptr_t Finder::FindULevel_ClearLevelComponents() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_ClearLevelComponents)
		return ServerOffsets::ULevel_ClearLevelComponents;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 80 A1 ? ? ? ? ? 48 8B F1 48 8B 99 ? ? ? ? 48 63 81 ? ? ? ? 48 8D 3C C3 48 3B DF 74 ? 48 8B 0B").Get();

	if (Addr) {
		ServerOffsets::ULevel_ClearLevelComponents = Addr - ImageBase;
		Log("ULevel_ClearLevelComponents found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_ClearLevelComponents));
		return ServerOffsets::ULevel_ClearLevelComponents;
	}
	return 0;
}

uintptr_t Finder::FindULevel_UpdateLevelComponents() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_UpdateLevelComponents)
		return ServerOffsets::ULevel_UpdateLevelComponents;
	Addr = Memcury::Scanner::FindPattern("44 0F B6 C2 33 D2").Get();

	if (Addr) {
		ServerOffsets::ULevel_UpdateLevelComponents = Addr - ImageBase;
		Log("ULevel_UpdateLevelComponents found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_UpdateLevelComponents));
		return ServerOffsets::ULevel_UpdateLevelComponents;
	}
	return 0;
}

uintptr_t Finder::FindULevel_IncrementalUpdateComponents() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_IncrementalUpdateComponents)
		return ServerOffsets::ULevel_IncrementalUpdateComponents;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC ? 8B 81 ? ? ? ? 45 0F B6 F0").Get();

	if (Addr) {
		ServerOffsets::ULevel_IncrementalUpdateComponents = Addr - ImageBase;
		Log("ULevel_IncrementalUpdateComponents found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_IncrementalUpdateComponents));
		return ServerOffsets::ULevel_IncrementalUpdateComponents;
	}
	return 0;
}

uintptr_t Finder::FindULevel_IncrementalUnregisterComponents() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_IncrementalUnregisterComponents)
		return ServerOffsets::ULevel_IncrementalUnregisterComponents;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 33 FF 8B 89").Get();

	if (Addr) {
		ServerOffsets::ULevel_IncrementalUnregisterComponents = Addr - ImageBase;
		Log("ULevel_IncrementalUnregisterComponents found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_IncrementalUnregisterComponents));
		return ServerOffsets::ULevel_IncrementalUnregisterComponents;
	}
	return 0;
}

uintptr_t Finder::FindULevel_RouteActorInitialize() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_RouteActorInitialize)
		return ServerOffsets::ULevel_RouteActorInitialize;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 55 41 56 41 57 48 83 EC ? 33 DB").Get();

	if (Addr) {
		ServerOffsets::ULevel_RouteActorInitialize = Addr - ImageBase;
		Log("ULevel_RouteActorInitialize found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_RouteActorInitialize));
		return ServerOffsets::ULevel_RouteActorInitialize;
	}
	return 0;
}

uintptr_t Finder::FindAActor_IsPendingKillPending() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_IsPendingKillPending)
		return ServerOffsets::AActor_IsPendingKillPending;
	Addr = Memcury::Scanner::FindPattern("F6 41 ? ? 75 ? 8B 41 ? 3B 05").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("F6 81 ? ? ? ? ? 75 ? 48 63 41 ? 3B 05 ? ? ? ? 7D ? 48 8D 0C 40").Get();
	}

	if (Addr) {
		ServerOffsets::AActor_IsPendingKillPending = Addr - ImageBase;
	}

	Log("AActor_IsPendingKillPending found at: 0x" + std::format("{:X}", ServerOffsets::AActor_IsPendingKillPending));
	return ServerOffsets::AActor_IsPendingKillPending;
}

uintptr_t Finder::FindULevel_ApplyWorldOffset() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_ApplyWorldOffset)
		return ServerOffsets::ULevel_ApplyWorldOffset;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 FF 0F 29 B4 24 ? ? ? ? 40 38 3D").Get();

	if (Addr) {
		ServerOffsets::ULevel_ApplyWorldOffset = Addr - ImageBase;
		Log("ULevel_ApplyWorldOffset found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_ApplyWorldOffset));
		return ServerOffsets::ULevel_ApplyWorldOffset;
	}
	return 0;
}

uintptr_t Finder::FindULevel_RegisterActorForAutoReceiveInput() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_RegisterActorForAutoReceiveInput)
		return ServerOffsets::ULevel_RegisterActorForAutoReceiveInput;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 41 8B F8 48 8D 4C 24").Get();

	if (Addr) {
		ServerOffsets::ULevel_RegisterActorForAutoReceiveInput = Addr - ImageBase;
		Log("ULevel_RegisterActorForAutoReceiveInput found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_RegisterActorForAutoReceiveInput));
		return ServerOffsets::ULevel_RegisterActorForAutoReceiveInput;
	}
	return 0;
}

uintptr_t Finder::FindULevel_PushPendingAutoReceiveInput() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevel_PushPendingAutoReceiveInput)
		return ServerOffsets::ULevel_PushPendingAutoReceiveInput;
	Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 56 41 54 41 55 41 56 48 83 EC ? 48 8B 02").Get();

	if (Addr) {
		ServerOffsets::ULevel_PushPendingAutoReceiveInput = Addr - ImageBase;
		Log("ULevel_PushPendingAutoReceiveInput found at: 0x" + std::format("{:X}", ServerOffsets::ULevel_PushPendingAutoReceiveInput));
		return ServerOffsets::ULevel_PushPendingAutoReceiveInput;
	}
	return 0;
}

uintptr_t Finder::FindAActor_HasNetOwner() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_HasNetOwner)
		return ServerOffsets::AActor_HasNetOwner;
	Addr = Memcury::Scanner::FindPattern("48 8B 89 ? ? ? ? 48 85 C9 75 ? 32 C0 C3 48 8B 81").Get();

	if (Addr) {
		ServerOffsets::AActor_HasNetOwner = Addr - ImageBase;
		Log("AActor_HasNetOwner found at: 0x" + std::format("{:X}", ServerOffsets::AActor_HasNetOwner));
		return ServerOffsets::AActor_HasNetOwner;
	}
	return 0;
}

uintptr_t Finder::FindAActor_HasLocalNetOwner() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_HasLocalNetOwner)
		return ServerOffsets::AActor_HasLocalNetOwner;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B D9 48 85 C0 74 ? 48 8B D8").Get();

	if (Addr) {
		ServerOffsets::AActor_HasLocalNetOwner = Addr - ImageBase;
		Log("AActor_HasLocalNetOwner found at: 0x" + std::format("{:X}", ServerOffsets::AActor_HasLocalNetOwner));
		return ServerOffsets::AActor_HasLocalNetOwner;
	}
	return 0;
}

uintptr_t Finder::FindUEngine_LoadMap() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_LoadMap)
		return ServerOffsets::UEngine_LoadMap;
	
	if (Version::Fortnite_Version == 1.91) {
		Addr = ImageBase + 0x25143C0;
	}
	else if (Version::Fortnite_Version == 1.10) {
		Addr = ImageBase + 0x1D01580;
	}
	else {
		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"LoadMap: %s").Get();
		if (StringAddr) {
			int Skipped = 0;

			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(StringAddr - i);
				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
					if (Skipped == 1) {
						Addr = reinterpret_cast<uintptr_t>(Ptr);
						break;
					}
					Skipped++;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UEngine_LoadMap = Addr - ImageBase;
	}

	Log("UEngine_LoadMap found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_LoadMap));
	return ServerOffsets::UEngine_LoadMap;
}

uintptr_t Finder::FindUEngine_LoadMapVFT() {
	if (ServerOffsets::UEngine_LoadMapVFT)
		return ServerOffsets::UEngine_LoadMapVFT;

	if (Version::Engine_Version == 4.16) {
		ServerOffsets::UEngine_LoadMapVFT = 0x7E;
	}

	Log("UEngine_LoadMapVFT found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_LoadMapVFT));
	return ServerOffsets::UEngine_LoadMapVFT;
}

uintptr_t Finder::FindFURL_IsInternal() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_IsInternal)
		return ServerOffsets::FURL_IsInternal;
	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 83 3D ? ? ? ? ? 48 8B C1").Get();

	if (Addr) {
		ServerOffsets::FURL_IsInternal = Addr - ImageBase;
	}
	Log("FURL_IsInternal found at: 0x" + std::format("{:X}", ServerOffsets::FURL_IsInternal));
	return ServerOffsets::FURL_IsInternal;
	return 0;
}

uintptr_t Finder::FindFURL_IsLocalInternal() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_IsLocalInternal)
		return ServerOffsets::FURL_IsLocalInternal;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 83 3D ? ? ? ? ? 48 8B D9 48 8D 0D").Get();

	if (Addr) {
		ServerOffsets::FURL_IsLocalInternal = Addr - ImageBase;
	}
	Log("FURL_IsLocalInternal found at: 0x" + std::format("{:X}", ServerOffsets::FURL_IsLocalInternal));
	return ServerOffsets::FURL_IsLocalInternal;
}

uintptr_t Finder::FindFURL_HasOption() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_HasOption)
		return ServerOffsets::FURL_HasOption;
	Addr = Memcury::Scanner::FindPattern("41 54 41 56 41 57 48 83 EC ? 4C 8B FA 4C 8B F1 48 C7 C0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 83 EC ? 45 33 C0 E8 ? ? ? ? 48 85 C0").Get();
	}

	if (Addr) {
		ServerOffsets::FURL_HasOption = Addr - ImageBase;
	}
	Log("FURL_HasOption found at: 0x" + std::format("{:X}", ServerOffsets::FURL_HasOption));
	return ServerOffsets::FURL_HasOption;
}

uintptr_t Finder::FindFURL_GetOption() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_GetOption)
		return ServerOffsets::FURL_GetOption;
	Addr = Memcury::Scanner::FindPattern("41 54 41 56 41 57 48 83 EC ? 4D 8B E0 4C 8B FA 4C 8B F1").Get();

	if (Addr) {
		ServerOffsets::FURL_GetOption = Addr - ImageBase;
	}
	Log("FURL_GetOption found at: 0x" + std::format("{:X}", ServerOffsets::FURL_GetOption));
	return ServerOffsets::FURL_GetOption;
}

uintptr_t Finder::FindFURL_AddOption() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_AddOption)
		return ServerOffsets::FURL_AddOption;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 57 48 83 EC ? 48 8B FA").Get();

	if (Addr) {
		ServerOffsets::FURL_AddOption = Addr - ImageBase;
	}
	Log("FURL_AddOption found at: 0x" + std::format("{:X}", ServerOffsets::FURL_AddOption));
	return ServerOffsets::FURL_AddOption;
}

uintptr_t Finder::FindFURL_RemoveOption() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_RemoveOption)
		return ServerOffsets::FURL_RemoveOption;
	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 4C 8B DC 4D 89 4B ? 4D 89 43").Get();

	if (Addr) {
		ServerOffsets::FURL_RemoveOption = Addr - ImageBase;
	}
	Log("FURL_RemoveOption found at: 0x" + std::format("{:X}", ServerOffsets::FURL_RemoveOption));
	return ServerOffsets::FURL_RemoveOption;
}

uintptr_t Finder::FindFURL_ToString() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_ToString)
		return ServerOffsets::FURL_ToString;
	Addr = Memcury::Scanner::FindPattern("40 53 57 41 57 48 83 EC ? 45 33 FF 48 89 6C 24").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 53 55 56 41 55 41 56 48 83 EC ? 45 33 ED").Get();
	}

	if (Addr) {
		ServerOffsets::FURL_ToString = Addr - ImageBase;
	}
	Log("FURL_ToString found at: 0x" + std::format("{:X}", ServerOffsets::FURL_ToString));
	return ServerOffsets::FURL_ToString;
}

uintptr_t Finder::FindFURL_GetHostPortString() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FURL_GetHostPortString)
		return ServerOffsets::FURL_GetHostPortString;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B DA 48 81 C1 ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 83 78").Get();
	if (Addr) {
		ServerOffsets::FURL_GetHostPortString = Addr - ImageBase;
	}
	Log("FURL_GetHostPortString found at: 0x" + std::format("{:X}", ServerOffsets::FURL_GetHostPortString));
	return ServerOffsets::FURL_GetHostPortString;
}

uintptr_t Finder::FindFWorldContext_SetCurrentWorld() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FWorldContext_SetCurrentWorld)
		return ServerOffsets::FWorldContext_SetCurrentWorld;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 85 D2 74 ? 8B 91").Get();

	if (Addr) {
		ServerOffsets::FWorldContext_SetCurrentWorld = Addr - ImageBase;
	}

	Log("FWorldContext_SetCurrentWorld found at: 0x" + std::format("{:X}", ServerOffsets::FWorldContext_SetCurrentWorld));
	return ServerOffsets::FWorldContext_SetCurrentWorld;
}

uintptr_t Finder::FindFWorldContext__ThisCurrentWorld()
{
	if (ServerOffsets::FWorldContext__ThisCurrentWorld)
		return ServerOffsets::FWorldContext__ThisCurrentWorld;
	if (!FindFWorldContext_SetCurrentWorld())
		return 0;

	uintptr_t func = ImageBase + FindFWorldContext_SetCurrentWorld();
	const size_t scanSize = 0x200;

	for (size_t i = 0; i + 14 <= scanSize; i++)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(func + i);

		if (p[0] == 0x48 && p[1] == 0x8B && p[2] == 0x93 &&
			p[7] == 0x48 && p[8] == 0x89 && p[9] == 0xBB)
		{
			uint32_t dispLoad = *reinterpret_cast<uint32_t*>(p + 3);
			uint32_t dispStore = *reinterpret_cast<uint32_t*>(p + 10);

			if (dispLoad == dispStore) {
				ServerOffsets::FWorldContext__ThisCurrentWorld = dispLoad;
			}
		}

		if (p[0] == 0x48 &&
			p[1] == 0x89 &&
			p[2] == 0xBB)
		{
			uint32_t disp = *reinterpret_cast<uint32_t*>(p + 3);
			ServerOffsets::FWorldContext__ThisCurrentWorld = disp;
			break;
		}
	}

	Log("FWorldContext__ThisCurrentWorld found at offset: 0x" + std::format("{:X}", ServerOffsets::FWorldContext__ThisCurrentWorld));
	return ServerOffsets::FWorldContext__ThisCurrentWorld;
}

uintptr_t Finder::FindUEngine_CreateNamedNetDriver() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_CreateNamedNetDriver)
		return ServerOffsets::UEngine_CreateNamedNetDriver;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 4C 89 4C 24 ? 4C 89 44 24 ? 55 56 57 48 83 EC ? 48 63 82").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 4C 89 4C 24 ? 4C 89 44 24 ? 57 48 83 EC ? 48 63 82").Get();
	}

	if (Addr) {
		ServerOffsets::UEngine_CreateNamedNetDriver = Addr - ImageBase;
	}

	Log("UEngine_CreateNamedNetDriver found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CreateNamedNetDriver));
	return ServerOffsets::UEngine_CreateNamedNetDriver;
}

uintptr_t Finder::FindUEngine_GetWorldContextFromWorldChecked() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_GetWorldContextFromWorldChecked)
		return ServerOffsets::UEngine_GetWorldContextFromWorldChecked;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"CANCEL").Get();
	if (StringAddr) {
		int CallCount = 0;

		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);

			if (*Ptr == 0xE8)
			{
				CallCount++;
				if (CallCount == 2)
				{
					int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
					Addr = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UEngine_GetWorldContextFromWorldChecked = Addr - ImageBase;
	}

	Log("UEngine_GetWorldContextFromWorldChecked found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetWorldContextFromWorldChecked));
	return ServerOffsets::UEngine_GetWorldContextFromWorldChecked;
}

uintptr_t Finder::FindUEngine_FindNamedNetDriver() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_FindNamedNetDriver)
		return ServerOffsets::UEngine_FindNamedNetDriver;
	Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 53 48 83 EC ? 49 8B D8 E8 ? ? ? ? 48 63 88 ? ? ? ? 85 C9 7E ? 4C 8B 88 ? ? ? ? 4C 8B D1 44 8B 5C 24 ? 45 33 C0 90 49 8B 01 48 85 C0 74 ? 44 39 98 ? ? ? ? 0F 94 C2 39 98 ? ? ? ? 0F 94 C1 84 D1 75 ? 49 FF C0 49 83 C1 ? 4D 3B C2 7C ? 33 C0 48 83 C4 ? 5B C3 CC").Get();
	if (Addr) {
		ServerOffsets::UEngine_FindNamedNetDriver = Addr - ImageBase;
	}
	Log("UEngine_FindNamedNetDriver found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_FindNamedNetDriver));
	return ServerOffsets::UEngine_FindNamedNetDriver;
}

uintptr_t Finder::FindUWorld_FindCollectionByType() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UWorld_FindCollectionByType)
		return ServerOffsets::UWorld_FindCollectionByType;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"LevelPrefixOverride=").Get();
	if (StringAddr) {
		int CallCount = 0;

		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0xE8)
			{
				CallCount++;

				if (CallCount == 2)
				{
					int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
					Addr = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UWorld_FindCollectionByType = Addr - ImageBase;
	}

	Log("UWorld_FindCollectionByType found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_FindCollectionByType));
	return ServerOffsets::UWorld_FindCollectionByType;
}

uintptr_t Finder::FindUEngine_DestroyNamedNetDriver() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_DestroyNamedNetDriver)
		return ServerOffsets::UEngine_DestroyNamedNetDriver;
	Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 4C 8B DC 57").Get();
	if (Addr) {
		ServerOffsets::UEngine_DestroyNamedNetDriver = Addr - ImageBase;
		Log("UEngine_DestroyNamedNetDriver found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_DestroyNamedNetDriver));
		return ServerOffsets::UEngine_DestroyNamedNetDriver;
	}
	return 0;
}

uintptr_t Finder::FindFParse_Param() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FParse_Param)
		return ServerOffsets::FParse_Param;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 66 83 39").Get();

	if (Addr) {
		ServerOffsets::FParse_Param = Addr - ImageBase;
	}

	Log("FParse_Param found at: 0x" + std::format("{:X}", ServerOffsets::FParse_Param));
	return ServerOffsets::FParse_Param;
}

uintptr_t Finder::FindAFortGameModeAthena_SetCurrentPlaylistId() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_SetCurrentPlaylistId)
		return ServerOffsets::AFortGameModeAthena_SetCurrentPlaylistId;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 80 3D ? ? ? ? ? 8B F2 48 8B D9 72 ? 48 8D 05 ? ? ? ? 41 B9 ? ? ? ? 4C 8D 05 ? ? ? ? 48 89 44 24 ? 33 D2 33 C9 E8 ? ? ? ? 48 8B CB").Get();

	if (Addr) {
		ServerOffsets::AFortGameModeAthena_SetCurrentPlaylistId = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_SetCurrentPlaylistId found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_SetCurrentPlaylistId));
	return ServerOffsets::AFortGameModeAthena_SetCurrentPlaylistId;
}

uintptr_t Finder::FindAFortGameMode_OnInitWorldRecordComplete() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_OnInitWorldRecordComplete)
		return ServerOffsets::AFortGameMode_OnInitWorldRecordComplete;
	Addr = Memcury::Scanner::FindPattern("41 55 41 57 48 83 EC ? 80 3D ? ? ? ? ? 48 89 5C 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameMode_OnInitWorldRecordComplete = Addr - ImageBase;
		Log("AFortGameMode_OnInitWorldRecordComplete found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_OnInitWorldRecordComplete));
		return ServerOffsets::AFortGameMode_OnInitWorldRecordComplete;
	}
	return 0;
}

uintptr_t Finder::FindUWorld__NextSwitchCountdown() {
	if (!Finder::FindUWorld_ServerTravel())
		return 0;
	if (ServerOffsets::UWorld__NextSwitchCountdown)
		return ServerOffsets::UWorld__NextSwitchCountdown;

	uintptr_t func = ImageBase + Finder::FindUWorld_ServerTravel();
	const size_t scanSize = 0x200;

	for (size_t i = 0; i + 11 <= scanSize; i++)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(func + i);

		if (p[0] == 0xC7 && p[1] == 0x83 &&
			p[6] == 0x00 && p[7] == 0x00 && p[8] == 0x00 && p[9] == 0x00)
		{
			uint32_t disp = *reinterpret_cast<uint32_t*>(p + 2);
			ServerOffsets::UWorld__NextSwitchCountdown = disp;
			break;
		}

		if (p[0] == 0x44 && p[1] == 0x89 && p[2] == 0xBB)
		{
			uint32_t disp = *reinterpret_cast<uint32_t*>(p + 3);
			ServerOffsets::UWorld__NextSwitchCountdown = disp;
			break;
		}
	}

	Log("UWorld__NextSwitchCountdown found at offset: 0x" + std::format("{:X}", ServerOffsets::UWorld__NextSwitchCountdown));
	return ServerOffsets::UWorld__NextSwitchCountdown;
}

uintptr_t Finder::FindAFortGameMode_SetCurrentPlaylistName() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_SetCurrentPlaylistName)
		return ServerOffsets::AFortGameMode_SetCurrentPlaylistName;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 54 24 ? 57 48 83 EC ? 48 8B D9 E8").Get();
	if (Addr) {
		ServerOffsets::AFortGameMode_SetCurrentPlaylistName = Addr - ImageBase;
		Log("AFortGameMode_SetCurrentPlaylistName found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_SetCurrentPlaylistName));
		return ServerOffsets::AFortGameMode_SetCurrentPlaylistName;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameMode_OnEndOfDay() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_OnEndOfDay)
		return ServerOffsets::AFortGameMode_OnEndOfDay;
	Addr = Memcury::Scanner::FindPattern("40 57 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F9 0F 85 ? ? ? ? 48 83 B9").Get();
	if (Addr) {
		ServerOffsets::AFortGameMode_OnEndOfDay = Addr - ImageBase;
		Log("AFortGameMode_OnEndOfDay found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_OnEndOfDay));
		return ServerOffsets::AFortGameMode_OnEndOfDay;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_CheckForAllPlayersIn() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_CheckForAllPlayersIn)
		return ServerOffsets::AFortGameModeZone_CheckForAllPlayersIn;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 56 57 48 81 EC ? ? ? ? 48 8B 81").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_CheckForAllPlayersIn = Addr - ImageBase;
		Log("AFortGameModeZone_CheckForAllPlayersIn found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_CheckForAllPlayersIn));
		return ServerOffsets::AFortGameModeZone_CheckForAllPlayersIn;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_RemoveKnownPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_RemoveKnownPlayer)
		return ServerOffsets::AFortGameModeZone_RemoveKnownPlayer;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 89 74 24 ? 48 8B DA").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_RemoveKnownPlayer = Addr - ImageBase;
		Log("AFortGameModeZone_RemoveKnownPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_RemoveKnownPlayer));
		return ServerOffsets::AFortGameModeZone_RemoveKnownPlayer;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_UpdateSessionForMatchmaking() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_UpdateSessionForMatchmaking)
		return ServerOffsets::AFortGameModeZone_UpdateSessionForMatchmaking;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 53 57 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 01 48 8B F9").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_UpdateSessionForMatchmaking = Addr - ImageBase;
		Log("AFortGameModeZone_UpdateSessionForMatchmaking found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_UpdateSessionForMatchmaking));
		return ServerOffsets::AFortGameModeZone_UpdateSessionForMatchmaking;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_NotifyPendingConnectionLost() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_NotifyPendingConnectionLost)
		return ServerOffsets::AFortGameModeZone_NotifyPendingConnectionLost;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 3D ? ? ? ? ? 48 8B DA 48 8B F9 72 ? 48 8D 54 24 ? 48 8B CB E8 ? ? ? ? 83 78 ? ? 74 ? 48 8B 10").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_NotifyPendingConnectionLost = Addr - ImageBase;
		Log("AFortGameModeZone_NotifyPendingConnectionLost found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_NotifyPendingConnectionLost));
		return ServerOffsets::AFortGameModeZone_NotifyPendingConnectionLost;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_OnZoneEnd() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_OnZoneEnd)
		return ServerOffsets::AFortGameModeZone_OnZoneEnd;
	Addr = Memcury::Scanner::FindPattern("40 53 41 56 48 83 EC ? 48 8B 01 4C 8B F1 FF 90 ? ? ? ? 49 8B CE").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_OnZoneEnd = Addr - ImageBase;
		Log("AFortGameModeZone_OnZoneEnd found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_OnZoneEnd));
		return ServerOffsets::AFortGameModeZone_OnZoneEnd;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_DoLogoutAnalytics() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_DoLogoutAnalytics)
		return ServerOffsets::AFortGameModeZone_DoLogoutAnalytics;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 48 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 78").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_DoLogoutAnalytics = Addr - ImageBase;
		Log("AFortGameModeZone_DoLogoutAnalytics found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_DoLogoutAnalytics));
		return ServerOffsets::AFortGameModeZone_DoLogoutAnalytics;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_HandleMatchHasStarted() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_HandleMatchHasStarted)
		return ServerOffsets::AFortGameModeZone_HandleMatchHasStarted;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 56 49 8D 6B ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 89 5B ? 48 8B F1 49 89 7B").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_HandleMatchHasStarted = Addr - ImageBase;
		Log("AFortGameModeZone_HandleMatchHasStarted found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_HandleMatchHasStarted));
		return ServerOffsets::AFortGameModeZone_HandleMatchHasStarted;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_HandleMatchHasEnded() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_HandleMatchHasEnded)
		return ServerOffsets::AFortGameModeZone_HandleMatchHasEnded;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 80 3D ? ? ? ? ? 72 ? 48 8D 05 ? ? ? ? 41 B9 ? ? ? ? 4C 8D 05 ? ? ? ? 48 89 44 24 ? 33 D2 33 C9 E8 ? ? ? ? C6 83").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_HandleMatchHasEnded = Addr - ImageBase;
		Log("AFortGameModeZone_HandleMatchHasEnded found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_HandleMatchHasEnded));
		return ServerOffsets::AFortGameModeZone_HandleMatchHasEnded;
	}

	return 0;
}

uintptr_t Finder::FindAFortGameModeZone_DoInitialRestartAnalytics() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_DoInitialRestartAnalytics)
		return ServerOffsets::AFortGameModeZone_DoInitialRestartAnalytics;
	Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 55 41 56 48 8D AC 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeZone_DoInitialRestartAnalytics = Addr - ImageBase;
		Log("AFortGameModeZone_DoInitialRestartAnalytics found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_DoInitialRestartAnalytics));
		return ServerOffsets::AFortGameModeZone_DoInitialRestartAnalytics;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_SetTimeOfDayManagerGameplayOverride() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride)
		return ServerOffsets::AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B B1 ? ? ? ? 48 8B DA 48 8B F9 48 85 F6 74 ? E8 ? ? ? ? 48 8B 4E ? 4C 8D 40 ? 48 63 40 ? 3B 41 ? 7F ? 48 8B D0 48 8B 41 ? 4C 39 04 D0 75 ? 48 8B CE E8 ? ? ? ? 84 C0 74 ? 80 3D").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride = Addr - ImageBase;
		Log("AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride));
		return ServerOffsets::AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_OnPlaylistDataLoaded() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_OnPlaylistDataLoaded)
		return ServerOffsets::AFortGameModeAthena_OnPlaylistDataLoaded;
	Addr = Memcury::Scanner::FindPattern("40 55 56 41 54 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B A1").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_OnPlaylistDataLoaded = Addr - ImageBase;
		Log("AFortGameModeAthena_OnPlaylistDataLoaded found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_OnPlaylistDataLoaded));
		return ServerOffsets::AFortGameModeAthena_OnPlaylistDataLoaded;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_ApplyPlaylistDataTables() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_ApplyPlaylistDataTables)
		return ServerOffsets::AFortGameModeAthena_ApplyPlaylistDataTables;
	Addr = Memcury::Scanner::FindPattern("40 55 56 41 55 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B B9").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_ApplyPlaylistDataTables = Addr - ImageBase;
		Log("AFortGameModeAthena_ApplyPlaylistDataTables found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_ApplyPlaylistDataTables));
		return ServerOffsets::AFortGameModeAthena_ApplyPlaylistDataTables;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_OnDedicatedServerRestarting() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_OnDedicatedServerRestarting)
		return ServerOffsets::AFortGameModeAthena_OnDedicatedServerRestarting;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B F8 48 85 C0 74 ? E8 ? ? ? ? 48 8B 4F ? 48 83 C0 ? 48 63 50 ? 3B 51 ? 7F ? 48 8B 49 ? 48 39 04 D1 75 ? 80 3D").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_OnDedicatedServerRestarting = Addr - ImageBase;
		Log("AFortGameModeAthena_OnDedicatedServerRestarting found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_OnDedicatedServerRestarting));
		return ServerOffsets::AFortGameModeAthena_OnDedicatedServerRestarting;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_SpawnInitialSafeZone() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_SpawnInitialSafeZone)
		return ServerOffsets::AFortGameModeAthena_SpawnInitialSafeZone;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 55 56 57 41 56 48 81 EC").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_SpawnInitialSafeZone = Addr - ImageBase;
		Log("AFortGameModeAthena_SpawnInitialSafeZone found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_SpawnInitialSafeZone));
		return ServerOffsets::AFortGameModeAthena_SpawnInitialSafeZone;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_AttemptChangeTeam() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_AttemptChangeTeam)
		return ServerOffsets::AFortGameModeAthena_AttemptChangeTeam;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 E8 ? ? ? ? 0F B6 D8 84 C0 74").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_AttemptChangeTeam = Addr - ImageBase;
		Log("AFortGameModeAthena_AttemptChangeTeam found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_AttemptChangeTeam));
		return ServerOffsets::AFortGameModeAthena_AttemptChangeTeam;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_PickTeam() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_PickTeam)
		return ServerOffsets::AFortGameModeAthena_PickTeam;
	Addr = Memcury::Scanner::FindPattern("40 55 56 41 54 41 55 41 57 48 83 EC ? 48 8B A9").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_PickTeam = Addr - ImageBase;
		Log("AFortGameModeAthena_PickTeam found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_PickTeam));
		return ServerOffsets::AFortGameModeAthena_PickTeam;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_HandleMatchHasStarted() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_HandleMatchHasStarted)
		return ServerOffsets::AFortGameModeAthena_HandleMatchHasStarted;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 41 57 48 8D 6C 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_HandleMatchHasStarted = Addr - ImageBase;
		Log("AFortGameModeAthena_HandleMatchHasStarted found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_HandleMatchHasStarted));
		return ServerOffsets::AFortGameModeAthena_HandleMatchHasStarted;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_FinishRestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_FinishRestartPlayer)
		return ServerOffsets::AFortGameModeAthena_FinishRestartPlayer;
	Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 45 32 E4").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_FinishRestartPlayer = Addr - ImageBase;
		Log("AFortGameModeAthena_FinishRestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_FinishRestartPlayer));
		return ServerOffsets::AFortGameModeAthena_FinishRestartPlayer;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_StartMatchAbandonProcess() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartMatchAbandonProcess)
		return ServerOffsets::AFortGameModeAthena_StartMatchAbandonProcess;
	Addr = Memcury::Scanner::FindPattern("40 55 53 56 41 54 41 55 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 E4").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartMatchAbandonProcess = Addr - ImageBase;
		Log("AFortGameModeAthena_StartMatchAbandonProcess found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartMatchAbandonProcess));
		return ServerOffsets::AFortGameModeAthena_StartMatchAbandonProcess;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_OnMatchAbandonTimeout() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_OnMatchAbandonTimeout)
		return ServerOffsets::AFortGameModeAthena_OnMatchAbandonTimeout;
	Addr = Memcury::Scanner::FindPattern("40 55 53 57 41 54 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_OnMatchAbandonTimeout = Addr - ImageBase;
		Log("AFortGameModeAthena_OnMatchAbandonTimeout found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_OnMatchAbandonTimeout));
		return ServerOffsets::AFortGameModeAthena_OnMatchAbandonTimeout;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_CheckWinConditionLastManStanding() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_CheckWinConditionLastManStanding)
		return ServerOffsets::AFortGameModeAthena_CheckWinConditionLastManStanding;
	Addr = Memcury::Scanner::FindPattern("44 89 4C 24 ? 4C 89 44 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8B EC").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_CheckWinConditionLastManStanding = Addr - ImageBase;
		Log("AFortGameModeAthena_CheckWinConditionLastManStanding found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_CheckWinConditionLastManStanding));
		return ServerOffsets::AFortGameModeAthena_CheckWinConditionLastManStanding;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_GrantVictoryToAllRemainingTeams() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_GrantVictoryToAllRemainingTeams)
		return ServerOffsets::AFortGameModeAthena_GrantVictoryToAllRemainingTeams;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 53 57 48 83 EC ? 48 8B 99").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_GrantVictoryToAllRemainingTeams = Addr - ImageBase;
		Log("AFortGameModeAthena_GrantVictoryToAllRemainingTeams found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_GrantVictoryToAllRemainingTeams));
		return ServerOffsets::AFortGameModeAthena_GrantVictoryToAllRemainingTeams;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame)
		return ServerOffsets::AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame;
	Addr = Memcury::Scanner::FindPattern("89 54 24 ? 48 89 4C 24 ? 56 41 55 41 56 48 81 EC").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame = Addr - ImageBase;
		Log("AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame));
		return ServerOffsets::AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_GrantLossToAllRemainingTeams() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_GrantLossToAllRemainingTeams)
		return ServerOffsets::AFortGameModeAthena_GrantLossToAllRemainingTeams;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 48 8B D9 72 ? 48 8D 05 ? ? ? ? 41 B9 ? ? ? ? 4C 8D 05 ? ? ? ? 48 89 44 24 ? 33 D2 33 C9 E8 ? ? ? ? B2").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_GrantLossToAllRemainingTeams = Addr - ImageBase;
		Log("AFortGameModeAthena_GrantLossToAllRemainingTeams found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_GrantLossToAllRemainingTeams));
		return ServerOffsets::AFortGameModeAthena_GrantLossToAllRemainingTeams;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_GrantLossToAllTeams() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_GrantLossToAllTeams)
		return ServerOffsets::AFortGameModeAthena_GrantLossToAllTeams;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 48 8B D9 72 ? 48 8D 05 ? ? ? ? 41 B9 ? ? ? ? 4C 8D 05 ? ? ? ? 48 89 44 24 ? 33 D2 33 C9 E8 ? ? ? ? 33 D2 48 8B CB 48 83 C4").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_GrantLossToAllTeams = Addr - ImageBase;
		Log("AFortGameModeAthena_GrantLossToAllTeams found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_GrantLossToAllTeams));
		return ServerOffsets::AFortGameModeAthena_GrantLossToAllTeams;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_GrantLosses() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_GrantLosses)
		return ServerOffsets::AFortGameModeAthena_GrantLosses;
	Addr = Memcury::Scanner::FindPattern("88 54 24 ? 56 41 55").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_GrantLosses = Addr - ImageBase;
		Log("AFortGameModeAthena_GrantLosses found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_GrantLosses));
		return ServerOffsets::AFortGameModeAthena_GrantLosses;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_ChoosePlayerStart_Creative() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_ChoosePlayerStart_Creative)
		return ServerOffsets::AFortGameModeAthena_ChoosePlayerStart_Creative;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 33 F6 4C 89 44 24 ? 44 89 74 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_ChoosePlayerStart_Creative = Addr - ImageBase;
		Log("AFortGameModeAthena_ChoosePlayerStart_Creative found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_ChoosePlayerStart_Creative));
		return ServerOffsets::AFortGameModeAthena_ChoosePlayerStart_Creative;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_StartWarmupPhase() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartWarmupPhase)
		return ServerOffsets::AFortGameModeAthena_StartWarmupPhase;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B B9 ? ? ? ? 48 8B D9 48 85 FF 0F 84").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartWarmupPhase = Addr - ImageBase;
		Log("AFortGameModeAthena_StartWarmupPhase found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartWarmupPhase));
		return ServerOffsets::AFortGameModeAthena_StartWarmupPhase;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_UpdateWarmupPhase() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_UpdateWarmupPhase)
		return ServerOffsets::AFortGameModeAthena_UpdateWarmupPhase;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B B9 ? ? ? ? 48 8B D9 0F 29 B4 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_UpdateWarmupPhase = Addr - ImageBase;
		Log("AFortGameModeAthena_UpdateWarmupPhase found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_UpdateWarmupPhase));
		return ServerOffsets::AFortGameModeAthena_UpdateWarmupPhase;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_StartAircraftPhase() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartAircraftPhase)
		return ServerOffsets::AFortGameModeAthena_StartAircraftPhase;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Engine_Version < 4.27)
		{
			auto sRef = Memcury::Scanner::FindStringRef(L"STARTAIRCRAFT").Get();

			if (!sRef)
				return 0;

			int numCalls = 0;

			for (int i = 0; i < 150; i++) {
				if (*(uint8_t*)(sRef + i) == 0xE8)
				{
					if (++numCalls == 2) {
						Addr = Memcury::Scanner(sRef + i).RelativeOffset(1).Get();
						break;
					}
					else {
						i += 4;
					}
				}
			}
		}
		else
		{
			auto sRef = Memcury::Scanner::FindStringRef(L"STAT_StartAircraftPhase").Get();

			if (!sRef)
				return 0;

			for (int i = 0; i < 1000; i++) {
				if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4) {
					Addr = sRef - i;
					break;
				}
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartAircraftPhase = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_StartAircraftPhase found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartAircraftPhase));
	return ServerOffsets::AFortGameModeAthena_StartAircraftPhase;
}

uintptr_t Finder::FindAFortGameModeAthena_StartSafeZonesPhase() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartSafeZonesPhase)
		return ServerOffsets::AFortGameModeAthena_StartSafeZonesPhase;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 53 55 56 57 48 81 EC ? ? ? ? 48 8B B1").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartSafeZonesPhase = Addr - ImageBase;
		Log("AFortGameModeAthena_StartSafeZonesPhase found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartSafeZonesPhase));
		return ServerOffsets::AFortGameModeAthena_StartSafeZonesPhase;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_IsSafeZoneAllowed() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_IsSafeZoneAllowed)
		return ServerOffsets::AFortGameModeAthena_IsSafeZoneAllowed;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 83 EC ? 48 8B 99 ? ? ? ? 45 0F B6 F8").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_IsSafeZoneAllowed = Addr - ImageBase;
		Log("AFortGameModeAthena_IsSafeZoneAllowed found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_IsSafeZoneAllowed));
		return ServerOffsets::AFortGameModeAthena_IsSafeZoneAllowed;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_StartEndGamePhase() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartEndGamePhase)
		return ServerOffsets::AFortGameModeAthena_StartEndGamePhase;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 4C 89 48 ? 4C 89 40 ? 55 41 56 48 8D 68").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartEndGamePhase = Addr - ImageBase;
		Log("AFortGameModeAthena_StartEndGamePhase found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartEndGamePhase));
		return ServerOffsets::AFortGameModeAthena_StartEndGamePhase;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_StartEndGamePhaseTeam() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_StartEndGamePhaseTeam)
		return ServerOffsets::AFortGameModeAthena_StartEndGamePhaseTeam;
	Addr = Memcury::Scanner::FindPattern("89 54 24 ? 55 41 54 41 56 41 57 48 81 EC").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_StartEndGamePhaseTeam = Addr - ImageBase;
		Log("AFortGameModeAthena_StartEndGamePhaseTeam found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_StartEndGamePhaseTeam));
		return ServerOffsets::AFortGameModeAthena_StartEndGamePhaseTeam;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_CreateAIDirector() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_CreateAIDirector)
		return ServerOffsets::AFortGameModeAthena_CreateAIDirector;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B D9 48 85 C0 74 ? F6 40 ? ? 74 ? E8").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_CreateAIDirector = Addr - ImageBase;
		Log("AFortGameModeAthena_CreateAIDirector found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_CreateAIDirector));
		return ServerOffsets::AFortGameModeAthena_CreateAIDirector;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_PostReloadConfig() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_PostReloadConfig)
		return ServerOffsets::AFortGameModeAthena_PostReloadConfig;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 8B 41 ? 48 8B D9 C1 E8 ? A8 ? 75 ? 48 8B 89 ? ? ? ? 48 85 C9").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_PostReloadConfig = Addr - ImageBase;
		Log("AFortGameModeAthena_PostReloadConfig found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_PostReloadConfig));
		return ServerOffsets::AFortGameModeAthena_PostReloadConfig;
	}
	return 0;
}

uintptr_t Finder::FindAFortGameModeAthena_ReadyToStartMatch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_ReadyToStartMatch)
		return ServerOffsets::AFortGameModeAthena_ReadyToStartMatch;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? F6 81 ? ? ? ? ? 74 ? 4C 8B 81 ? ? ? ? 49 8B C0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? F6 81 ? ? ? ? ? 48 8B D9 74 ? 48 8B 05").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameModeAthena_ReadyToStartMatch = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_ReadyToStartMatch found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_ReadyToStartMatch));
	return ServerOffsets::AFortGameModeAthena_ReadyToStartMatch;
}

uintptr_t Finder::FindAFortGameStateAthena_OnRep_CurrentPlaylistInfo() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameStateAthena_OnRep_CurrentPlaylistInfo)
		return ServerOffsets::AFortGameStateAthena_OnRep_CurrentPlaylistInfo;
	Addr = Memcury::Scanner::FindPattern("40 56 48 81 EC ? ? ? ? 48 89 6C 24").Get();
	if (Addr) {
		ServerOffsets::AFortGameStateAthena_OnRep_CurrentPlaylistInfo = Addr - ImageBase;
		Log("AFortGameStateAthena_OnRep_CurrentPlaylistInfo found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameStateAthena_OnRep_CurrentPlaylistInfo));
		return ServerOffsets::AFortGameStateAthena_OnRep_CurrentPlaylistInfo;
	}
	return 0;
}

uintptr_t Finder::FindAGameMode_InitGame() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameMode_InitGame)
		return ServerOffsets::AGameMode_InitGame;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 07").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B 03 48 8B CB 48 8B 15").Get();
	}

	if (Addr) {
		ServerOffsets::AGameMode_InitGame = Addr - ImageBase;
	}

	Log("AGameMode_InitGame found at: 0x" + std::format("{:X}", ServerOffsets::AGameMode_InitGame));
	return ServerOffsets::AGameMode_InitGame;
}

uintptr_t Finder::FindAFortGameMode_InitGame() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_InitGame)
		return ServerOffsets::AFortGameMode_InitGame;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 B9").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 4C 89 64 24 ? 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 01").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameMode_InitGame = Addr - ImageBase;
	}

	Log("AFortGameMode_InitGame found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_InitGame));
	return ServerOffsets::AFortGameMode_InitGame;
}

uintptr_t Finder::FindAFortGameModeZone_InitGame() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_InitGame)
		return ServerOffsets::AFortGameModeZone_InitGame;

	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 53 57 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 70 ? 48 8B F9 4C 89 60 ? 4D 8B E0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 45 33 E4 49 8B F8").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameModeZone_InitGame = Addr - ImageBase;
	}

	Log("AFortGameModeZone_InitGame found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_InitGame));
	return ServerOffsets::AFortGameModeZone_InitGame;
}

uintptr_t Finder::FindAFortGameModeAthena_InitGame() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_InitGame)
		return ServerOffsets::AFortGameModeAthena_InitGame;

	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 58 ? 48 8B F9").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 45 33 FF 49 8B D9").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameModeAthena_InitGame = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_InitGame found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_InitGame));
	return ServerOffsets::AFortGameModeAthena_InitGame;
}

uintptr_t Finder::FindAFortGameModeZone_InitGameState() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_InitGameState)
		return ServerOffsets::AFortGameModeZone_InitGameState;

	Addr = Memcury::Scanner::FindPattern("40 53 56 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B B3").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 55 41 56 48 83 EC ? 48 8B E9").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameModeZone_InitGameState = Addr - ImageBase;
	}

	Log("AFortGameModeZone_InitGameState found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_InitGameState));
	return ServerOffsets::AFortGameModeZone_InitGameState;
}

uintptr_t Finder::FindAFortGameModeAthena_InitGameState() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_InitGameState)
		return ServerOffsets::AFortGameModeAthena_InitGameState;

	Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B B7 ? ? ? ? 48 85 F6 0F 84 ? ? ? ? E8 ? ? ? ? 48 8B 56 ? 4C 8D 40 ? 48 63 40 ? 3B 42 ? 0F 8F ? ? ? ? 48 8B C8 48 8B 42 ? 4C 39 04 C8 0F 85 ? ? ? ? 48 8D 4C 24").Get();

	if (Addr) {
		ServerOffsets::AFortGameModeAthena_InitGameState = Addr - ImageBase;
	}

	if (!ServerOffsets::AFortGameModeAthena_InitGameState) {
		if (Version::Fortnite_CL == 3700114) {
			ServerOffsets::AFortGameModeAthena_InitGameState = 0x57D710;
		}
	}

	Log("AFortGameModeAthena_InitGameState found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_InitGameState));
	return ServerOffsets::AFortGameModeAthena_InitGameState;
}

uintptr_t Finder::FindUFortPlaylistManager_InitializePlaylists() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UFortPlaylistManager_InitializePlaylists)
		return ServerOffsets::UFortPlaylistManager_InitializePlaylists;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 F6 48 89 75").Get();
	if (Addr) {
		ServerOffsets::UFortPlaylistManager_InitializePlaylists = Addr - ImageBase;
	}
	Log("UFortPlaylistManager_InitializePlaylists found at: 0x" + std::format("{:X}", ServerOffsets::UFortPlaylistManager_InitializePlaylists));
	return ServerOffsets::UFortPlaylistManager_InitializePlaylists;
}

uintptr_t Finder::FindUNetDriver_TickFlush() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetDriver_TickFlush)
		return ServerOffsets::UNetDriver_TickFlush;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		Addr = Version::Engine_Version == 4.16 ? Memcury::Scanner::FindPattern("4C 8B DC 55 53 56 57 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 41 0F 29 7B").Get() : 0;

		if (Version::Engine_Version == 4.19)
			Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 45 0F 29 43 ? 45 0F 29 4B ? 48 8B 05 ? ? ? ? 48").Get();

		else if (Version::Engine_Version >= 4.27 && Version::Engine_Version < 5.0)
		{
			Addr = Memcury::Scanner::FindPattern(
				"48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 8A")
				.Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 48 8B "
					"05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F")
				.Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 48 8B "
					"05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F9 48 89 4D 38 48 8D 4D 40")
				.Get();
		}
		else if (!Addr)
		{
			auto sRef = Memcury::Scanner::FindStringRef(L"STAT_NetTickFlush", false).Get();
			if (!sRef && Version::Engine_Version == 4.20) {
				Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 45 0F 29 43 ? 45 0F 29 4B ? 48 8B 05 ? ? ? ? 48").Get();
			}
			else {
				for (int i = 0; i < 1000; i++)
				{
					auto Ptr = (uint8_t*)(sRef - i);

					if (*Ptr == 0x48 && *(Ptr + 1) == 0x8b && *(Ptr + 2) == 0xc4)
					{
						Addr = uint64_t(Ptr);
						break;
					}
					else if (*Ptr == 0x4c && *(Ptr + 1) == 0x8b && *(Ptr + 2) == 0xdc)
					{
						Addr = uint64_t(Ptr);
						break;
					}
					else if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c)
					{
						Addr = uint64_t(Ptr);
						break;
					}
					else if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
					{
						Addr = uint64_t(Ptr);
						break;
					}
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver_TickFlush = Addr - ImageBase;
	}

	Log("UNetDriver_TickFlush found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_TickFlush));
	return ServerOffsets::UNetDriver_TickFlush;
}

uintptr_t Finder::FindUReplicationDriver_ServerReplicateActors() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UReplicationDriver_ServerReplicateActors)
		return ServerOffsets::UReplicationDriver_ServerReplicateActors;
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 41 ? 49 89 5B ? 49 89 7B").Get();
	if (Addr) {
		ServerOffsets::UReplicationDriver_ServerReplicateActors = Addr - ImageBase;
	}
	Log("UReplicationDriver_ServerReplicateActors found at: 0x" + std::format("{:X}", ServerOffsets::UReplicationDriver_ServerReplicateActors));
	return ServerOffsets::UReplicationDriver_ServerReplicateActors;
}

uintptr_t Finder::FindAFortWorldManager_HandleWorldQueryComplete() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortWorldManager_HandleWorldQueryComplete)
		return ServerOffsets::AFortWorldManager_HandleWorldQueryComplete;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 53 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 70 ? 48 8B F9 4C 89 60 ? 4C 89 68").Get();
	if (Addr) {
		ServerOffsets::AFortWorldManager_HandleWorldQueryComplete = Addr - ImageBase;
	}
	Log("AFortWorldManager_HandleWorldQueryComplete found at: 0x" + std::format("{:X}", ServerOffsets::AFortWorldManager_HandleWorldQueryComplete));
	return ServerOffsets::AFortWorldManager_HandleWorldQueryComplete;
}

uintptr_t Finder::FindAFortGameModeAthena_SpawnDefaultPawnFor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_SpawnDefaultPawnFor)
		return ServerOffsets::AFortGameModeAthena_SpawnDefaultPawnFor;

	Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F0 48 8B FA 48 8B F1 48 85 D2").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B FA 48 8B D9 48 85 D2 0F 84 ? ? ? ? E8 ? ? ? ? 48 8B 4F").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameModeAthena_SpawnDefaultPawnFor = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_SpawnDefaultPawnFor found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_SpawnDefaultPawnFor));
	return ServerOffsets::AFortGameModeAthena_SpawnDefaultPawnFor;
}

uintptr_t Finder::FindAGameModeBase_SpawnDefaultPawnFor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_SpawnDefaultPawnFor)
		return ServerOffsets::AGameModeBase_SpawnDefaultPawnFor;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 55 56 41 56 49 8D 6B ? 48 81 EC ? ? ? ? 41 0F 29 73").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B 98").Get();
	}

	if (Addr) {
		ServerOffsets::AGameModeBase_SpawnDefaultPawnFor = Addr - ImageBase;
	}

	Log("AGameModeBase_SpawnDefaultPawnFor found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_SpawnDefaultPawnFor));
	return ServerOffsets::AGameModeBase_SpawnDefaultPawnFor;
}

uintptr_t Finder::FindAGameModeBase_SpawnDefaultPawnAtTransform() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_SpawnDefaultPawnAtTransform)
		return ServerOffsets::AGameModeBase_SpawnDefaultPawnAtTransform;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F9 49 8B F0").Get();
	if (Addr) {
		ServerOffsets::AGameModeBase_SpawnDefaultPawnAtTransform = Addr - ImageBase;
	}
	Log("AGameModeBase_SpawnDefaultPawnAtTransform found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_SpawnDefaultPawnAtTransform));
	return ServerOffsets::AGameModeBase_SpawnDefaultPawnAtTransform;
}

uintptr_t Finder::FindAGameModeBase_RestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_RestartPlayer)
		return ServerOffsets::AGameModeBase_RestartPlayer;
	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 56 48 83 EC ? F6 42 ? ? 48 8B DA").Get();
	if (Addr) {
		ServerOffsets::AGameModeBase_RestartPlayer = Addr - ImageBase;
	}
	Log("AGameModeBase_RestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_RestartPlayer));
	return ServerOffsets::AGameModeBase_RestartPlayer;
}

uintptr_t Finder::FindAFortGameMode_RestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_RestartPlayer)
		return ServerOffsets::AFortGameMode_RestartPlayer;
	Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 48 89 7C 24 ? 4C 89 64 24 ? 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B 01").Get();
	if (Addr) {
		ServerOffsets::AFortGameMode_RestartPlayer = Addr - ImageBase;
	}
	Log("AFortGameMode_RestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_RestartPlayer));
	return ServerOffsets::AFortGameMode_RestartPlayer;
}

uintptr_t Finder::FindAFortGameModeAthena_RestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_RestartPlayer)
		return ServerOffsets::AFortGameModeAthena_RestartPlayer;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 89 74 24 ? 48 8B F1 48 8B DA").Get();
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_RestartPlayer = Addr - ImageBase;
	}
	Log("AFortGameModeAthena_RestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_RestartPlayer));
	return ServerOffsets::AFortGameModeAthena_RestartPlayer;
}

uintptr_t Finder::FindAGameModeBase_FinishRestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_FinishRestartPlayer)
		return ServerOffsets::AGameModeBase_FinishRestartPlayer;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 48 8B 92").Get();
	if (Addr) {
		ServerOffsets::AGameModeBase_FinishRestartPlayer = Addr - ImageBase;
	}
	Log("AGameModeBase_FinishRestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_FinishRestartPlayer));
	return ServerOffsets::AGameModeBase_FinishRestartPlayer;
}

uintptr_t Finder::FindAGameModeBase_FinishRestartPlayerVFT() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_FinishRestartPlayerVFT)
		return ServerOffsets::AGameModeBase_FinishRestartPlayerVFT;

	if (Version::Engine_Version == 4.16) {
		Addr = 0xF3;
	}
	
	if (Addr) {
		ServerOffsets::AGameModeBase_FinishRestartPlayerVFT = Addr;
	}

	Log("AGameModeBase_FinishRestartPlayerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_FinishRestartPlayerVFT));
	return ServerOffsets::AGameModeBase_FinishRestartPlayerVFT;
}

uintptr_t Finder::FindAFortGameMode_FinishRestartPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_FinishRestartPlayer)
		return ServerOffsets::AFortGameMode_FinishRestartPlayer;
	Addr = Memcury::Scanner::FindPattern("40 53 57 41 56 48 83 EC ? 48 8B DA").Get();
	if (Addr) {
		ServerOffsets::AFortGameMode_FinishRestartPlayer = Addr - ImageBase;
	}
	Log("AFortGameMode_FinishRestartPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_FinishRestartPlayer));
	return ServerOffsets::AFortGameMode_FinishRestartPlayer;
}

uintptr_t Finder::FindAPlayerController_ServerAcknowledgePossession() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::APlayerController_ServerAcknowledgePossession)
		return ServerOffsets::APlayerController_ServerAcknowledgePossession;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 3D ? ? ? ? ? 48 8B DA 48 8B F9 0F 82").Get();
	if (Addr) {
		ServerOffsets::APlayerController_ServerAcknowledgePossession = Addr - ImageBase;
	}
	Log("APlayerController_ServerAcknowledgePossession found at: 0x" + std::format("{:X}", ServerOffsets::APlayerController_ServerAcknowledgePossession));
	return ServerOffsets::APlayerController_ServerAcknowledgePossession;
}

uintptr_t Finder::FindUAbilitySystemComponent_ClientActivateAbilityFailed() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UAbilitySystemComponent_ClientActivateAbilityFailed)
		return ServerOffsets::UAbilitySystemComponent_ClientActivateAbilityFailed;
	
	if (Version::Engine_Version == 4.16) {
		Addr = Memcury::Scanner::FindPattern("89 54 24 ? 55 53 57 41 56 48 8B EC").Get();
	}
	
	if (Addr) {
		ServerOffsets::UAbilitySystemComponent_ClientActivateAbilityFailed = Addr - ImageBase;
	}

	Log("UAbilitySystemComponent_ClientActivateAbilityFailed found at: 0x" + std::format("{:X}", ServerOffsets::UAbilitySystemComponent_ClientActivateAbilityFailed));
	return ServerOffsets::UAbilitySystemComponent_ClientActivateAbilityFailed;
}

uintptr_t Finder::FindAGameModeBase_HandleStartingNewPlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AGameModeBase_HandleStartingNewPlayer)
		return ServerOffsets::AGameModeBase_HandleStartingNewPlayer;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? F6 81 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? E8 ? ? ? ? 84 C0 75 ? 48 8B D7").Get();
	if (Addr) {
		ServerOffsets::AGameModeBase_HandleStartingNewPlayer = Addr - ImageBase;
	}

	Log("AGameModeBase_HandleStartingNewPlayer found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_HandleStartingNewPlayer));
	return ServerOffsets::AGameModeBase_HandleStartingNewPlayer;
}

uintptr_t Finder::FindULevelStreamingDynamic_LoadLevelInstance() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreamingDynamic_LoadLevelInstance)
		return ServerOffsets::ULevelStreamingDynamic_LoadLevelInstance;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B BC 24 ? ? ? ? 48 8B DA 4D 8B F0 48 8B D1 41 B8 ? ? ? ? 49 8B E9 C6 07 ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 48 85 C0 74 ? 33 C0").Get();
	if (Addr) {
		ServerOffsets::ULevelStreamingDynamic_LoadLevelInstance = Addr - ImageBase;
	}
	Log("ULevelStreamingDynamic_LoadLevelInstance found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreamingDynamic_LoadLevelInstance));
	return ServerOffsets::ULevelStreamingDynamic_LoadLevelInstance;
}

uintptr_t Finder::FindULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr)
		return ServerOffsets::ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B BC 24 ? ? ? ? 48 8B DA 4D 8B F0 48 8B D1 41 B8 ? ? ? ? 49 8B E9 C6 07 ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 48 85 C0 74 ? 83 7B").Get();
	if (Addr) {
		ServerOffsets::ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr = Addr - ImageBase;
	}
	Log("ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr found at: 0x" + std::format("{:X}", ServerOffsets::ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr));
	return ServerOffsets::ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr;
}

uintptr_t Finder::FindAFortGameStateAthena_OnPlaylistDataLoadCompleted() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameStateAthena_OnPlaylistDataLoadCompleted)
		return ServerOffsets::AFortGameStateAthena_OnPlaylistDataLoadCompleted;
	Addr = Memcury::Scanner::FindPattern("40 55 53 41 55 48 8B EC 48 81 EC ? ? ? ? 48 8B 99").Get();
	if (Addr) {
		ServerOffsets::AFortGameStateAthena_OnPlaylistDataLoadCompleted = Addr - ImageBase;
	}
	Log("AFortGameStateAthena_OnPlaylistDataLoadCompleted found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameStateAthena_OnPlaylistDataLoadCompleted));
	return ServerOffsets::AFortGameStateAthena_OnPlaylistDataLoadCompleted;
}

uintptr_t Finder::FindAFortGameStateAthena_UpdatePlaylistDependentData() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameStateAthena_UpdatePlaylistDependentData)
		return ServerOffsets::AFortGameStateAthena_UpdatePlaylistDependentData;
	Addr = Memcury::Scanner::FindPattern("40 55 56 41 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B A9").Get();
	if (Addr) {
		ServerOffsets::AFortGameStateAthena_UpdatePlaylistDependentData = Addr - ImageBase;
	}
	Log("AFortGameStateAthena_UpdatePlaylistDependentData found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameStateAthena_UpdatePlaylistDependentData));
	return ServerOffsets::AFortGameStateAthena_UpdatePlaylistDependentData;
}

uintptr_t Finder::FindUGameplayStatics_BeginDeferredActorSpawnFromClass() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UGameplayStatics_BeginDeferredActorSpawnFromClass)
		return ServerOffsets::UGameplayStatics_BeginDeferredActorSpawnFromClass;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UGameplayStatics::BeginSpawningActorFromClass: can not spawn an actor from a NULL class").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = reinterpret_cast<uintptr_t>(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UGameplayStatics_BeginDeferredActorSpawnFromClass = Addr - ImageBase;
	}

	Log("UGameplayStatics_BeginDeferredActorSpawnFromClass found at: 0x" + std::format("{:X}", ServerOffsets::UGameplayStatics_BeginDeferredActorSpawnFromClass));
	return ServerOffsets::UGameplayStatics_BeginDeferredActorSpawnFromClass;
}

uintptr_t Finder::FindAActor_FinishSpawning() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_FinishSpawning)
		return ServerOffsets::AActor_FinishSpawning;

	Addr = Memcury::Scanner::FindPattern("40 55 53 56 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F B6 81").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 55 53 56 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F B6 81").Get();
	}

	if (Addr) {
		ServerOffsets::AActor_FinishSpawning = Addr - ImageBase;
	}

	Log("AActor_FinishSpawning found at: 0x" + std::format("{:X}", ServerOffsets::AActor_FinishSpawning));
	return ServerOffsets::AActor_FinishSpawning;
}

uintptr_t Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid)
		return ServerOffsets::UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid;
	
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC ? 49 8B E9 4D 8B F0", false).Get();

		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 55 56 41 56 48 83 EC 50", false).Get();

		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 4D 8B F1 4D 8B F8", false).Get();

		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 49 8B E9 4D 8B F8 48 8B DA 48 8B F9 BE ? ? ? ? 48", false).Get();

		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 70 49 8B E9 4D 8B F8 48 8B DA 48 8B F9").Get();

		if (!Addr)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B 1A 4D 8B F1").Get();
	}
	
	if (Addr) {
		ServerOffsets::UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid = Addr - ImageBase;
	}

	Log("UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid found at: 0x" + std::format("{:X}", ServerOffsets::UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid));
	return ServerOffsets::UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid;
}

uintptr_t Finder::FindABuildingSMActor_ReplaceBuildingActor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::ABuildingSMActor_ReplaceBuildingActor)
		return ServerOffsets::ABuildingSMActor_ReplaceBuildingActor;
	
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		auto sRef = Memcury::Scanner::FindStringRef(L"STAT_Fort_BuildingSMActorReplaceBuildingActor", false);
		if (!sRef.Get()) {
			Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 56 57 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 45").Get();
		}
		else {
			Addr = sRef.ScanFor(Version::Engine_Version == 4.20 || (Version::Engine_Version == 4.21 && Version::Fortnite_Version < 6.30) || Version::Engine_Version >= 4.27 ? std::vector<uint8_t> { 0x48, 0x8B, 0xC4 } : std::vector<uint8_t>{ 0x4C, 0x8B }, false, 0).Get();
		}
	}
	
	if (Addr) {
		ServerOffsets::ABuildingSMActor_ReplaceBuildingActor = Addr - ImageBase;
	}

	Log("ABuildingSMActor_ReplaceBuildingActor found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingSMActor_ReplaceBuildingActor));
	return ServerOffsets::ABuildingSMActor_ReplaceBuildingActor;
}

uintptr_t Finder::FindABuildingSMActor_ReplaceBuildingActorVFT() {
	if (ServerOffsets::ABuildingSMActor_ReplaceBuildingActorVFT)
		return ServerOffsets::ABuildingSMActor_ReplaceBuildingActorVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.BuildingSMActor"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindABuildingSMActor_ReplaceBuildingActor() + ImageBase))
		{
			ServerOffsets::ABuildingSMActor_ReplaceBuildingActorVFT = i;
			break;
		}
	}

	Log("ABuildingSMActor_ReplaceBuildingActorVFT found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingSMActor_ReplaceBuildingActorVFT));
	return ServerOffsets::ABuildingSMActor_ReplaceBuildingActorVFT;
}

uintptr_t Finder::FindAFortGameMode_SpawnDefaultPawnFor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_SpawnDefaultPawnFor)
		return ServerOffsets::AFortGameMode_SpawnDefaultPawnFor;

	Addr = Memcury::Scanner::FindPattern("40 55 56 57 41 54 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4D 8B F0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 55 57 41 54 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 49 8B F8").Get();
	}

	if (Addr) {
		ServerOffsets::AFortGameMode_SpawnDefaultPawnFor = Addr - ImageBase;
	}

	Log("AFortGameMode_SpawnDefaultPawnFor found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_SpawnDefaultPawnFor));
	return ServerOffsets::AFortGameMode_SpawnDefaultPawnFor;
}

uintptr_t Finder::FindFRotator_Quaternion() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FRotator_Quaternion)
		return ServerOffsets::FRotator_Quaternion;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 0F 28 2D ? ? ? ? F3 0F 10 51").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 83 EC ? F3 0F 10 41 ? 0F 57 C9 F3 0F 10 51 ? F3 0F 10 19").Get();
	}

	if (Addr) {
		ServerOffsets::FRotator_Quaternion = Addr - ImageBase;
	}

	Log("FRotator_Quaternion found at: 0x" + std::format("{:X}", ServerOffsets::FRotator_Quaternion));
	return ServerOffsets::FRotator_Quaternion;
}

uintptr_t Finder::FindAActor_SetNetDormancy() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_SetNetDormancy)
		return ServerOffsets::AActor_SetNetDormancy;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? 8B F2 48 8B D9 8B 89").Get();
	if (Addr) {
		ServerOffsets::AActor_SetNetDormancy = Addr - ImageBase;
	}
	Log("AActor_SetNetDormancy found at: 0x" + std::format("{:X}", ServerOffsets::AActor_SetNetDormancy));
	return ServerOffsets::AActor_SetNetDormancy;
}

uintptr_t Finder::FindUAbilitySystemComponent_GiveAbilityAndActivateOnce() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UAbilitySystemComponent_GiveAbilityAndActivateOnce)
		return ServerOffsets::UAbilitySystemComponent_GiveAbilityAndActivateOnce;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Engine_Version == 4.26)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 49 8B 40 10 49 8B D8 48 8B FA 48 8B F1", false).Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 40 49 8B 40 10 49").Get();
		}
		else {
			auto sRef = Memcury::Scanner::FindStringRef(L"GiveAbilityAndActivateOnce called on ability %s on the client, not allowed!", true, 0, Version::Engine_Version >= 5.0).Get();

			for (int i = 0; i < 1000; i++)
			{
				if (*(uint8_t*)(sRef - i) == 0x40 && *(uint8_t*)(sRef - i + 1) == 0x55) {
					Addr = sRef - i;
					break;
				}
				else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C) {
					Addr = sRef - i;
					break;
				}
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::UAbilitySystemComponent_GiveAbilityAndActivateOnce = Addr - ImageBase;
	}

	Log("UAbilitySystemComponent_GiveAbilityAndActivateOnce found at: 0x" + std::format("{:X}", ServerOffsets::UAbilitySystemComponent_GiveAbilityAndActivateOnce));
	return ServerOffsets::UAbilitySystemComponent_GiveAbilityAndActivateOnce;
}

uintptr_t Finder::FindAFortPickup_FinishedTargetSpline() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_FinishedTargetSpline)
		return ServerOffsets::AFortPickup_FinishedTargetSpline;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"%s did not have PickupTargetInventoryOwnerInterface to pickup %s, already dead: %d").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x4C && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xDC) {
				Addr = reinterpret_cast<uintptr_t>(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortPickup_FinishedTargetSpline = Addr - ImageBase;
	}

	Log("AFortPickup_FinishedTargetSpline found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_FinishedTargetSpline));
	return ServerOffsets::AFortPickup_FinishedTargetSpline;
}

uintptr_t Finder::FindAFortPickup_ForceFinishedTargetSpline() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_ForceFinishedTargetSpline)
		return ServerOffsets::AFortPickup_ForceFinishedTargetSpline;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B F9 48 89 44 24 ? E8 ? ? ? ? 48 8D 54 24 ? 48 8B C8 E8 ? ? ? ? 48 85 C0 74 ? 80 78 ? ? 74 ? 48 8B CF 48 89 74 24 ? E8 ? ? ? ? 48 8D 97 ? ? ? ? 48 8B C8 48 8B F0 E8 ? ? ? ? 48 85 C0 74 ? 48 8B 97 ? ? ? ? 48 8B CE E8 ? ? ? ? 48 8B CF").Get();
	if (Addr) {
		ServerOffsets::AFortPickup_ForceFinishedTargetSpline = Addr - ImageBase;
	}
	Log("AFortPickup_ForceFinishedTargetSpline found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_ForceFinishedTargetSpline));
	return ServerOffsets::AFortPickup_ForceFinishedTargetSpline;
}

uintptr_t Finder::FindAFortPlayerPawn_ForceFinishIncomingPickups() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickups)
		return ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickups;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B 81 ? ? ? ? 48 8B F9 83 E8").Get();
	if (Addr) {
		ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickups = Addr - ImageBase;
	}
	Log("AFortPlayerPawn_ForceFinishIncomingPickups found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickups));
	return ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickups;
}

uintptr_t Finder::FindAFortPlayerPawn_ForceFinishIncomingPickupsVFT() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickupsVFT)
		return ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickupsVFT;
	
	if (Version::Engine_Version == 4.16) {
		Addr = 0x194;
	}

	if (Addr) {
		ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickupsVFT = Addr;
	}

	Log("AFortPlayerPawn_ForceFinishIncomingPickupsVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickupsVFT));
	return ServerOffsets::AFortPlayerPawn_ForceFinishIncomingPickupsVFT;
}

uintptr_t Finder::FindAFortPickup_SetPickupTarget() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_SetPickupTarget)
		return ServerOffsets::AFortPickup_SetPickupTarget;

	auto sRef = Memcury::Scanner::FindStringRef(L"%s: Attempted to spawn non-world item %s!", false, 0, Version::Fortnite_Version >= 17, false);

	if (!sRef.IsValid())
		sRef = Memcury::Scanner::FindStringRef(L"Attempted to spawn non-world item %s!", false, 0, Version::Fortnite_Version >= 17, false);

	for (int i = 0; i < 0x1500; i++)
	{
		auto Ptr = (uint8_t*)(sRef.Get() - i);

		if (*Ptr == 0x40 && (*(Ptr + 1) == 0x53 || *(Ptr + 1) == 0x55)) {
			Addr = uint64_t(Ptr);
			break;
		}
		else if (*Ptr == 0x48 && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xC4) {
			Addr = uint64_t(Ptr);
			break;
		}
		else if (*Ptr == 0x4C && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xDC) {
			Addr = uint64_t(Ptr);
			break;
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortPickup_SetPickupTarget = Addr - ImageBase;
	}

	Log("AFortPickup_SetPickupTarget found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_SetPickupTarget));
	return ServerOffsets::AFortPickup_SetPickupTarget;
}

uintptr_t Finder::FindAFortPickup_UpdateSpecialActorStat() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_UpdateSpecialActorStat)
		return ServerOffsets::AFortPickup_UpdateSpecialActorStat;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? 80 B9 ? ? ? ? ? 0F B6 F2 0F 29 74 24 ? 48 8B D9 0F 28 F2 0F 85 ? ? ? ? 48 89 7C 24").Get();
	if (Addr) {
		ServerOffsets::AFortPickup_UpdateSpecialActorStat = Addr - ImageBase;
	}
	Log("AFortPickup_UpdateSpecialActorStat found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_UpdateSpecialActorStat));
	return ServerOffsets::AFortPickup_UpdateSpecialActorStat;
}

uintptr_t Finder::FindAFortPickup_GivePickupTo() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_GivePickupTo)
		return ServerOffsets::AFortPickup_GivePickupTo;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 41 0F B6 F8 48 8B D9 75 ? 48 85 D2 74 ? F3 0F 10 15").Get();
	if (Addr) {
		ServerOffsets::AFortPickup_GivePickupTo = Addr - ImageBase;
	}
	Log("AFortPickup_GivePickupTo found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_GivePickupTo));
	return ServerOffsets::AFortPickup_GivePickupTo;
}

uintptr_t Finder::FindAFortPickup_GivePickupToVFT() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_GivePickupToVFT)
		return ServerOffsets::AFortPickup_GivePickupToVFT;
	
	uintptr_t BaseAddr = Finder::FindAFortPickup_FinishedTargetSpline();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0x49 && *(Ptr + 1) == 0xFF) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset) / 8;
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortPickup_GivePickupToVFT = Addr;
	}

	Log("AFortPickup_GivePickupToVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_GivePickupToVFT));
	return ServerOffsets::AFortPickup_GivePickupToVFT;
}

uintptr_t Finder::FindAFortQuickBars_GetFocusedQuickBar() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortQuickBars_GetFocusedQuickBar)
		return ServerOffsets::AFortQuickBars_GetFocusedQuickBar;
	Addr = Memcury::Scanner::FindPattern("32 C0 48 8B D1 3C").Get();
	if (Addr) {
		ServerOffsets::AFortQuickBars_GetFocusedQuickBar = Addr - ImageBase;
	}
	Log("AFortQuickBars_GetFocusedQuickBar found at: 0x" + std::format("{:X}", ServerOffsets::AFortQuickBars_GetFocusedQuickBar));
	return ServerOffsets::AFortQuickBars_GetFocusedQuickBar;
}

uintptr_t Finder::FindABuildingContainer_SpawnLootVFT() {
	if (ServerOffsets::ABuildingContainer_SpawnLootVFT)
		return ServerOffsets::ABuildingContainer_SpawnLootVFT;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ABuildingContainer::ServerOnAttemptInteract %s failed for %s").Get();

	if (StringAddr)
	{
		for (int i = 0; i < 300; i++)
		{
			if (*(uint8*)(StringAddr - i + 0) == 0x41 && *(uint8*)(StringAddr - i + 1) == 0xff)
			{
				ServerOffsets::ABuildingContainer_SpawnLootVFT = *(uint32_t*)(StringAddr - i + 2) / 8;
			}
		}
	}
	else
	{
		uintptr_t ServerOnAttemptInteract_Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 0F B6 02 48 8B D9 FE C8").Get();

		for (int i = 0; i < 150; i++)
		{
			if (*(uint8*)(ServerOnAttemptInteract_Addr + i + 0) == 0x41 && *(uint8*)(ServerOnAttemptInteract_Addr + i + 1) == 0xff)
			{
				ServerOffsets::ABuildingContainer_SpawnLootVFT = *(uint32_t*)(ServerOnAttemptInteract_Addr + i + 3) / 8;
			}
		}
	}

	Log("ABuildingContainer_SpawnLootVFT found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingContainer_SpawnLootVFT));
	return ServerOffsets::ABuildingContainer_SpawnLootVFT;
}

uintptr_t Finder::FindAFortPickup_SetupForMovementCompToss() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_SetupForMovementCompToss)
		return ServerOffsets::AFortPickup_SetupForMovementCompToss;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 0F B6 81 ? ? ? ? 48 8B D9 3C ? 0F 85 ? ? ? ? 80 B9 ? ? ? ? ? 0F 85 ? ? ? ? 0F B6 D0").Get();
	if (Addr) {
		ServerOffsets::AFortPickup_SetupForMovementCompToss = Addr - ImageBase;
	}
	Log("AFortPickup_SetupForMovementCompToss found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_SetupForMovementCompToss));
	return ServerOffsets::AFortPickup_SetupForMovementCompToss;
}

uintptr_t Finder::FindAFortPickup_SetPickupItems() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_SetPickupItems)
		return ServerOffsets::AFortPickup_SetPickupItems;
	
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Engine_Version == 4.16 || Version::Engine_Version == 4.19) {
			Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9").Get();
			if (!Addr) {
				Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 80 B9 ? ? ? ? ? 45 0F B6 F1").Get();
			}
		}
		else if (Version::Fortnite_Version <= 3.3) {
			Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9 49 8B F8 48 8B F1 0F 85 ? ? ? ? 48 83 7A").Get();
		}
		else if (Version::Engine_Version == 4.20) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 20 80 B9 ? ? ? ? ? 45 0F B6 F1 49 8B E8").Get();
		}
		else if (Version::Engine_Version == 4.21)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 57 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 41 0F B6", false).Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9").Get();
		}
		else if (Version::Engine_Version == 4.22) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 41 56 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 45 0F B6 F1").Get();
		}
		else if (Version::Engine_Version == 4.23) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 41 56 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 45 0F B6 F1 4D").Get();
		}
		else if (Version::Engine_Version == 4.24 || Version::Engine_Version == 4.25) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 57 41 57 48 83 EC ? 80 B9").Get();
		}
		else if (Version::Engine_Version == 4.26)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 57 41 57 48 83 EC ? 80 B9").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 57 41 57 48 83 EC ? 80 B9").Get();
		}
		else if (Version::Engine_Version == 4.27) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 80 B9 ? ? ? ? ? 45 8A").Get();
		}
		else if (Version::Engine_Version >= 5.0)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 80 B9 ? ? ? ? ? 45 8A F9").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 80 B9 ? ? ? ? ? 45 8A F1").Get();
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortPickup_SetPickupItems = Addr - ImageBase;
	}

	Log("AFortPickup_SetPickupItems found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_SetPickupItems));
	return ServerOffsets::AFortPickup_SetPickupItems;
}

uintptr_t Finder::FindAFortPickup_SetPickupItemsVFT() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPickup_SetPickupItemsVFT)
		return ServerOffsets::AFortPickup_SetPickupItemsVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortPickup"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindAFortPickup_SetPickupItems() + ImageBase))
		{
			Addr = i;
			break;
		}
	}

	if (Addr) {
		ServerOffsets::AFortPickup_SetPickupItemsVFT = Addr;
	}

	Log("AFortPickup_SetPickupItemsVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPickup_SetPickupItemsVFT));
	return ServerOffsets::AFortPickup_SetPickupItemsVFT;
}

uintptr_t Finder::FindAFortInventory_Destroyed() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_Destroyed)
		return ServerOffsets::AFortInventory_Destroyed;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 80 BB ? ? ? ? ? 74 ? 80 3D").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_Destroyed = Addr - ImageBase;
	}
	Log("AFortInventory_Destroyed found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_Destroyed));
	return ServerOffsets::AFortInventory_Destroyed;
}

uintptr_t Finder::FindAFortInventory_DumpInventoryToLog() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_DumpInventoryToLog)
		return ServerOffsets::AFortInventory_DumpInventoryToLog;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 85 D2 0F 84 ? ? ? ? 48 8B 9A").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_DumpInventoryToLog = Addr - ImageBase;
	}
	Log("AFortInventory_DumpInventoryToLog found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_DumpInventoryToLog));
	return ServerOffsets::AFortInventory_DumpInventoryToLog;
}

uintptr_t Finder::FindAFortInventory_GetOverflowFromAddingItem() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_GetOverflowFromAddingItem)
		return ServerOffsets::AFortInventory_GetOverflowFromAddingItem;
	Addr = Memcury::Scanner::FindPattern("40 53 55 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B 05").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_GetOverflowFromAddingItem = Addr - ImageBase;
	}
	Log("AFortInventory_GetOverflowFromAddingItem found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetOverflowFromAddingItem));
	return ServerOffsets::AFortInventory_GetOverflowFromAddingItem;
}

uintptr_t Finder::FindAFortInventory_GetOverflowFromAddingItemInExistingStack() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_GetOverflowFromAddingItemInExistingStack)
		return ServerOffsets::AFortInventory_GetOverflowFromAddingItemInExistingStack;
	Addr = Memcury::Scanner::FindPattern("48 89 4C 24 ? 55 53 56 57 41 55 41 56 48 8B EC 48 83 EC ? 4C 8B 71").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_GetOverflowFromAddingItemInExistingStack = Addr - ImageBase;
	}
	Log("AFortInventory_GetOverflowFromAddingItemInExistingStack found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetOverflowFromAddingItemInExistingStack));
	return ServerOffsets::AFortInventory_GetOverflowFromAddingItemInExistingStack;
}

uintptr_t Finder::FindAFortInventory_GetOverflowFromAddingItemInNewStack() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_GetOverflowFromAddingItemInNewStack)
		return ServerOffsets::AFortInventory_GetOverflowFromAddingItemInNewStack;
	Addr = Memcury::Scanner::FindPattern("44 89 4C 24 ? 44 89 44 24 ? 48 89 4C 24 ? 41 54").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_GetOverflowFromAddingItemInNewStack = Addr - ImageBase;
	}
	Log("AFortInventory_GetOverflowFromAddingItemInNewStack found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetOverflowFromAddingItemInNewStack));
	return ServerOffsets::AFortInventory_GetOverflowFromAddingItemInNewStack;
}

uintptr_t Finder::FindAFortInventory_GetInventoryUsed() {
	if (ServerOffsets::AFortInventory_GetInventoryUsed)
		return ServerOffsets::AFortInventory_GetInventoryUsed;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 33 DB 8B EA 48 8B F1 8B FB 48 85 C9 74 ? ? ? ? FF 50 ? 4C 8B F0 E8 ? ? ? ? 49 8B 4E ? 48 05 ? ? ? ? 48 63 50 ? 3B 91 ? ? ? ? 7F ? 48 8B 89 ? ? ? ? ? ? ? ? 49 0F 44 FE 85 ED 74").Get();

	if (!Addr) {
		uintptr_t execGetBackpackItemCounts_ADDR = (uintptr_t)(((UFunction*)FUObjectArray::FindObject("Function /Script/FortniteUI.FortInventoryContext.GetBackpackItemCounts"))->Func);
		uintptr_t GetBackpackItemCounts_Impl_ADDR = 0x0;

		uintptr_t C3_Point = 0x0; //bro if this exec function didnt have 0xC3 at the end i would fucking die

		for (int i = 0; i < 1000; i++)
		{
			uintptr_t CurrentAddr = execGetBackpackItemCounts_ADDR + i;

			if (*(uint8*)(CurrentAddr) == 0xC3)
			{
				C3_Point = CurrentAddr;
				break;
			}
		}

		for (int i = 0; i < 40; i++)
		{
			uintptr_t CurrentAddr2 = C3_Point - i;

			if (*(uint8*)(CurrentAddr2) == 0xE8) { //look for call
				GetBackpackItemCounts_Impl_ADDR = Utils::GetCallDestination(CurrentAddr2);
				break;
			}
		}

		uint8 Skipped = 0x0;

		for (int i = 0; i < 70; i++)
		{
			//we looking for the second call instruction

			uintptr_t CurrentAddress3 = GetBackpackItemCounts_Impl_ADDR + i;

			if (*(uint8*)(CurrentAddress3) == 0xe8)
			{
				if (Skipped == 1)
				{
					Addr = Utils::GetCallDestination(CurrentAddress3);
					break;
				}
				Skipped++;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortInventory_GetInventoryUsed = Addr - ImageBase;
	}

	Log("AFortInventory_GetInventoryUsed found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetInventoryUsed));
	return ServerOffsets::AFortInventory_GetInventoryUsed;
}

uintptr_t Finder::FindAFortInventory_InitializeExistingItem() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_InitializeExistingItem)
		return ServerOffsets::AFortInventory_InitializeExistingItem;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Failed to find ItemDefinition for WorldItem with Template %s!").Get();
	if (StringAddr)
	{
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0xE8) {
				int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
				uintptr_t Target = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
				Addr = Target;
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortInventory_InitializeExistingItem = Addr - ImageBase;
	}

	Log("AFortInventory_InitializeExistingItem found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_InitializeExistingItem));
	return ServerOffsets::AFortInventory_InitializeExistingItem;
}

uintptr_t Finder::FindAFortInventory_GetMaxStackSize() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortInventory_GetMaxStackSize)
		return ServerOffsets::AFortInventory_GetMaxStackSize;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 33 D2 48 85 C9").Get();
	if (Addr) {
		ServerOffsets::AFortInventory_GetMaxStackSize = Addr - ImageBase;
	}
	Log("AFortInventory_GetMaxStackSize found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetMaxStackSize));
	return ServerOffsets::AFortInventory_GetMaxStackSize;
}

uintptr_t Finder::FindUKismetSystemLibrary_GetEngineVersion() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UKismetSystemLibrary_GetEngineVersion)
		return ServerOffsets::UKismetSystemLibrary_GetEngineVersion;
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3").Get();
	if (!Addr) {
		Memcury::Scanner::FindPattern("2B 2B 46 6F 72 74 6E 69 74 65 2B 52 65 6C 65 61 73 65 2D ? ? ? ?").Get();
	}

	if (Addr) {
		ServerOffsets::UKismetSystemLibrary_GetEngineVersion = Addr - ImageBase;
	}
	Log("UKismetSystemLibrary_GetEngineVersion found at: 0x" + std::format("{:X}", ServerOffsets::UKismetSystemLibrary_GetEngineVersion));
	return ServerOffsets::UKismetSystemLibrary_GetEngineVersion;
}

uintptr_t Finder::FindUEngine_GetWorldContextFromWorld() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_GetWorldContextFromWorld)
		return ServerOffsets::UEngine_GetWorldContextFromWorld;
	Addr = Memcury::Scanner::FindPattern("48 8B 81 ? ? ? ? 48 63 89 ? ? ? ? 48 8D 0C C8 48 3B C1 74 ? 66 0F 1F 84 00 ? ? ? ? 4C 8B 00 49 39 90 ? ? ? ? 74 ? 48 83 C0 ? 48 3B C1 75 ? 33 C0").Get();
	if (Addr) {
		ServerOffsets::UEngine_GetWorldContextFromWorld = Addr - ImageBase;
	}
	Log("UEngine_GetWorldContextFromWorld found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_GetWorldContextFromWorld));
	return ServerOffsets::UEngine_GetWorldContextFromWorld;
}

uintptr_t Finder::FindUEngine_CreateNewWorldContext() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_CreateNewWorldContext)
		return ServerOffsets::UEngine_CreateNewWorldContext;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B F2 48 8B F9 48 8D 91").Get();
	if (Addr) {
		ServerOffsets::UEngine_CreateNewWorldContext = Addr - ImageBase;
	}
	Log("UEngine_CreateNewWorldContext found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CreateNewWorldContext));
	return ServerOffsets::UEngine_CreateNewWorldContext;
}

uintptr_t Finder::FindUEngine_CreateNetDriver_Local() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UEngine_CreateNetDriver_Local)
		return ServerOffsets::UEngine_CreateNetDriver_Local;
	static bool bInitialized = false;

	Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 53 56 57 41 56 41 57 48 81 EC ? ? ? ? 48 63 81").Get();
	if (Addr) {
		bInitialized = true;
	}

	if (!bInitialized)
	{
		bInitialized = true;

		Addr = Memcury::Scanner::FindPattern("49 8B D8 48 8B F9 E8 ?? ?? ?? ?? 48 8B D0 4C 8B C3 48 8B CF 48 8B 5C 24 ?? 48 83 C4 ?? 5F E9 ?? ?? ?? ??").Get();
		if (!Addr)
		{
			Addr = Memcury::Scanner::FindPattern("E8 ?? ?? ?? ?? 4C 8B 44 24 ?? 48 8B D0 48 8B CB E8 ?? ?? ?? ?? 48 83 C4 ?? 5B C3").Get();
			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("33 D2 E8 ?? ?? ?? ?? 48 8B D0 4C 8B C3 48 8B CF E8 ?? ?? ?? ?? 48 8B 5C 24 ?? 48 83 C4 ?? 5F C3").Get();
		}

		if (Addr)
		{
			for (int i = 0; i < 0x200; i++)
			{
				auto Ptr = (uint8_t*)(Addr - i);

				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c)
				{
					Addr = uint64_t(Ptr);
					break;
				}
				else if (*Ptr == 0x4C && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x44)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::UEngine_CreateNetDriver_Local = Addr - ImageBase;
	}

	Log("UEngine_CreateNetDriver_Local found at: 0x" + std::format("{:X}", ServerOffsets::UEngine_CreateNetDriver_Local));
	return ServerOffsets::UEngine_CreateNetDriver_Local;
}

uintptr_t Finder::FindUPlayer_GetPlayerController() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UPlayer_GetPlayerController)
		return ServerOffsets::UPlayer_GetPlayerController;
	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8B C2 48 8B F9 48 85 D2 75 ? 48 8B 41").Get();
	if (Addr) {
		ServerOffsets::UPlayer_GetPlayerController = Addr - ImageBase;
	}
	Log("UPlayer_GetPlayerController found at: 0x" + std::format("{:X}", ServerOffsets::UPlayer_GetPlayerController));
	return ServerOffsets::UPlayer_GetPlayerController;
}

uintptr_t Finder::FindUPlayer_SwitchController() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UPlayer_SwitchController)
		return ServerOffsets::UPlayer_SwitchController;
	Addr = Memcury::Scanner::FindPattern("48 8B 41 ? 48 85 C0 74 ? 48 C7 80").Get();
	if (Addr) {
		ServerOffsets::UPlayer_SwitchController = Addr - ImageBase;
	}
	Log("UPlayer_SwitchController found at: 0x" + std::format("{:X}", ServerOffsets::UPlayer_SwitchController));
	return ServerOffsets::UPlayer_SwitchController;
}

uintptr_t Finder::FindUNetConnection_IsNetReady() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetConnection_IsNetReady)
		return ServerOffsets::UNetConnection_IsNetReady;
	Addr = Memcury::Scanner::FindPattern("84 D2 74 ? 8B 81 ? ? ? ? F7 D8").Get();
	if (Addr) {
		ServerOffsets::UNetConnection_IsNetReady = Addr - ImageBase;
	}
	Log("UNetConnection_IsNetReady found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_IsNetReady));
	return ServerOffsets::UNetConnection_IsNetReady;
}

uintptr_t Finder::FindUNetConnection_IsNetReadyVFT() {
	if (ServerOffsets::UNetConnection_IsNetReadyVFT)
		return ServerOffsets::UNetConnection_IsNetReadyVFT;
	uintptr_t Addr = 0;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.NetConnection"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindUNetConnection_IsNetReady() + ImageBase))
		{
			Addr = i;
			break;
		}
	}
	 
	if (Addr) {
		ServerOffsets::UNetConnection_IsNetReadyVFT = Addr;
	}

	Log("UNetConnection_IsNetReadyVFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_IsNetReadyVFT));
	return ServerOffsets::UNetConnection_IsNetReadyVFT;
}

uintptr_t Finder::FindUNetDriver__ReplicationFrame() {
	if (ServerOffsets::UNetDriver__ReplicationFrame)
		return ServerOffsets::UNetDriver__ReplicationFrame;
	
	ServerOffsets::UNetDriver__ReplicationFrame = *Memcury::Scanner::FindStringRef(
		L"Attempt to replicate function '%s' on Actor '%s' while it is in the middle of variable replication!"
	).ScanFor(
		{ 0x41, 0xFF },
		true
	).AbsoluteOffset(Version::Engine_Version >= 4.19 ? 3 : 4)
	.GetAs<uint32_t*>();

	Log("UNetDriver__ReplicationFrame found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__ReplicationFrame));
	return ServerOffsets::UNetDriver__ReplicationFrame;
}

uintptr_t Finder::FindUNetDriver_GetNetMode() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetDriver_GetNetMode)
		return ServerOffsets::UNetDriver_GetNetMode;
	
	uintptr_t BaseAddr = FindUWorld_InternalGetNetMode();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0xE9) {
				int32_t Rel = *reinterpret_cast<int32_t*>(Ptr + 1);
				uintptr_t Target = reinterpret_cast<uintptr_t>(Ptr) + 5 + Rel;
				Addr = Target;
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::UNetDriver_GetNetMode = Addr - ImageBase;
	}

	Log("UNetDriver_GetNetMode found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_GetNetMode));
	return ServerOffsets::UNetDriver_GetNetMode;
}

uintptr_t Finder::FindAPlayerController_GetViewTarget() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::APlayerController_GetViewTarget)
		return ServerOffsets::APlayerController_GetViewTarget;
	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8B B9 ? ? ? ? 48 85 FF 74 ? 48 83 BF").Get();
	if (Addr) {
		ServerOffsets::APlayerController_GetViewTarget = Addr - ImageBase;
	}
	Log("APlayerController_GetViewTarget found at: 0x" + std::format("{:X}", ServerOffsets::APlayerController_GetViewTarget));
	return ServerOffsets::APlayerController_GetViewTarget;
}

uintptr_t Finder::FindUNetDriver__NetworkObjects() {
	if (ServerOffsets::UNetDriver__NetworkObjects)
		return ServerOffsets::UNetDriver__NetworkObjects;
	
	uintptr_t strAddr = Memcury::Scanner::FindStringRef(L"FlushDormancy: %s. Connection: %s").Get();

	for (int i = 0; i < 300; i++)
	{
		uintptr_t addr = strAddr + i;

		if (*(uint8*)(addr) == 0x49 && *(uint8*)(addr + 1) == 0x8B && *(uint8*)(addr + 2) == 0x89)
		{
			ServerOffsets::UNetDriver__NetworkObjects = *((uint32*)(addr + 3));
			break;
		}
	}

	Log("UNetDriver__NetworkObjects found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__NetworkObjects));
	return ServerOffsets::UNetDriver__NetworkObjects;
}

uintptr_t Finder::FindUNetDriver_IsAdaptiveNetUpdateFrequencyEnabled() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled)
		return ServerOffsets::UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled;
	
	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 48 8B 1D ? ? ? ? 74 ? FF 15 ? ? ? ? 3B 05 ? ? ? ? 74 ? 3B 05 ? ? ? ? 74 ? B8 ? ? ? ? 83 3C 83 ? 0F 9F C0").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? 00 48 8B 1D ? ? ? ? 74 ? FF 15 ? ? ? ? 3B 05 ? ? ? ? 0F 94 C0 EB ? B0 ? 0F B6 C0 48 83 F0 ? ? ? ? 00 0F 9F C0").Get();
	}

	if (Addr) {
		ServerOffsets::UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled = Addr - ImageBase;
	}

	Log("UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled));
	return ServerOffsets::UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled;
}

uintptr_t Finder::FindFNetworkObjectList_Remove() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FNetworkObjectList_Remove)
		return ServerOffsets::FNetworkObjectList_Remove;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 0F 84 ? ? ? ? 55 56 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B F2").Get();
	
	if (Addr) {
		ServerOffsets::FNetworkObjectList_Remove = Addr - ImageBase;
	}

	Log("FNetworkObjectList_Remove found at: 0x" + std::format("{:X}", ServerOffsets::FNetworkObjectList_Remove));
	return ServerOffsets::FNetworkObjectList_Remove;
}

uintptr_t Finder::FindUNetConnection__ActorChannels() {
	if (ServerOffsets::UNetConnection__ActorChannels)
		return ServerOffsets::UNetConnection__ActorChannels;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"NotifyStreamingLevelUnload: %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0x4D && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0x8B) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__ActorChannels = Addr;
	}

	Log("UNetConnection__ActorChannels found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__ActorChannels));
	return ServerOffsets::UNetConnection__ActorChannels;
}

uintptr_t Finder::FindUActorChannel__LastUpdateTime() {
	if (ServerOffsets::UActorChannel__LastUpdateTime)
		return ServerOffsets::UActorChannel__LastUpdateTime;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ActorChannel[%d]: Sending ObjKeys: %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0xF2 && *(Ptr + 1) == 0x41) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 5);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UActorChannel__LastUpdateTime = Addr;
	}

	Log("UActorChannel__LastUpdateTime found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel__LastUpdateTime));
	return ServerOffsets::UActorChannel__LastUpdateTime;
}

uintptr_t Finder::FindUNetConnection__TimeSensitive() {
	if (ServerOffsets::UNetConnection__TimeSensitive)
		return ServerOffsets::UNetConnection__TimeSensitive;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Attempting to send data before handshake is complete. %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0xC6 && *(Ptr + 1) == 0x87) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 2);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__TimeSensitive = Addr;
	}

	Log("UNetConnection__TimeSensitive found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__TimeSensitive));
	return ServerOffsets::UNetConnection__TimeSensitive;
}

uintptr_t Finder::FindFNetViewer_Constructor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FNetViewer_Constructor)
		return ServerOffsets::FNetViewer_Constructor;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef("ServerForwardView").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? ? ? ? 48 8B D9 48 8B 42").Get();
	}

	if (Addr) {
		ServerOffsets::FNetViewer_Constructor = Addr - ImageBase;
	}

	Log("FNetViewer_Constructor found at: 0x" + std::format("{:X}", ServerOffsets::FNetViewer_Constructor));
	return ServerOffsets::FNetViewer_Constructor;
}

uintptr_t Finder::FindAPlayerController_SendClientAdjustment() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::APlayerController_SendClientAdjustment)
		return ServerOffsets::APlayerController_SendClientAdjustment;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Engine_Version >= 5.4)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B D8 48 85 C0 74 ? 80 78").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 91 ? ? ? ? 48 8B D9 83 FA").Get();
		}
		else if (Version::Fortnite_Version >= 25.00)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 80 B8 ? ? ? ? ? 75 ? 8B 93").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 83 3D ? ? ? ? ? 48 8B D9 75").Get();
		}
		else if (Version::Fortnite_Version >= 23.00)
		{
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 83 3D ? ? ? ? ? 48 8B D9 75").Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 83 3D ? ? ? ? ? 48 8B F1").Get();
		}
		else if (Version::Fortnite_Version >= 20.00)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 91 ? ? ? ? 48 8B D9 83 FA").Get();
		else
			Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B 99 ? ? ? ? 48 39 99 ? ? ? ? 74 0A 48 83 B9", false).Get();
	}

	if (Addr) {
		ServerOffsets::APlayerController_SendClientAdjustment = Addr - ImageBase;
	}

	Log("APlayerController_SendClientAdjustment found at: 0x" + std::format("{:X}", ServerOffsets::APlayerController_SendClientAdjustment));
	return ServerOffsets::APlayerController_SendClientAdjustment;
}

uintptr_t Finder::FindAPlayerController_SendClientAdjustmentVFT() {
	if (ServerOffsets::APlayerController_SendClientAdjustmentVFT)
		return ServerOffsets::APlayerController_SendClientAdjustmentVFT;
	uintptr_t Addr = 0;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.PlayerController"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAPlayerController_SendClientAdjustment() + ImageBase))
		{
			ServerOffsets::APlayerController_SendClientAdjustmentVFT = i;
			break;
		}
	}

	if (Addr) {
		ServerOffsets::APlayerController_SendClientAdjustmentVFT = Addr;
	}

	Log("APlayerController_SendClientAdjustmentVFT found at: 0x" + std::format("{:X}", ServerOffsets::APlayerController_SendClientAdjustmentVFT));
	return ServerOffsets::APlayerController_SendClientAdjustmentVFT;
}

uintptr_t Finder::FindAActor_GetNetPriority() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_GetNetPriority)
		return ServerOffsets::AActor_GetNetPriority;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F9 49 8B F0 48 8B EA 48 8B D9 4D 85 C9").Get();

	if (Addr) {
		ServerOffsets::AActor_GetNetPriority = Addr - ImageBase;
	}

	Log("AActor_GetNetPriority found at: 0x" + std::format("{:X}", ServerOffsets::AActor_GetNetPriority));
	return ServerOffsets::AActor_GetNetPriority;
}

uintptr_t Finder::FindAActor_GetNetPriorityVFT() {
	if (ServerOffsets::AActor_GetNetPriorityVFT)
		return ServerOffsets::AActor_GetNetPriorityVFT;

	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x6E;
	}

	if (Addr) {
		ServerOffsets::AActor_GetNetPriorityVFT = Addr;
	}

	Log("AActor_GetNetPriorityVFT found at: 0x" + std::format("{:X}", ServerOffsets::AActor_GetNetPriorityVFT));
	return ServerOffsets::AActor_GetNetPriorityVFT;
}

uintptr_t Finder::FindUNetDriver__NetTag() {
	if (ServerOffsets::UNetDriver__NetTag)
		return ServerOffsets::UNetDriver__NetTag;
	uintptr_t Addr = 0;
	
	uintptr_t DestroyedStartupOrDormantActors_Addr = FindUNetDriver__DestroyedStartupOrDormantActors();
	if (DestroyedStartupOrDormantActors_Addr) {
		// may change on 5.0 or above
		Addr = DestroyedStartupOrDormantActors_Addr - 0x48;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__NetTag = Addr;
	}

	Log("UNetDriver__NetTag found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__NetTag));
	return ServerOffsets::UNetDriver__NetTag;
}

uintptr_t Finder::FindUNetDriver__DebugRelevantActors() {
	if (ServerOffsets::UNetDriver__DebugRelevantActors)
		return ServerOffsets::UNetDriver__DebugRelevantActors;
	uintptr_t Addr = 0;
	if (Version::Engine_Version == 4.16) {
		Addr = 0x1A4;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__DebugRelevantActors = Addr;
	}

	Log("UNetDriver__DebugRelevantActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__DebugRelevantActors));
	return ServerOffsets::UNetDriver__DebugRelevantActors;
}

uintptr_t Finder::FindUNetDriver__LastPrioritizedActors() {
	if (ServerOffsets::UNetDriver__LastPrioritizedActors)
		return ServerOffsets::UNetDriver__LastPrioritizedActors;

	uintptr_t Addr = 0;
	if (Version::Engine_Version == 4.16) {
		Addr = 0x1A8;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__LastPrioritizedActors = Addr;
	}

	Log("UNetDriver__LastPrioritizedActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__LastPrioritizedActors));
	return ServerOffsets::UNetDriver__LastPrioritizedActors;
}

uintptr_t Finder::FindUNetConnection__TickCount() {
	if (ServerOffsets::UNetConnection__TickCount)
		return ServerOffsets::UNetConnection__TickCount;
	uintptr_t Addr = 0;
	
	UClass* StaticClass = UNetConnection::StaticClass();
	if (StaticClass) {
		uintptr_t BaseOffset = StaticClass->GetPropertyOffset("LastReceiveTime");
		if (BaseOffset > 0) {
			Addr = BaseOffset + 0x2C;
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__TickCount = Addr;
	}

	Log("UNetConnection__TickCount found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__TickCount));
	return ServerOffsets::UNetConnection__TickCount;
}

uintptr_t Finder::FindUNetDriver__DestroyedStartupOrDormantActors() {
	if (ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors)
		return ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"AddClientConnection: Added client connection: %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0x4C && *(Ptr + 1) == 0x8D && *(Ptr + 2) == 0x9E) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors = Addr;
	}

	Log("UNetDriver__DestroyedStartupOrDormantActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors));
	return ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors;
}

uintptr_t Finder::FindAActor_IsRelevancyOwnerFor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_IsRelevancyOwnerFor)
		return ServerOffsets::AActor_IsRelevancyOwnerFor;
	Addr = Memcury::Scanner::FindPattern("4C 3B C1 0F 94 C0").Get();

	if (Addr) {
		ServerOffsets::AActor_IsRelevancyOwnerFor = Addr - ImageBase;
	}
	Log("AActor_IsRelevancyOwnerFor found at: 0x" + std::format("{:X}", ServerOffsets::AActor_IsRelevancyOwnerFor));
	return ServerOffsets::AActor_IsRelevancyOwnerFor;
}

uintptr_t Finder::FindAActor_IsRelevancyOwnerForVFT() {
	if (ServerOffsets::AActor_IsRelevancyOwnerForVFT)
		return ServerOffsets::AActor_IsRelevancyOwnerForVFT;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x86;
	}

	if (Addr) {
		ServerOffsets::AActor_IsRelevancyOwnerForVFT = Addr;
	}

	Log("AActor_IsRelevancyOwnerForVFT found at: 0x" + std::format("{:X}", ServerOffsets::AActor_IsRelevancyOwnerForVFT));
	return ServerOffsets::AActor_IsRelevancyOwnerForVFT;
}

uintptr_t Finder::FindUActorChannel__RelevantTime() {
	if (ServerOffsets::UActorChannel__RelevantTime)
		return ServerOffsets::UActorChannel__RelevantTime;
	uintptr_t Addr = 0;
	
	uintptr_t BaseAddr = FindUActorChannel__LastUpdateTime();
	if (BaseAddr) {
		Addr = BaseAddr - 0x8;
	}

	if (Addr) {
		ServerOffsets::UActorChannel__RelevantTime = Addr;
	}

	Log("UActorChannel__RelevantTime found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel__RelevantTime));
	return ServerOffsets::UActorChannel__RelevantTime;
}

uintptr_t Finder::FindUActorChannel_Close() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UActorChannel_Close)
		return ServerOffsets::UActorChannel_Close;
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 33 FF 48 8B D9 89 7C 24").Get();

	if (Addr) {
		ServerOffsets::UActorChannel_Close = Addr - ImageBase;
	}
	Log("UActorChannel_Close found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel_Close));
	return ServerOffsets::UActorChannel_Close;
}

uintptr_t Finder::FindUActorChannel_StartBecomingDormant() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UActorChannel_StartBecomingDormant)
		return ServerOffsets::UActorChannel_StartBecomingDormant;
	Addr = Memcury::Scanner::FindPattern("48 8B C4 55 53 56 57 48 8D 68 ? 48 81 EC ? ? ? ? 80 3D").Get();

	if (Addr) {
		ServerOffsets::UActorChannel_StartBecomingDormant = Addr - ImageBase;
	}
	Log("UActorChannel_StartBecomingDormant found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel_StartBecomingDormant));
	return ServerOffsets::UActorChannel_StartBecomingDormant;
}

uintptr_t Finder::FindAActor_IsNetRelevantFor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_IsNetRelevantFor)
		return ServerOffsets::AActor_IsNetRelevantFor;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Actor %s / %s has no root component in AActor::IsNetRelevantFor. (Make bAlwaysRelevant=true?)").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AActor_IsNetRelevantFor = Addr - ImageBase;
	}

	Log("AActor_IsNetRelevantFor found at: 0x" + std::format("{:X}", ServerOffsets::AActor_IsNetRelevantFor));
	return ServerOffsets::AActor_IsNetRelevantFor;
}

uintptr_t Finder::FindAActor_IsNetRelevantForVFT() {
	if (ServerOffsets::AActor_IsNetRelevantForVFT)
		return ServerOffsets::AActor_IsNetRelevantForVFT;
	uintptr_t Addr = 0;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.Actor"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindAActor_IsNetRelevantFor() + ImageBase))
		{
			ServerOffsets::AActor_IsNetRelevantForVFT = i;
			break;
		}
	}

	if (Addr) {
		ServerOffsets::AActor_IsNetRelevantForVFT = Addr;
	}

	Log("AActor_IsNetRelevantForVFT found at: 0x" + std::format("{:X}", ServerOffsets::AActor_IsNetRelevantForVFT));
	return ServerOffsets::AActor_IsNetRelevantForVFT;
}

uintptr_t Finder::FindFDebug_AssertFailed() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FDebug_AssertFailed)
		return ServerOffsets::FDebug_AssertFailed;

	Addr = Memcury::Scanner::FindPattern("4C 89 4C 24 ? 53 55 56 41 56").Get();
	if (Addr) {
		ServerOffsets::FDebug_AssertFailed = Addr - ImageBase;
	}

	Log("FDebug_AssertFailed found at: 0x" + std::format("{:X}", ServerOffsets::FDebug_AssertFailed));
	return ServerOffsets::FDebug_AssertFailed;
}

uintptr_t Finder::FindUNetConnection__State() {
	if (ServerOffsets::UNetConnection__State)
		return ServerOffsets::UNetConnection__State;
	uintptr_t Addr = 0;

	uintptr_t BaseAddr = Finder::FindUNetDriver_TickDispatch();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0x83 && *(Ptr + 1) == 0xB9) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 2);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__State = Addr;
	}

	Log("UNetConnection__State found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__State));
	return ServerOffsets::UNetConnection__State;
}

uintptr_t Finder::FindAActor_GetWorld() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AActor_GetWorld)
		return ServerOffsets::AActor_GetWorld;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 8B 41 ? 48 8B D1 C1 E8").Get();

	if (Addr) {
		ServerOffsets::AActor_GetWorld = Addr - ImageBase;
	}

	Log("AActor_GetWorld found at: 0x" + std::format("{:X}", ServerOffsets::AActor_GetWorld));
	return ServerOffsets::AActor_GetWorld;
}

uintptr_t Finder::FindUWorld__TimeSeconds() {
	if (ServerOffsets::UWorld__TimeSeconds)
		return ServerOffsets::UWorld__TimeSeconds;

	uintptr_t straddr = Memcury::Scanner::FindStringRef(L"DeactivateSystem @ %fs %s").Get();

	for (int i = 0; i < 60; i++)
	{
		uintptr_t addr = straddr - i;

		if (*(uint8*)(addr) == 0xF3 && *(uint8*)(addr + 1) == 0x0F && *(uint8*)(addr + 2) == 0x10)
		{
			ServerOffsets::UWorld__TimeSeconds = *(uint32_t*)(addr + 4);
			break;
		}
	}

	Log("UWorld__TimeSeconds found at: 0x" + std::format("{:X}", ServerOffsets::UWorld__TimeSeconds));
	return ServerOffsets::UWorld__TimeSeconds;
}

uintptr_t Finder::FindUNetConnection__DestroyedStartupOrDormantActors() {
	if (ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors)
		return ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Adding actor NetGUID <%s> to new connection's destroy list").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr + i);
			if (*Ptr == 0x49 && *(Ptr + 1) == 0x8D && *(Ptr + 2) == 0x8F) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors = Addr;
	}

	Log("UNetConnection__DestroyedStartupOrDormantActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors));
	return ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors;
}

uintptr_t Finder::FindUNetConnection__ClientWorldPackageName() {
	if (ServerOffsets::UNetConnection__ClientWorldPackageName)
		return ServerOffsets::UNetConnection__ClientWorldPackageName;
	uintptr_t Addr = 0;

	uintptr_t BaseAddr = Finder::FindUNetDriver_IsLevelInitializedForActor();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0x49 && *(Ptr + 1) == 0x39) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__ClientWorldPackageName = Addr;
	}

	Log("UNetConnection__ClientWorldPackageName found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__ClientWorldPackageName));
	return ServerOffsets::UNetConnection__ClientWorldPackageName;
}

uintptr_t Finder::FindUNetConnection__ClientVisibleLevelNames() {
	if (ServerOffsets::UNetConnection__ClientVisibleLevelNames)
		return ServerOffsets::UNetConnection__ClientVisibleLevelNames;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ServerUpdateLevelVisibility() Added '%s'").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if ((*Ptr == 0x49 || *Ptr == 0x48)
				&& *(Ptr + 1) == 0x8D
				&& (*(Ptr + 2) == 0x8F || *(Ptr + 2) == 0x8D)) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection__ClientVisibleLevelNames = Addr;
	}

	Log("UNetConnection__ClientVisibleLevelNames found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__ClientVisibleLevelNames));
	return ServerOffsets::UNetConnection__ClientVisibleLevelNames;
}

uintptr_t Finder::FindUNetConnection_CreateChannel() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetConnection_CreateChannel)
		return ServerOffsets::UNetConnection_CreateChannel;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Created channel %i of type %i").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x56) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetConnection_CreateChannel = Addr - ImageBase;
	}

	Log("UNetConnection_CreateChannel found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_CreateChannel));
	return ServerOffsets::UNetConnection_CreateChannel;
}

uintptr_t Finder::FindUActorChannel_SetChannelActorForDestroy() {
	static uintptr_t Addr = 0;
	if (Addr == -1) {
		return 0;
	}
	if (ServerOffsets::UActorChannel_SetChannelActorForDestroy)
		return ServerOffsets::UActorChannel_SetChannelActorForDestroy;

	auto sRef = Memcury::Scanner::FindStringRef(L"SetChannelActorForDestroy: Channel %d. NetGUID <%s> Path: %s. Bits: %d", false, 0, Version::Fortnite_Version >= 19, true).Get();
	if (sRef) {
		for (int i = 0; i < 2000; i++)
		{
			if (*(uint8_t*)(sRef - i) == 0x40 && *(uint8_t*)(sRef - i + 1) == 0x55)
				Addr = sRef - i;
		}
	}

	if (Addr) {
		ServerOffsets::UActorChannel_SetChannelActorForDestroy = Addr - ImageBase;
	}
	else {
		Addr = -1;
	}

	Log("UActorChannel_SetChannelActorForDestroy found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel_SetChannelActorForDestroy));
	return ServerOffsets::UActorChannel_SetChannelActorForDestroy;
}

uintptr_t Finder::FindFOutBunch_Constructor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FOutBunch_Constructor)
		return ServerOffsets::FOutBunch_Constructor;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 41 0F B6 F8").Get();

	if (Addr) {
		ServerOffsets::FOutBunch_Constructor = Addr - ImageBase;
	}

	Log("FOutBunch_Constructor found at: 0x" + std::format("{:X}", ServerOffsets::FOutBunch_Constructor));
	return ServerOffsets::FOutBunch_Constructor;
}

uintptr_t Finder::FindUNetDriver__LastRelevantActors() {
	if (ServerOffsets::UNetDriver__LastRelevantActors)
		return ServerOffsets::UNetDriver__LastRelevantActors;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x1B4;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__LastRelevantActors = Addr;
	}

	Log("UNetDriver__LastRelevantActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__LastRelevantActors));
	return ServerOffsets::UNetDriver__LastRelevantActors;
}

uintptr_t Finder::FindUNetDriver__LastSentActors() {
	if (ServerOffsets::UNetDriver__LastSentActors)
		return ServerOffsets::UNetDriver__LastSentActors;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x1C4;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__LastSentActors = Addr;
	}

	Log("UNetDriver__LastSentActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__LastSentActors));
	return ServerOffsets::UNetDriver__LastSentActors;
}

uintptr_t Finder::FindUNetDriver__LastNonRelevantActors() {
	if (ServerOffsets::UNetDriver__LastNonRelevantActors)
		return ServerOffsets::UNetDriver__LastNonRelevantActors;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x1D4;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__LastNonRelevantActors = Addr;
	}

	Log("UNetDriver__LastNonRelevantActors found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__LastNonRelevantActors));
	return ServerOffsets::UNetDriver__LastNonRelevantActors;
}

uintptr_t Finder::FindUNetDriver__GuidCache() {
	if (ServerOffsets::UNetDriver__GuidCache)
		return ServerOffsets::UNetDriver__GuidCache;
	uintptr_t Addr = 0;

	UClass* StaticClass = UNetDriver::StaticClass();
	if (StaticClass) {
		uintptr_t BaseOffset = StaticClass->GetPropertyOffset("WorldPackage");
		if (BaseOffset <= 0) {
			BaseOffset = StaticClass->GetPropertyOffset("World");
		}

		if (BaseOffset > 0) {
			Addr = BaseOffset + 0x8;
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver__GuidCache = Addr;
	}

	Log("UNetDriver__GuidCache found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__GuidCache));
	return ServerOffsets::UNetDriver__GuidCache;
}

uintptr_t Finder::FindFNetGUIDCache_SupportsObject() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FNetGUIDCache_SupportsObject)
		return ServerOffsets::FNetGUIDCache_SupportsObject;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"FNetGUIDCache::SupportsObject: %s NOT Supported.").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = reinterpret_cast<uintptr_t>(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::FNetGUIDCache_SupportsObject = Addr - ImageBase;
	}

	Log("FNetGUIDCache_SupportsObject found at: 0x" + std::format("{:X}", ServerOffsets::FNetGUIDCache_SupportsObject));
	return ServerOffsets::FNetGUIDCache_SupportsObject;
}

uintptr_t Finder::FindUObject_GetArchetypeFromRequiredInfo() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UObject_GetArchetypeFromRequiredInfo)
		return ServerOffsets::UObject_GetArchetypeFromRequiredInfo;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"%s had RF_NeedLoad when searching supers for an archetype of %s in %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = reinterpret_cast<uintptr_t>(Ptr);
				break;
			}
		}
	}
	else {
		Log("Failed to find string reference for UObject_GetArchetypeFromRequiredInfo, trying pattern scan...");
	}

	if (Addr) {
		ServerOffsets::UObject_GetArchetypeFromRequiredInfo = Addr - ImageBase;
	}

	Log("UObject_GetArchetypeFromRequiredInfo found at: 0x" + std::format("{:X}", ServerOffsets::UObject_GetArchetypeFromRequiredInfo));
	return ServerOffsets::UObject_GetArchetypeFromRequiredInfo;
}

uintptr_t Finder::FindUActorChannel_SetChannelActor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UActorChannel_SetChannelActor)
		return ServerOffsets::UActorChannel_SetChannelActor;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UActorChannel::SetChannelActor: actor %s is not in the same level collection as the net driver (%s)!").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x4C && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xDC) {
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x4C && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xC4) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 8B C4 55 53 57 41 54 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 45 33 E4").Get();
	}

	if (Addr) {
		ServerOffsets::UActorChannel_SetChannelActor = Addr - ImageBase;
	}

	Log("UActorChannel_SetChannelActor found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel_SetChannelActor));
	return ServerOffsets::UActorChannel_SetChannelActor;
}

uintptr_t Finder::FindUChannel__NumInRec() {
	if (ServerOffsets::UChannel__NumInRec)
		return ServerOffsets::UChannel__NumInRec;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UChannel::ReceivedRawBunch: Too many reliable messages queued up").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x81 && *(Ptr + 1) == 0x7B) {
				uint8_t Offset = *(Ptr + 2);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UChannel__NumInRec = Addr;
	}

	Log("UChannel__NumInRec found at: 0x" + std::format("{:X}", ServerOffsets::UChannel__NumInRec));
	return ServerOffsets::UChannel__NumInRec;
}

uintptr_t Finder::FindUChannel__NumOutRec() {
	if (ServerOffsets::UChannel__NumOutRec)
		return ServerOffsets::UChannel__NumOutRec;
	uintptr_t Addr = 0;

	uintptr_t BaseAddr = Finder::FindUChannel__NumInRec();
	if (BaseAddr) {
		Addr = BaseAddr + 4;
	}

	if (Addr) {
		ServerOffsets::UChannel__NumOutRec = Addr;
	}

	Log("UChannel__NumOutRec found at: 0x" + std::format("{:X}", ServerOffsets::UChannel__NumOutRec));
	return ServerOffsets::UChannel__NumOutRec;
}

uintptr_t Finder::FindUActorChannel_ReplicateActor() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UActorChannel_ReplicateActor)
		return ServerOffsets::UActorChannel_ReplicateActor;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ReplicateActor: bPausedUntilReliableACK is ending now that reliables have been ACK'd. %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x55) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UActorChannel_ReplicateActor = Addr - ImageBase;
	}

	Log("UActorChannel_ReplicateActor found at: 0x" + std::format("{:X}", ServerOffsets::UActorChannel_ReplicateActor));
	return ServerOffsets::UActorChannel_ReplicateActor;
}

uintptr_t Finder::FindUNetDriver_TickDispatch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetDriver_TickDispatch)
		return ServerOffsets::UNetDriver_TickDispatch;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UNetDriver::TickDispatch: Very long time between ticks. DeltaTime: %2.2f, Realtime: %2.2f. %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x57) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver_TickDispatch = Addr - ImageBase;
	}

	Log("UNetDriver_TickDispatch found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_TickDispatch));
	return ServerOffsets::UNetDriver_TickDispatch;
}

uintptr_t Finder::FindUNetDriver__SendCycles() {
	if (ServerOffsets::UNetDriver__SendCycles)
		return ServerOffsets::UNetDriver__SendCycles;
	uintptr_t Addr = 0;

	uintptr_t BaseAddr = Finder::FindUNetDriver_TickDispatch();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 3);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver__SendCycles = Addr;
	}

	Log("UNetDriver__SendCycles found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__SendCycles));
	return ServerOffsets::UNetDriver__SendCycles;
}

uintptr_t Finder::FindUNetDriver__RecvCycles() {
	if (ServerOffsets::UNetDriver__RecvCycles)
		return ServerOffsets::UNetDriver__RecvCycles;
	uintptr_t Addr = 0;

	if (Finder::FindUNetDriver__SendCycles()) {
		Addr = Finder::FindUNetDriver__SendCycles() + 4;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__RecvCycles = Addr;
	}

	Log("UNetDriver__RecvCycles found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__RecvCycles));
	return ServerOffsets::UNetDriver__RecvCycles;
}

uintptr_t Finder::FindUNetDriver__LastTickDispatchRealtime() {
	if (ServerOffsets::UNetDriver__LastTickDispatchRealtime)
		return ServerOffsets::UNetDriver__LastTickDispatchRealtime;
	uintptr_t Addr = 0;

	uintptr_t BaseAddr = Finder::FindUNetDriver_TickDispatch();
	if (BaseAddr) {
		BaseAddr += ImageBase;

		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(BaseAddr + i);
			if (*Ptr == 0xF2 && *(Ptr + 1) == 0x0F && *(Ptr + 2) == 0x11 && *(Ptr + 3) == 0x97) {
				int32_t Offset = *reinterpret_cast<int32_t*>(Ptr + 4);
				Addr = static_cast<uintptr_t>(Offset);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UNetDriver__LastTickDispatchRealtime = Addr;
	}

	Log("UNetDriver__LastTickDispatchRealtime found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__LastTickDispatchRealtime));
	return ServerOffsets::UNetDriver__LastTickDispatchRealtime;
}

uintptr_t Finder::FindUNetConnection_CleanUp() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UNetConnection_CleanUp)
		return ServerOffsets::UNetConnection_CleanUp;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 45 33 ED 48 8B F1").Get();

	if (Addr) {
		ServerOffsets::UNetConnection_CleanUp = Addr - ImageBase;
	}

	Log("UNetConnection_CleanUp found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_CleanUp));
	return ServerOffsets::UNetConnection_CleanUp;
}

uintptr_t Finder::FindUNetConnection_CleanUpVFT() {
	if (ServerOffsets::UNetConnection_CleanUpVFT)
		return ServerOffsets::UNetConnection_CleanUpVFT;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x55;
	}

	if (Addr) {
		ServerOffsets::UNetConnection_CleanUpVFT = Addr;
	}

	Log("UNetConnection_CleanUpVFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_CleanUpVFT));
	return ServerOffsets::UNetConnection_CleanUpVFT;
}

uintptr_t Finder::FindUNetDriver__bIsStandbyCheckingEnabled() {
	if (ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled)
		return ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled;

	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x180;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled = Addr;
	}

	Log("UNetDriver__bIsStandbyCheckingEnabled found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled));
	return ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled;
}

uintptr_t Finder::FindUNetDriver__bHasStandbyCheatTriggered() {
	if (ServerOffsets::UNetDriver__bHasStandbyCheatTriggered)
		return ServerOffsets::UNetDriver__bHasStandbyCheatTriggered;

	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x181;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__bHasStandbyCheatTriggered = Addr;
	}

	Log("UNetDriver__bHasStandbyCheatTriggered found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__bHasStandbyCheatTriggered));
	return ServerOffsets::UNetDriver__bHasStandbyCheatTriggered;
}

uintptr_t Finder::FindAActor__CreationTime() {
	if (ServerOffsets::AActor__CreationTime)
		return ServerOffsets::AActor__CreationTime;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x14C;
	}

	if (Addr) {
		ServerOffsets::AActor__CreationTime = Addr;
	}

	Log("AActor__CreationTime found at: 0x" + std::format("{:X}", ServerOffsets::AActor__CreationTime));
	return ServerOffsets::AActor__CreationTime;
}

uintptr_t Finder::FindUNetDriver__JoinInProgressStandbyWaitTime() {
	if (ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime)
		return ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x330;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime = Addr;
	}

	Log("UNetDriver__JoinInProgressStandbyWaitTime found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime));
	return ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime;
}

uintptr_t Finder::FindUNetConnection__LastRecvAckTime() {
	if (ServerOffsets::UNetConnection__LastRecvAckTime)
		return ServerOffsets::UNetConnection__LastRecvAckTime;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x200;
	}

	if (Addr) {
		ServerOffsets::UNetConnection__LastRecvAckTime = Addr;
	}

	Log("UNetConnection__LastRecvAckTime found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection__LastRecvAckTime));
	return ServerOffsets::UNetConnection__LastRecvAckTime;
}

uintptr_t Finder::FindUNetDriver__StandbyRxCheatTime() {
	if (ServerOffsets::UNetDriver__StandbyRxCheatTime)
		return ServerOffsets::UNetDriver__StandbyRxCheatTime;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x184;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__StandbyRxCheatTime = Addr;
	}

	Log("UNetDriver__StandbyRxCheatTime found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__StandbyRxCheatTime));
	return ServerOffsets::UNetDriver__StandbyRxCheatTime;
}

uintptr_t Finder::FindUNetDriver__StandbyTxCheatTime() {
	if (ServerOffsets::UNetDriver__StandbyTxCheatTime)
		return ServerOffsets::UNetDriver__StandbyTxCheatTime;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x188;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__StandbyTxCheatTime = Addr;
	}

	Log("UNetDriver__StandbyTxCheatTime found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__StandbyTxCheatTime));
	return ServerOffsets::UNetDriver__StandbyTxCheatTime;
}

uintptr_t Finder::FindUNetDriver__BadPingThreshold() {
	if (ServerOffsets::UNetDriver__BadPingThreshold)
		return ServerOffsets::UNetDriver__BadPingThreshold;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x184;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__BadPingThreshold = Addr;
	}

	Log("UNetDriver__BadPingThreshold found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__BadPingThreshold));
	return ServerOffsets::UNetDriver__BadPingThreshold;
}

uintptr_t Finder::FindUNetDriver__PercentMissingForRxStandby() {
	if (ServerOffsets::UNetDriver__PercentMissingForRxStandby)
		return ServerOffsets::UNetDriver__PercentMissingForRxStandby;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x190;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__PercentMissingForRxStandby = Addr;
	}

	Log("UNetDriver__PercentMissingForRxStandby found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__PercentMissingForRxStandby));
	return ServerOffsets::UNetDriver__PercentMissingForRxStandby;
}

uintptr_t Finder::FindUNetDriver__PercentMissingForTxStandby() {
	if (ServerOffsets::UNetDriver__PercentMissingForTxStandby)
		return ServerOffsets::UNetDriver__PercentMissingForTxStandby;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x194;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__PercentMissingForTxStandby = Addr;
	}

	Log("UNetDriver__PercentMissingForTxStandby found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__PercentMissingForTxStandby));
	return ServerOffsets::UNetDriver__PercentMissingForTxStandby;
}

uintptr_t Finder::FindUNetDriver__PercentForBadPing() {
	if (ServerOffsets::UNetDriver__PercentForBadPing)
		return ServerOffsets::UNetDriver__PercentForBadPing;
	uintptr_t Addr = 0;

	if (Version::Engine_Version == 4.16) {
		Addr = 0x198;
	}

	if (Addr) {
		ServerOffsets::UNetDriver__PercentForBadPing = Addr;
	}

	Log("UNetDriver__PercentForBadPing found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver__PercentForBadPing));
	return ServerOffsets::UNetDriver__PercentForBadPing;
}

uintptr_t Finder::FindAFortGameSession_ValidatePlayer() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameSession_ValidatePlayer)
		return ServerOffsets::AFortGameSession_ValidatePlayer;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Game already ended.").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"No reservation in game.").Get();
	}
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortGameSession_ValidatePlayer = Addr - ImageBase;
	}

	Log("AFortGameSession_ValidatePlayer found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameSession_ValidatePlayer));
	return ServerOffsets::AFortGameSession_ValidatePlayer;
}

uintptr_t Finder::FindUPackageMap_WriteObject() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UPackageMap_WriteObject)
		return ServerOffsets::UPackageMap_WriteObject;

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 44 24 ? 48 8B 08 48 85 C9").Get();

	if (Addr) {
		ServerOffsets::UPackageMap_WriteObject = Addr - ImageBase;
	}

	Log("UPackageMap_WriteObject found at: 0x" + std::format("{:X}", ServerOffsets::UPackageMap_WriteObject));
	return ServerOffsets::UPackageMap_WriteObject;
}

uintptr_t Finder::FindUChannel_SendBunch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UChannel_SendBunch)
		return ServerOffsets::UChannel_SendBunch;

	Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 48 89 54 24 ? 55 56 57 41 57").Get();

	if (Addr) {
		ServerOffsets::UChannel_SendBunch = Addr - ImageBase;
	}

	Log("UChannel_SendBunch found at: 0x" + std::format("{:X}", ServerOffsets::UChannel_SendBunch));
	return ServerOffsets::UChannel_SendBunch;
}

uintptr_t Finder::FindUChannel_SendBunchVFT() {
	if (ServerOffsets::UChannel_SendBunchVFT)
		return ServerOffsets::UChannel_SendBunchVFT;
	
	if (Version::Engine_Version == 4.16) {
		ServerOffsets::UChannel_SendBunchVFT = 0x4F;
	}

	Log("UChannel_SendBunchVFT found at: 0x" + std::format("{:X}", ServerOffsets::UChannel_SendBunchVFT));
	return ServerOffsets::UChannel_SendBunchVFT;
}

uintptr_t Finder::FindFNetworkObjectList_ClearRecentlyDormantConnection() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FNetworkObjectList_ClearRecentlyDormantConnection)
		return ServerOffsets::FNetworkObjectList_ClearRecentlyDormantConnection;

	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 49 8B F8 4D 8B C1").Get();

	if (Addr) {
		ServerOffsets::FNetworkObjectList_ClearRecentlyDormantConnection = Addr - ImageBase;
	}

	Log("FNetworkObjectList_ClearRecentlyDormantConnection found at: 0x" + std::format("{:X}", ServerOffsets::FNetworkObjectList_ClearRecentlyDormantConnection));
	return ServerOffsets::FNetworkObjectList_ClearRecentlyDormantConnection;
}

uintptr_t Finder::FindFNetworkObjectList_Add() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FNetworkObjectList_Add)
		return ServerOffsets::FNetworkObjectList_Add;

	Addr = Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 53 56 57 41 56 48 8B EC 48 81 EC ? ? ? ? 49 8B D8").Get();

	if (Addr) {
		ServerOffsets::FNetworkObjectList_Add = Addr - ImageBase;
	}

	Log("FNetworkObjectList_Add found at: 0x" + std::format("{:X}", ServerOffsets::FNetworkObjectList_Add));
	return ServerOffsets::FNetworkObjectList_Add;
}

uintptr_t Finder::FindUKismetSystemLibrary_ExecuteConsoleCommand() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UKismetSystemLibrary_ExecuteConsoleCommand)
		return ServerOffsets::UKismetSystemLibrary_ExecuteConsoleCommand;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 49 8B C0 48 8B DA 4D 85 C0").Get();

	if (Addr) {
		ServerOffsets::UKismetSystemLibrary_ExecuteConsoleCommand = Addr - ImageBase;
	}

	Log("UKismetSystemLibrary_ExecuteConsoleCommand found at: 0x" + std::format("{:X}", ServerOffsets::UKismetSystemLibrary_ExecuteConsoleCommand));
	return ServerOffsets::UKismetSystemLibrary_ExecuteConsoleCommand;
}

uintptr_t Finder::FindAFortGameMode_SetCurrentPlaylistId() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameMode_SetCurrentPlaylistId)
		return ServerOffsets::AFortGameMode_SetCurrentPlaylistId;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B DA 48 8B F9 83 FA ? 76").Get();

	if (Addr) {
		ServerOffsets::AFortGameMode_SetCurrentPlaylistId = Addr - ImageBase;
	}

	Log("AFortGameMode_SetCurrentPlaylistId found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_SetCurrentPlaylistId));
	return ServerOffsets::AFortGameMode_SetCurrentPlaylistId;
}

uintptr_t Finder::FindCollectGarbage() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::CollectGarbage)
		return ServerOffsets::CollectGarbage;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 8B F9 0F B6 DA").Get();

	if (Addr) {
		ServerOffsets::CollectGarbage = Addr - ImageBase;
	}

	Log("CollectGarbage found at: 0x" + std::format("{:X}", ServerOffsets::CollectGarbage));
	return ServerOffsets::CollectGarbage;
}

uintptr_t Finder::FindCollectGarbageInternal() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::CollectGarbageInternal)
		return ServerOffsets::CollectGarbageInternal;

	auto sRef = Memcury::Scanner::FindStringRef(L"CollectGarbageInternal() is flushing async loading").Get();
	if (sRef)
	{
		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(sRef - i);

			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xC4)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::CollectGarbageInternal = Addr - ImageBase;
	}

	Log("CollectGarbageInternal found at: 0x" + std::format("{:X}", ServerOffsets::CollectGarbageInternal));
	return ServerOffsets::CollectGarbageInternal;
}

uintptr_t Finder::FindAFortGameModeZone_ChoosePlayerStart() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeZone_ChoosePlayerStart)
		return ServerOffsets::AFortGameModeZone_ChoosePlayerStart;

	Addr = Memcury::Scanner::FindPattern("40 55 53 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B FA").Get();

	if (Addr) {
		ServerOffsets::AFortGameModeZone_ChoosePlayerStart = Addr - ImageBase;
	}

	Log("AFortGameModeZone_ChoosePlayerStart found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_ChoosePlayerStart));
	return ServerOffsets::AFortGameModeZone_ChoosePlayerStart;
}

uintptr_t Finder::FindAFortPlayerControllerAthena_ClientRestart() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerControllerAthena_ClientRestart)
		return ServerOffsets::AFortPlayerControllerAthena_ClientRestart;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 44 0F B6 B9").Get();

	if (Addr) {
		ServerOffsets::AFortPlayerControllerAthena_ClientRestart = Addr - ImageBase;
	}

	Log("AFortPlayerControllerAthena_ClientRestart found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerControllerAthena_ClientRestart));
	return ServerOffsets::AFortPlayerControllerAthena_ClientRestart;
}

uintptr_t Finder::FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetup() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetup)
		return ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetup;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ApplyHomebaseEffectsOnPlayerSetup failed with no ability object").Get();
	if (StringAddr) {
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x55) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetup = Addr - ImageBase;
	}

	Log("AFortGameState_ApplyHomebaseEffectsOnPlayerSetup found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetup));
	return ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetup;
}

uintptr_t Finder::FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT() {
	if (ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT)
		return ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortGameState"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetup() + ImageBase))
		{
			ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT = i;
			break;
		}
	}

	Log("AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT));
	return ServerOffsets::AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT;
}

uintptr_t Finder::FindUMcpProfileGroup_SendRequestNow() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::UMcpProfileGroup_SendRequestNow)
		return ServerOffsets::UMcpProfileGroup_SendRequestNow;

	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		auto sRef = Memcury::Scanner::FindStringRef(L"MCP-Profile: Dispatching request to %s", true, 0, Version::Fortnite_Version >= 19).Get();
		if (!sRef)
			sRef = Memcury::Scanner::FindStringRef(L"MCP-Profile: Dispatching request to %s - ContextCredentials: %s", true, 0, Version::Fortnite_Version >= 19).Get();

		if (!sRef)
			return 0;

		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(sRef - i);

			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x8b && *(Ptr + 2) == 0xc4)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UMcpProfileGroup_SendRequestNow = Addr - ImageBase;
	}

	Log("UMcpProfileGroup_SendRequestNow found at: 0x" + std::format("{:X}", ServerOffsets::UMcpProfileGroup_SendRequestNow));
	return ServerOffsets::UMcpProfileGroup_SendRequestNow;
}

uintptr_t Finder::FindAFortPlayerPawn_ServerChoosePart() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerPawn_ServerChoosePart)
		return ServerOffsets::AFortPlayerPawn_ServerChoosePart;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F8 8B F2 48 8B D9 E8").Get();

	if (Addr) {
		ServerOffsets::AFortPlayerPawn_ServerChoosePart = Addr - ImageBase;
	}

	Log("AFortPlayerPawn_ServerChoosePart found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerPawn_ServerChoosePart));
	return ServerOffsets::AFortPlayerPawn_ServerChoosePart;
}

uintptr_t Finder::FindAFortPlayerState_InitializeHero() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerState_InitializeHero)
		return ServerOffsets::AFortPlayerState_InitializeHero;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 56 41 57 48 8B EC 48 83 EC ? 48 8B B9").Get();

	if (Addr) {
		ServerOffsets::AFortPlayerState_InitializeHero = Addr - ImageBase;
	}

	Log("AFortPlayerState_InitializeHero found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerState_InitializeHero));
	return ServerOffsets::AFortPlayerState_InitializeHero;
}

uintptr_t Finder::FindAFortPlayerState_RefreshCharacterCustomization() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerState_RefreshCharacterCustomization)
		return ServerOffsets::AFortPlayerState_RefreshCharacterCustomization;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 49 8B F8 48 8B D9 84 D2 74 ? 48 8D 81").Get();

	if (Addr) {
		ServerOffsets::AFortPlayerState_RefreshCharacterCustomization = Addr - ImageBase;
	}

	Log("AFortPlayerState_RefreshCharacterCustomization found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerState_RefreshCharacterCustomization));
	return ServerOffsets::AFortPlayerState_RefreshCharacterCustomization;
}

uintptr_t Finder::FindFName_GetComparisonNameEntry() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FName_GetComparisonNameEntry)
		return ServerOffsets::FName_GetComparisonNameEntry;

	Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 4C 8B C8").Get();
	
	if (Addr) {
		ServerOffsets::FName_GetComparisonNameEntry = Addr - ImageBase;
	}

	Log("FName_GetComparisonNameEntry found at: 0x" + std::format("{:X}", ServerOffsets::FName_GetComparisonNameEntry));
	return ServerOffsets::FName_GetComparisonNameEntry;
}

uintptr_t Finder::FindFString_Append() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FString_Append)
		return ServerOffsets::FString_Append;
	
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 63 F0 48 8B FA 48 8B D9").Get();
	
	if (Addr) {
		ServerOffsets::FString_Append = Addr - ImageBase;
	}

	Log("FString_Append found at: 0x" + std::format("{:X}", ServerOffsets::FString_Append));
	return ServerOffsets::FString_Append;
}

uintptr_t Finder::FindFString_AppendInt() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FString_AppendInt)
		return ServerOffsets::FString_AppendInt;
	
	Addr = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B ? 55 56 41 56 48 81 EC ? ? ? ? 48 8B 05").Get();
	
	if (Addr) {
		ServerOffsets::FString_AppendInt = Addr - ImageBase;
	}

	Log("FString_AppendInt found at: 0x" + std::format("{:X}", ServerOffsets::FString_AppendInt));
	return ServerOffsets::FString_AppendInt;
}

uintptr_t Finder::FindFString_ToBool() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::FString_ToBool)
		return ServerOffsets::FString_ToBool;
	
	Addr = Memcury::Scanner::FindPattern("83 79 ? 00 74 ? ? ? ? E9 ? ? ? ? 48 8D 0D").Get();
	
	if (Addr) {
		ServerOffsets::FString_ToBool = Addr - ImageBase;
	}

	Log("FString_ToBool found at: 0x" + std::format("{:X}", ServerOffsets::FString_ToBool));
	return ServerOffsets::FString_ToBool;
}

uintptr_t Finder::FindAFortGameModeAthena_FinishWorldInitialization() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameModeAthena_FinishWorldInitialization)
		return ServerOffsets::AFortGameModeAthena_FinishWorldInitialization;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Can't find a FortAthenaMapInfo placed in map.  Skipping warmup and aircraft phases.").Get();
	if (StringAddr) {
		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_FinishWorldInitialization = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_FinishWorldInitialization found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_FinishWorldInitialization));
	return ServerOffsets::AFortGameModeAthena_FinishWorldInitialization;
}

uintptr_t Finder::FindAFortGameMode_FinishWorldInitializationVFT() {
	if (ServerOffsets::AFortGameMode_FinishWorldInitializationVFT)
		return ServerOffsets::AFortGameMode_FinishWorldInitializationVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortGameModeAthena"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortGameModeAthena_FinishWorldInitialization() + ImageBase))
		{
			ServerOffsets::AFortGameMode_FinishWorldInitializationVFT = i;
			break;
		}
	}

	Log("AFortGameMode_FinishWorldInitializationVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_FinishWorldInitializationVFT));
	return ServerOffsets::AFortGameMode_FinishWorldInitializationVFT;
}

uintptr_t Finder::FindAFortGameStateAthena_SetCurrentPlaylistId() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId)
		return ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId;
	
	//Addr = Memcury::Scanner::FindPattern("").Get(); // Dont have anything for this yet
	
	if (Addr) {
		ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId = Addr - ImageBase;
	}

	bInitialized = true;
	Log("AFortGameStateAthena_SetCurrentPlaylistId found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId));
	return ServerOffsets::AFortGameStateAthena_SetCurrentPlaylistId;
}

uintptr_t Finder::FindAFortPlayerController_OnReadyToStartMatch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerController_OnReadyToStartMatch)
		return ServerOffsets::AFortPlayerController_OnReadyToStartMatch;
	
	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 8B 81 ? ? ? ? 48 8B F9 A8 ? 0F 85 ? ? ? ? 83 C8").Get();
	
	if (Addr) {
		ServerOffsets::AFortPlayerController_OnReadyToStartMatch = Addr - ImageBase;
	}

	Log("AFortPlayerController_OnReadyToStartMatch found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_OnReadyToStartMatch));
	return ServerOffsets::AFortPlayerController_OnReadyToStartMatch;
}

uintptr_t Finder::FindAFortPlayerControllerZone_OnReadyToStartMatch() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerControllerZone_OnReadyToStartMatch)
		return ServerOffsets::AFortPlayerControllerZone_OnReadyToStartMatch;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ClearCachedReports IN OnReadyToStartMatch!").Get();
	if (StringAddr) {
		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x4C && *(Ptr + 1) == 0x8B)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortPlayerControllerZone_OnReadyToStartMatch = Addr - ImageBase;
	}

	Log("AFortPlayerControllerZone_OnReadyToStartMatch found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerControllerZone_OnReadyToStartMatch));
	return ServerOffsets::AFortPlayerControllerZone_OnReadyToStartMatch;
}

uintptr_t Finder::FindAFortPlayerController_OnReadyToStartMatchVFT() {
	if (ServerOffsets::AFortPlayerController_OnReadyToStartMatchVFT)
		return ServerOffsets::AFortPlayerController_OnReadyToStartMatchVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortPlayerControllerZone"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortPlayerControllerZone_OnReadyToStartMatch() + ImageBase))
		{
			ServerOffsets::AFortPlayerController_OnReadyToStartMatchVFT = i;
			break;
		}
	}

	Log("AFortPlayerController_OnReadyToStartMatchVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_OnReadyToStartMatchVFT));
	return ServerOffsets::AFortPlayerController_OnReadyToStartMatchVFT;
}

uintptr_t Finder::FindAFortPlayerControllerAthena_SpawnQuickBars() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerControllerAthena_SpawnQuickBars)
		return ServerOffsets::AFortPlayerControllerAthena_SpawnQuickBars;
	
	//Addr = Memcury::Scanner::FindPattern("").Get(); Havent got this one yet
	
	if (Addr) {
		ServerOffsets::AFortPlayerControllerAthena_SpawnQuickBars = Addr - ImageBase;
	}

	Log("AFortPlayerControllerAthena_SpawnQuickBars found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerControllerAthena_SpawnQuickBars));
	return ServerOffsets::AFortPlayerControllerAthena_SpawnQuickBars;
}

uintptr_t Finder::FindAFortPlayerControllerAthena_SetupQuickBars() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortPlayerControllerAthena_SetupQuickBars)
		return ServerOffsets::AFortPlayerControllerAthena_SetupQuickBars;
	
	//Addr = Memcury::Scanner::FindPattern("").Get(); Havent got this one yet
	
	if (Addr) {
		ServerOffsets::AFortPlayerControllerAthena_SetupQuickBars = Addr - ImageBase;
	}

	Log("AFortPlayerControllerAthena_SetupQuickBars found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerControllerAthena_SetupQuickBars));
	return ServerOffsets::AFortPlayerControllerAthena_SetupQuickBars;
}

uintptr_t Finder::FindAFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup() {
	static uintptr_t Addr = 0;
	if (ServerOffsets::AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup)
		return ServerOffsets::AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup;
	
	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? ? ? ? 48 8B F2 49 8B D1").Get();
	
	if (Addr) {
		ServerOffsets::AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup = Addr - ImageBase;
	}
	Log("AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup));
	return ServerOffsets::AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup;
}

uintptr_t Finder::FindAActor_GetNetOwnerVFT() {
	if (ServerOffsets::AActor_GetNetOwnerVFT)
		return ServerOffsets::AActor_GetNetOwnerVFT;

	if (Version::Engine_Version == 4.16) {
		ServerOffsets::AActor_GetNetOwnerVFT = 0x89;
	}

	Log("AActor_GetNetOwnerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AActor_GetNetOwnerVFT));
	return ServerOffsets::AActor_GetNetOwnerVFT;
}

uintptr_t Finder::FindUAbilitySystemComponent_FindAbilitySpecFromHandle() {
	if (ServerOffsets::UAbilitySystemComponent_FindAbilitySpecFromHandle)
		return ServerOffsets::UAbilitySystemComponent_FindAbilitySpecFromHandle;
	uintptr_t Addr = 0;
	
	Addr = Memcury::Scanner::FindPattern("48 8B 81 ? ? ? ? 48 63 89 ? ? ? ? 4C 6B C1 ? 4C 03 C0 49 3B C0 74 ? 66 0F 1F 44 00 ? 39 50").Get();

	if (Addr) {
		ServerOffsets::UAbilitySystemComponent_FindAbilitySpecFromHandle = Addr - ImageBase;
	}

	Log("UAbilitySystemComponent_FindAbilitySpecFromHandle found at: 0x" + std::format("{:X}", ServerOffsets::UAbilitySystemComponent_FindAbilitySpecFromHandle));
	return ServerOffsets::UAbilitySystemComponent_FindAbilitySpecFromHandle;
}

uintptr_t Finder::FindUChannel_Close() {
	if (ServerOffsets::UChannel_CloseVFT)
		return ServerOffsets::UChannel_CloseVFT;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UChannel::Close: Sending CloseBunch. ChIndex == 0. Name: %s").Get();
	if (StringAddr) {
		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x53)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UChannel_CloseVFT = Addr - ImageBase;
	}

	Log("UChannel_CloseVFT found at: 0x" + std::format("{:X}", ServerOffsets::UChannel_CloseVFT));
	return ServerOffsets::UChannel_CloseVFT;
}

uintptr_t Finder::FindUChannel_CloseVFT() {
	if (ServerOffsets::UChannel_CloseVFT)
		return ServerOffsets::UChannel_CloseVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.Channel"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindUChannel_Close() + ImageBase))
		{
			ServerOffsets::UChannel_CloseVFT = i;
			break;
		}
	}

	Log("UChannel_CloseVFT found at: 0x" + std::format("{:X}", ServerOffsets::UChannel_CloseVFT));
	return ServerOffsets::UChannel_CloseVFT;
}

uintptr_t Finder::FindUChannel_StartBecomingDormantVFT() {
	if (ServerOffsets::UChannel_StartBecomingDormantVFT)
		return ServerOffsets::UChannel_StartBecomingDormantVFT;

	if (Version::Engine_Version == 4.16) {
		ServerOffsets::UChannel_StartBecomingDormantVFT = 0x51;
	}

	Log("UChannel_StartBecomingDormantVFT found at: 0x" + std::format("{:X}", ServerOffsets::UChannel_StartBecomingDormantVFT));
	return ServerOffsets::UChannel_StartBecomingDormantVFT;
}

uintptr_t Finder::FindUNetConnection_GetUChildConnectionVFT() {
	if (ServerOffsets::UNetConnection_GetUChildConnectionVFT)
		return ServerOffsets::UNetConnection_GetUChildConnectionVFT;

	if (Version::Engine_Version == 4.16) {
		ServerOffsets::UNetConnection_GetUChildConnectionVFT = 0x46;
	}

	Log("UNetConnection_GetUChildConnectionVFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetConnection_GetUChildConnectionVFT));
	return ServerOffsets::UNetConnection_GetUChildConnectionVFT;
}

uintptr_t Finder::FindUNetDriver_IsServerVFT() {
	if (ServerOffsets::UNetDriver_IsServerVFT)
		return ServerOffsets::UNetDriver_IsServerVFT;

	if (Version::Engine_Version == 4.16) {
		ServerOffsets::UNetDriver_IsServerVFT = 0x5E;
	}

	Log("UNetDriver_IsServerVFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_IsServerVFT));
	return ServerOffsets::UNetDriver_IsServerVFT;
}

uintptr_t Finder::FindUNetDriver_IsLevelInitializedForActor() {
	if (ServerOffsets::UNetDriver_IsLevelInitializedForActor)
		return ServerOffsets::UNetDriver_IsLevelInitializedForActor;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 49 8B D8 48 8B FA").Get();

	if (Addr) {
		ServerOffsets::UNetDriver_IsLevelInitializedForActor = Addr - ImageBase;
	}

	Log("UNetDriver_IsLevelInitializedForActor found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_IsLevelInitializedForActor));
	return ServerOffsets::UNetDriver_IsLevelInitializedForActor;
}

uintptr_t Finder::FindUNetDriver_IsLevelInitializedForActorVFT() {
	if (ServerOffsets::UNetDriver_IsLevelInitializedForActorVFT)
		return ServerOffsets::UNetDriver_IsLevelInitializedForActorVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.NetDriver"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindUNetDriver_IsLevelInitializedForActor() + ImageBase))
		{
			ServerOffsets::UNetDriver_IsLevelInitializedForActorVFT = i;
			break;
		}
	}

	Log("UNetDriver_IsLevelInitializedForActorVFT found at: 0x" + std::format("{:X}", ServerOffsets::UNetDriver_IsLevelInitializedForActorVFT));
	return ServerOffsets::UNetDriver_IsLevelInitializedForActorVFT;
}

uintptr_t Finder::FindUObject_GetWorldVFT() {
	if (ServerOffsets::UObject_GetWorldVFT)
		return ServerOffsets::UObject_GetWorldVFT;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::UObject_GetWorldVFT;

	// please somebody help me make a finder for ts

	if (!bInitialized) bInitialized = true;
	Log("UObject_GetWorldVFT found at: 0x" + std::format("{:X}", ServerOffsets::UObject_GetWorldVFT));
	return ServerOffsets::UObject_GetWorldVFT;
}

uintptr_t Finder::FindAFortPlayerController_SpawnQuickBarsVFT() {
	if (ServerOffsets::AFortPlayerController_SpawnQuickBarsVFT)
		return ServerOffsets::AFortPlayerController_SpawnQuickBarsVFT;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::AFortPlayerController_SpawnQuickBarsVFT;

	// Not found this yet!

	bInitialized = true;
	Log("AFortPlayerController_SpawnQuickBarsVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_SpawnQuickBarsVFT));
	return ServerOffsets::AFortPlayerController_SpawnQuickBarsVFT;
}

uintptr_t Finder::FindAFortPlayerController_SetupQuickBarsVFT() {
	if (ServerOffsets::AFortPlayerController_SetupQuickBarsVFT)
		return ServerOffsets::AFortPlayerController_SetupQuickBarsVFT;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::AFortPlayerController_SetupQuickBarsVFT;

	// Not found this yet!

	bInitialized = true;
	Log("AFortPlayerController_SetupQuickBarsVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_SetupQuickBarsVFT));
	return ServerOffsets::AFortPlayerController_SetupQuickBarsVFT;
}

uintptr_t Finder::FindAFortGameModeZone_CreateAIDirectorVFT() {
	if (ServerOffsets::AFortGameModeZone_CreateAIDirectorVFT)
		return ServerOffsets::AFortGameModeZone_CreateAIDirectorVFT;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::AFortGameModeZone_CreateAIDirectorVFT;

	// Not found this yet!

	bInitialized = true;
	Log("AFortGameModeZone_CreateAIDirectorVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_CreateAIDirectorVFT));
	return ServerOffsets::AFortGameModeZone_CreateAIDirectorVFT;
}

uintptr_t Finder::FindAFortGameModeZone_CreateAIGoalManager() {
	if (ServerOffsets::AFortGameModeZone_CreateAIGoalManager)
		return ServerOffsets::AFortGameModeZone_CreateAIGoalManager;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"nullptr AI Goal Manager class specified by Game Data; created default AI Goal manager anyway, but some settings may be missing.").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C && *(Ptr + 3) == 0x24 && *(Ptr + 4) == 0x08)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortGameModeZone_CreateAIGoalManager = Addr - ImageBase;
	}

	Log("AFortGameModeZone_CreateAIGoalManager found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_CreateAIGoalManager));
	return ServerOffsets::AFortGameModeZone_CreateAIGoalManager;
}

uintptr_t Finder::FindAFortGameModeZone_CreateAIGoalManagerVFT() {
	if (ServerOffsets::AFortGameModeZone_CreateAIGoalManagerVFT)
		return ServerOffsets::AFortGameModeZone_CreateAIGoalManagerVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortGameModeZone"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortGameModeZone_CreateAIGoalManager() + ImageBase))
		{
			ServerOffsets::AFortGameModeZone_CreateAIGoalManagerVFT = i;
			break;
		}
	}

	Log("AFortGameModeZone_CreateAIGoalManagerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeZone_CreateAIGoalManagerVFT));
	return ServerOffsets::AFortGameModeZone_CreateAIGoalManagerVFT;
}

uintptr_t Finder::FindUObject_NeedsLoadForClientVFT() {
	if (ServerOffsets::UObject_NeedsLoadForClientVFT)
		return ServerOffsets::UObject_NeedsLoadForClientVFT;

	Log("UObject_NeedsLoadForClientVFT found at: 0x" + std::format("{:X}", ServerOffsets::UObject_NeedsLoadForClientVFT));
	return ServerOffsets::UObject_NeedsLoadForClientVFT;
}

uintptr_t Finder::FindUFortWorldItemDefinition_ServerExecuteVFT() {
	if (ServerOffsets::UFortWorldItemDefinition_ServerExecuteVFT)
		return ServerOffsets::UFortWorldItemDefinition_ServerExecuteVFT;

	uintptr_t Addr = 0;
	Addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 56 57 41 56 48 83 EC ? 49 8B E8 48 8B F2 4C 8B F1 E8 ? ? ? ? 48 85 ED").Get();

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortWeaponItemDefinition"))->GetDefaultObject()->VTable;
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(Addr))
		{
			ServerOffsets::UFortWorldItemDefinition_ServerExecuteVFT = i;
			break;
		}
	}

	Log("UFortWorldItemDefinition_ServerExecuteVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItemDefinition_ServerExecuteVFT));
	return ServerOffsets::UFortWorldItemDefinition_ServerExecuteVFT;
}

uintptr_t Finder::FindAFortInventory_GetInventoryCapacity()
{
	if (ServerOffsets::AFortInventory_GetInventoryCapacity)
		return ServerOffsets::AFortInventory_GetInventoryCapacity;

	uintptr_t execGetBackpackItemCounts_ADDR = (uintptr_t)(((UFunction*)FUObjectArray::FindObject("Function /Script/FortniteUI.FortInventoryContext.GetBackpackItemCounts"))->Func);
	uintptr_t GetBackpackItemCounts_Impl_ADDR = 0x0;

	uintptr_t C3_Point = 0x0; //bro if this exec function didnt have 0xC3 at the end i would fucking die

	for (int i = 0; i < 1000; i++)
	{
		uintptr_t CurrentAddr = execGetBackpackItemCounts_ADDR + i;

		if (*(uint8*)(CurrentAddr) == 0xC3)
		{
			C3_Point = CurrentAddr;
			break;
		}
	}

	for (int i = 0; i < 40; i++)
	{
		uintptr_t CurrentAddr2 = C3_Point - i;

		if (*(uint8*)(CurrentAddr2) == 0xE8) { //look for call
			GetBackpackItemCounts_Impl_ADDR = Utils::GetCallDestination(CurrentAddr2);
			break;
		}
	}

	uint8 Skipped = 0x0;

	for (int i = 0; i < 70; i++)
	{
		//we looking for the third call instruction

		uintptr_t CurrentAddress3 = GetBackpackItemCounts_Impl_ADDR + i;

		if (*(uint8*)(CurrentAddress3) == 0xe8)
		{
			if (Skipped == 2)
			{
				ServerOffsets::AFortInventory_GetInventoryCapacity = Utils::GetCallDestination(CurrentAddress3) - ImageBase;
				break;
			}
			Skipped++;
		}
		
	}

	if (!ServerOffsets::AFortInventory_GetInventoryCapacity) {
		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"GetInventoryCapacity is returning an overridden value of %i").Get();
		if (StringAddr)
		{
			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(StringAddr - i);
				if (*Ptr == 0x40 && *(Ptr + 1) == 0x53)
				{
					ServerOffsets::AFortInventory_GetInventoryCapacity = uint64_t(Ptr) - ImageBase;
					break;
				}
			}
		}
	}

	Log("AFortInventory_GetInventoryCapacity found at: 0x" + std::format("{:X}", ServerOffsets::AFortInventory_GetInventoryCapacity));
	return ServerOffsets::AFortInventory_GetInventoryCapacity;
}

uintptr_t Finder::FindABuildingSMActor_PostUpdate()
{
	if (ServerOffsets::ABuildingSMActor_PostUpdate)
		return ServerOffsets::ABuildingSMActor_PostUpdate;

	uintptr_t stringaddr = Memcury::Scanner::FindStringRef(L"ABuildingSMActor::PostUpdate() Building: %s, AltMeshIdx: %d").Get();

	for (int i = 0; i < 200; i++)
	{
		uintptr_t CurrentAddr = stringaddr - i;

		if (*(uint8*)(CurrentAddr) == 0x40 && *(uint8*)(CurrentAddr + 1) == 0x53)
		{
			ServerOffsets::ABuildingSMActor_PostUpdate = CurrentAddr - ImageBase;
			break;
		}
	}

	Log("ABuildingSMActor_PostUpdate found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingSMActor_PostUpdate));
	return ServerOffsets::ABuildingSMActor_PostUpdate;
}

uintptr_t Finder::FindABuildingActor_PostUpdateVFT()
{
	if (ServerOffsets::ABuildingActor_PostUpdateVFT)
		return ServerOffsets::ABuildingActor_PostUpdateVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.BuildingSMActor"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 500; i++)
	{
		if (VFT[i] == (void*)(FindABuildingSMActor_PostUpdate() + ImageBase))
		{
			ServerOffsets::ABuildingActor_PostUpdateVFT = i;
			break;
		}
	}

	Log("ABuildingActor_PostUpdateVFT found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingActor_PostUpdateVFT));
	return ServerOffsets::ABuildingActor_PostUpdateVFT;
}

uintptr_t Finder::FindUFortWorldItem_SetOwningInventory() {
	if (ServerOffsets::UFortWorldItem_SetOwningInventory)
		return ServerOffsets::UFortWorldItem_SetOwningInventory;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 85 D2 74 ? 80 BA ? ? ? ? ? 75 ? 48 89 91").Get();
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 85 D2 74 ? 80 BA ? ? ? ? ? 75 ? 48 81 C1").Get();
	}
	if (!Addr) {
		Addr = Memcury::Scanner::FindPattern("48 85 D2 74 ? 48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 48 8B CA E8 ? ? ? ? 83 F8 ? 75").Get();
	}

	if (Addr) {
		ServerOffsets::UFortWorldItem_SetOwningInventory = Addr - ImageBase;
	}

	Log("UFortWorldItem_SetOwningInventory found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetOwningInventory));
	return ServerOffsets::UFortWorldItem_SetOwningInventory;
}

uintptr_t Finder::FindUFortWorldItem_SetOwningInventoryVFT() {
	if (ServerOffsets::UFortWorldItem_SetOwningInventoryVFT)
		return ServerOffsets::UFortWorldItem_SetOwningInventoryVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortWorldItem"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 512; i++)
	{
		if (VFT[i] == (void*)(FindUFortWorldItem_SetOwningInventory() + ImageBase))
		{
			ServerOffsets::UFortWorldItem_SetOwningInventoryVFT = i;
			break;
		}
	}

	Log("UFortWorldItem_SetOwningInventoryVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetOwningInventoryVFT));
	return ServerOffsets::UFortWorldItem_SetOwningInventoryVFT;
}

uintptr_t Finder::FindUFortWorldItem_SetLoadedAmmoVFT() {
	if (ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT)
		return ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT;

	bool bHasPhantomReserveAmmo = FFortItemEntry::StaticStruct()->FindPropertyByName("PhantomReserveAmmo") != nullptr;
	uintptr_t SetOwningInventoryIdx = FindUFortWorldItem_SetOwningInventoryVFT();

	if (bHasPhantomReserveAmmo) {
		ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT = uint32(SetOwningInventoryIdx - (Version::Engine_Version < 4.27 ? 2 : 3));
	}
	else {
		ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT = uint32(SetOwningInventoryIdx - 1);
	}

	Log("UFortWorldItem_SetLoadedAmmoVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT));
	return ServerOffsets::UFortWorldItem_SetLoadedAmmoVFT;
}

uintptr_t Finder::FindUFortWorldItem_SetPhantomReserveAmmoVFT() {
	if (ServerOffsets::UFortWorldItem_SetPhantomReserveAmmoVFT)
		return ServerOffsets::UFortWorldItem_SetPhantomReserveAmmoVFT;

	bool bHasPhantomReserveAmmo = FFortItemEntry::StaticStruct()->FindPropertyByName("PhantomReserveAmmo") != nullptr;
	if (bHasPhantomReserveAmmo) {
		uintptr_t SetOwningInventoryIdx = FindUFortWorldItem_SetOwningInventoryVFT();

		ServerOffsets::UFortWorldItem_SetPhantomReserveAmmoVFT = uint32(SetOwningInventoryIdx - (Version::Engine_Version < 4.27 ? 1 : 2));
	}

	Log("UFortWorldItem_SetPhantomReserveAmmoVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetPhantomReserveAmmoVFT));
	return ServerOffsets::UFortWorldItem_SetPhantomReserveAmmoVFT;
}

uintptr_t Finder::FindUFortWorldItem_SetInInventoryOverflowVFT() {
	if (ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT)
		return ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT;

	bool bHasPhantomReserveAmmo = FFortItemEntry::StaticStruct()->FindPropertyByName("PhantomReserveAmmo") != nullptr;
	uintptr_t SetOwningInventoryIdx = FindUFortWorldItem_SetOwningInventoryVFT();

	if (bHasPhantomReserveAmmo) {
		// im not 100% about this part
		ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT = uint32(SetOwningInventoryIdx - (Version::Engine_Version < 4.27 ? 8 : 9));
	}
	else {
		ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT = uint32(SetOwningInventoryIdx - 5);
	}

	Log("UFortWorldItem_SetInInventoryOverflowVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT));
	return ServerOffsets::UFortWorldItem_SetInInventoryOverflowVFT;
}

uintptr_t Finder::FindUFortWorldItem_SetDurabilityVFT() {
	if (ServerOffsets::UFortWorldItem_SetDurabilityVFT)
		return ServerOffsets::UFortWorldItem_SetDurabilityVFT;

	uintptr_t SetInInventoryOverflowIdx = FindUFortWorldItem_SetInInventoryOverflowVFT();
	if (!SetInInventoryOverflowIdx)
		return 0;

	ServerOffsets::UFortWorldItem_SetDurabilityVFT = uint32(SetInInventoryOverflowIdx - 1);

	Log("UFortWorldItem_SetDurabilityVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWorldItem_SetDurabilityVFT));
	return ServerOffsets::UFortWorldItem_SetDurabilityVFT;
}

uintptr_t Finder::FindAFortPlayerController_RemoveInventoryItem() {
	if (ServerOffsets::AFortPlayerController_RemoveInventoryItem)
		return ServerOffsets::AFortPlayerController_RemoveInventoryItem;
	uintptr_t Addr = 0;
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		std::vector<uint8_t> funcStart = Version::Engine_Version == 4.16
			? std::vector<uint8_t> { 0x44, 0x88, 0x4C }
		: (Version::Fortnite_Version >= 16 && (Version::Fortnite_Version < 20 || Version::Fortnite_Version >= 22) ? std::vector<uint8_t> { 0x48, 0x8B, 0xC4 }
		: std::vector<uint8_t>{ 0x48, 0x89, 0x5C });

		auto sRef = FindNameRef(L"ServerRemoveInventoryItem", 0, false);
		uintptr_t uFuncCall = 0;
		for (int i = 0; i < 2000; i++)
		{
			if (Version::Engine_Version == 4.16)
			{
				if (*(uint8_t*)(sRef - i) == 0x44 && *(uint8_t*)(sRef - i + 1) == 0x88 && *(uint8_t*)(sRef - i + 2) == 0x4C)
				{
					uFuncCall = sRef - i;
					break;
				}
			}
			else
			{
				if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C)
				{
					uFuncCall = sRef - i;
					break;
				}
				else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4)
				{
					uFuncCall = sRef - i;
					break;
				}
			}
		}

		auto ServerRemoveInventoryItemCall = Memcury::Scanner::FindPointerRef((PVOID)uFuncCall, 0, true);

		for (int i = 0; i < 400; i++)
		{
			if (
				*(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i) == 0x48
				&& *(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i + 1) == 0x89
				&& *(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i + 2) == 0x5C
				) {
				Addr = ServerRemoveInventoryItemCall.Get() - i;
				break;
			}
				

			else if (
				*(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i) == 0x48
				&& *(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i + 1) == 0x83
				&& *(uint8_t*)(ServerRemoveInventoryItemCall.Get() - i + 2) == 0xEC
				) {
				Addr = ServerRemoveInventoryItemCall.Get() - i;
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortPlayerController_RemoveInventoryItem = Addr - ImageBase;
	}

	Log("AFortPlayerController_RemoveInventoryItem found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_RemoveInventoryItem));
	return ServerOffsets::AFortPlayerController_RemoveInventoryItem;
}

uintptr_t Finder::FindAFortPlayerController_RemoveInventoryItemVFT() {
	if (ServerOffsets::AFortPlayerController_RemoveInventoryItemVFT)
		return ServerOffsets::AFortPlayerController_RemoveInventoryItemVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortPlayerControllerAthena"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindAFortPlayerController_RemoveInventoryItem() + ImageBase))
		{
			ServerOffsets::AFortPlayerController_RemoveInventoryItemVFT = i;
			break;
		}
	}

	Log("AFortPlayerController_RemoveInventoryItemVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_RemoveInventoryItemVFT));
	return ServerOffsets::AFortPlayerController_RemoveInventoryItemVFT;
}

uintptr_t Finder::FindUFortControllerComponent_Aircraft_EnterAircraft() {
	if (ServerOffsets::UFortControllerComponent_Aircraft_EnterAircraft)
		return ServerOffsets::UFortControllerComponent_Aircraft_EnterAircraft;
	uintptr_t Addr = 0;
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		auto sRef = Memcury::Scanner::FindStringRef(L"EnterAircraft: [%s] is attempting to enter aircraft after having already exited.", true, 0, Version::Fortnite_Version >= 19)
			.Get();

		if (!sRef)
			return 0;

		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(sRef - i);

			if (*Ptr == 0x48 && (*(Ptr + 1) == 0x83 || *(Ptr + 1) == 0x81) && *(Ptr + 2) == 0xEC)
			{
				sRef = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x8D && *(Ptr + 2) == 0xAC)
			{
				sRef = uint64_t(Ptr);
				break;
			}
		}
		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(sRef - i);

			if (*(uint8_t*)(sRef - i) == 0x40 && (*(uint8_t*)(sRef - i + 1) == 0x53 || *(uint8_t*)(sRef - i + 1) == 0x55))
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (Version::Fortnite_Version >= 15 && *Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x74)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortControllerComponent_Aircraft_EnterAircraft = Addr - ImageBase;
	}

	Log("UFortControllerComponent_Aircraft_EnterAircraft found at: 0x" + std::format("{:X}", ServerOffsets::UFortControllerComponent_Aircraft_EnterAircraft));
	return ServerOffsets::UFortControllerComponent_Aircraft_EnterAircraft;
}

uintptr_t Finder::FindAController_InitPlayerState() {
	if (ServerOffsets::AController_InitPlayerState)
		return ServerOffsets::AController_InitPlayerState;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 83 F8 ? 0F 84 ? ? ? ? ? ? ? 48 8B CF 48 89 5C 24 ? 48 89 74 24").Get();

	if (Addr) {
		ServerOffsets::AController_InitPlayerState = Addr - ImageBase;
	}

	Log("AController_InitPlayerState found at: 0x" + std::format("{:X}", ServerOffsets::AController_InitPlayerState));
	return ServerOffsets::AController_InitPlayerState;
}

uintptr_t Finder::FindAController_InitPlayerStateVFT() {
	if (ServerOffsets::AController_InitPlayerStateVFT)
		return ServerOffsets::AController_InitPlayerStateVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.Controller"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAController_InitPlayerState() + ImageBase))
		{
			ServerOffsets::AController_InitPlayerStateVFT = i;
			break;
		}
	}

	Log("AController_InitPlayerStateVFT found at: 0x" + std::format("{:X}", ServerOffsets::AController_InitPlayerStateVFT));
	return ServerOffsets::AController_InitPlayerStateVFT;
}

uintptr_t Finder::FindAActor_BeginPlay() {
	if (ServerOffsets::AActor_BeginPlay)
		return ServerOffsets::AActor_BeginPlay;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("4C 8B DC 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? ? ? ? F3 0F 10 89").Get();

	if (Addr) {
		ServerOffsets::AActor_BeginPlay = Addr - ImageBase;
	}

	Log("AActor_BeginPlay found at: 0x" + std::format("{:X}", ServerOffsets::AActor_BeginPlay));
	return ServerOffsets::AActor_BeginPlay;
}

uintptr_t Finder::FindAActor_BeginPlayVFT() {
	if (ServerOffsets::AActor_BeginPlayVFT)
		return ServerOffsets::AActor_BeginPlayVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.Actor"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAActor_BeginPlay() + ImageBase))
		{
			ServerOffsets::AActor_BeginPlayVFT = i;
			break;
		}
	}

	Log("AActor_BeginPlayVFT found at: 0x" + std::format("{:X}", ServerOffsets::AActor_BeginPlayVFT));
	return ServerOffsets::AActor_BeginPlayVFT;
}

uintptr_t Finder::FindAGameModeBase_SpawnPlayerController() {
	if (ServerOffsets::AGameModeBase_SpawnPlayerController)
		return ServerOffsets::AGameModeBase_SpawnPlayerController;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("40 53 55 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B F9 4D 8B F1").Get();

	if (Addr) {
		ServerOffsets::AGameModeBase_SpawnPlayerController = Addr - ImageBase;
	}

	Log("AGameModeBase_SpawnPlayerController found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_SpawnPlayerController));
	return ServerOffsets::AGameModeBase_SpawnPlayerController;
}

uintptr_t Finder::FindAGameModeBase_SpawnPlayerControllerVFT() {
	if (ServerOffsets::AGameModeBase_SpawnPlayerControllerVFT)
		return ServerOffsets::AGameModeBase_SpawnPlayerControllerVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.GameModeBase"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAGameModeBase_SpawnPlayerController() + ImageBase))
		{
			ServerOffsets::AGameModeBase_SpawnPlayerControllerVFT = i;
			break;
		}
	}

	Log("AGameModeBase_SpawnPlayerControllerVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_SpawnPlayerControllerVFT));
	return ServerOffsets::AGameModeBase_SpawnPlayerControllerVFT;
}

uintptr_t Finder::FindAFortGameModeAthena_AddToAlivePlayers() {
	if (ServerOffsets::AFortGameModeAthena_AddToAlivePlayers)
		return ServerOffsets::AFortGameModeAthena_AddToAlivePlayers;
	uintptr_t Addr = 0;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"FortGameModeAthena: Player [%s] added to alive players list (Team [%d]).  Player count is now [%d].  Team count is now [%d]. ").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"FortGameModeAthena::AddToAlivePlayers: Player [%s] PC [%s] doesn't have a valid PvP team, and won't be added to the alive players list.").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"FortGameModeAthena::AddToAlivePlayers: Player [%s] PC [%s] added to alive players list (Team [%d]).  Player count is now [%d].  Team count is now [%d].").Get();
	}

	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x53)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x85 && *(Ptr + 2) == 0xD2)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortGameModeAthena_AddToAlivePlayers = Addr - ImageBase;
	}

	Log("AFortGameModeAthena_AddToAlivePlayers found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameModeAthena_AddToAlivePlayers));
	return ServerOffsets::AFortGameModeAthena_AddToAlivePlayers;
}

uintptr_t Finder::FindUFortAnalytics_SetGameSessionID() {
	if (ServerOffsets::UFortAnalytics_SetGameSessionID)
		return ServerOffsets::UFortAnalytics_SetGameSessionID;
	uintptr_t Addr = 0;

	if (Version::Fortnite_Version == 1.91) {
		Addr = ImageBase + 0x3C7C80;
	}
	else if (Version::Fortnite_Version == 1.10) {
		Addr = ImageBase + 0x3DBE70;
	}
	else {
		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Core.GameSessionIDChanged").Get();
		if (StringAddr) {
			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(StringAddr - i);
				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::UFortAnalytics_SetGameSessionID = Addr - ImageBase;
	}

	Log("UFortAnalytics_SetGameSessionID found at: 0x" + std::format("{:X}", ServerOffsets::UFortAnalytics_SetGameSessionID));
	return ServerOffsets::UFortAnalytics_SetGameSessionID;
}

uintptr_t Finder::FindUFortAnalytics_SetGameStateClassName() {
	if (ServerOffsets::UFortAnalytics_SetGameStateClassName)
		return ServerOffsets::UFortAnalytics_SetGameStateClassName;
	uintptr_t Addr = 0;

	if (Version::Fortnite_Version == 1.91) {
		Addr = ImageBase + 0x3C8040;
	}
	else if (Version::Fortnite_Version == 1.10) {
		Addr = ImageBase + 0x3DC300;
	}
	else {
		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Core.GameStateClassNameChanged").Get();
		if (StringAddr) {
			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(StringAddr - i);
				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortAnalytics_SetGameStateClassName = Addr - ImageBase;
	}

	Log("UFortAnalytics_SetGameStateClassName found at: 0x" + std::format("{:X}", ServerOffsets::UFortAnalytics_SetGameStateClassName));
	return ServerOffsets::UFortAnalytics_SetGameStateClassName;
}

uintptr_t Finder::FindAFortGameMode_PickTeam() {
	if (ServerOffsets::AFortGameMode_PickTeam)
		return ServerOffsets::AFortGameMode_PickTeam;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Failed to find a team, resorting to smallest team %s!").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"PickTeam for %s preferred: %s actual: %s").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"Failed to find a team, resorting to smallest team %d!").Get();
	}
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"PickTeam for %s preferred: %d actual: %d").Get();
	}

	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortGameMode_PickTeam = Addr - ImageBase;
	}

	Log("AFortGameMode_PickTeam found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_PickTeam));
	return ServerOffsets::AFortGameMode_PickTeam;
}

uintptr_t Finder::FindAFortGameMode_PickTeamVFT() {
	if (ServerOffsets::AFortGameMode_PickTeamVFT)
		return ServerOffsets::AFortGameMode_PickTeamVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortGameMode"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortGameMode_PickTeam() + ImageBase))
		{
			ServerOffsets::AFortGameMode_PickTeamVFT = i;
			break;
		}
	}

	Log("AFortGameMode_PickTeamVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameMode_PickTeamVFT));
	return ServerOffsets::AFortGameMode_PickTeamVFT;
}

uintptr_t Finder::FindAGameModeBase_GetGameSessionClass() {
	if (ServerOffsets::AGameModeBase_GetGameSessionClass)
		return ServerOffsets::AGameModeBase_GetGameSessionClass;
	static bool bInitialized = false;
	if (bInitialized)
	{
		return ServerOffsets::AGameModeBase_GetGameSessionClass;
	}
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 83 B9 ? ? ? ? 00 48 8B DA 48 8B F9 74 ? E8 ? ? ? ? 48 8B 97").Get();

	if (Addr) {
		ServerOffsets::AGameModeBase_GetGameSessionClass = Addr - ImageBase;
	}

	if (!bInitialized) {
		bInitialized = true;
	}
	Log("AGameModeBase_GetGameSessionClass found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_GetGameSessionClass));
	return ServerOffsets::AGameModeBase_GetGameSessionClass;
}

uintptr_t Finder::FindAGameModeBase_GetGameSessionClassVFT() {
	if (ServerOffsets::AGameModeBase_GetGameSessionClassVFT)
		return ServerOffsets::AGameModeBase_GetGameSessionClassVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/Engine.GameModeBase"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAGameModeBase_GetGameSessionClass() + ImageBase))
		{
			ServerOffsets::AGameModeBase_GetGameSessionClassVFT = i;
			break;
		}
	}

	Log("AGameModeBase_GetGameSessionClassVFT found at: 0x" + std::format("{:X}", ServerOffsets::AGameModeBase_GetGameSessionClassVFT));
	return ServerOffsets::AGameModeBase_GetGameSessionClassVFT;
}

uintptr_t Finder::FindAFortGameSessionDedicated_FinalizeCreation() {
	if (ServerOffsets::AFortGameSessionDedicated_FinalizeCreation)
		return ServerOffsets::AFortGameSessionDedicated_FinalizeCreation;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Dedicated Server Ready!").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
			{
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortGameSessionDedicated_FinalizeCreation = Addr - ImageBase;
	}

	Log("AFortGameSessionDedicated_FinalizeCreation found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameSessionDedicated_FinalizeCreation));
	return ServerOffsets::AFortGameSessionDedicated_FinalizeCreation;
}

uintptr_t Finder::FindAFortGameSessionDedicated_FinalizeCreationPatch1() {
	if (ServerOffsets::AFortGameSessionDedicated_FinalizeCreationPatch1)
		return ServerOffsets::AFortGameSessionDedicated_FinalizeCreationPatch1;
	uintptr_t Addr = 0;
	uintptr_t FinalizeCreationAddr = FindAFortGameSessionDedicated_FinalizeCreation() + ImageBase;

	for (int i = 0; i < 512; i++)
	{
		auto Ptr = (uint8_t*)(FinalizeCreationAddr + i);
		if (*Ptr == 0x0F && *(Ptr + 1) == 0x84 && *(Ptr + 2) == 0x9C)
		{
			Addr = uint64_t(Ptr + 1);
			break;
		}
		else if (*Ptr == 0x0F && *(Ptr + 1) == 0x84 && *(Ptr + 2) == 0x97)
		{
			Addr = uint64_t(Ptr + 1);
			break;
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortGameSessionDedicated_FinalizeCreationPatch1 = Addr - ImageBase;
	}

	Log("AFortGameSessionDedicated_FinalizeCreationPatch1 found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameSessionDedicated_FinalizeCreationPatch1));
	return ServerOffsets::AFortGameSessionDedicated_FinalizeCreationPatch1;
}

uintptr_t Finder::FindAFortGameSessionDedicated_FinalizeCreationVFT() {
	if (ServerOffsets::AFortGameSessionDedicated_FinalizeCreationVFT)
		return ServerOffsets::AFortGameSessionDedicated_FinalizeCreationVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortGameSessionDedicated"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindAFortGameSessionDedicated_FinalizeCreation() + ImageBase))
		{
			ServerOffsets::AFortGameSessionDedicated_FinalizeCreationVFT = i;
			break;
		}
	}

	Log("AFortGameSessionDedicated_FinalizeCreationVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameSessionDedicated_FinalizeCreationVFT));
	return ServerOffsets::AFortGameSessionDedicated_FinalizeCreationVFT;
}

uintptr_t Finder::FindAFortPlayerController_CanAffordToPlaceBuildableClass() {
	if (ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClass)
		return ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClass;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Resource not found! Resource Type is %i, might be invalid").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x57)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClass = Addr - ImageBase;
	}

	Log("AFortPlayerController_CanAffordToPlaceBuildableClass found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClass));
	return ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClass;
}

uintptr_t Finder::FindAFortPlayerController_CanAffordToPlaceBuildableClassVFT() {
	if (ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClassVFT)
		return ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClassVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortPlayerController"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindAFortPlayerController_CanAffordToPlaceBuildableClass() + ImageBase))
		{
			ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClassVFT = i;
			break;
		}
	}

	Log("AFortPlayerController_CanAffordToPlaceBuildableClassVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClassVFT));
	return ServerOffsets::AFortPlayerController_CanAffordToPlaceBuildableClassVFT;
}

uintptr_t Finder::FindAFortPlayerController_PayBuildableClassPlacementCost() {
	if (ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCost)
		return ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCost;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Failed to remove item %s during pay building costs, item duplicated!").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCost = Addr - ImageBase;
	}

	Log("AFortPlayerController_PayBuildableClassPlacementCost found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCost));
	return ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCost;
}

uintptr_t Finder::FindAFortPlayerController_PayBuildableClassPlacementCostVFT() {
	if (ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCostVFT)
		return ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCostVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortPlayerController"))->GetDefaultObject()->VTable;

	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindAFortPlayerController_PayBuildableClassPlacementCost() + ImageBase))
		{
			ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCostVFT = i;
			break;
		}
	}

	Log("AFortPlayerController_PayBuildableClassPlacementCostVFT found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCostVFT));
	return ServerOffsets::AFortPlayerController_PayBuildableClassPlacementCostVFT;
}

uintptr_t Finder::FindABuildingSMActor_SetEditingPlayer() {
	if (ServerOffsets::ABuildingSMActor_SetEditingPlayer)
		return ServerOffsets::ABuildingSMActor_SetEditingPlayer;
	uintptr_t Addr = 0;

	Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? 48 83 B9 ? ? ? ? 00").Get();

	if (Addr) {
		ServerOffsets::ABuildingSMActor_SetEditingPlayer = Addr - ImageBase;
	}

	Log("ABuildingSMActor_SetEditingPlayer found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingSMActor_SetEditingPlayer));
	return ServerOffsets::ABuildingSMActor_SetEditingPlayer;
}

uintptr_t Finder::FindABuildingSMActor_SetEditingPlayerVFT() {
	if (ServerOffsets::ABuildingSMActor_SetEditingPlayerVFT)
		return ServerOffsets::ABuildingSMActor_SetEditingPlayerVFT;
	
	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.BuildingSMActor"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindABuildingSMActor_SetEditingPlayer() + ImageBase))
		{
			ServerOffsets::ABuildingSMActor_SetEditingPlayerVFT = i;
			break;
		}
	}

	Log("ABuildingSMActor_SetEditingPlayerVFT found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingSMActor_SetEditingPlayerVFT));
	return ServerOffsets::ABuildingSMActor_SetEditingPlayerVFT;
}

uintptr_t Finder::FindUClass_GetSparseClassData() {
	if (ServerOffsets::UClass_GetSparseClassData)
		return ServerOffsets::UClass_GetSparseClassData;
	uintptr_t Addr = 0;
	static bool bInitialized = false;
	if (bInitialized) {
		return ServerOffsets::UClass_GetSparseClassData;
	}

	Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 81 ? ? ? ? 45 33 C0 4C 8B C9").Get();
	if (!Addr)
		Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 ? 48 83 C4 ? C3").Get();
	if (!Addr)
		Addr = Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 81 ? ? ? ? 45 33 C0 48 85 C0 75").Get();
	
	if (Addr) {
		ServerOffsets::UClass_GetSparseClassData = Addr - ImageBase;
	}

	bInitialized = true;
	Log("UClass_GetSparseClassData found at: 0x" + std::format("{:X}", ServerOffsets::UClass_GetSparseClassData));
	return ServerOffsets::UClass_GetSparseClassData;
}

uintptr_t Finder::FindUFortKismetLibrary_GetWeaponStatsRow() {
	if (ServerOffsets::UFortKismetLibrary_GetWeaponStatsRow)
		return ServerOffsets::UFortKismetLibrary_GetWeaponStatsRow;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UFortKismetLibrary::GetWeaponStatsRow").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortKismetLibrary_GetWeaponStatsRow = Addr - ImageBase;
	}

	Log("UFortKismetLibrary_GetWeaponStatsRow found at: 0x" + std::format("{:X}", ServerOffsets::UFortKismetLibrary_GetWeaponStatsRow));
	return ServerOffsets::UFortKismetLibrary_GetWeaponStatsRow;
}

uintptr_t Finder::FindFFortBaseWeaponStats_GetDurability() {
	if (ServerOffsets::FFortBaseWeaponStats_GetDurability)
		return ServerOffsets::FFortBaseWeaponStats_GetDurability;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Base Durability Lookup").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::FFortBaseWeaponStats_GetDurability = Addr - ImageBase;
	}

	Log("FFortBaseWeaponStats_GetDurability found at: 0x" + std::format("{:X}", ServerOffsets::FFortBaseWeaponStats_GetDurability));
	return ServerOffsets::FFortBaseWeaponStats_GetDurability;
}

uintptr_t Finder::FindUFortWeaponItemDefinition_GetMaxDurability() {
	if (ServerOffsets::UFortWeaponItemDefinition_GetMaxDurability)
		return ServerOffsets::UFortWeaponItemDefinition_GetMaxDurability;
	uintptr_t Addr = 0;

	uintptr_t GetDurabilityAddr = FindFFortBaseWeaponStats_GetDurability() + ImageBase;
	if (GetDurabilityAddr) {
		uintptr_t XrefAddr = Memcury::Scanner::FindPointerRef((LPVOID)GetDurabilityAddr, 4).Get();
		if (XrefAddr) {
			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(XrefAddr - i);
				if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
				{
					Addr = uint64_t(Ptr);
					break;
				}
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortWeaponItemDefinition_GetMaxDurability = Addr - ImageBase;
	}

	Log("UFortWeaponItemDefinition_GetMaxDurability found at: 0x" + std::format("{:X}", ServerOffsets::UFortWeaponItemDefinition_GetMaxDurability));
	return ServerOffsets::UFortWeaponItemDefinition_GetMaxDurability;
}

uintptr_t Finder::FindUFortWeaponItemDefinition_GetMaxDurabilityVFT() {
	if (ServerOffsets::UFortWeaponItemDefinition_GetMaxDurabilityVFT)
		return ServerOffsets::UFortWeaponItemDefinition_GetMaxDurabilityVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.FortWeaponItemDefinition"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 1024; i++)
	{
		if (VFT[i] == (void*)(FindUFortWeaponItemDefinition_GetMaxDurability() + ImageBase))
		{
			ServerOffsets::UFortWeaponItemDefinition_GetMaxDurabilityVFT = i;
			break;
		}
	}

	Log("UFortWeaponItemDefinition_GetMaxDurabilityVFT found at: 0x" + std::format("{:X}", ServerOffsets::UFortWeaponItemDefinition_GetMaxDurabilityVFT));
	return ServerOffsets::UFortWeaponItemDefinition_GetMaxDurabilityVFT;
}

uintptr_t Finder::FindAFortPlayerState_ApplyCharacterCustomization() {
	if (ServerOffsets::AFortPlayerState_ApplyCharacterCustomization)
		return ServerOffsets::AFortPlayerState_ApplyCharacterCustomization;
	uintptr_t Addr = 0;
	static bool bInitialized = false;

	if (!bInitialized)
	{
		bInitialized = true;

		if (Version::Fortnite_Version == 17.50) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 4C 8B EA").Get();
		}
		else if (Version::Fortnite_Version >= 24)
		{
			Addr = Memcury::Scanner::FindPattern(
				"48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F1")
				.Get();

			if (!Addr)
				Addr = Memcury::Scanner::FindPattern(
					"48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B C2")
				.Get();

			if (!Addr)
				Addr
				= Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 80 B9").Get();

			return Addr;
		}
		else if (std::floor(Version::Fortnite_Version) == 22) {
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 80 B9").Get();
		}
		else {
			auto sRef = Memcury::Scanner::FindStringRef(
				L"AFortPlayerState::ApplyCharacterCustomization - Failed initialization, using default parts. Player Controller: %s PlayerState: %s, HeroId: %s", false, 0,
				Version::Fortnite_Version >= 17, Version::Fortnite_Version < 20.40 && Version::Fortnite_Version != 19.01)
				.Get();

			if (sRef) {
				for (int i = 100; i < 7000; i++)
				{
					if (*(uint8_t*)(sRef - i) == 0x40 && (*(uint8_t*)(sRef - i + 1) == 0x53 || *(uint8_t*)(sRef - i + 1) == 0x55)) {
						Addr = sRef - i;
						break;
					}

					if (Version::Fortnite_Version >= 15)
					{
						if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C) {
							Addr = sRef - i;
							break;
						}
					}

					if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4) {
						Addr = sRef - i;
						break;
					}

					if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x54 && *(uint8_t*)(sRef - i + 5) == 0x55) {
						Addr = sRef - i;
						break;
					}
				}
			}
			else {
				Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 50 10 55 57 48 8D 68 A1 48 81 EC ? ? ? ? 80 B9").Get();
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::AFortPlayerState_ApplyCharacterCustomization = Addr - ImageBase;
	}

	Log("AFortPlayerState_ApplyCharacterCustomization found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerState_ApplyCharacterCustomization));
	return ServerOffsets::AFortPlayerState_ApplyCharacterCustomization;
}

uintptr_t Finder::FindAFortGameSessionDedicated_OnUpdateComplete() {
	if (ServerOffsets::AFortGameSessionDedicated_OnUpdateComplete)
		return ServerOffsets::AFortGameSessionDedicated_OnUpdateComplete;
	uintptr_t Addr = 0;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"OnUpdateComplete %d").Get();
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortGameSessionDedicated_OnUpdateComplete = Addr - ImageBase;
	}

	Log("AFortGameSessionDedicated_OnUpdateComplete found at: 0x" + std::format("{:X}", ServerOffsets::AFortGameSessionDedicated_OnUpdateComplete));
	return ServerOffsets::AFortGameSessionDedicated_OnUpdateComplete;
}

uintptr_t Finder::FindUNavigationSystem_CreateNavigationSystem() {
	if (ServerOffsets::UNavigationSystem_CreateNavigationSystem)
		return ServerOffsets::UNavigationSystem_CreateNavigationSystem;
	uintptr_t Addr = 0;
	
	Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 57 48 83 EC ? 33 F6 48 8B F9 48 85 C9").Get();

	if (Addr) {
		ServerOffsets::UNavigationSystem_CreateNavigationSystem = Addr - ImageBase;
	}

	Log("UNavigationSystem_CreateNavigationSystem found at: 0x" + std::format("{:X}", ServerOffsets::UNavigationSystem_CreateNavigationSystem));
	return ServerOffsets::UNavigationSystem_CreateNavigationSystem;
}

uintptr_t Finder::FindABuildingContainer_ServerOnAttemptInteract() {
	if (ServerOffsets::ABuildingContainer_ServerOnAttemptInteract)
		return ServerOffsets::ABuildingContainer_ServerOnAttemptInteract;
	uintptr_t Addr = 0;
	
	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ABuildingContainer::ServerOnAttemptInteract %s failed for %s").Get();
	if (StringAddr)
	{
		
	}
	else
	{
		Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC ? 0F B6 02 48 8B D9 FE C8").Get();
	}

	if (Addr) {
		ServerOffsets::ABuildingContainer_ServerOnAttemptInteract = Addr - ImageBase;
	}

	Log("ABuildingContainer_ServerOnAttemptInteract found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingContainer_ServerOnAttemptInteract));
	return ServerOffsets::ABuildingContainer_ServerOnAttemptInteract;
}

uintptr_t Finder::FindABuildingActor_ServerOnAttemptInteractVFT() {
	if (ServerOffsets::ABuildingActor_ServerOnAttemptInteractVFT)
		return ServerOffsets::ABuildingActor_ServerOnAttemptInteractVFT;

	void** VFT = ((UClass*)FUObjectArray::FindObject("Class /Script/FortniteGame.BuildingContainer"))->GetDefaultObject()->VTable;
	
	for (int i = 0; i < 2048; i++)
	{
		if (VFT[i] == (void*)(FindABuildingContainer_ServerOnAttemptInteract() + ImageBase))
		{
			ServerOffsets::ABuildingActor_ServerOnAttemptInteractVFT = i;
			break;
		}
	}

	Log("ABuildingContainer_ServerOnAttemptInteractVFT found at: 0x" + std::format("{:X}", ServerOffsets::ABuildingActor_ServerOnAttemptInteractVFT));
	return ServerOffsets::ABuildingActor_ServerOnAttemptInteractVFT;
}

uintptr_t Finder::FindUWorld_ListenPatch() {
	if (ServerOffsets::UWorld_ListenPatch)
		return ServerOffsets::UWorld_ListenPatch;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::UWorld_ListenPatch;

	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"LoadMap: failed to Listen(%s)").Get();
	if (StringAddr) {
		int Skipped = 0;

		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0xE8)
			{
				if (Skipped == 1) {
					Addr = uint64_t(Ptr);
					break;
				}

				Skipped++;
			}
		}
	}
	
	if (Addr) {
		ServerOffsets::UWorld_ListenPatch = Addr - ImageBase;
	}

	bInitialized = true;
	Log("UWorld_ListenPatch found at: 0x" + std::format("{:X}", ServerOffsets::UWorld_ListenPatch));
	return ServerOffsets::UWorld_ListenPatch;
}

uintptr_t Finder::FindFDedicatedServerUrlContext_Constructor() {
	if (ServerOffsets::FDedicatedServerUrlContext_Constructor)
		return ServerOffsets::FDedicatedServerUrlContext_Constructor;
	uintptr_t Addr = 0;

	// idk
	
	if (Addr) {
		ServerOffsets::FDedicatedServerUrlContext_Constructor = Addr - ImageBase;
	}

	Log("FDedicatedServerUrlContext_Constructor found at: 0x" + std::format("{:X}", ServerOffsets::FDedicatedServerUrlContext_Constructor));
	return ServerOffsets::FDedicatedServerUrlContext_Constructor;
}

uintptr_t Finder::FindUFortQuestManager_SendCustomStatEvent() {
	if (ServerOffsets::UFortQuestManager_SendCustomStatEvent)
		return ServerOffsets::UFortQuestManager_SendCustomStatEvent;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"SendCustomStatEvent: Called with invalid or non custom stat %s!").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"UFortQuestManager::SendCustomStatEven: Called with invalid or non custom stat %s!").Get();
	}
	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortQuestManager_SendCustomStatEvent = Addr - ImageBase;
	}

	Log("UFortQuestManager_SendCustomStatEvent found at: 0x" + std::format("{:X}", ServerOffsets::UFortQuestManager_SendCustomStatEvent));
	return ServerOffsets::UFortQuestManager_SendCustomStatEvent;
}

uintptr_t Finder::FindUFortQuestManager_QueueStatEvent() {
	if (ServerOffsets::UFortQuestManager_QueueStatEvent)
		return ServerOffsets::UFortQuestManager_QueueStatEvent;
	uintptr_t Addr = 0;

	auto sRef = Memcury::Scanner::FindStringRef(L"UFortQuestManager::QueueStatEvent: %s tried to queue a stat event, but the player controller is null. \n\t Event: %s", false, 0,Version::Fortnite_Version >= 19, false);
	if (sRef.IsValid()) {
		for (int i = 0; i < 2000; i++)
		{
			auto Ptr = (uint8_t*)(sRef.Get() - i);

			if (*Ptr == 0x48 && *(Ptr + 1) == 0x8B && *(Ptr + 2) == 0xC4) {
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5C) {
				Addr = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x40 && *(Ptr + 1) == 0x55) {
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortQuestManager_QueueStatEvent = Addr - ImageBase;
	}

	Log("UFortQuestManager_QueueStatEvent found at: 0x" + std::format("{:X}", ServerOffsets::UFortQuestManager_QueueStatEvent));
	return ServerOffsets::UFortQuestManager_QueueStatEvent;
}

uintptr_t Finder::FindUFortQuestManager_SendStatEvent() {
	if (ServerOffsets::UFortQuestManager_SendStatEvent)
		return ServerOffsets::UFortQuestManager_SendStatEvent;
	static bool bInitialized = false;
	if (bInitialized)
		return ServerOffsets::UFortQuestManager_SendStatEvent;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"SendStatEvent for %s: %d quests listening to event %s %s TargetTags:%s SourceTags:%s ContextTags:%s").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"SendStatEvent for %s: %d quests listening to flushed event %s %s %s %s %s").Get();
	}

	if (StringAddr) {
		for (int i = 0; i < 4096; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x55)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortQuestManager_SendStatEvent = Addr - ImageBase;
	}

	if (!bInitialized) bInitialized = true;
	Log("UFortQuestManager_SendStatEvent found at: 0x" + std::format("{:X}", ServerOffsets::UFortQuestManager_SendStatEvent));
	return ServerOffsets::UFortQuestManager_SendStatEvent;
}

uintptr_t Finder::FindUFortQuestManager_ProcessPendingStatEvents() {
	if (ServerOffsets::UFortQuestManager_ProcessPendingStatEvents)
		return ServerOffsets::UFortQuestManager_ProcessPendingStatEvents;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"ProcessPendingStatEvents for %s: %s").Get();
	if (!StringAddr) {
		StringAddr = Memcury::Scanner::FindStringRef(L"LogPendingChanges: ProcessPendingStatEvents for %s: Context:(%s) %d Changes:%s").Get();
	}

	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x4C)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UFortQuestManager_ProcessPendingStatEvents = Addr - ImageBase;
	}

	Log("UFortQuestManager_ProcessPendingStatEvents found at: 0x" + std::format("{:X}", ServerOffsets::UFortQuestManager_ProcessPendingStatEvents));
	return ServerOffsets::UFortQuestManager_ProcessPendingStatEvents;
}

uintptr_t Finder::FindAFortPlayerController_PayBuildingRepairCost() {
	if (ServerOffsets::AFortPlayerController_PayBuildingRepairCost)
		return ServerOffsets::AFortPlayerController_PayBuildingRepairCost;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Failed to remove item %s during repair, item duplicated!").Get();

	if (StringAddr) {
		for (int i = 0; i < 1024; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0x40 && *(Ptr + 1) == 0x57)
			{
				Addr = uint64_t(Ptr);
				break;
			}
		}
	}

	if (Addr) {
		ServerOffsets::AFortPlayerController_PayBuildingRepairCost = Addr - ImageBase;
	}

	Log("AFortPlayerController_PayBuildingRepairCost found at: 0x" + std::format("{:X}", ServerOffsets::AFortPlayerController_PayBuildingRepairCost));
	return ServerOffsets::AFortPlayerController_PayBuildingRepairCost;
}

uintptr_t Finder::FindUObject_CanCreateInCurrentContext() {
	if (ServerOffsets::UObject_CanCreateInCurrentContext)
		return ServerOffsets::UObject_CanCreateInCurrentContext;
	uintptr_t Addr = 0;

	uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Unable to spawn class '%s' due to client/server context.").Get();
	if (StringAddr) {
		int Skipped = 0;
		for (int i = 0; i < 512; i++)
		{
			auto Ptr = (uint8_t*)(StringAddr - i);
			if (*Ptr == 0xE8)
			{
				if (Skipped == 1) {
					Addr = Utils::GetCallDestination((uintptr_t)Ptr);
					break;
				}
				Skipped++;
			}
		}
	}

	if (Addr) {
		ServerOffsets::UObject_CanCreateInCurrentContext = Addr - ImageBase;
	}

	Log("UObject_CanCreateInCurrentContext found at: 0x" + std::format("{:X}", ServerOffsets::UObject_CanCreateInCurrentContext));
	return ServerOffsets::UObject_CanCreateInCurrentContext;
}

void Finder::SetupOffsets() {
	ServerOffsets::FFrame__CurrentNativeFunction = Version::Fortnite_Version >= 20.20 ? 0x90 : 0x88;
	ServerOffsets::FFrame__PropertyChainForCompiledIn = Version::Fortnite_Version >= 20.20 ? 0x88 : 0x80;
	ServerOffsets::FFrame__Next = Version::Fortnite_Version >= 24.30 ? 0x18 : (Version::Fortnite_Version >= 12.10 ? 0x20 : 0x28);

	Log("FFrame__CurrentNativeFunction offset: 0x" + std::format("{:X}", ServerOffsets::FFrame__CurrentNativeFunction));
	Log("FFrame__PropertyChainForCompiledIn offset: 0x" + std::format("{:X}", ServerOffsets::FFrame__PropertyChainForCompiledIn));
	Log("FFrame__Next offset: 0x" + std::format("{:X}", ServerOffsets::FFrame__Next));

	if (Version::Engine_Version < 4.22)
		ServerOffsets::ExecFunction = 0xB0;
	else if (Version::Engine_Version >= 4.22 && Version::Engine_Version < 4.25)
		ServerOffsets::ExecFunction = 0xC0;
	else if (Version::Fortnite_Version >= 12.00 && Version::Fortnite_Version < 12.10)
		ServerOffsets::ExecFunction = 0xC8;
	else if (Version::Fortnite_Version >= 12.10 && Version::Fortnite_Version <= 12.61)
		ServerOffsets::ExecFunction = 0xF0;
	else
		ServerOffsets::ExecFunction = 0xD8;

	ServerOffsets::UStruct__SuperStruct = Version::Engine_Version >= 4.22 ? 0x40 : 0x30;
	ServerOffsets::UStruct__Children = Version::Engine_Version >= 4.22 ? 0x48 : 0x38;
	ServerOffsets::UStruct__PropertiesSize = Version::Fortnite_Version >= 12.10 ? 0x58 : (Version::Engine_Version >= 4.22 ? 0x50 : 0x40);
	ServerOffsets::UStruct__MinAlignment = ServerOffsets::UStruct__PropertiesSize + 0x4;

	Log("UStruct__SuperStruct offset: 0x" + std::format("{:X}", ServerOffsets::UStruct__SuperStruct));
	Log("UStruct__Children offset: 0x" + std::format("{:X}", ServerOffsets::UStruct__Children));
	Log("UStruct__PropertiesSize offset: 0x" + std::format("{:X}", ServerOffsets::UStruct__PropertiesSize));
	Log("UStruct__MinAlignment offset: 0x" + std::format("{:X}", ServerOffsets::UStruct__MinAlignment));

	ServerOffsets::UProperty__Offset_Internal = Version::Fortnite_Version >= 12.10 && Version::Fortnite_Version < 20 ? 0x4c : (Version::Fortnite_Version >= 24.30 ? 0x3c : 0x44);
	ServerOffsets::UProperty__ElementSize = ServerOffsets::UProperty__Offset_Internal - 0x10;
	ServerOffsets::UProperty__PropertyFlags = ServerOffsets::UProperty__Offset_Internal - 0xC;

	Log("UProperty__Offset_Internal offset: 0x" + std::format("{:X}", ServerOffsets::UProperty__Offset_Internal));
	Log("UProperty__ElementSize offset: 0x" + std::format("{:X}", ServerOffsets::UProperty__ElementSize));
	Log("UProperty__PropertyFlags offset: 0x" + std::format("{:X}", ServerOffsets::UProperty__PropertyFlags));

	if (Version::Engine_Version >= 4.25) {
		ServerOffsets::UField__Next = Version::Fortnite_Version >= 24.30 ? 0x18 : 0x20;
		ServerOffsets::UField__Name = Version::Fortnite_Version >= 24.30 ? 0x20 : 0x28;
	}
	else {
		ServerOffsets::UField__Next = 0x28;
		ServerOffsets::UField__Name = 0x18;
	}

	ServerOffsets::UBoolProperty__FieldMask = Version::Fortnite_Version >= 24.30 ? 0x6b : (Version::Fortnite_Version >= 12.10 && Version::Fortnite_Version < 20 ? 0x7b : 0x73);

	Log("UField__Next offset: 0x" + std::format("{:X}", ServerOffsets::UField__Next));
	Log("UField__Name offset: 0x" + std::format("{:X}", ServerOffsets::UField__Name));
	Log("UBoolProperty__FieldMask offset: 0x" + std::format("{:X}", ServerOffsets::UBoolProperty__FieldMask));

	if (ConfigurationManager::GetConfig().bIsClient) {
		return; // return early if this is the client
	}

	FindProcessEventVFT();

	FindUStruct_FindPropertyByName();

	FindUWorld_ServerTravel();
	FindUWorld__TimeSeconds();
	FindUWorld__NextSwitchCountdown();
	FindUWorld_InternalGetNetMode();
	FindUWorld_FindCollectionByType();

	FindUEngine_GetWorldContextFromWorldChecked();
	FindUEngine_CreateNetDriver_Local();

	FindAActor_InternalGetNetMode();
	FindAActor_FinishSpawning();

	FindFWorldContext__ThisCurrentWorld();

	FindUObjectBase_IsValidLowLevel();
	FindUObjectBase_IsValidLowLevelFast();

	FindUObject_NeedsLoadForClientVFT();
	FindUObject_GetWorldVFT();

	FindUNetDriver__ReplicationFrame();
	FindUNetDriver__NetworkObjects();
	FindUNetDriver__NetTag();
	FindUNetDriver__DebugRelevantActors();
	FindUNetDriver__DestroyedStartupOrDormantActors();
	FindUNetDriver__LastPrioritizedActors();
	FindUNetDriver__LastRelevantActors();
	FindUNetDriver__LastSentActors();
	FindUNetDriver__LastNonRelevantActors();
	FindUNetDriver__GuidCache();
	FindUNetDriver__SendCycles();
	FindUNetDriver__RecvCycles();
	FindUNetDriver__LastTickDispatchRealtime();
	FindUNetDriver__bIsStandbyCheckingEnabled();
	FindUNetDriver__bHasStandbyCheatTriggered();
	FindUNetDriver__JoinInProgressStandbyWaitTime();
	FindUNetDriver__StandbyRxCheatTime();
	FindUNetDriver__StandbyTxCheatTime();
	FindUNetDriver__BadPingThreshold();
	FindUNetDriver__PercentMissingForRxStandby();
	FindUNetDriver__PercentMissingForTxStandby();
	FindUNetDriver__PercentForBadPing();
	FindUNetDriver_TickFlush();
	FindUNetDriver_TickDispatch();
	FindUNetDriver_SetWorld();
	FindUNetDriver_SetWorldVFT();
	FindUNetDriver_InitListenVFT();
	FindUNetDriver_IsServerVFT();
	FindUNetDriver_GetNetMode();

	FindUNetConnection__ActorChannels();
	FindUNetConnection__TimeSensitive();
	FindUNetConnection__TickCount();
	FindUNetConnection__State();
	FindUNetConnection__DestroyedStartupOrDormantActors();
	FindUNetConnection__ClientWorldPackageName();
	FindUNetConnection__ClientVisibleLevelNames();
	FindUNetConnection__LastRecvAckTime();
	FindUNetConnection_GetUChildConnectionVFT();

	FindUActorChannel__LastUpdateTime();
	FindUActorChannel__RelevantTime();
	FindUActorChannel_SetChannelActorForDestroy();

	FindUChannel__NumInRec();
	FindUChannel__NumOutRec();

	FindAActor__CreationTime();

	FindGHandle();

	FindAGameSession__NextPlayerID();

	FindAFortGameMode_FinishWorldInitializationVFT();

	FindAFortGameModeAthena_AddToAlivePlayers();
	FindAFortGameModeAthena_StartAircraftPhase();

	FindAGameModeBase_GetGameSessionClass();
	FindAGameModeBase_GetGameSessionClassVFT();

	FindUClass_FindFunctionByName();
	FindUClass_GetSparseClassData();

	FindUEngine_LoadMap();

	FindInternalServerTryActivateAbilityVFT();

	FindAFortPlayerController_OnReadyToStartMatchVFT();
	FindAFortPlayerController_RemoveInventoryItemVFT();

	FindAFortPlayerControllerZone_OnReadyToStartMatch();
	FindAFortPlayerControllerZone_ClientOnPawnDied();

	FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT();

	FindAFortGameStateAthena_SetCurrentPlaylistId();

	FindUFortControllerComponent_Aircraft_EnterAircraft();

	FindUMcpProfileGroup_SendRequestNow();

	FindABuildingActor_OnDamageServer();
	FindABuildingActor_PostUpdateVFT();

	FindABuildingContainer_SpawnLootVFT();

	FindAFortPickup_GivePickupToVFT();
	FindAFortPickup_SetPickupItemsVFT();

	FindUFortWorldItem_SetOwningInventoryVFT();
	FindUFortWorldItem_SetLoadedAmmoVFT();
	FindUFortWorldItem_SetPhantomReserveAmmoVFT();
	FindUFortWorldItem_SetInInventoryOverflowVFT();
	FindUFortWorldItem_SetDurabilityVFT();

	FindUFortAnalytics_SetGameSessionID();
	FindUFortAnalytics_SetGameStateClassName();

	FindAFortGameSessionDedicated_FinalizeCreation();
	FindAFortGameSessionDedicated_FinalizeCreationPatch1();
	FindAFortGameSessionDedicated_FinalizeCreationVFT();
	FindAFortGameSessionDedicated_OnUpdateComplete();

	FindUKismetSystemLibrary_ExecuteConsoleCommand();

	FindStaticFindObject();
	FindStaticLoadObject();

	FindFRotator_Quaternion();

	FindUGameplayStatics_BeginDeferredActorSpawnFromClass();

	FindAFortGameModeZone_CreateAIDirectorVFT();
	FindAFortGameModeZone_CreateAIGoalManagerVFT();

	FindUObjectBaseUtility_GetInterfaceAddress();

	FindUWorld_GetWorldSettings();

	FindUEngine_GetMaxTickRateVFT();

	FindUNetDriver_IsAdaptiveNetUpdateFrequencyEnabled();

	FindAActor_IsPendingKillPending();
	FindAActor_CallPreReplication();

	FindFNetworkObjectList_Remove();

	FindFNetViewer_Constructor();

	FindAPlayerController_SendClientAdjustmentVFT();

	FindUNetDriver_IsLevelInitializedForActorVFT();

	FindAActor_IsNetRelevantForVFT();
	FindAActor_GetNetOwnerVFT();

	FindUNetConnection_IsNetReadyVFT();

	FindFMemory_Free();
	FindFMemory_Realloc();

	FindFNetGUIDCache_SupportsObject();

	FindUNetConnection_CreateChannel();

	FindUActorChannel_SetChannelActor();
	FindUActorChannel_ReplicateActor();

	FindUObject_GetArchetypeFromRequiredInfo();

	FindAFortPlayerController_SpawnQuickBarsVFT();
	FindAFortPlayerController_SetupQuickBarsVFT();

	FindAFortInventory_InitializeExistingItem();

	FindUFortWorldItemDefinition_ServerExecuteVFT();

	FindUAbilitySystemComponent_FindAbilitySpecFromHandle();
	FindUAbilitySystemComponent_InternalTryActivateAbility();

	FindUChannel_CloseVFT();

	FindAFortGameSession_ValidatePlayer();

	FindAActor_BeginPlay();
	FindAActor_BeginPlayVFT();

	FindAFortPlayerController_OnReadyToStartMatch();

	FindAFortGameStateAthena_SetCurrentPlaylistId();

	FindAFortGameModeZone_CreateAIDirectorVFT();

	FindAFortInventory_GetInventoryUsed();
	FindAFortInventory_GetInventoryCapacity();

	FindUObjectBaseUtility_GetPathName();

	FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid();
	FindUFortKismetLibrary_GetWeaponStatsRow();

	FindFFortBaseWeaponStats_GetDurability();

	FindUFortWeaponItemDefinition_GetMaxDurability();
	FindUFortWeaponItemDefinition_GetMaxDurabilityVFT();

	FindAGameMode_AddInactivePlayer();
	FindAGameMode_AddInactivePlayerVFT();

	FindUNetDriver_GetNetMode();
	FindUNetDriver_IsLevelInitializedForActorVFT();

	FindAFortPlayerState_ApplyCharacterCustomization();

	FindAFortPickup_SetPickupItemsVFT();
	FindAFortPickup_SetPickupTarget();

	FindAFortPlayerController_CanAffordToPlaceBuildableClass();
	FindAFortPlayerController_CanAffordToPlaceBuildableClassVFT();
	FindAFortPlayerController_PayBuildableClassPlacementCost();
	FindAFortPlayerController_PayBuildableClassPlacementCostVFT();

	FindUWorld_SetNavigationSystem();

	FindUNavigationSystem_CreateNavigationSystem();

	FindFURL_HasOption();

	FindCollectGarbageInternal();

	FindABuildingActor_ServerOnAttemptInteractVFT();

	FindStep();
	FindStepExplicitProperty();

	FindUWorld_ListenPatch();

	FindFDedicatedServerUrlContext_Constructor();

	FindABuildingSMActor_SetEditingPlayer();
	FindABuildingSMActor_ReplaceBuildingActor();

	FindUFortQuestManager_QueueStatEvent();
	FindUFortQuestManager_SendCustomStatEvent();
	FindUFortQuestManager_SendStatEvent();
	FindUFortQuestManager_ProcessPendingStatEvents();

	FindAFortPlayerController_PayBuildingRepairCost();

	FindUAbilitySystemComponent_GiveAbilityAndActivateOnce();

	FindAbilitySpecCDOConstructor();

	FindUObject_CanCreateInCurrentContext();

	return;
}