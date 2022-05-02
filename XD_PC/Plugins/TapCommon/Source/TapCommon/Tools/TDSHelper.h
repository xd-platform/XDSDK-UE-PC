#pragma once
#include "TDUDebuger.h"
#include "Blueprint/UserWidget.h"

class TAPCOMMON_API TDSHelper
{
public:
	
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

	static UTexture2D* GenerateQrCode(const FString& string);


	static FString GetRandomStr(int len);

	static void GenerateBrush(FSlateBrush& Brush, const FString& TexturePath);

	// 把Adder加到Object中。
	static void JsonObjectAppend(TSharedPtr<FJsonObject>& Object, const TSharedPtr<FJsonObject>& Adder);

	// 把Adder加到Object中。
	static void JsonObjectAddNotEmptyString(TSharedPtr<FJsonObject>& Object, const FString& Key, const FString& Value);

	template<typename TReturn, typename... TArgs>
	static TReturn InvokeFunctionByName(FString CLassName, FString FunctionName, TArgs&&... args)
	{
		TReturn Value;
		if (CLassName.IsEmpty() || FunctionName.IsEmpty())
		{
			TDUDebuger::ErrorLog("类名或者方法名不得为空");
			return Value;
		}
		UClass* ResultClass = FindObject<UClass>(ANY_PACKAGE, *CLassName);
		if (ResultClass)
		{
			UFunction* CallBack = ResultClass->FindFunctionByName(FName(*FunctionName));
			if (CallBack)
			{
				TTuple<TArgs..., TReturn> params(Forward<TArgs>(args)..., Value);
				ResultClass->ProcessEvent(CallBack, &params);
				//copy back out params
				for (TFieldIterator<FProperty> i(CallBack); i; ++i)
				{
				    FProperty* prop = *i;
				    if (prop->PropertyFlags & CPF_OutParm)
				    {
				        void* propBuffer = prop->ContainerPtrToValuePtr<void*>(&params);
				        prop->CopyCompleteValue(&Value, propBuffer);
				    }
				}
				return Value;
			}
		}
		TDUDebuger::WarningLog("映射调用失败");
		return Value;
	}

};
