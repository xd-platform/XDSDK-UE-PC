// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TauDB.generated.h"

struct TauDBInitConfig
{
	
	// 注册游戏时获得的APP ID
	FString AppIdOrClientId;

	// 分包渠道名称，可为空
	FString Channel;

	// 游戏版本
	FString GameVersion;

	// 是否是国内
	bool isCN;

	// 自定义属性
	TSharedPtr<FJsonObject> Properties;

};

/**
 * 
 */

namespace TauDBLoginType
{
	static FString TapTap = "TapTap";
	static FString WeiXin = "WeiXin";
	static FString QQ = "QQ";
	static FString Tourist = "Tourist";
	static FString Apple = "Apple";
	static FString ZhiFuBao = "ZhiFuBao";
	static FString Facebook = "Facebook";
	static FString Google = "Google";
	static FString Twitter = "Twitter";
	static FString PhoneNumber = "PhoneNumber";
}


UCLASS()
class TAPDB_API UTauDB : public UObject
{
	GENERATED_BODY()
public:

	
	static void StartByAppID(const TauDBInitConfig& Config);
	static void StartByClientID(const TauDBInitConfig& Config);

	/// 记录一个用户（不是游戏角色！！！！），需要保证唯一性
	/// @param UserId 用户ID。不同用户需要保证ID的唯一性
	/// @param LoginType 登录方式
	/// @param Properties 自定义属性
	static void SetUser(const FString& UserId, const FString& LoginType = FString(), TSharedPtr<FJsonObject> Properties = nullptr);
	static void ClearUser();

	/**
	* 目前TapDB SDK仅支持单实例模式，如果多次调用onStart方法，只有最初传入的appid生效。调用该函数可以获取生效的信息
	* 返回值：包含appId和channel的NSDictionary，对应的value均可能为nil
	*/
	static TSharedPtr<FJsonObject> GetStartInfo();

	/// 获取当前的设备标识
	static FString GetDeviceId();

	/**
	* 设置用户等级，初次设置时或升级时调用
	* level: 等级
	*/
	static void SetLevel(int Level);

	/**
	* 设置用户服务器，初次设置或更改服务器的时候调用
	* server: 服务器
	*/
	static void SetServer(const FString& Server);

	/// 设置用户名
	/// @param Name 必传，长度大于0并小于等于256，用户名
	static void SetName(const FString& Name);

	/**
	* 充值成功时调用
	* orderId: 订单ID，可为空
	* product: 产品名称，可为空
	* amount: 充值金额（单位分，即无论什么币种，都需要乘以100）
	* currencyType: 货币类型，可为空，参考：人民币 CNY，美元 USD；欧元 EUR
	* payment: 支付方式，可为空，如：支付宝
	* properties: 事件属性，需要在控制后台预先进行配置,值为长度大于0并小于等于256的字符串或绝对值小于1E11的浮点数
	*/
	static void ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties = nullptr);

	/**
	 * 自定义事件
	 * eventName: 事件代码，需要在控制后台预先进行配置
	 * properties: 事件属性，需要在控制后台预先进行配置,值为长度大于0并小于等于256的字符串或绝对值小于1E11的浮点数
	 */
	static void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);

	/// 初始化设备属性操作
	/// @param Properties 属性字典
	static void DeviceInitialize(TSharedPtr<FJsonObject> Properties);

	/// 更新设备属性操作
	/// @param Properties 属性字典
	static void DeviceUpdate(TSharedPtr<FJsonObject> Properties);

	/// 设备属性增加操作
	/// @param Properties 属性字典 暂时只支持数字属性
	static void DeviceAdd(TSharedPtr<FJsonObject> Properties);

	/// 初始化用户属性操作
	/// @param Properties 属性字典
	static void UserInitialize(TSharedPtr<FJsonObject> Properties);

	/// 更新用户属性操作
	/// @param Properties 属性字典
	static void UserUpdate(TSharedPtr<FJsonObject> Properties);

	/// 用户属性增加操作
	/// @param Properties 属性字典 暂时只支持数字属性
	static void UserAdd(TSharedPtr<FJsonObject> Properties);


	/// 添加静态事件属性，每个事件都将会发送
	/// @param StaticProperties 属性字典
	static void RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties);


	/// 删除添加的静态事件属性
	static void ClearStaticProperties();

	/// 删除添加的某个静态事件属性
	/// @param PropertyName 属性Key
	static void UnregisterStaticProperty(const FString& PropertyName);

	/// 添加动态事件属性，每次发送事件会调用dynamicPropertiesCaculator
	/// @param DynamicPropertiesCaculator 动态属性回调，需返回希望上传的属性字典
	static void RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator);

private:
	static void Start(bool isClientID, const TauDBInitConfig& Config);
	
};
