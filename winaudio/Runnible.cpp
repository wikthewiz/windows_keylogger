#include "StdAfx.h"
#include "Runnible.h"

Runnible::Runnible(void):m_Enabled(false){}

Runnible::~Runnible(void)
{
}
void Runnible::Enable()
{
	if (!m_Enabled)
	{
		OnEnable();
	}
}
void Runnible::Disable()
{
	if(m_Enabled)
	{
		OnDisable();
	}
}

bool Runnible::IsEnabled()
{
	return m_Enabled;
}
