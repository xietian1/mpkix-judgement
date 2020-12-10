//
// Created by xt on 12/9/20.
//

//aka Levenshtein distance

#include "edit_distance_common.hpp"
// get the edit_alignment() function
#include <boost/algorithm/sequence/edit_distance.hpp>
using boost::algorithm::sequence::edit_distance;
using namespace boost::algorithm::sequence::parameter;
using boost::algorithm::sequence::unit_cost;

#include <stdio.h>
#include <string>
#include <stdint.h>
#include <iomanip>
#include <sstream>


//OPE library
#include "sope_encode.h"
#include "ope.hh"
using namespace std;
#include <chrono>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/pem.h>

int do_evp_sign(FILE * rsa_pkey_file, FILE * in_file)
{
    OpenSSL_add_all_digests();
    ERR_load_crypto_strings();

    EVP_PKEY * pkey = PEM_read_PrivateKey(rsa_pkey_file, NULL, NULL, NULL);
    if (pkey == NULL) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    fseek(in_file, 0, SEEK_END);
    const size_t lSize = ftell(in_file);
    rewind(in_file);

    EVP_MD_CTX md_ctx;
    EVP_SignInit(&md_ctx, EVP_sha1());

    size_t len;
    unsigned char buffer[4096];

    size_t bytesLeft = lSize;
    while (bytesLeft > 0) {
        const size_t count = (bytesLeft > sizeof(buffer) ? sizeof(buffer) : bytesLeft);
        len = fread(buffer, 1, count, in_file);
        if (len != count) {
            fprintf(stderr, "Read error! len (%u) != count (%u).\n", len, count);
            EVP_PKEY_free(pkey);
            return 1;
        }
        if (!EVP_SignUpdate(&md_ctx, buffer, len))
        {
            ERR_print_errors_fp(stderr);
            EVP_PKEY_free(pkey);
            return 1;
        }
        bytesLeft -= len;
    }

    unsigned int sig_len;
    unsigned char * sig = (unsigned char *)malloc(EVP_PKEY_size(pkey));

    if (!sig) {
        fprintf(stderr, "Couldn't allocate %u bytes of memory.\n", EVP_PKEY_size(pkey));
        EVP_PKEY_free(pkey);
        return 1;
    }

    //run 1000 times to see the time

    //for (int x  = 0; x < 1000; x++){
    if (!EVP_SignFinal(&md_ctx, sig, &sig_len, pkey))
    {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(pkey);
        free(sig);
        return 1;
    }
    //}

    /*
    FILE * fTemp = fopen("out.sig", "wb");
    if (fTemp) {
        printf("Writing the signature to a file. Number of bytes: %u.\n", sig_len);
        fwrite(sig, 1, sig_len, fTemp);
        fclose(fTemp);
    }

    printf("Signature: \n");
    for (unsigned int i = 0; i != sig_len; ++i)
    {
        printf("%02x", sig[i]);
        if (i % 16 == 15)
            printf("\n");
    }
    printf("\n");
    */
    //printf("free!!!\n");
    EVP_PKEY_free(pkey);
    free(sig);

    return 0;
}




int main(int argc, char** argv) {
    char const* str1_user = "Oh, hello world.";
    char const* str1_id = "Hello world!!";

// edit distance time

    //stringstream_tuple_output<unit_cost, char const*> out;
    //unsigned dist = edit_distance(str1_user, str1_id, _script = out);
    //std::cout << "dist= " << dist << "   edit operations= " << out.ss.str() << "\n";
    auto t1 = chrono::high_resolution_clock::now();
    for (int x = 0; x < 1000; x++){
        unsigned dist = edit_distance(str1_user, str1_id);
    }
    //cout << "dist=" << dist << endl;
    auto t2 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    cout << "Edit Distance:" << duration1 << endl;


    // plaintext range's length in bits (plaintexts are in [0, 2**P-1]
    unsigned int P = 32;
    // ciphertext range's length in bits (ciphertexts are in [0, 2**C-1]
    unsigned int C = 64;

    //OPE o("S0M3 $TR@NG Key", P, C);
    OPE o("S0M3$TR@NGKeySSSSSSSSSSSSSSSS", P, C);


    // it works with ZZ instead of usual integers
    NTL::ZZ m1 = NTL::to_ZZ(13);
    NTL::ZZ m2 = NTL::to_ZZ(50);

    auto t3 = chrono::high_resolution_clock::now();
    for (int x  = 0; x < 1000; x++){
        NTL::ZZ c1 = o.encrypt(m1);
    }

    //NTL::ZZ c2 = o.encrypt(m2);

    //cout << "m1 = " << m1 << endl;
    //cout << "m2 = " << m2 << endl;
    //cout << "enc(m1) = " << c1 << endl;
    //cout << "enc(m2) = " << c2 << endl;
    auto t4 = chrono::high_resolution_clock::now();

    auto duration2 = chrono::duration_cast<chrono::microseconds>(t4 - t3).count();
    cout << "1000 times OPE:" << duration2 << endl;


//sha256 1000 times test
    auto t5 = chrono::high_resolution_clock::now();
    for (int x  = 0; x < 1000; x++){
        auto v = sha256::hash(str1_user);
    }
    auto t6 = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::microseconds>(t6 - t5).count();
    cout << "1000 SHA256:" << duration3 << endl;

    t1 = chrono::high_resolution_clock::now();
    for (int x = 0; x < 1000; x++){
        FILE *keyfile = fopen("/home/xt/mpkix-judgement/key-and-file/key", "r");
        FILE *contentfile = fopen("/home/xt/mpkix-judgement/key-and-file/info", "r");
        do_evp_sign(keyfile, contentfile);
    }

    t2 = chrono::high_resolution_clock::now();

    duration1 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    cout << "PKI222:" << duration1 << endl;


    /*
    if (c1 < c2){
        cout << "Preserving the order!" << endl;
    }else{
        cout << "o.O ????? OPE not working!" << endl;
    }

    NTL::ZZ dec_m1 = o.decrypt(c1);
    NTL::ZZ dec_m2 = o.decrypt(c2);

    if (m1 == dec_m1 && m2 == dec_m2){
        cout << "Decryption working fine." << endl;
    }else{
        cout << "Decryption NOT working." << endl;
    }
    */

    return 0;
}