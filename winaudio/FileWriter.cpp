#include "StdAfx.h"
#include "FileWriter.h"
#include <stdio.h>

FileWriter::FileWriter(const TCHAR *fileName)
{
	m_strFileName = (TCHAR*) malloc( sizeof(TCHAR) * MAX_PATH );
	swprintf_s( m_strFileName, MAX_PATH, fileName);
	m_pFile = NULL;
}

FileWriter::~FileWriter(void)
{
	free(m_strFileName);
}

void FileWriter::Write(TCHAR ** str, int count)
{
	if (OpenFile())
	{
		fwrite(*str, sizeof(TCHAR), count, m_pFile) ;
		fflush(m_pFile);
		CloseFile();
	}
}

bool FileWriter::OpenFile()
{
	m_pFile = _wfopen(m_strFileName, L"a+, ccs=UNICODE");
	return m_pFile != NULL;
}
void FileWriter::CloseFile()
{
	fclose(m_pFile);
	m_pFile = NULL;
}
