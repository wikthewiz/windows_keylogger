#pragma once

class Crypto
{
public:
	Crypto(void);
	virtual ~Crypto(void);
	virtual void Encrypt(const TCHAR *clearText, TCHAR *cipherText) = 0;
	virtual void Decrypt(const TCHAR *cipherText, TCHAR *clearText) = 0;
	virtual int GetNeededBytes(const TCHAR *) = 0;
};
