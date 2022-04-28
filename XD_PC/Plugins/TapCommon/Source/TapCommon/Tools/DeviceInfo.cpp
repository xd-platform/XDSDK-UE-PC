#include "DeviceInfo.h"

FString DeviceInfo::GetCPU()
{
	return FPlatformMisc::GetCPUBrand();
}
	
FString DeviceInfo::GetGPU()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}

// ++UE4+Release-4.27-CL-18319896
FString DeviceInfo::GetBuildVersion()
{
	return FApp::GetBuildVersion();
}

// 4.27.2-18319896+++UE4+Release-4.27
FString DeviceInfo::GetEngineVersion()
{
	return FEngineVersion::Current().ToString();
}

// 4.27.0-17155196+++UE4+Release-4.27
FString DeviceInfo::GetCompatibleEngineVersion()
{
	return FEngineVersion::CompatibleWith().ToString();
}
	
//OS: 10.16 (21E258)
FString DeviceInfo::GetOSVersion()
{
	return FPlatformMisc::GetOSVersion();
}

int DeviceInfo::GetScreenWidth()
{
	if (GEngine)
	{
		return GEngine->GameViewport->Viewport->GetSizeXY().X;
	} else
	{
		return 0;
	}
	
}

int DeviceInfo::GetScreenHeight()
{
	if (GEngine)
	{
		return GEngine->GameViewport->Viewport->GetSizeXY().Y;
	} else
	{
		return 0;
	}
}

FString DeviceInfo::GetLoginId()
{
	return FPlatformMisc::GetLoginId();
}

// FString DeviceInfo::GetMacAddress()
// {
// 	auto MacAddr = FPlatformMisc::GetMacAddress();
// 	TArray<FString> MacAddrStr;
// 	for (TArray<uint8>::TConstIterator it(MacAddr); it; ++it)
// 	{
// 		MacAddrStr.Add(FString::Printf(TEXT("%02x"), *it));
// 	}
// 	return FString::Join(MacAddrStr, TEXT(":"));
// }

FString DeviceInfo::GetPlatform()
{
	FString os = "";
#if PLATFORM_IOS
	os = "iOS";
#elif PLATFORM_ANDROID
	os = "Android";
#elif PLATFORM_MAC
	os = "macOS";
#elif PLATFORM_WINDOWS
	os = "Windows";
#elif PLATFORM_LINUX
	os = "Linux";
#endif
	return os;
}

FString DeviceInfo::GetProjectName()
{
	return FApp::GetProjectName();
}

FString DeviceInfo::GetDeviceId()
{
	return FPlatformMisc::GetDeviceId();
}

FString DeviceInfo::GetProjectVersion()
{
	// 待实现
	return "1.0.1";
}




