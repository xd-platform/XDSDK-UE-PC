#pragma once

class XURegionConfig {
public:

	virtual ~XURegionConfig() = default;
	virtual FString BaseHost();

	//获取配置
	FString InitSDKUrl();
	
	// login
	FString UserProfileUrl();
	
	//游客
	FString CommonLoginUrl();
	
	// 与leanClound同步
	FString LoginSynUrl();

	// 获取用户绑定信息
	FString BindListUrl();

	// 绑定接口
	FString BindInterfaceUrl();

	// 解绑接口
	FString UnbindInterfaceUrl();

	// 查询补款订单信息
	FString PaybackListUrl();
	

	static TSharedPtr<XURegionConfig>& Get();

private:
	static TSharedPtr<XURegionConfig> Instance;
};

