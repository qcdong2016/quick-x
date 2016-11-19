
#ifndef __CC_EXTENSION_CCCRYPTO_H_
#define __CC_EXTENSION_CCCRYPTO_H_

#include "CCPlatformMacros.h"
#include <string>

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
    static std::string encodeBase64(const char* input, int inputLen);
    
    /** @brief Get length of Decoding Base 64 */
    static int decodeBase64Len(const char* input);

    /** @brief Decoding Base64 string to data, return decoded data length */
    static char* decodeBase64(const char* input, unsigned long& sizeInOut);
    static std::string decodeBase64(const char* input);
    
    /** @brief Calculate MD5, get MD5 hex*/
    static std::string MD5(void* input, int inputLength);
    
    static std::string MD5String(const char* input, int inputLength);
    
    static std::string MD5File(const char* path);


private:
    CCCrypto(void) {}
};

NS_CC_END

#endif // __CC_EXTENSION_CCCRYPTO_H_
