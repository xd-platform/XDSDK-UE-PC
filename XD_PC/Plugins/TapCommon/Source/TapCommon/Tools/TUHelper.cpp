#include "TUHelper.h"
#include "qrcodegen.hpp"
#include "ImageUtils.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#if PLATFORM_WINDOWS
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#endif


UTexture2D* TUHelper::GenerateQrCode(const FString& string)
{
	qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(TCHAR_TO_UTF8(*string), qrcodegen::QrCode::Ecc::LOW);
	int border = 0;
	int size = qr.getSize() + border * 2;
	TArray<FColor> pixels;
	pixels.SetNumZeroed(size * size);
	FColor black = FColor::Black;
	FColor white = FColor::White;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			FColor color = qr.getModule(x, y) ? black : white;
			pixels[(x + border) + (y + border) * size] = color;
		}
	}
	UTexture2D* texture = UTexture2D::CreateTransient(size, size, EPixelFormat::PF_B8G8R8A8);
	void* data = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(data, pixels.GetData(), size * size * 4);
	texture->PlatformData->Mips[0].BulkData.Unlock();
	texture->UpdateResource();
	texture->Filter = TextureFilter::TF_Nearest;
	return texture;
}

FString TUHelper::GetRandomStr(int len)
{
	FString LetterStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	FString SB;
	int LetterLen = LetterStr.Len();
	for (int i = 0; i < len; i++){
		SB += LetterStr[FMath::RandRange(0, LetterLen-1)];
	}

	return SB;
}

void TUHelper::GenerateBrush(FSlateBrush& Brush, const FString& TexturePath)
{
	UTexture2D *Texture = LoadObject<UTexture2D>(nullptr, *TexturePath);
	if(IsValid(Texture) && Brush.GetResourceObject() != Texture)
	{
		Brush.SetResourceObject(Texture);

		if (Texture) // Since this texture is used as UI, don't allow it affected by budget.
			{
			Texture->bForceMiplevelsToBeResident = true;
			Texture->bIgnoreStreamingMipBias = true;
			}

		if (Texture)
		{
			Brush.ImageSize.X = Texture->GetSizeX();
			Brush.ImageSize.Y = Texture->GetSizeY();
		}
	}
}

void TUHelper::JsonObjectAppend(TSharedPtr<FJsonObject>& Object, const TSharedPtr<FJsonObject>& Adder)
{
	if (Object.IsValid() && Adder.IsValid())
	{
		Object->Values.Append(Adder->Values);
	}
}

void TUHelper::JsonObjectAddNotEmptyString(TSharedPtr<FJsonObject>& Object, const FString& Key, const FString& Value)
{
	if (Value.Len() > 0)
	{
		Object->SetStringField(Key, Value);
	}
}

FString TUHelper::CombinParameters(const TSharedPtr<FJsonObject>& parameters, bool isEncode) {
	TArray<FString> keyValues;
	for (auto para : parameters->Values)
	{
		FString value;
		if (para.Value->TryGetString(value))
		{
			keyValues.Add(para.Key + "=" +  (isEncode ? FGenericPlatformHttp::UrlEncode(value) : value));
		}
	}
	return FString::Join(keyValues, TEXT("&"));
}

#if PLATFORM_MAC
void TUHelper::ActivateItself() {
	FString Path = FPlatformProcess::BaseDir();
	int32 index = Path.Find(TEXT(".app"));
	if (index == INDEX_NONE) {
		return;
	}
	Path.LeftInline(index + 4);
	
	TUDebuger::DisplayShow(Path);
	FMacPlatformProcess::LaunchFileInDefaultExternalApplication(*Path);
}
#endif

#if PLATFORM_WINDOWS
void TDSHelper::ActivateItself() {
	DWORD dwProcID = GetCurrentProcessId();
	HWND hWnd = GetTopWindow(GetDesktopWindow());
	while(hWnd)
	{
		DWORD dwWndProcID = 0;
		GetWindowThreadProcessId(hWnd, &dwWndProcID);
		if(dwWndProcID == dwProcID)
		{
			break;
		}
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	if (!hWnd)
	{
		return;
	}
	HWND hCurWnd = ::GetForegroundWindow();
	DWORD dwMyID = ::GetCurrentThreadId();
	DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
	::AttachThreadInput(dwCurID, dwMyID, TRUE);   
	::SetForegroundWindow(hWnd);
	::AttachThreadInput(dwCurID, dwMyID, FALSE);
}
#endif

