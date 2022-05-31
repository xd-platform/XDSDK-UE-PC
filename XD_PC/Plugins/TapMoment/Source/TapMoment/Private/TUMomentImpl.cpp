#include "TUMomentImpl.h"

TSharedPtr<TUMomentImpl> TUMomentImpl::Instance = nullptr;

TSharedPtr<TUMomentImpl>& TUMomentImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new TUMomentImpl);
	}
	return Instance;
}

void TUMomentImpl::Init(const TUMomentType::Config& InitConfig) {
	Config = InitConfig;
}
