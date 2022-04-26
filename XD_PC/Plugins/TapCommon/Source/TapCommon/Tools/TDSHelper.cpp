#include "TDSHelper.h"
#include "qrcodegen.hpp"
#include "ImageUtils.h"

void TDSHelper::Debug(FString Message, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
	}
}

UTexture2D* TDSHelper::GenerateQrCode(UObject* parent, const FString& string)
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
	UTexture2D* texture = UTexture2D::CreateTransient(size, size, EPixelFormat::PF_B8G8R8A8, "QRCode");
	void* data = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(data, pixels.GetData(), size * size * 4);
	texture->PlatformData->Mips[0].BulkData.Unlock();
	texture->UpdateResource();
	texture->Filter = TextureFilter::TF_Nearest;
	return texture;
}
