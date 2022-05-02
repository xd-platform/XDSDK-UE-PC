#include "DeviceInfo.h"
#include "SocketSubsystem.h"
#include "TAUCommonStorage.h"


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

FString DeviceInfo::GetInstallId()
{
	FString InstallID = DataStorage<FTAUCommonStorage>::LoadString(FTAUCommonStorage::InstallID);
	if (InstallID.Len() <= 0)
	{
		InstallID = FGuid::NewGuid().ToString();
		DataStorage<FTAUCommonStorage>::SaveString(FTAUCommonStorage::InstallID, InstallID);
	}
	return InstallID;
	
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
	FString OS;
#if PLATFORM_IOS
	OS = "iOS";
#elif PLATFORM_ANDROID
	OS = "Android";
#elif PLATFORM_MAC
	OS = "macOS";
#elif PLATFORM_WINDOWS
	OS = "Windows";
#elif PLATFORM_LINUX
	OS = "Linux";
#endif
	return OS;
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

FString DeviceInfo::GetIpv4()
{
	ISocketSubsystem *socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	FString HostName;
	FString Ipv4;
	if (socket->GetHostName(HostName))
	{
		FAddressInfoResult GAIRequest = socket->GetAddressInfo(*HostName, nullptr,
			EAddressInfoFlags::AllResultsWithMapping | EAddressInfoFlags::OnlyUsableAddresses,
			FNetworkProtocolTypes::IPv6);

		// Start packing the addresses we got to the results.
		if (GAIRequest.ReturnCode == SE_NO_ERROR)
		{
			for (auto Result : GAIRequest.Results)
			{
				FString IPStr = Result.Address->ToString(false);
				if (IPStr != "127.0.0.1" && IPStr != "0.0.0.0")
				{
					Ipv4 = IPStr;
				}
			}
		}
	}
	return Ipv4;
}

FString DeviceInfo::GetIpv6()
{
	ISocketSubsystem *socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	FString HostName;
	FString Ipv6;
	if (socket->GetHostName(HostName))
	{
		FAddressInfoResult GAIRequest = socket->GetAddressInfo(*HostName, nullptr,
			EAddressInfoFlags::AllResultsWithMapping | EAddressInfoFlags::OnlyUsableAddresses,
			FNetworkProtocolTypes::IPv6);

		// Start packing the addresses we got to the results.
		if (GAIRequest.ReturnCode == SE_NO_ERROR)
		{
			if (GAIRequest.Results.Num() > 0)
			{
				Ipv6 = GAIRequest.Results.Last().Address->ToString(false);
			}
		}
	}
	return Ipv6;
}




