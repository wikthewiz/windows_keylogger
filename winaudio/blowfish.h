#pragma once

#include "Crypto.h"

typedef struct {
  unsigned long P[16 + 2];
  unsigned long S[4][256];
} BLOWFISH_CTX;

class BlowfishCrypto : Crypto
{
public:
	BlowfishCrypto(unsigned char *key, int keyLen);
	virtual ~BlowfishCrypto();
	void Encrypt(const TCHAR *clearText, TCHAR *cipherText);
	void Decrypt(const TCHAR *cipherText, TCHAR *clearText);
	int GetNeededBytes(const TCHAR *);
private:
	static void encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
	static void decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
	void doCrypto(const TCHAR *cryptoText, TCHAR *buffer, void crypto(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr) );
	void init(unsigned char *key, int keyLen);
	BLOWFISH_CTX *m_ctx;
};



#define PLAIN_TEXT L"Detta är en massa text som ska kryptas"

#if _DEBUG
static bool andvancedTest()
{
	TCHAR *startText = PLAIN_TEXT;
	BlowfishCrypto *crypto = new BlowfishCrypto((unsigned char*)"TESTKEY", 7);
	int neededBytes = crypto->GetNeededBytes(startText);

	TCHAR *cipherText = (TCHAR*)malloc(neededBytes);
	TCHAR *clearText = (TCHAR*)malloc(neededBytes);

	memset(cipherText,0,neededBytes);
	memset(clearText,0,neededBytes);
	crypto->Encrypt(startText, cipherText);
	crypto->Decrypt(cipherText, clearText);

	int count = wcslen(cipherText);
	int count1 = wcslen(clearText);
	bool success = wcscmp(clearText,startText) == 0;


	free(clearText);
	free(cipherText);
	delete crypto;
	crypto = NULL;

	return success;

}


static bool RunCryptoTest()
{	
	// Dont forget to check for memory leaks
	for (int i = 0; i < 1000; i++ )
	{
		if (!andvancedTest())
		{
			return false;
		}
	}
	return true;
}

#endif



