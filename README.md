# XDGSDK-UEPC-6.0

目前支持海外

## 项目依赖库文件

将插件SDK拷贝到项目Plugins文件夹，然后设置依赖库

```c#
PrivateDependencyModuleNames.AddRange(new string[] { "XDGSDK", "Json", "JsonUtilities" });
```

## API介绍

先导入XDGSDK的头文件
```c
#include "XDUE.h"
```

### 初始化

调用`XDUE::InitSDK`方法，`Result`表示结果，后续API的使用，请基于`Result`成功的时候在调用（后续版本会不在强依赖Init成功）
```c++
    XUType::Config Config;
    Config.ClientId = ClientId;
    Config.RegionType = (XUType::RegionType)RegionType;
    XDUE::InitSDK(Config, [](bool Result, FString Message)
    {
        if (Result)
        {
            TUDebuger::DisplayShow(Message);
        } else
        {
            TUDebuger::WarningShow(Message);
        }
    });
```

### 登录接口

`LoginType`是`Default`类型，代表了自动登录，如果已经登录的状态，会重新更新用户状态，如果没有登录，那么会提示登录失败。
```c++
    XDUE::LoginByType((XUType::LoginType)LoginType, [](FXUUser User)
    {
        TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
    }, [](FXUError Error)
    {
        TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
    });
```

### 退出登录
XDSDK支持13种语言，TapSDK仅支持7种，如果XDSDK设置的语言种类超出了TapSDK的支持，那么在TapSDK中显示的是英语（目前TapSDK仅涉及登录模块）
```c++
    XDUE::Logout();
```

### 多语言
XDSDK支持13种语言，TapSDK仅支持7种，如果XDSDK设置的语言种类超出了TapSDK的支持，那么在TapSDK中显示的是英语（目前TapSDK仅涉及登录模块）
```c++
    XDUE::SetLanguage(LangType);
```

### 打开用户中心
目前提供了绑定和解绑的回调。
```c++
    XDUE::OpenUserCenter(
        [](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
            if (Error.IsValid()) {
                TUDebuger::DisplayShow( FString::Printf(TEXT("绑定失败, Error: %s"), *Error->msg));
            }
            else {
                TUDebuger::DisplayShow( FString::Printf(TEXT("绑定成功, Type: %d"), Type));
            }
        },
        [](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
            if (Error.IsValid()) {
                TUDebuger::DisplayShow( FString::Printf(TEXT("解绑失败, Error: %s"), *Error->msg));
            }
            else {
                TUDebuger::DisplayShow( FString::Printf(TEXT("解绑成功, Type: %d"), Type));
            }
        }
    );
```

### 检查补款
```c++
    XDUE::CheckPay([](XUType::CheckPayType CheckType)
    {
        switch (CheckType)
        {
        case XUType::iOSAndAndroid:
            TUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
            break;
        case XUType::iOS:
            TUDebuger::DisplayShow(TEXT("iOS"));
            break;
        case XUType::Android:
            TUDebuger::DisplayShow(TEXT("Android"));
            break;
        case XUType::None:
            TUDebuger::DisplayShow(TEXT("None"));
            break;
        }
    }, [](FXUError Error)
    {
        TUDebuger::DisplayShow(Error.msg);
    });
```

### 打开客服中心

```c++
    XDUE::OpenCustomerCenter("serverId", "roleId", "roleName");
```

### 打开网页支付

```c++
    XDUE::OpenWebPay(ServerId, RoleId);
```

### 是否允许推送服务
韩国同意协议的时候有个是否统一推送的勾选项，韩国用户登录完后，可以用这个接口获取是否能推送。
```c++
    if (XDUE::IsPushServiceEnable()) {
        TUDebuger::DisplayShow("Push Service Enable");
    } else {
        TUDebuger::DisplayShow("Push Service Disable");
    }
```


