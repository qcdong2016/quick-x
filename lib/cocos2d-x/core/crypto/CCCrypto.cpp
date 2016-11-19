
#include "CCCrypto.h"
#include "ccMacros.h"

#include "md5.h"
extern "C" {
#include "libbase64.h"
#include "xxtea.h"
}


NS_CC_BEGIN

unsigned char* CCCrypto::encryptXXTEA(unsigned char* plaintext,
                                      int plaintextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_encrypt(plaintext, (xxtea_long)plaintextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

unsigned char* CCCrypto::decryptXXTEA(unsigned char* ciphertext,
                                      int ciphertextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_decrypt(ciphertext, (xxtea_long)ciphertextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

int CCCrypto::encodeBase64Len(int inputLength)
{
    return Base64encode_len(inputLength);
}

int CCCrypto::decodeBase64Len(const char* input)
{
    return Base64decode_len(input);
}

char* CCCrypto::decodeBase64(const char *input, unsigned long& sizeInOut)
{
	size_t bufferSize = Base64decode_len((const char*)input);

	char *buffer = bufferSize ? new char[bufferSize] : NULL;

	if (!buffer) return nullptr;

	sizeInOut = Base64decode(buffer, (const char*)input);

	return buffer;
}

std::string CCCrypto::decodeBase64(const char* input)
{
    size_t bufferSize = Base64decode_len((const char*)input);
    std::string ret(bufferSize, 0);
    
    Base64decode(&ret[0], (const char*)input);
    
    return ret;
}

char* CCCrypto::encodeBase64(const char *input, unsigned long& sizeInOut)
{
	size_t bufferSize = Base64encode_len(sizeInOut);

	char *buffer = bufferSize ? new char[bufferSize] : NULL;

	if (!buffer) return nullptr;

	sizeInOut = Base64encode(buffer, (const char*)input, sizeInOut);

	return buffer;
}

std::string CCCrypto::encodeBase64(const char* input, int inputLen)
{
    size_t bufferSize = Base64encode_len(inputLen);
    std::string ret(bufferSize, 0);
    
    Base64encode(&ret[0], (const char*)input, inputLen);
    
    return ret;
}

static std::string MD5_FinalHex(MD5_CTX* ctx)
{
    std::string bin(CCCrypto::MD5_BUFFER_LENGTH, 0);
    MD5_Final((unsigned char*)(&bin[0]), ctx);
    
    static const char* hextable = "0123456789abcdef";
    
    std::string ret;
    
    //    int hexLength = binLength * 2 + 1;
    //    char* hex = new char[hexLength];
    //    memset(hex, 0, sizeof(char) * hexLength);
    
    //    int ci = 0;
    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = bin[i];
        //        hex[ci++] = hextable[(c >> 4) & 0x0f];
        //        hex[ci++] = hextable[c & 0x0f];
        
        ret += hextable[(c >> 4) & 0x0f];
        ret += hextable[c & 0x0f];
        
    }
    
    return ret;
}

std::string CCCrypto::MD5(void* input, int inputLength)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input, inputLength);
    
    return MD5_FinalHex(&ctx);
}

std::string CCCrypto::MD5File(const char* path)
{
    FILE *file = fopen(path, "rb");
    
    CCAssert(file, "can't open file");
    
    MD5_CTX ctx;
    MD5_Init(&ctx);
    
    int i;
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    while ((i = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        MD5_Update(&ctx, buffer, (unsigned) i);
    }
    
    fclose(file);
    return MD5_FinalHex(&ctx);
}

std::string CCCrypto::MD5String(const char* input, int inputLength)
{
    return MD5((void*)input, inputLength);
}



NS_CC_END
