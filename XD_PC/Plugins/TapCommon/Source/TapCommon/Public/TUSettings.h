#pragma once

class TAPCOMMON_API TUSettings {
	
public:
	// 设置Widget控件的层级。
	static void SetUILevel(int32 Level);
	static int32 GetUILevel();

	// static void SetUILevel(int32 Level);
	// static int32 Get();
	
private:
	int32 UILevel = 10;;
	UGameInstance* GameInstance = nullptr;
	static TUSettings * Instance;
	static TUSettings& Get();
};
