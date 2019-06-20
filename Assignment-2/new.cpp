#include <memory>
using std::unique_ptr;

#include <openssl/bn.h>
#include <openssl/rsa.h>

#include <cassert>
#define ASSERT assert

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;

int main(int argc, char* argv[])
{
    int rc;

    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);

    rc = BN_set_word(bn.get(), RSA_F4);
    ASSERT(rc == 1);

    rc = RSA_generate_key_ex(rsa.get(), 2048, bn.get(), NULL);
    ASSERT(rc == 1);

    RSA_ptr rsa_pub(RSAPublicKey_dup(rsa.get()), ::RSA_free);
    RSA_ptr rsa_priv(RSAPrivateKey_dup(rsa.get()), ::RSA_free);

    fprintf(stdout, "\n");
    RSA_print_fp(stdout, rsa_pub.get(), 0);

    fprintf(stdout, "\n");
    RSA_print_fp(stdout, rsa_priv.get(), 0);

    return 0;
}
