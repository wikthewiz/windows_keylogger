#pragma once
#include "Writer.h"

class FileWriter : public Writer
{
public:
	FileWriter(const TCHAR * fileName);
	~FileWriter(void);

	void Write(TCHAR **,int count);
private:
	TCHAR *m_strFileName;
	FILE *m_pFile;

	bool OpenFile();
	void CloseFile();
};
