#pragma once
#include "Blueprint/UserWidget.h"

class TAPCOMMON_API TDSHelper
{
public:

	static void Debug(FString Message, float Duration = 3.f);
	
	
	template <class UIType>
	static UIType* GetWidgetUI(UUserWidget *widget, FName name)
	{
		UIType* view = Cast<UIType>(widget->GetWidgetFromName(name));
		if (view == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s cant find"), *name.ToString());
		}
		return view;
	}
};
