#pragma once
#include "pch.h"

#include "Version.h"

class Patches {
public:
	static void ApplyPatches() {
		// i like to keep patches at a minimal and if i can i would like to figure out a way to not use these patches

		if (Version::Fortnite_Version >= 1.72 && Version::Fortnite_Version <= 1.9) {
			uintptr_t Patch1 = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 F6 48 8B F9 89 B1 ? ? ? ? 48 8B 89 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 89 B7").Get();
			if (Patch1) {
				MH_CreateHook((LPVOID)Patch1, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch1 - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch2 = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B ? 49 89 73 ? 49 89 7B ? 55 41 54 41 55 41 56 41 57 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 45 0F 29 A3").Get();
			if (Patch2) {
				MH_CreateHook((LPVOID)Patch2, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch2 - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch3 = Memcury::Scanner::FindPattern("40 53 55 56 48 81 EC ? ? ? ? 33 F6").Get();
			if (Patch3) {
				MH_CreateHook((LPVOID)Patch3, True, nullptr);
				Log("Patched: " + std::to_string(Patch3 - ImageBase) + " with True");
			}
		}
	}
};