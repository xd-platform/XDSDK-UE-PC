#include "TUSettings.h"

TUSettings * TUSettings::Instance = nullptr;

void TUSettings::SetUILevel(int32 Level) {
	Get().UILevel = Level;
}

int32 TUSettings::GetUILevel() {
	return Get().UILevel;
}

TUSettings& TUSettings::Get() {
	if (Instance == nullptr) {
		Instance = new TUSettings();
	}
	return *Instance;
}
