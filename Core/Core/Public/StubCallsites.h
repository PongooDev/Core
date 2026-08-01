#pragma once
#include "pch.h"

#include <functional>
#include <vector>

namespace StubCallsites {
	inline uintptr_t FromString(const wchar_t* String)
	{
		auto Ref = Memcury::Scanner::FindStringRef(String, false);
		return Ref.IsValid() ? Ref.FindFunctionStart().Get() : 0;
	}

	inline uintptr_t FromReflection(const char* FunctionPath)
	{
		auto Fn = (UFunction*)FUObjectArray::FindObject(FunctionPath);
		if (!Fn || !Fn->Func)
			return 0;

		uintptr_t Func = (uintptr_t)Fn->Func;

		if (Memcury::Scanner(Func).FindFunctionEnd().Get() > Func)
			return Func;

		for (int i = 0; i < 0x40; i++)
		{
			uintptr_t Cursor = Func + i;

			if (*(uint8*)Cursor == 0xE9)
				return Memcury::PE::Address(Cursor).RelativeOffset(1).Get();
		}

		return Func;
	}

	inline uintptr_t FromVTable(UClass* Class, uintptr_t Index)
	{
		if (!Class || !Index)
			return 0;

		return (uintptr_t)Class->GetDefaultObject()->VTable[Index];
	}

	inline uintptr_t FromOffset(uintptr_t Offset)
	{
		return Offset ? Offset + ImageBase : 0;
	}

	inline uintptr_t FromSignature(const char* Signature)
	{
		return Memcury::Scanner::FindPattern(Signature, false).Get();
	}

	inline uintptr_t FromXref(const char* CalleeSignature)
	{
		uintptr_t Callee = Memcury::Scanner::FindPattern(CalleeSignature, false).Get();
		if (!Callee)
			return 0;

		auto Refs = FindBranchRefsToAddress(Callee);
		if (Refs.size() != 1)
			return 0;

		return Memcury::Scanner(Refs[0]).FindFunctionStart().Get();
	}

	inline uintptr_t ResolveStub(uintptr_t Function)
	{
		if (!Function)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Target = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();
			if (IsReturnNullStub(Target))
				return Target;
		}

