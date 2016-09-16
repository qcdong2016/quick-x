
#ifndef __CC_EXTENSION_CCCRYPTO_H_
#define __CC_EXTENSION_CCCRYPTO_H_

#include "cocos2dx_extra.h"



NS_CC_BEGIN

class CCCrypto
{
public:
    static const int MD5_BUFFER_LENGTH = 16;

    /** @brief Encrypt data with XXTEA algorithm, return ciphertext, free ciphertext after used */
    static unsigned char* encryptXXTEA(unsigned char* plaintext,
                                       int plaintextLength,
                                       unsigned char* key,
                                       int keyLength,
                                       int* resultLength);
    
    /** @brief Decrypt data with XXTEA algorithm, return plaintext, free plaintext after used */
    static unsigned char* decryptXXTEA(unsigned char* ciphertext,
                                       int ciphertextLength,
                                       unsigned char* key,
                                       int keyLength,
                                       int* resultLength);

    /** @brief Get length of encoding data with Base64 algorithm */
    static int encodeBase64Len(int inputLength);
    
    /** @brief Encoding data with Base64 algorithm, return encoded string length */
    static char* encodeBase64(const char* input, unsigned long& sizeInOut);
    
    /** @brief Get length of Decoding Base 64 */
    static int decodeBase64Len(const char* input);

    /** @brief Decoding Base64 string to data, return decoded data length */
    static char* decodeBase64(const char* input, unsigned long& sizeInOut);
    
    /** @brief Calculate MD5, get MD5 code (not string) */
    static void MD5(void* input, int inputLength,
                    unsigned char* output);
    
    static void MD5File(const char* path, unsigned char* output);
    
    

    static const string MD5String(void* input, int inputLength);
    
#pragma mark -
#pragma mark for Lua
    
#if CC_LUA_ENGINE_ENABLED > 0

    
    /** @brief Encrypt data with XXTEA algorithm, return ciphertext string and length, return nil if failed */
    static int encryptXXTEALua(const char* plaintext,
                                      int plaintextLength,
                                      const char* key,
                                      int keyLength);
    
    /** @brief Decrypt data with XXTEA algorithm, return plaintext string and length, return nil if failed */
    static int decryptXXTEALua(const char* ciphertext,
                                      int ciphertextLength,
                                      const char* key,
                                      int keyLength);
    
    /** @brief Encoding data with Base64 algorithm, return encoded string */
    static int encodeBase64Lua(const char* input, int inputLength)
    {
        return encodingBase64Lua(false, input, inputLength);
    }
    
    /** @brief Decoding Base64 string to data, return decoded data length */
    static int decodeBase64Lua(const char* input)
    {
        return encodingBase64Lua(true, input, (int)strlen(input));
    }
    
    /** @brief Calculate MD5, return MD5 string */
    static int MD5Lua(char* input, bool isRawOutput);

    static int MD5FileLua(const char* path);

#endif /* CC_LUA_ENGINE_ENABLED */
    
#pragma mark -
#pragma mark private methods
    
private:
    CCCrypto(void) {}
    
#if CC_LUA_ENGINE_ENABLED > 0

    
    static int encodingBase64Lua(bool isDecoding,
                                        const char* input,
                                        int inputLength);
    
    static char* bin2hex(unsigned char* bin, int binLength);
#endif /* CC_LUA_ENGINE_ENABLED */
    
};

NS_CC_END

#endif // __CC_EXTENSION_CCCRYPTO_H_
