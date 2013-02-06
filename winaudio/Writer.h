#pragma once

class Writer
{
public:
	Writer(void);
	~Writer(void);

	virtual void Write(TCHAR **,int count) = 0;
};
