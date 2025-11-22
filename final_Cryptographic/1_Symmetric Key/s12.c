#include <err.h>
#include <string.h>
#include <openssl/evp.h>

#define KEY "0001020304050607"
#define IV "0001020304050607"
#define keyLength (128 / 8)
#define ivLength (128 / 8)
#define MESSAGE "In cryptography, a public key certificate, also known as a digital certificate or identity certificate, is an electronic document used to prove the validity of a public key."

int decryptExample(EVP_CIPHER_CTX *ctx, unsigned char *ctbuf, int ctlen, unsigned char *ptbuf, int *ptlen)
{
    int len;

    if (!EVP_DecryptUpdate(ctx, ptbuf, &len, ctbuf, ctlen))
        err(-1, "EVP_DecryptUpdate");
    
    *ptlen = len;

    if (!EVP_DecryptFinal_ex(ctx, ptbuf + *ptlen, &len))
        err(-1, "EVP_DecryptFinal_ex()");
    
    *ptlen += len;
    return *ptlen;
}

int encryptExample(EVP_CIPHER_CTX *ctx, unsigned char *ptbuf, int ptlen, unsigned char *ctbuf, int *ctlen)
{
    int tmp;

    if (!EVP_EncryptUpdate(ctx, ctbuf, &tmp, ptbuf, ptlen))
        err(-1, "EVP_EncryptUpdate()");

    *ctlen = tmp;

    if (!EVP_EncryptFinal_ex(ctx, ctbuf + *ctlen, &tmp))
        err(-1, "EVP_EncryptFinal_ex()");

    *ctlen += tmp;
    return *ctlen;
}
36 int main() {
37     int ctlen, ptlen;
38     unsigned char *ctbuf, *ptbuf; // Changed to unsigned char* for consistency
39     EVP_CIPHER_CTX *ctx;
40
41     if (!(ctx = EVP_CIPHER_CTX_new()))
42         err(1, "EVP_CIPHER_CTX_new() error");
43
44     // for encryption
45     if (1 != EVP_EncryptInit(ctx, EVP_aes_128_cbc(), KEY, IV))
46         err(1, "EVP_EncryptInit() error");
47
48     // Allocate enough space for ciphertext (plaintext + one block size)
49     ctbuf = OPENSSL_malloc(strlen(MESSAGE) + EVP_MAX_BLOCK_LENGTH);
50     encryptExample(ctx, (unsigned char *)MESSAGE, strlen(MESSAGE), ctbuf, &ctlen);
51
52     // for decryption
53     if (1 != EVP_DecryptInit(ctx, EVP_aes_128_cbc(), (KEY), IV))
54         err(1, "EVP_DecryptInit() error");
55
56     // Allocate enough space for plaintext
57     ptbuf = OPENSSL_malloc(ctlen);
58     decryptExample(ctx, ctbuf, ctlen, ptbuf, &ptlen);
59
60     // Ensure null termination for printing
61     ptbuf[ptlen] = '\0';
62
63     printf("original message: [%s]\n\n", MESSAGE);
64     printf("decrypted message: [%s]\n", ptbuf);
65
66     OPENSSL_free(ctbuf);
67     OPENSSL_free(ptbuf);
68     EVP_CIPHER_CTX_free(ctx);
69
70     return 0; // Added return statement
71 }