#pragma once
#include "Writer.h"

class FileWriter : public Writer
{
public:
	FileWriter(const TCHAR * fileName);
	virtual ~FileWriter(void);

	void Write(TCHAR *,int count);
private:
	FILE *m_pFile;
	TCHAR *m_strFileName;

	bool OpenFile();
	void CloseFile();
};
