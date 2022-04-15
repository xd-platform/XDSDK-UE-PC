#pragma once

class TDSHttpObject
{
public:
	enum Type
	{
		Empty,
		Int,
		Float,
		String,
	};

	Type m_type;
	int m_intValue = 0;
	float m_floatValue = 0;
	FString m_stringValue;

	TDSHttpObject();
	TDSHttpObject(int value);
	TDSHttpObject(float value);
	TDSHttpObject(FString value);
	
};
