#include "TduOpenSSL.h"
#include <openssl/sha.h>
// #include <openssl/hmac.h>


FString TduOpenSSL::LexToString(const TArray<uint8>& Content) {
	FString Result;
	for (TArray<uint8>::TConstIterator it(Content); it; ++it) {
		Result += FString::Printf(TEXT("%02x"), *it);
	}
	return Result;
}

TArray<uint8> TduOpenSSL::Sha256(const TArray<uint8>& Content) {
	TArray<uint8> Sign;
	Sign.SetNum(32); // sha256 固定32位
	SHA256(Content.GetData(), Content.Num(), Sign.GetData());
	return Sign;
}

// TArray<uint8> TduOpenSSL::HmacSha256(const TArray<uint8>& Content, const TArray<uint8>& Key) {
// 	TArray<uint8> Sign;
// 	Sign.SetNum(32); // sha256 固定32位
// 	unsigned int OutputLen = 0;
// 	HMAC(EVP_sha256(), Key.GetData(), Key.Num(), Content.GetData(), Content.Num(), Sign.GetData(), &OutputLen);
// 	return Sign;
// }

