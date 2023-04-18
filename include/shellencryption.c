#include <stdio.h>
#include <sodium.h>
#include <unistd.h>
#include "../include.h"


char* hash(const char *password, const char *hash_key)
{
    unsigned char hashed_password[crypto_generichash_BYTES];
    unsigned char key[crypto_generichash_KEYBYTES];
    char *hashed_password_hex = malloc(crypto_generichash_BYTES * 2 + 1);

    if (hashed_password_hex == NULL)
    {
        printf("Erreur lors de l'allocation de la mémoire pour le hash\n");
        exit(1);
    }

    if (sodium_init() < 0)
    {
        printf("Erreur lors de l'initialisation de libsodium\n");
        exit(1);
    }

    if (crypto_generichash(key, sizeof key, hash_key, my_strlen(hash_key), NULL, 0) != 0)
    {
        printf("Erreur lors de la génération de la clé de hashage\n");
        exit(1);
    }

    if (crypto_generichash(hashed_password, sizeof hashed_password, password, my_strlen(password), key, sizeof key) != 0)
    {
        printf("Erreur lors de la génération du hash de mot de passe\n");
        exit(1);
    }

    sodium_bin2hex(hashed_password_hex, crypto_generichash_BYTES * 2 + 1, hashed_password, sizeof hashed_password);

    return hashed_password_hex;
}

int encrypt(const char *target_file, const char *source_file, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE];
    unsigned char  buf_out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_t, *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    unsigned char  tag;

    fp_s = fopen(source_file, "rb");
    fp_t = fopen(target_file, "wb");
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
    fwrite(header, 1, sizeof header, fp_t);
    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen,
                                                   NULL, 0, tag);
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);
    fclose(fp_t);
    fclose(fp_s);
    return 0;
}

int decrypt(const char *target_file, const char *source_file, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  buf_out[CHUNK_SIZE];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_t, *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    int            ret = -1;
    unsigned char  tag;

    fp_s = fopen(source_file, "rb");
    fp_t = fopen(target_file, "wb");
    fread(header, 1, sizeof header, fp_s);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
        goto ret; /* incomplete header */
    }
    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
                                                       buf_in, rlen, NULL, 0) != 0) {
            goto ret; /* corrupted chunk */
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && ! eof) {
            goto ret; /* premature end (end of file reached before the end of the stream) */
        }
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);

    ret = 0;
    ret:
    fclose(fp_t);
    fclose(fp_s);
    return ret;
}