#pragma once
#include "TapUELogin.h"
#include "TULoginFriendResult.h"


class TULoginImpl {
public:
	virtual ~TULoginImpl() = default;

	static TSharedPtr<TULoginImpl>& Get();

	TapUELogin::Config Config;
	
	virtual void Init(TapUELogin::Config _Config);
	
	virtual TSharedPtr<FTULoginProfileModel> GetProfile();
	
	virtual void FetchProfile(TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack);

	virtual TSharedPtr<FTUAccessToken> GetAccessToken();

	virtual void Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack);

	virtual void Logout();
	
	virtual void ChangeLanguage(TapUELogin::LanguageType LanguageType);
	
	virtual void GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack);
	
	virtual void QueryMutualList(FString Cursor, int Size, TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack);

private:
	static TSharedPtr<TULoginImpl> Instance;
};