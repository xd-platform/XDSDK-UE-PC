#include "XURegionConfig.h"
#include "XUImpl.h"


class XURegionConfigCN: public XURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://xdsdk-6.xd.cn";
		// @"https://tds-dev.xindong.com";
	}
};

class XURegionConfigIO: public XURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://xdsdk-intnl-6.xd.com";
		// "https://test-xdsdk-intnl-6.xd.com"; //测试
	}
};

TSharedPtr<XURegionConfig> XURegionConfig::Instance = nullptr;

FString XURegionConfig::InitSDKUrl() {
	return BaseHost() / "/api/init/v1/config";
}

FString XURegionConfig::UserProfileUrl() {
	return BaseHost() / "/api/account/v1/info";
}

FString XURegionConfig::CommonLoginUrl() {
	return BaseHost() / "/api/login/v1/union";
}

FString XURegionConfig::LoginSynUrl() {
	return BaseHost() / "/api/login/v1/syn";
}

FString XURegionConfig::BindListUrl() {
	return BaseHost() / "/api/account/v1/bind/list";
}

FString XURegionConfig::BindInterfaceUrl() {
	return BaseHost() / "/api/account/v1/bind";
}

FString XURegionConfig::UnbindInterfaceUrl() {
	return BaseHost() / "/api/account/v1/unbind";
}

FString XURegionConfig::PaybackListUrl() {
	return BaseHost() / "/order/v1/user/repayOrders";
}

FString XURegionConfig::UploadAgreementUrl() {
	return BaseHost() / "/api/account/v1/agreement/confirm";
}

TSharedPtr<XURegionConfig>& XURegionConfig::Get() {
	if (!Instance.IsValid()) {
		if (XUImpl::Get()->Config.RegionType == XUType::CN) {
			Instance = MakeShareable(new XURegionConfigCN);
		} else {
			Instance = MakeShareable(new XURegionConfigIO);
		}
	}
	return Instance;
}
