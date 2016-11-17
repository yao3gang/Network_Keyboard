#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "types.h"
#include "ctrlprotocol.h"

int main()
{
	printf("sizeof(ifly_DeviceInfo_t): %d\n", sizeof(ifly_DeviceInfo_t));

	return 0;
}



