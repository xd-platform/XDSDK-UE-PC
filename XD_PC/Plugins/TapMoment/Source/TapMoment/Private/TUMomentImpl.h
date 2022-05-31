#pragma once
#include "TUMomentType.h"

class TUMomentImpl {
public:

	TUMomentType::Config Config;

	virtual ~TUMomentImpl() = default;

	static TSharedPtr<TUMomentImpl>& Get();
	
	virtual void Init(const TUMomentType::Config& InitConfig);

private:
	static TSharedPtr<TUMomentImpl> Instance;
};
