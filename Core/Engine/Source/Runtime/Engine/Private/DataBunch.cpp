#include "pch.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataBunch.h"

FOutBunch::FOutBunch(UChannel* InChannel, bool bInClose)
{
	FOutBunch* (*FOutBunchInternal)(FOutBunch*, UChannel*, bool) = decltype(FOutBunchInternal)(ImageBase + Finder::FindFOutBunch_Constructor());
	FOutBunchInternal(this, InChannel, bInClose);
}