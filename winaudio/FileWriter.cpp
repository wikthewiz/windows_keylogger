#include "StdAfx.h"
#include "FileWriter.h"
#include <stdio.h>

FileWriter::FileWriter(const TCHAR *fileName):
	m_pFile(NULL)
{
	m_strFileName = (TCHAR*) malloc( sizeof(TCHAR) * MAX_PATH );
	swprintf_s( m_strFileName, MAX_PATH, fileName);
}

FileWriter::~FileWriter(void)
{
	if (m_strFileName != NULL)
	{
		free(m_strFileName);
		m_strFileName = NULL;
	}
}

void FileWriter::Write(TCHAR *str, int count)
{
	if (OpenFile())
	{
		fwrite(str,  count, sizeof(TCHAR),m_pFile) ;
		fflush(m_pFile);
		CloseFile();
	}
}

bool FileWriter::OpenFile()
{
	m_pFile = _wfopen(m_strFileName, L"ab" /* "a+,ccs=UTF-8"*/);
	return m_pFile != NULL;
}
void FileWriter::CloseFile()
{
	fclose(m_pFile);
	m_pFile = NULL;
}
