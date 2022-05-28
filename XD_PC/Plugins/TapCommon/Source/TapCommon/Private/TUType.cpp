#include "TUType.h"

namespace TUType {

	TSharedPtr<Config> Config::Instance = nullptr;
	
	TSharedPtr<Config>& Config::Get() {
		if (!Instance.IsValid()) {
			Instance = MakeShareable(new Config);
		}
		return Instance;
	}
}


