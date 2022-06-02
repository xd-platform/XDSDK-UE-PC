#pragma once
#include "TUType.h"

namespace TUMomentType {

	class TAPMOMENT_API Config {
	public:
		FString ClientID;
		TUType::RegionType RegionType;
		FString AppId_CN;  // PC 国内论坛ID
		FString AppId_IO;  // PC 国外论坛ID
	};

	enum Orientation {
		DEFAULT = -1,
		LANDSCAPE = 0,
		PORTRAIT = 1
	};
	


	class TAPMOMENT_API Page {
		static FString ShortCut;
		static FString ShortCutKey;
		static FString User;
		static FString UserKey;
	};
}
