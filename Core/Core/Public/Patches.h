#pragma once
#include "pch.h"

#include "Version.h"

class Patches {
public:
	static void ApplyPatches() {
		// i like to keep patches at a minimal and if i can i would like to figure out a way to not use these patches

		if (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 3.5)
		{
			uintptr_t WidgetCrashPatch = Memcury::Scanner::FindStringRef(L"Unable to create the widget {0}, no outer provided.").FindFunctionStart().Get();
			
			if (WidgetCrashPatch) {
				MH_CreateHook(LPVOID(WidgetCrashPatch), RetNullptr, 0);
				Log("Patched: " + std::to_string(WidgetCrashPatch - ImageBase) + " with RetNullptr");
			}
		}

		/*if (Version::Fortnite_Version == 3.3) { // replgraph very unfinished lol
			uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UFortReplicationGraph is disabled").Get();
			if (StringAddr) {
				for (int i = 0; i < 256; i++) {
					auto Ptr = (uint8_t*)(StringAddr - i);
					if (*Ptr == 0x75) {
						PatchByte((uintptr_t)Ptr, 0xEB);
						Log("Patched: " + std::to_string((uintptr_t)Ptr - ImageBase) + " with 0xEB");
						break;
					}
				}
			}
		}*/

		if (Version::Fortnite_Version >= 3.5 && Version::Fortnite_Version <= 3.6) {
			uintptr_t Patch8 = Memcury::Scanner::FindPattern("48 8B C4 57 48 81 EC ? ? ? ? 4C 8B 82").Get();
			if (Patch8) {
				MH_CreateHook((LPVOID)Patch8, True, nullptr);
				Log("Patched: " + std::to_string(Patch8 - ImageBase) + " with True"); // bool AFortPoiVolume::OverlapsPawn
			}
		}
	}
};