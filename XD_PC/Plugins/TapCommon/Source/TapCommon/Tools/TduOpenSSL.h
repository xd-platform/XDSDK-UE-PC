#pragma once

class TAPCOMMON_API TduOpenSSL {
public:
	
	static FString LexToString(const TArray<uint8>& Content);
	
	static TArray<uint8> Sha256(const TArray<uint8>& Content);

	// static TArray<uint8> HmacSha256(const TArray<uint8>& Content, const TArray<uint8>& Key);

};
