#pragma once

class TapLoginImpl
{
public:
	static void Init(const FString& clientID, bool isCn, bool roundCorner);

	static TapLoginImpl& Get();

private:
	static TapLoginImpl* Singleton;
};
