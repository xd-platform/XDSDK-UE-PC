#pragma once
#include "TUBootStrapImpl.h"


class TUBootStrapMobileImpl: public TUBootStrapImpl{
public:
	TUBootStrapMobileImpl();
	virtual void Init(const TUType::Config& Config) override;
	virtual void Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void Logout() override;
	virtual TSharedPtr<FTapUser> GetUser() override;
	virtual void SetPreferLanguage(TUType::LanguageType LangType) override;

private:
	void DealLoginCallBack(const FString& ResultStr, TFunction<void(const FTapUser& User)> SuccessBlock,
	                              TFunction<void(const FTUError& Error)> FailBlock);

#if PLATFORM_IOS
	void DealLoginCallBack(TDSUser* user, NSError* error, TFunction<void(const FTapUser& User)> SuccessBlock,
								  TFunction<void(const FTUError& Error)> FailBlock);
#endif
	

	
};