		return 0;
	}

	inline uintptr_t FindCall(uintptr_t Function, uintptr_t Stub, int Depth = 2)
	{
		if (!Function || !Stub || Depth < 0)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();
		if (End <= Function)
			return 0;

		if (uintptr_t Addr = FindCallRefInRange(Function, End, Stub))
			return Addr;

		if (Depth == 0)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Callee = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();

			if (uintptr_t Addr = FindCall(Callee, Stub, Depth - 1))
				return Addr;
		}

		return 0;
	}

	inline uintptr_t ResolveEmptyStub(uintptr_t Function)
	{
		if (!Function)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();

		for (uintptr_t Cursor = Function; Cursor + 7 <= End; Cursor++)
		{
			auto* b = reinterpret_cast<const uint8_t*>(Cursor);

			if ((b[0] != 0x48 && b[0] != 0x4C) || b[1] != 0x8D || (b[2] & 0xC7) != 0x05)
				continue;

			uintptr_t Target = Cursor + 7 + *reinterpret_cast<const int32_t*>(Cursor + 3);
			if (IsEmptyStub(Target))
				return Target;
		}

		return 0;
	}

	inline bool IsBoundIntoObject(uintptr_t Addr)
	{
		auto* b = reinterpret_cast<const uint8_t*>(Addr);

		int LeaReg = ((b[2] >> 3) & 7) + (b[0] == 0x4C ? 8 : 0);

		for (uintptr_t Cursor = Addr + 7; Cursor < Addr + 7 + 0x20; Cursor++)
		{
			auto* m = reinterpret_cast<const uint8_t*>(Cursor);

			if ((m[0] != 0x48 && m[0] != 0x4C) || m[1] != 0x89)
				continue;

			int Mod = (m[2] >> 6) & 3;
			int Reg = ((m[2] >> 3) & 7) + (m[0] == 0x4C ? 8 : 0);
			int Rm = m[2] & 7;

			if (Reg != LeaReg)
				continue;

			if ((Mod == 1 || Mod == 2) && Rm != 4)
				return true;
		}

		return false;
	}

	inline uintptr_t FindLea(uintptr_t Function, uintptr_t Stub, int Depth = 2)
	{
		if (!Function || !Stub || Depth < 0)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();
		if (End <= Function)
			return 0;

		uintptr_t Fallback = 0;

		for (uintptr_t Cursor = Function; Cursor < End; )
		{
			uintptr_t Addr = FindLeaRefInRange(Cursor, End, Stub);
			if (!Addr)
				break;

			if (IsBoundIntoObject(Addr))
				return Addr;

			if (!Fallback)
				Fallback = Addr;

			Cursor = Addr + 1;
		}

		if (Fallback)
			return Fallback;

		if (Depth == 0)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Callee = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();

			if (uintptr_t Addr = FindLea(Callee, Stub, Depth - 1))
				return Addr;
		}

		return 0;
	}

	using FLocator = std::function<uintptr_t()>;

	inline FLocator ByString(const wchar_t* String) { return [=] { return FromString(String); }; }
	inline FLocator ByReflection(const char* FunctionPath) { return [=] { return FromReflection(FunctionPath); }; }
	inline FLocator ByVTable(UClass* Class, uintptr_t Index) { return [=] { return FromVTable(Class, Index); }; }
	inline FLocator ByOffset(uintptr_t Offset) { return [=] { return FromOffset(Offset); }; }
	inline FLocator BySignature(const char* Signature) { return [=] { return FromSignature(Signature); }; }
	inline FLocator ByXref(const char* CalleeSignature) { return [=] { return FromXref(CalleeSignature); }; }

	struct FSite {
		const char* Name;
		std::vector<FLocator> Locators;

		// PatchStub only: which stripped branch inside the located function to take, in address
		// order. Leave at 0 unless the function calls more than one stripped helper.
		int Skip = 0;
	};

	inline void Patch(const char* Label, uintptr_t Stub, void* Detour, std::initializer_list<FSite> Sites, bool bWarnIfNotFound = true)
	{
		for (const auto& Site : Sites)
		{
			uintptr_t Addr = 0;

			for (const auto& Locator : Site.Locators)
			{
				uintptr_t Function = Locator();
				if (!Function)
					continue;

				Addr = FindCall(Function, Stub);
				if (Addr)
					break;
			}

			if (!Addr) {
				if (bWarnIfNotFound) {
					Log(std::string("Failed to find patch for ") + Label + ": " + Site.Name);
				}
				continue;
			}

			Log(std::string(Label) + " Patch: " + Site.Name + " @ 0x" + std::format("{:X}", Addr - ImageBase));
			PatchCallFar(Addr, Detour);
		}
	}

	inline void PatchBound(const char* Label, uintptr_t Stub, void* Detour, std::initializer_list<FSite> Sites, bool bWarnIfNotFound = true)
	{
		for (const auto& Site : Sites)
		{
			uintptr_t Addr = 0;

			for (const auto& Locator : Site.Locators)
			{
				uintptr_t Function = Locator();
				if (!Function)
					continue;

				Addr = FindLea(Function, Stub);
				if (Addr)
					break;
			}

			if (!Addr) {
				if (bWarnIfNotFound) {
					Log(std::string("Failed to find patch for ") + Label + ": " + Site.Name);
				}
				continue;
			}

			Log(std::string(Label) + " Patch: " + Site.Name + " @ 0x" + std::format("{:X}", Addr - ImageBase));
			PatchLeaFar(Addr, Detour);
		}
	}

	// ---------------------------------------------------------------------------------
	// Stripped helpers.
	//
	// Some helpers are compiled out of the shipping build - the callsite is intact but the
	// callee is a bare `retn` or a `return 0`. These cannot go through Patch/FindCall: the
	// branch to them is often a tail call, and it can be conditional
	// (`jnz UserMathErrorFunction`), so there is no E8 to match.
	//
	// Everything below matches on the callee *being a stub* rather than on a known address,
	// which is a far tighter filter than an address compare, and it never widens the scan
	// used by Patch.
	// ---------------------------------------------------------------------------------

	inline bool IsInImage(uintptr_t Addr)
	{
		static uintptr_t End = [] {
			auto* Dos = (IMAGE_DOS_HEADER*)ImageBase;
			auto* Nt = (IMAGE_NT_HEADERS64*)(ImageBase + Dos->e_lfanew);
			return ImageBase + Nt->OptionalHeader.SizeOfImage;
			}();

		return Addr >= ImageBase && Addr < End;
	}

	// call/jmp/jcc rel32 - fills Target and returns the instruction length, or 0. Targets
	// outside the module are rejected: scanning byte by byte turns plenty of operand bytes
	// into plausible looking branches.
	inline int BranchAt(uintptr_t Cursor, uintptr_t& Target)
	{
		auto* b = reinterpret_cast<const uint8*>(Cursor);
		int Length = 0;

		if (b[0] == 0xE8 || b[0] == 0xE9)
		{
			Target = Cursor + 5 + *reinterpret_cast<const int32_t*>(Cursor + 1);
			Length = 5;
		}
		else if (b[0] == 0x0F && (b[1] & 0xF0) == 0x80)
		{
			Target = Cursor + 6 + *reinterpret_cast<const int32_t*>(Cursor + 2);
			Length = 6;
		}

		return (Length && IsInImage(Target)) ? Length : 0;
	}

	inline bool IsStrippedStub(uintptr_t Addr)
	{
		if (!Addr || !IsInImage(Addr))
			return false;

		auto* b = reinterpret_cast<const uint8*>(Addr);

		if (b[0] == 0xC3 || b[0] == 0xC2)
			return true;

		if ((b[0] == 0x33 || b[0] == 0x31) && b[1] == 0xC0 && (b[2] == 0xC3 || b[2] == 0xC2))
			return true;

		if (b[0] == 0x48 && (b[1] == 0x33 || b[1] == 0x31) && b[2] == 0xC0 && (b[3] == 0xC3 || b[3] == 0xC2))
			return true;

		return false;
	}

	// UFunction::Func is the exec thunk; the implementation is what it hands off to last
	inline uintptr_t FromReflectionImpl(const char* FunctionPath)
	{
		uintptr_t Thunk = FromReflection(FunctionPath);
		if (!Thunk)
			return 0;

		uintptr_t End = Memcury::Scanner(Thunk).FindFunctionEnd().Get();
		if (End <= Thunk)
			return Thunk;

		uintptr_t Impl = 0;
		for (uintptr_t Cursor = Thunk; Cursor + 6 <= End; Cursor++)
		{
			uintptr_t Target = 0;
			if (BranchAt(Cursor, Target) && Target != Thunk)
				Impl = Target;
		}

		return Impl ? Impl : Thunk;
	}

	inline FLocator ByReflectionImpl(const char* FunctionPath) { return [=] { return FromReflectionImpl(FunctionPath); }; }

	// where Function branches to a helper that got compiled out. Re-entry is prevented with a
	// visited list rather than by skipping targets that look like they sit inside Function -
	// FindFunctionEnd often overshoots into the next function, and that would silently drop
	// the very call we need to follow.
	// FindFunctionEnd cannot size a small leaf function - no prologue to unwind, no epilogue
	// to recognise - and the helpers that call a stripped stub are exactly that shape:
	//   test rcx, rcx / jz / test rdx, rdx / jnz <stub> / xor eax, eax / retn
	// When it fails, walk to the first ret instead of giving up.
	inline uintptr_t FunctionEnd(uintptr_t Function)
	{
		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();
		if (End > Function)
			return End;

		for (uintptr_t Cursor = Function; Cursor < Function + 0x80; Cursor++)
		{
			if (*(uint8*)Cursor == 0xC3)
				return Cursor + 1;
		}

		return 0;
	}

	// Skip counts matches before returning one, so a function that calls two different stripped
	// helpers can be patched twice - they share a single stub body, so the ordinal is the only
	// thing telling them apart.
	inline uintptr_t FindStubBranchInternal(uintptr_t Function, int Depth, int& Skip, std::vector<uintptr_t>& Visited)
	{
		if (!Function || Depth < 0 || !IsInImage(Function))
			return 0;

		for (uintptr_t Seen : Visited)
		{
			if (Seen == Function)
				return 0;
		}
		Visited.push_back(Function);

		uintptr_t End = FunctionEnd(Function);
		if (End <= Function)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 6 <= End; Cursor++)
		{
			uintptr_t Target = 0;
			if (BranchAt(Cursor, Target) && Target != Function && IsStrippedStub(Target) && Skip-- == 0)
				return Cursor;
		}

		if (Depth == 0)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 6 <= End; Cursor++)
		{
			uintptr_t Callee = 0;
			if (!BranchAt(Cursor, Callee) || Callee == Function)
				continue;

			if (uintptr_t Addr = FindStubBranchInternal(Callee, Depth - 1, Skip, Visited))
				return Addr;
		}

		return 0;
	}

	inline uintptr_t FindStubBranch(uintptr_t Function, int Skip = 0, int Depth = 3)
	{
		std::vector<uintptr_t> Visited;
		return FindStubBranchInternal(Function, Depth, Skip, Visited);
	}

	// rewrites the branch's rel32 to reach Detour, keeping the opcode so a tail jump stays a
	// tail jump and a conditional stays conditional
	inline bool PatchBranchFar(uintptr_t Addr, void* Detour)
	{
		uintptr_t Target = 0;
		int Length = BranchAt(Addr, Target);
		if (!Length)
			return false;

		uintptr_t Base = Addr & ~0xFFFFull;
		void* Trampoline = nullptr;

		for (uintptr_t Cursor = Base; Cursor > Base - 0x80000000ull; Cursor -= 0x10000)
		{
			Trampoline = VirtualAlloc((void*)Cursor, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (Trampoline)
				break;
		}

		if (!Trampoline)
			return false;

		uint8 Jmp[] = {
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};
		memcpy(&Jmp[6], &Detour, sizeof(Detour));
		memcpy(Trampoline, Jmp, sizeof(Jmp));

		int32_t Rel = (int32_t)((uintptr_t)Trampoline - (Addr + Length));
		return PatchBytes((void*)(Addr + Length - 4), &Rel, sizeof(Rel));
	}

	inline void PatchStub(const char* Label, void* Detour, std::initializer_list<FSite> Sites, bool bWarnIfNotFound = true)
	{
		for (const auto& Site : Sites)
		{
			uintptr_t Addr = 0;

			for (const auto& Locator : Site.Locators)
			{
				uintptr_t Function = Locator();
				if (!Function)
					continue;

				Addr = FindStubBranch(Function);
				if (Addr)
					break;
			}

			if (!Addr) {
				if (bWarnIfNotFound) {
					Log(std::string("Failed to find stub for ") + Label + ": " + Site.Name);
				}
				continue;
			}

			uintptr_t Stub = 0;
			BranchAt(Addr, Stub);

			Log(std::string(Label) + " Stub Patch: " + Site.Name + " @ 0x" + std::format("{:X}", Addr - ImageBase));

			PatchBranchFar(Addr, Detour);
		}
	}
}
