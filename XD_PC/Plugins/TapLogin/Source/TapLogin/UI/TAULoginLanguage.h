#pragma once

class ILoginLang
{
public:
	virtual ~ILoginLang() = default;
	
	virtual FString TitleUse() = 0;

	virtual FString TitleLogin() = 0;

	virtual FString QrTitleLogin() = 0;

	virtual FString QrRefresh() = 0;

	virtual FString QrNoticeUse() = 0;

	virtual FString QrNoticeClient() = 0;

	virtual FString QrNoticeScanToLogin() = 0;

	virtual FString WebLogin() = 0;

	virtual FString WebNotice() = 0;

	virtual FString WebButtonJumpToWeb() = 0;

	virtual FString QrNoticeCancel() = 0;

	virtual FString QrNoticeCancel2() = 0;

	virtual FString QrnNoticeSuccess() = 0;
	
	virtual FString QrnNoticeSuccess2() = 0;

	virtual FString WebNoticeLogin() = 0;

	virtual FString WebNoticeFail() = 0;

	virtual FString WebNoticeFail2() = 0;
};

class TAULoginLanguage
{
public:

	

	static TSharedPtr<ILoginLang> GetCurrentLang();

private:

	TAULoginLanguage();
	
	TSharedPtr<ILoginLang> cn;

	TSharedPtr<ILoginLang> io;
	
	static TSharedPtr<TAULoginLanguage> Instance;
	
	static TSharedPtr<TAULoginLanguage>& Get();
	
};

class LoginLangCN : public ILoginLang
{
public:
	virtual FString TitleUse() override
	{
		return "使用";
	}

	virtual FString TitleLogin() override
	{
		return "账号登录";
	}

	virtual FString QrTitleLogin() override
	{
		return "安全扫码登录";
	}

	virtual FString QrRefresh() override
	{
		return "点击刷新";
	}

	virtual FString QrNoticeUse() override
	{
		return "请使用";
	}

	virtual FString QrNoticeClient() override
	{
		return "客户端";
	}

	virtual FString QrNoticeScanToLogin() override
	{
		return "扫描二维码登录";
	}

	virtual FString WebLogin() override
	{
		return "使用网页浏览器完成授权";
	}

	virtual FString WebNotice() override
	{
		return "点击下方按钮前往网页浏览器，\n授权 TapTap 账号，完成后将自动返回游戏。";
	}
        
	virtual FString WebButtonJumpToWeb() override
	{
		return "跳转至 TapTap";
	}

	virtual FString QrNoticeCancel() override
	{
		return "您已取消此次登录";
	}

	virtual FString QrNoticeCancel2() override
	{
		return "请重新扫码";
	}

	virtual FString QrnNoticeSuccess() override
	{
		return "扫码成功";
	}

	virtual FString QrnNoticeSuccess2() override
	{
		return "请在手机上确认";
	}

	virtual FString WebNoticeLogin() override
	{
		return "正在登录中，请稍后";
	}

	virtual FString WebNoticeFail() override
	{
		return "登录失败";
	}

	virtual FString WebNoticeFail2() override
	{
		return "请重新点击打开网页进行授权";
	}
};

class LoginLangIO : public ILoginLang
{
public:

	virtual FString TitleUse() override
	{
		return "Log In with ";
	}

	virtual FString TitleLogin() override
	{
		return "";
	}

	virtual FString QrTitleLogin() override
	{
		return "Via QR Code";
	}

	virtual FString QrRefresh() override
	{
		return "Refresh";
	}

	virtual FString QrNoticeUse() override
	{
		return "Use ";
	}

	virtual FString QrNoticeClient() override
	{
		return " App";
	}

	virtual FString QrNoticeScanToLogin() override
	{
		return "to scan the code";
	}

	virtual FString WebLogin() override
	{
		return "Via Web Browser";
	}

	virtual FString WebNotice() override
	{
		return "Allow permission to log in with TapTap,\nYou'll be redirected back after login.";
	}

	virtual FString WebButtonJumpToWeb() override
	{
		return "Go to TapTap";
	}
        
	virtual FString QrNoticeCancel() override
	{
		return "Failed to log in";
	}

	virtual FString QrNoticeCancel2() override
	{
		return "Please try again";
	}

	virtual FString QrnNoticeSuccess() override
	{
		return "Success";
	}

	virtual FString QrnNoticeSuccess2() override
	{
		return "Please confirm login on your phone";
	}

	virtual FString WebNoticeLogin() override
	{
		return "Logging in";
	}

	virtual FString WebNoticeFail() override
	{
		return "Failed to log in";
	}

	virtual FString WebNoticeFail2() override
	{
		return "Please try again";
	}
};
