#include "TDSHttpObject.h"


TDSHttpObject::TDSHttpObject()
{
	m_type = Empty;
}

TDSHttpObject::TDSHttpObject(int value) :m_intValue(value)
{
	m_type = Int;
}
 
TDSHttpObject::TDSHttpObject(float value) :m_floatValue(value)
{
	m_type = Float;
}

TDSHttpObject::TDSHttpObject(FString value) :m_stringValue(value)
{
	m_type = String;
}

