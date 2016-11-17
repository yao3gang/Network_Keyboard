#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "types.h"
#include "ctimer.h"
#include "cthread.h"

#include "biz.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "net.h"

int main(void)
{
	struct sockaddr_in remote;
	int sock;
	int ret;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		ERR_PRINT("sock create failed\n");
		return 0;
	}

	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr("192.168.1.114");
	remote.sin_port = htons(8670);

	ret = connect(sock, (struct sockaddr *)&remote, sizeof(remote));
	if (ret)
	{
		ERR_PRINT("connect failed\n");
		return 0;
	}

	ret = SendTcpConnHead(sock, 0x1);
	if (ret)
	{
		ERR_PRINT("SendTcpConnHead failed\n");
		return 0;
	}

	//char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	ifly_cp_header_t cphead;

	memset(&cphead, 0, sizeof(ifly_cp_header_t));
	cphead.length = htonl(sizeof(cphead)+0);
	cphead.type = htons(CTRL_COMMAND);
	cphead.event = htons(CTRL_CMD_GETDEVICEINFO);
	cphead.number = htons(100);
	cphead.version = htons(CTRL_VERSION);

	//memcpy(sendbuf, &cphead, sizeof(cphead));

	ret = loopsend(sock, (char *)&cphead, sizeof(ifly_cp_header_t));
	if(ret)
	{
		ERR_PRINT("Send cmd failed\n");
		return 0;
	}

 	ifly_cp_header_t cprcvhead;
	ret = looprecv(sock, (char *)&cprcvhead, sizeof(ifly_cp_header_t));
	if (ret != SUCCESS)
	{
		ERR_PRINT("sock_cli recv ifly_cp_header_t failed, %s\n", strerror(errno));
		return 0;
	}
	
	cprcvhead.length	= ntohl(cprcvhead.length);
	cprcvhead.type		= ntohs(cprcvhead.type);
	cprcvhead.version	= ntohs(cprcvhead.version);
	cprcvhead.number	= ntohs(cprcvhead.number);
	cprcvhead.event		= ntohs(cprcvhead.event);

	printf("sock_cli recv: \n");
	printf("\ttype: %d\n", cprcvhead.type);
	printf("\tevent: %d\n", cprcvhead.event);
	printf("\tlength: %d\n", cprcvhead.length);

	int msglen;

	//有消息，目前只回应客户命令
	if(cprcvhead.length > sizeof(ifly_cp_header_t))
	{
		msglen = cprcvhead.length - sizeof(ifly_cp_header_t);

		ret = looprecv(sock, recvbuf, msglen);
		if (ret != SUCCESS)
		{
			DBG_PRINT("sock_cli recv msg failed\n");
			return 0;
		}

		if(cprcvhead.type == CTRL_ACK)
		{
			ifly_DeviceInfo_t dev_info;
			struct in_addr in;
			
			memcpy(&dev_info, recvbuf, sizeof(ifly_DeviceInfo_t));
			in.s_addr = dev_info.deviceIP;
			
			printf("device info: \n");
			printf("\tIP: %s\n", inet_ntoa(in));
			printf("\tPort: %d\n", ntohs(dev_info.devicePort));
			printf("\tdev type: %d\n", dev_info.nNVROrDecoder);
		}
	}

	close(sock);

	return 0;
}




