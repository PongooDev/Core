#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/PackageMapClient.h"

bool FNetGUIDCache::SupportsObject(const UObject* Object) const
{
	bool (*SupportsObjectInternal)(const FNetGUIDCache*, const UObject*) = decltype(SupportsObjectInternal)(ImageBase + Finder::FindFNetGUIDCache_SupportsObject());
	return SupportsObjectInternal(this, Object);
}