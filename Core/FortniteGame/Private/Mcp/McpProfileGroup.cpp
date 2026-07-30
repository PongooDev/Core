#include "pch.h"
#include "FortniteGame/Public/Mcp/McpProfileGroup.h"

void UMcpProfileGroup::SendRequestNow(UMcpProfileGroup* This, void* HttpRequest, EContextCredentials ContextCredentials)
{
	//Log("UMcpProfileGroup::SendRequestNow called");
	if (Version::Fortnite_Version <= 7.30) {
		*(EContextCredentials*)(__int64(HttpRequest) + (Version::Fortnite_Version >= 4.2 ? 0x28 : 0x60)) = EContextCredentials::CXC_Public;
	}
	else {
		ContextCredentials = EContextCredentials::CXC_Public;
	}

	return SendRequestNowOG(This, HttpRequest, ContextCredentials);
}
