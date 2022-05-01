#pragma once
#include "TAUDBEnum.h"

class TAUDBEvent
{
public:
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
		TAUDBEnum::EventType EventType = TAUDBEnum::Normal,
		TSharedPtr<FJsonObject> SystemParams = nullptr
		);

	/**
	* 事件操作 add update initial
	* properties：操作属性
	*/
	void InitialEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TAUDBEnum::EventType EventType);
	void AddEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TAUDBEnum::EventType EventType);
	void UpdateEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties, TAUDBEnum::EventType EventType);

	


private:


	
	FString Index;
	FString ClientId;
	FString Key;
	FString OldKey;     // 带 game_analysis- 前缀
	FString Identify;
};
