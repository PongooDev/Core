#pragma once
#include "pch.h"

#include "Configuration.h"

#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectMacros.h"

class UObject;

class Utils {
public:
	static void InitConsole(FCoreConfig& Config);

	static uintptr_t ResolveRipRelative(uintptr_t instr, int dispOffset, int instrLen);

	static void DumpGObjects(bool bLog = false);
	static void DumpClassProperties(const char* ClassName);
	static void DumpEnumProperties(const char* EnumName);
	static uint8 GetEnumValueFromName(const char* EnumName, const char* EnumMemberName);

    static void SetClientObservedStat(AFortPlayerPawn* Pawn, FName StatName, int32 StatValue);

	static void Hook();

	static FString GetDefaultMapName(FCoreConfig& Config);

	static FString BuildTravelURL(FString MapPath, std::vector<std::pair<std::string, std::string>> Options = {});

	static void SetLogVerbosity();

	static void RemoveLocalPlayer();

	static bool SetupDedicatedServer(FCoreConfig& Config);

	static bool LoadWorld(FCoreConfig& Config);

    static uintptr_t GetCallDestination(uintptr_t callAddr);

    static double NowSeconds();

	static UObject* GetObjectFromString(const std::string& InString, EClassCastFlags Flags = EClassCastFlags::RF_NoFlags);
	static UObject* GetObjectFromString(const std::string& InString, UClass* Class = nullptr);

    static wchar_t* StringToWChar(const std::string& input);

	static std::string StringToLower(const std::string& str);
};

class FCommandParser
{
private:
    std::vector<std::string> Args;
    std::string FullCommand;

public:
    FCommandParser(const std::string& Command)
        : FullCommand(Command)
    {
        ParseArguments(Command);
    }

    std::string GetCommand() const
    {
        return Args.empty() ? "" : Args[0];
    }

    int32 GetArgCount() const
    {
        return Args.size() > 0 ? (int32)(Args.size() - 1) : 0;
    }

    std::string GetArg(int32 Index) const
    {
        int32 ActualIndex = Index + 1;
        if (ActualIndex >= 0 && ActualIndex < (int32)Args.size())
            return Args[ActualIndex];
        return "";
    }

    int32 GetArgInt(int32 Index, int32 DefaultValue = 0) const
    {
        std::string arg = GetArg(Index);
        if (arg.empty())
            return DefaultValue;

        try {
            return std::stoi(arg);
        }
        catch (...) {
            return DefaultValue;
        }
    }

    float GetArgFloat(int32 Index, float DefaultValue = 0.0f) const
    {
        std::string arg = GetArg(Index);
        if (arg.empty())
            return DefaultValue;

        try {
            return std::stof(arg);
        }
        catch (...) {
            return DefaultValue;
        }
    }

    bool GetArgBool(int32 Index, bool DefaultValue = false) const
    {
        std::string arg = GetArg(Index);
        if (arg.empty())
            return DefaultValue;

        std::string lower = arg;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "true" || lower == "1" || lower == "yes" || lower == "on")
            return true;
        if (lower == "false" || lower == "0" || lower == "no" || lower == "off")
            return false;

        return DefaultValue;
    }

    std::string GetRemainingArgs(int32 StartIndex = 0) const
    {
        int32 ActualIndex = StartIndex + 1;
        if (ActualIndex >= (int32)Args.size())
            return "";

        std::string result;
        for (int32 i = ActualIndex; i < (int32)Args.size(); ++i)
        {
            if (!result.empty())
                result += " ";
            result += Args[i];
        }
        return result;
    }

    bool IsCommand(const std::string& CommandName) const
    {
        std::string cmd = GetCommand();
        std::string target = CommandName;

        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);

        return cmd == target;
    }

    const std::string& GetFullCommand() const
    {
        return FullCommand;
    }

private:
    void ParseArguments(const std::string& Command)
    {
        std::istringstream stream(Command);
        std::string token;
        bool inQuotes = false;
        std::string currentArg;

        for (char c : Command)
        {
            if (c == '"')
            {
                inQuotes = !inQuotes;
            }
            else if (c == ' ' && !inQuotes)
            {
                if (!currentArg.empty())
                {
                    Args.push_back(currentArg);
                    currentArg.clear();
                }
            }
            else
            {
                currentArg += c;
            }
        }

        if (!currentArg.empty())
        {
            Args.push_back(currentArg);
        }
    }
};