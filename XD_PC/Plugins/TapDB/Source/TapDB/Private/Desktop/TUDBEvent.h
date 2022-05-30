#pragma once
#include "TUDBEnum.h"

namespace TUDBEventKey
{
	static FString const NAME_KEY                 = "name";
	static FString const PROPERTIES_KEY           = "properties";
	static FString const CUSTOM_PROPERTIES_KEY    = "customProperties";

	static FString const MODULE_KEY               = "module";
	static FString const TYPE_KEY                 = "type";

	static FString const OPENID_KEY               = "open_id";
	static FString const CLIENTID_KEY             = "client_id";
	static FString const LOGIN_TYPE_KEY           = "login_type";
}

class TUDBEvent
{
public:
	virtual ~TUDBEvent() = default;
	
	TSharedPtr<FJsonObject> SysProperties;
	TSharedPtr<FJsonObject> CommonProperties;
	TSharedPtr<FJsonObject> CustomStaticProperties;
	TFunction<TSharedPtr<FJsonObject>()> CustomDynamicPropertiesCaculator;

	
	/**
	** 发送一个带属性的统计事件
	* name：事件名称
	* systemParams: 系统字段
	* properties：事件属性
	* customProperties: 自定义属性
	* eventType： 事件类型
	*/
	void TrackEvent(const FString& Name,
		TSharedPtr<FJsonObject> CustomProperties = nullptr,
		TSharedPtr<FJsonObject> Properties = nullptr,
		TUDBEnum::EventType EventType = TUDBEnum::Normal,
		TSharedPtr<FJsonObject> SystemParams = nullptr
		);

	/**
	* 事件操作 add update initial
	* properties：操作属性
	*/
	void InitialEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TUDBEnum::EventType EventType);
	void AddEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TUDBEnum::EventType EventType);
	void UpdateEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TUDBEnum::EventType EventType);

	void SetClientId(const FString& _ClientId);

	virtual FString GetEventCatogery() = 0;
	virtual FString GetIdentifyKey() = 0;
	virtual FString GetOpenId();
	virtual bool Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties);
	virtual void SaveIdentify(const FString& identify);
	virtual FString GetSavedIdentify();
	virtual bool HasSavedIdentify();
	virtual void ClearIdentify();

	FString GetIdentify();
	void AutoIdentifyWithProperties(const TSharedPtr<FJsonObject>& properties);
	
private:
	
	// FString Account;
	FString ClientId;
	FString Key;
	FString _Identify;

	void GenerateKey(const FString& KeyString);
	TSharedPtr<FJsonObject> GetSystemParams();

	TSharedPtr<FJsonObject>CombinedProperties(const TSharedPtr<FJsonObject>& Properties, const TSharedPtr<FJsonObject>& CustomProperties);

};

class TUDBEventUser : public TUDBEvent
{
public:
	TUDBEventUser(const FString& ClientID);
	virtual FString GetEventCatogery() override;
	virtual FString GetIdentifyKey() override;
	virtual FString GetOpenId() override;
	virtual bool Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties) override;
	virtual void SaveIdentify(const FString& identify) override;
	virtual FString GetSavedIdentify() override;
	virtual bool HasSavedIdentify() override;
	virtual void ClearIdentify() override;
};

class TUDBEventMobile : public TUDBEvent
{
public:
	TUDBEventMobile(const FString& ClientID);
	virtual FString GetEventCatogery() override;
	virtual FString GetIdentifyKey() override;
	virtual bool Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties) override;

};
