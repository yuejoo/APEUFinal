#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>       
#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#define BUF_SIZ 64
#define ENCRYPT 1
#define DECRYPT 0

unsigned char decryptedtext[BUF_SIZ];
unsigned char key[256];
unsigned char iv[128];
unsigned char buffer[BUF_SIZ];
unsigned char ciphertext[2*BUF_SIZ];



int dflg, eflg, hflg, pflg, sflg;
char* passphrase,* salt;

void usage()
{
	printf("Usage: aed [ −deh] −p passphrase −s salt\n");
	exit(EXIT_FAILURE);
}

void saltErr()
{
	printf("The salt needs to be exactly 8 hexadecimal characters.\n");
	exit(EXIT_FAILURE);
}

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
}

int genKey(unsigned char* passwd, unsigned char* salt){
	int len = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt , passwd,
			strlen((char*)passwd),1,key,iv);
	if(len <= 0)
		handleErrors();
	return len;
}



int ssl()
{
	memset(buffer, '\0', BUF_SIZ + 1);
	genKey(passphrase, salt);
	int len = 0;

	/* Initialise the library */
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();

	int type;
	if(dflg)
		type = DECRYPT;
	if(eflg)
		type = ENCRYPT;

	int rlen;
	unsigned char *plaintext = buffer;
	EVP_CIPHER_CTX *ctx;
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	while((rlen=read(STDIN_FILENO, buffer, BUF_SIZ)) != 0){

		plaintext = buffer;

		EVP_CIPHER_CTX_init(ctx);
		EVP_CIPHER_CTX_set_padding(ctx,type);

		if(EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv, type) != 1)
			handleErrors();

		if(EVP_CipherUpdate(ctx, ciphertext, &len, plaintext, rlen)!=1 )
			handleErrors();

		if(rlen == BUF_SIZ){
			if (write(STDOUT_FILENO, ciphertext, BUF_SIZ) != BUF_SIZ) {
				perror("Can not write to STDOUT");
				exit(EXIT_FAILURE);
			}
			EVP_CIPHER_CTX_cleanup(ctx);
		}
		memset(buffer, '\0', BUF_SIZ + 1);
	}
	int temp;
	if(EVP_CipherFinal_ex(ctx, ciphertext+len, &temp) == 0) 
		handleErrors();
	len = len + temp;	
	if (write(STDOUT_FILENO, ciphertext, len) != len) {
		perror("Can not write to STDOUT");
		exit(EXIT_FAILURE);
	}
	EVP_CIPHER_CTX_cleanup(ctx);

	return 0;
}

int checkIfHex(char* in){
	int i;
	for(i=0; i<strlen(in); i++){
		if(!isxdigit(in[i]))
			return 0;
	}
	return 1;
}


int main(int argc, char *argv[])
{
	int ch;
	while ((ch = getopt(argc, argv, "dehp:s:"))!= -1)
	{
		switch (ch)
		{
			case 'h':
				usage();
				break;
			case 's':
				salt = optarg;
				break;
			case 'd':
				dflg = 1;
				break;
			case 'e':
				eflg = 1;
				break;
			case 'p':
				passphrase = optarg;
				break;
			default:
				usage();
				return EXIT_SUCCESS;
		}
	}

	if(passphrase == NULL || salt == NULL){ 
		printf("Missing arguments of '-s' '-p'\n");
		usage();
	}

	if(strlen(salt)!=8 || !checkIfHex(salt))
		saltErr();

	ssl();

	return 0;
}
