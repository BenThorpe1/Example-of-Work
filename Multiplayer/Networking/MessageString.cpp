#include "stdafx.h"


MessageString::MessageString()
{
	m_type = MessageType::STRING;
	char temp[] = ".";
	strcpy_s(m_text, strlen(temp)+1 , temp);	
	printf("%s MS9 \n", m_text);

}
MessageString::~MessageString()
{

}
void MessageString::SetText(const char* a_value)
{
	int i = (unsigned)strlen(a_value) + 1;
	strcpy_s(m_text,i,a_value);
	printf("%s MS20 \n", m_text);

}

int MessageString::Size()
{
	return strlen(m_text);
}

const char* MessageString::GetText()
{ 
	printf("%s MS27 \n" ,m_text);
	return  m_text; 
}