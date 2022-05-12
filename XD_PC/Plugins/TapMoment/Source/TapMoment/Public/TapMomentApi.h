#pragma once

struct TAPMOMENT_API TapMomentConfig {
	bool IsCN;
	FString AppId;
};

class TAPMOMENT_API TapMomentApi {
public:

	static void Init(TapMomentConfig Config);

	static void OpenWebTopic();
	
};
