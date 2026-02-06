#pragma once

#include "CoreMinimal.h"

/**
 * Packet Types
 */
enum class EPacketType : uint8
{
	Packet_None = 0,
	Packet_LoginReq = 1,
	Packet_LoginRes = 2,
};

/**
 * Header structure for all packets.
 * 1 byte for Type.
 */
#pragma pack(push, 1)
struct FPacketHeader
{
	EPacketType PacketType;
	
	FPacketHeader() : PacketType(EPacketType::Packet_None) {}
	FPacketHeader(EPacketType InType) : PacketType(InType) {}
};

/**
 * Login Request Packet (Client -> Server)
 */
struct FPacketLoginRequest : public FPacketHeader
{
	char UserID[32]; // Fixed size buffer for ID (UTF-8)

	FPacketLoginRequest() : FPacketHeader(EPacketType::Packet_LoginReq)
	{
		FMemory::Memzero(UserID, sizeof(UserID));
	}
};

/**
 * Login Response Packet (Server -> Client)
 */
struct FPacketLoginResponse : public FPacketHeader
{
	bool bSuccess;

	FPacketLoginResponse() : FPacketHeader(EPacketType::Packet_LoginRes)
	{
		bSuccess = false;
	}
};
#pragma pack(pop)
