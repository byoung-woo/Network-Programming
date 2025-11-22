#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
// #include "common.h" // 필요한 경우 추가

#define KEY "0001020304050607"
#define IV "0001020304050607"
#define keyLength (128 / 8)
#define ivLength (128 / 8)

#define BlockLength 16
#define CIPHER EVP_aes_128_cbc()
#define KeyLength (128 / 8) 
#define IVLength (128 / 8) 
#define MESSAGE "In cryptography, a public key certificate, also known as a digital certificate or identity certificate, is an electronic document used to prove the validity of a public key."

// 문자열을 16진수로 출력하는 보조 함수
void asc2hex(char *name, const char *cp) {
    int i;
    printf("%s is 0x", name);
    for (i = 0; i < strlen(cp); i++) 
        printf("%02x", (unsigned char)cp[i]);
    printf("\n");
}
// 복호화 (Decrypt) 함수
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

// 암호화 (Encrypt) 함수
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
int main(int argc, char *argv[]) {
    int fileInfid, outfid;
    int inlen, outlen, readlen;
    unsigned char key[KeyLength + 1], iv[IVLength + 1];
    unsigned char readData[BlockLength + 1];
    EVP_CIPHER_CTX *ctx;
    unsigned char *ct = NULL; // 암호문 블록 버퍼

    if (argc != 3) {
        printf("Usage: a.out plaintextFile encryptedOutFile \n");
        exit(1);
    }

    // 1. 파일 열기
    if ((fileInfid = open(argv[1], O_RDONLY)) < 0) {
        printf("can't open input file \n");
        exit(1);
    }
    if ((outfid = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        printf("can't open output file \n");
        exit(1);
    }

    // 2. 키/IV 설정
    memset(key, 0, KeyLength + 1);
    memset(iv, 0, IVLength + 1);
    memcpy(key, "0001020304050607", KeyLength); // aes128, key=128bits
    memcpy(iv, "0001020304050607", IVLength);

    // 3. 컨텍스트 생성 및 암호화 초기화
    if (!(ctx = EVP_CIPHER_CTX_new()))
        exit(1);
    EVP_EncryptInit(ctx, CIPHER, key, iv);
    
    // 4. 버퍼 할당 및 파일 크기 확인
    ct = malloc(EVP_CIPHER_block_size(ctx)); 
    inlen = lseek(fileInfid, 0, SEEK_END);
    lseek(fileInfid, 0, SEEK_SET);

    int pos = 0;
    // 5. 블록 단위 암호화 루프
    do {
        if ((readlen = read(fileInfid, readData, BlockLength)) <= 0) {
            if (readlen < 0) printf("read from [%s] file error.\n", argv[1]);
            break;
        }
        
        if (EVP_EncryptUpdate(ctx, ct, &outlen, readData, readlen) <= 0)
            printf("EVP_EncryptUpdate() error.\n");

        write(outfid, ct, outlen);
        pos += readlen;
    } while (pos < inlen);

    // 6. 최종 마무리 및 출력
    if (EVP_EncryptFinal_ex(ctx, ct, &outlen) <= 0)
        printf("EVP_EncryptFinal_ex() error.\n");

    write(outfid, ct, outlen);

    asc2hex("Key", (const char*)key);
    asc2hex("IV", (const char*)iv);

    // 7. 자원 해제
    close(outfid);
    close(fileInfid);
    EVP_CIPHER_CTX_free(ctx);
    free(ct); 
    
    return 0; 
}