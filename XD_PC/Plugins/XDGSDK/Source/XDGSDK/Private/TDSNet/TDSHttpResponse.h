#pragma once
#include "CoreMinimal.h"

DEFINE_LOG_CATEGORY_STATIC(TDSHttpLog, Log, All);

class TDSHttpResponse
{
public:
	enum State
	{
		success,      // 200 - 206
		clientError,  // other
		serverError,  // >= 500
		networkError, // connection error
	};

	State state = clientError;

	int32 httpCode;

	TArray<uint8> content;

	/**
	 * Return all headers in an array in "Name: Value" format.
	 *
	 * @return the header array of strings
	 */
	TArray<FString> headers;

};
