#pragma once
#include "TauDB.h"
#include "TUDBEvent.h"

class TauDBImplement
{
public:
	static TauDBImplement& Get();

	void Init(const TauDBInitConfig& Config, bool isAccount);

	void SetUser(const FString& UserId, TSharedPtr<FJsonObject> Properties = nullptr, const FString& LoginType = FString());
	void ClearUser();

	TSharedPtr<FJsonObject> GetStartInfo();

	FString GetDeviceId();

	void SetLevel(int Level);

	void SetServer(const FString& Server);

	void SetName(const FString& Name);

	void ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties);

	void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);

	void DeviceInitialize(TSharedPtr<FJsonObject> Properties);

	void DeviceUpdate(TSharedPtr<FJsonObject> Properties);

	void DeviceAdd(TSharedPtr<FJsonObject> Properties);

	void UserInitialize(TSharedPtr<FJsonObject> Properties);

	void UserUpdate(TSharedPtr<FJsonObject> Properties);
	
	void UserAdd(TSharedPtr<FJsonObject> Properties);

	void RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties);

	void ClearStaticProperties();

	void UnregisterStaticProperty(const FString& PropertyName);
	
	void RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator);

private:
	static FString TAPDB_VERSION;

	static TauDBImplement *Singleton;
	static FString First_login_type;
	static FString First_open_id;
	
	TSharedPtr<TUDBEventMobile> TapDBEventMobile;
	TSharedPtr<TUDBEventUser> TapDBEventUser;

	static TSharedPtr<FJsonObject> CustomOperationProperties(const TSharedPtr<FJsonObject>& Properties);

	bool CheckIdentify();
	void SendPlayTime(int Interval);


	static bool CheckStringParam(const FString& Para, const FString& ParaName);

	TSharedPtr<FDateTime> StartTime;
	TSharedPtr<FDateTime> PauseTime;

	void RegisterCoreDelegate();

	void StartOperation(const TSharedPtr<FJsonObject>& Properties);


};
