#pragma once

class Writer
{
public:
	Writer(void);
	virtual ~Writer(void);

	virtual void Write(TCHAR *,int count) = 0;
};
