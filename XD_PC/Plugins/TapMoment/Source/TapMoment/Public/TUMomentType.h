#pragma once
#include "TUType.h"

namespace TUMomentType {

	class TAPMOMENT_API Config {
	public:
		TUType::RegionType RegionType;
		FString AppId_CN;  // 国内论坛ID
		FString AppId_IO;  // 国外论坛ID
	};
}
