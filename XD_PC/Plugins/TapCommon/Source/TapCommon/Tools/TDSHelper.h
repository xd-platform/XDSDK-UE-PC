#pragma once
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

	template<typename... TReturns, typename... TArgs>
static void InvokeFunction(UClass* objClass, UObject* obj, UFunction* func, TTuple<TReturns...>& outParams, TArgs&&... args)
{
    objClass = obj != nullptr ? obj->GetClass() : objClass;
    UObject* context = obj != nullptr ? obj : objClass;
    uint8* outPramsBuffer = (uint8*)&outParams;

    if (func->HasAnyFunctionFlags(FUNC_Native)) //quick path for c++ functions
    {
        TTuple<TArgs..., TReturns...> params(Forward<TArgs>(args)..., TReturns()...);
        context->ProcessEvent(func, &params);
        //copy back out params
        for (TFieldIterator<FProperty> i(func); i; ++i)
        {
            FProperty* prop = *i;
            if (prop->PropertyFlags & CPF_OutParm)
            {
                void* propBuffer = prop->ContainerPtrToValuePtr<void*>(&params);
                prop->CopyCompleteValue(outPramsBuffer, propBuffer);
                outPramsBuffer += prop->GetSize();
            }
        }
        return;
    }

    TTuple<TArgs...> inParams(Forward<TArgs>(args)...);
    void* funcPramsBuffer = (uint8*)FMemory_Alloca(func->ParmsSize);
    uint8* inPramsBuffer = (uint8*)&inParams;

    for (TFieldIterator<FProperty> i(func); i; ++i)
    {
        FProperty* prop = *i;
        if (prop->GetFName().ToString().StartsWith("__"))
        {
            //ignore private param like __WolrdContext of function in blueprint funcion library
            continue;
        }
        void* propBuffer = prop->ContainerPtrToValuePtr<void*>(funcPramsBuffer);
        if (prop->PropertyFlags & CPF_OutParm)
        {
            prop->CopyCompleteValue(propBuffer, outPramsBuffer);
            outPramsBuffer += prop->GetSize();
        }
        else if (prop->PropertyFlags & CPF_Parm)
        {
            prop->CopyCompleteValue(propBuffer, inPramsBuffer);
            inPramsBuffer += prop->GetSize();
        }
    }

    context->ProcessEvent(func, funcPramsBuffer);   //call function
    outPramsBuffer = (uint8*)&outParams;    //reset to begin

    //copy back out params
    for (TFieldIterator<FProperty> i(func); i; ++i)
    {
        FProperty* prop = *i;
        if (prop->PropertyFlags & CPF_OutParm)
        {
            void* propBuffer = prop->ContainerPtrToValuePtr<void*>(funcPramsBuffer);
            prop->CopyCompleteValue(outPramsBuffer, propBuffer);
            outPramsBuffer += prop->GetSize();
        }
    }
}

	template<typename... TReturns, typename... TArgs>
	static void InvokeFunctionByName(FName functionName, TTuple<TReturns...>& outParams, TArgs&&... args)
	{
		/*
		错误！在PIE模式下，有可能会获得SKEL_XXX_C:Func这个对象，里面的Script为空
		UFunction* func = FindObject<UFunction>(ANY_PACKAGE, *functionName.ToString()); 
		*/
		UFunction* func = (UFunction*)StaticFindObjectFast(UFunction::StaticClass(), nullptr, functionName, false, true, RF_Transient); //exclude SKEL_XXX_C:Func
		InvokeFunction<TReturns...>(func->GetOuterUClass(), nullptr, func, outParams,Forward<TArgs>(args)...);
	}

	// FString oO = UReflectionLibrary::InvokeFunctionByName<FString>(TEXT("TestFunc5"), 10, Actor);

};
