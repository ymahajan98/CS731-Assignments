#include<bits/stdc++.h>
#include<string>
#include<openssl/conf.h>
#include<openssl/evp.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/crypto.h>
#include<openssl/sha.h>
using namespace std;

//Characters of base58
string ALPHABET="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
//This is the algorithm o base-x's library github link https://github.com/cryptocoinjs/base-x/blob/master/index.js
//This function is directly taken from there
int EncodeBase58(const unsigned char *bytes, int len, unsigned char result[]) {
    unsigned char digits[len * 137 / 100];
    int digitslen = 1;
    for (int i = 0; i < len; i++) {
        unsigned int carry = (unsigned int) bytes[i];
        for (int j = 0; j < digitslen; j++) {
            carry += (unsigned int) (digits[j]) << 8;
            digits[j] = (unsigned char) (carry % 58);
            carry /= 58;
        }
        while (carry > 0) {
            digits[digitslen++] = (unsigned char) (carry % 58);
            carry /= 58;
        }
    }
    int resultlen = 0;
    for (; resultlen < len && bytes[resultlen] == 0;)
        result[resultlen++] = '1';
    for (int i = 0; i < digitslen; i++)
        result[resultlen + i] = ALPHABET[digits[digitslen - 1 - i]];
    result[digitslen + resultlen] = 0;
    return digitslen + resultlen;
}
int main(){
    //Declared variables
    size_t pri_len, pub_len;
    char *pri_key;
    char *pub_key;
    //Declared rsa variable
    RSA *rsa=NULL;
    BIGNUM *bne=NULL;

    //Generated keys
    bne = BN_new();
    BN_set_word(bne,RSA_F4);
    rsa = RSA_new();
    RSA_generate_key_ex(rsa,2048,bne,NULL);

    // This part of converting rsa keys into char array was searched on the net
	BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, rsa, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, rsa);

    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    pri_key = (char*)malloc(pri_len + 1);
    pub_key = (char*)malloc(pub_len + 1);

    //USED BIO and PREM to read the private and public keys into strings
    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    //Printed the public key
    printf("\n%s\n", pub_key);

    unsigned char arr[SHA256_DIGEST_LENGTH];

    //Hashed the public key
    SHA256_CTX sha2;
    SHA256_Init(&sha2);
    SHA256_Update(&sha2,&pub_key,pub_len+1);
    SHA256_Final(arr,&sha2);

    //Encoded in base58
    unsigned char b_58[SHA256_DIGEST_LENGTH*137/100];
    EncodeBase58(arr,SHA256_DIGEST_LENGTH,b_58);

    //Printed the result
    cout<<"Public key hashed by SHA256 and base58-encoded is:"<<endl;
    printf("%s\n",b_58);
}
