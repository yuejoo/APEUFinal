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


int xflg,cflg;

void usage()
{
	printf("Usage: sish [ −x] [ -c command]\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int ch;
	while ((ch = getopt(argc, argv, "dehp:s:"))!= -1)
	{
		switch (ch)
		{
			case 'x':
				usage();
				break;
			case 'c':
				break;
			default:
				usage();
				return EXIT_SUCCESS;
		}
	}
	return 0;
}
