#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/route.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <netinet/tcp.h>
#include <netdb.h>




#include "types.h"
#include "ctrlprotocol.h"

#define	 ETH_NAME "eth0"

int lessIP_u32(u32 ipa, u32 ipb)
{
	return ntohl(ipa) < ntohl(ipb) ? 1:0;
}

int SetHWAddr(const char *pBuf)
{
	int sock = -1;
	//struct sockaddr_in sin;
	struct sockaddr sa;
	struct ifreq ifr;
	u8 mac[6];
	int i;
	const char *strptr = NULL;
	char tempstr[4] = {0};
	
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("SetHWAddr:socket");
		return -1;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
	
	strptr = pBuf;
	char *endptr = NULL;
	
	memset(mac,0,sizeof(mac));
	for(i=0;i<6;i++)
	{
		memset(tempstr,0,sizeof(tempstr));
		strncpy(tempstr, strptr, 2);
		mac[i] = strtoul(tempstr,&endptr,16);
		strptr += 3;
	}
	
#if 1//csp modify
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
	{
		perror("SetHWAddr:SIOCGIFFLAGS-1");
	}
	
	//system("ifconfig eth0 down");
	
	ifr.ifr_flags &= (~IFF_UP);
	if(ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
	{
		perror("SetHWAddr:SIOCSIFFLAGS-1");
	}
#endif
	
	if(ioctl(sock,SIOCGIFHWADDR,&ifr)<0)
	{
		perror("SetHWAddr:ioctl-1");
		close(sock);
		return 0;
	}
	
	memcpy(&sa,&ifr.ifr_addr,sizeof(sa));
	memcpy(sa.sa_data,mac,sizeof(mac));
	memcpy(&ifr.ifr_addr,&sa,sizeof(sa));
	//printf("sethwaddr : (%02x:%02x:%02x:%02x:%02x:%02x)\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	if(ioctl(sock,SIOCSIFHWADDR,&ifr)<0)
	{
		perror("SetHWAddr:ioctl-2");
		close(sock);
		return -1;
	}
	
#if 1//csp modify
	//system("ifconfig eth0 up");
	
	ifr.ifr_flags |= IFF_UP;
	if(ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
	{
		perror("SetHWAddr:SIOCSIFFLAGS-2");
	}
#endif
	
	close(sock);
	
	return 0;
}

int GetHWAddr(char *pBuf)
{
	int	sock;
	//struct sockaddr_in sin;
	struct sockaddr sa;
	struct ifreq ifr;
	u8 mac[6];
    
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return -1;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
    
	memset(mac,0,sizeof(mac));
	if(ioctl(sock,SIOCGIFHWADDR,&ifr)<0)
	{
		perror("GetHWAddr-ioctl");
		close(sock);
		return -1;
	}
    
	memcpy(&sa,&ifr.ifr_addr,sizeof(sa));
	memcpy(mac,sa.sa_data,sizeof(mac));
	fprintf(stdout,"%s   mac:  %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",ETH_NAME,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    sprintf(pBuf,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	
	close(sock);
	
	return 0;
}

int SetNetMask(const u32 dwIp)
{
	int	sock;
	//struct sockaddr_in sin;
	//struct sockaddr sa;
	struct ifreq ifr;
    
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return -1;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
	
	if(ioctl(sock,SIOCGIFNETMASK,&ifr)<0)
	{
		perror("SetNetMask:ioctl--1");
		close(sock);
		return -1;
	}
    
	((struct sockaddr_in*)(&ifr.ifr_netmask))->sin_addr.s_addr = dwIp;
	if(ioctl(sock,SIOCSIFNETMASK,&ifr)<0)
	{
		perror("SetNetMask:ioctl--2");
		close(sock);
		return -1;
	}
	
	close(sock);
	
	return 0;
}

u32 GetNetMask()
{
	int	sock;
	struct sockaddr_in sin;
	//struct sockaddr sa;
	struct ifreq ifr;
    
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return 0;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
    
	if(ioctl(sock,SIOCGIFNETMASK,&ifr)<0)
	{
		perror("GetNetMask-ioctl");
		close(sock);
		return 0;
	}
    
	memcpy(&sin,&ifr.ifr_netmask,sizeof(sin));
	fprintf(stdout,"%s   mask: %s\n",ETH_NAME,inet_ntoa(sin.sin_addr));
    
	close(sock);
	
	return sin.sin_addr.s_addr;
}

int SetLocalIp(const u32 dwIp)
{
	int	sock;
	struct ifreq ifr;
	
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return -1;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
    
	if(ioctl(sock,SIOCGIFADDR,&ifr)<0)
	{
		perror("SetLocalIp:ioctl--1");
		printf("SetLocalIp:ioctl:error:(%d,%s)\n",errno,strerror(errno));
		close(sock);
		return 0;
	}
	
	((struct sockaddr_in*)(&ifr.ifr_addr))->sin_addr.s_addr = dwIp;
	if(ioctl(sock,SIOCSIFADDR,&ifr)<0)
	{
		perror("SetLocalIp:ioctl--2");
		printf("SetLocalIp:ioctl:error:(%d,%s)\n",errno,strerror(errno));
		close(sock);
		return -1;
	}
	
	close(sock);
	
	return 0;
}

u32 GetLocalIp()
{
	int	sock;
	struct sockaddr_in sin;
	//struct sockaddr sa;
	struct ifreq ifr;
    
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return 0;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
    
	if(ioctl(sock,SIOCGIFADDR,&ifr)<0)
	{
		perror("GetLocalIp-ioctl");
		close(sock);
		return 0;
	}
    
	memcpy(&sin,&ifr.ifr_addr,sizeof(sin));
	//fprintf(stdout,"%s   ip:   %s\n",ETH_NAME,inet_ntoa(sin.sin_addr));
    
	close(sock);
	
	return sin.sin_addr.s_addr;
}

int SetDefaultGateway(const u32 dwIp)
{
	int err = 0;
#if 1
	int skfd;
	struct rtentry rt;
	//int err;
	
	skfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (skfd < 0)
		return -1;
	
	/* Delete existing defalt gateway */
	memset(&rt, 0, sizeof(rt));
	
	rt.rt_dst.sa_family = AF_INET;
	((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;
	
	rt.rt_genmask.sa_family = AF_INET;
	((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;
	
	rt.rt_flags = RTF_UP;
	
	err = ioctl(skfd, SIOCDELRT, &rt);
	if ((err == 0 || errno == ESRCH) && dwIp) {
		/* Set default gateway */
		memset(&rt, 0, sizeof(rt));
		
		rt.rt_dst.sa_family = AF_INET;
		((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;
		
		rt.rt_gateway.sa_family = AF_INET;
		((struct sockaddr_in *)&rt.rt_gateway)->sin_addr.s_addr = dwIp;
		
		rt.rt_genmask.sa_family = AF_INET;
		((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;
		
		rt.rt_flags = RTF_UP | RTF_GATEWAY;
		
		err = ioctl(skfd, SIOCADDRT, &rt);
	}
	
	close(skfd);
#endif

	return err;
}

u32 GetDefaultGateway()
{
	FILE *fp;
	char devname[64];//flags[16], sdest[16], sgw[16];
	unsigned long int d, g, m;
	int flgs, ref, use, metric, mtu, win, ir;
	struct in_addr sin;
	//struct in_addr mask;
	
	u32 dwGateway = 0;
	
	fp = fopen("/proc/net/route", "r");
	if(fp == NULL)
	{
		return dwGateway;
	}
	
	/* Skip the first line. */
	if(fscanf(fp, "%*[^\n]\n") < 0)
	{
		fclose(fp);
		return dwGateway;		/* Empty or missing line, or read error. */
	}
	while(1)
	{
		int r;
		r = fscanf(fp,"%63s%lx%lx%X%d%d%d%lx%d%d%d\n",
			devname, &d, &g, &flgs, &ref, &use, &metric, &m,
			&mtu, &win, &ir);
		if(r != 11)
		{
			/* EOF with no (nonspace) chars read. */
			if((r < 0) && feof(fp))
			{
				break;
			}
			else
			{
				continue;
			}
		}
		if(strcmp(devname,ETH_NAME) == 0 && d == 0 && (flgs&RTF_GATEWAY))
		{
			dwGateway = g;
			sin.s_addr = dwGateway;
			printf("default gateway:%s\n",inet_ntoa(sin));
			break;
		}
	}
	
	fclose(fp);
	
	printf("default gateway:0x%08x\n",dwGateway);
	
	return dwGateway;
}

int	SetDNSServer(const u32 dwIp)
{
	FILE *fp = NULL;
	struct in_addr sin;
	char context[64];
	
	fp = fopen("/etc/resolv.conf","w");
	if(fp == NULL)
	{
		return -1;
		//printf("-----SetDNSServer--error--\n");
	}
	sin.s_addr = dwIp;
	sprintf(context,"nameserver %s\n",inet_ntoa(sin));
	fwrite(context,strlen(context),1,fp);
	sprintf(context,"search localdomain");
	fwrite(context,strlen(context),1,fp);
	
	fclose(fp);
	//printf("-----SetDNSServer--OK--\n");
	return 0;
}

int AddDNSServer(const u32 dwIp)
{
	FILE *fp = NULL;
	struct in_addr sin;
	char context[64];
	
	fp = fopen("/etc/resolv.conf","a");
	if(fp == NULL)
	{
		return -1;
	}
	
	sin.s_addr = dwIp;
	sprintf(context,"\nnameserver %s",inet_ntoa(sin));
	fwrite(context,strlen(context),1,fp);
	
	fclose(fp);
	
	return 0;
}

VD_BOOL CheckNetLink()
{
    int s = -1;
	VD_BOOL bRet = FALSE;
	
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        perror("CheckNetLink-socket");
        return FALSE;
    }
	
	struct ifreq ifr;
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ-1] = 0;
	
	//printf("CheckNetLink interface:%s,IFNAMSIZ:%d\n",ifr.ifr_name,IFNAMSIZ);
	
	if(ioctl(s, SIOCGIFFLAGS, &ifr) < 0)
	{
		//system("ifconfig");
		perror("CheckNetLink-SIOCGIFFLAGS");
	}
	else
	{
		//system("ifconfig");
		//printf("flags=0x%x,IFF_UP=0x%x,IFF_RUNNING=0x%x,flags&IFF_UP=0x%x,flags&IFF_RUNNING=0x%x\n",
		//	ifr.ifr_flags,IFF_UP,IFF_RUNNING,ifr.ifr_flags&IFF_UP,ifr.ifr_flags&IFF_RUNNING);
		//printf("flags=0x%x\n", ifr.ifr_flags);
		if ((ifr.ifr_flags&IFF_UP) && (ifr.ifr_flags&IFF_RUNNING))
			bRet = TRUE;
	}
	
	close(s);
	
    return bRet;
}

int createMulticastSocket(u32 multicast_addr, u16 port)
{
	int serSocket = INVALID_SOCKET;
	struct sockaddr_in ser;
	
	memset(&ser,0,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = INADDR_ANY;
	
	serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serSocket < 0)
	{
		ERR_PRINT("create socket failed, %s\n", strerror(errno));
		return INVALID_SOCKET;
	}
	
	/************************************************************************/
	/*绑定socket                                                            */
	/************************************************************************/
	if(bind(serSocket,(struct sockaddr*)&ser,sizeof(ser)) == SOCKET_ERROR)
	{
		ERR_PRINT("bind failed, %s\n", strerror(errno));
		close(serSocket);
		return INVALID_SOCKET;
	}
	
	/************************************************************************/
	/*加入组播                                                */
	/************************************************************************/	
	struct ip_mreq mreq;
	memset(&mreq,0,sizeof(struct ip_mreq));
	mreq.imr_multiaddr.s_addr = multicast_addr;
	mreq.imr_interface.s_addr = INADDR_ANY;//GetLocalIp();//INADDR_ANY;
	
	if(setsockopt(serSocket,IPPROTO_IP,IP_ADD_MEMBERSHIP,(const char*)&mreq,sizeof(mreq)) == SOCKET_ERROR)
	{
		ERR_PRINT("IP_ADD_MEMBERSHIP failed, %s\n", strerror(errno));
		close(serSocket);
		return INVALID_SOCKET;
	}

	return serSocket;
}

int createServerSocket(u16 port, u32 listen_len)
{
	int serSocket = INVALID_SOCKET;
	struct sockaddr_in ser;
	
	memset(&ser,0,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = INADDR_ANY;
	
	serSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serSocket < 0)
	{
		ERR_PRINT("create socket failed, %s\n", strerror(errno));
		return INVALID_SOCKET;
	}
	
	/************************************************************************/
	/*绑定socket                                                            */
	/************************************************************************/
	if(bind(serSocket, (struct sockaddr*)&ser,sizeof(ser)) == SOCKET_ERROR)
	{
		ERR_PRINT("bind failed, %s\n", strerror(errno));
		close(serSocket);
		return INVALID_SOCKET;
	}

	if(listen(serSocket, listen_len) == SOCKET_ERROR)
	{
		ERR_PRINT("bind failed, %s\n", strerror(errno));
		close(serSocket);
		return INVALID_SOCKET;
	}

	return serSocket;
}

int looprecv(SOCKET s, char * buf, unsigned int rcvsize)
{
	u32 remian = rcvsize;
	u32 recvlen = 0;
	int ret = 0;
	
	while(remian > 0)
	{
		ret = recv(s, buf+recvlen, remian, 0);
		if(ret <= 0)
		{
			return -FAILURE;
		}
		
		recvlen += ret;
		remian -= ret;
	}	
	
	return SUCCESS;
}

int loopsend(SOCKET s, char * buf, unsigned int sndsize)
{
	int remian = sndsize;
	int sendlen = 0;
	int ret = 0;
	while(remian > 0)
	{
		ret=send(s, buf+sendlen, remian, 0);
		if(ret <= 0)
		{
			//DBG_PRINT("fd: %d, sndsize: %d, ret: %d, errno(%d, %s)\n", 
				//s, sndsize, ret, errno, strerror(errno));
			return -FAILURE;
		}
		
		sendlen += ret;
		remian -= ret;
	}
	
	return SUCCESS;
}

int SetKeepAlive(SOCKHANDLE hSock)
{
	////KeepAlive实现，单位秒  //zlb201109
	int keepAlive = 1;//设定KeepAlive  
	int keepIdle = 5;//开始首次KeepAlive探测前的TCP空闭时间  
	int keepInterval = 5;//两次KeepAlive探测间的时间间隔  
	int keepCount = 3;//判定断开前的KeepAlive探测次数  
	
	if (setsockopt(hSock,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1)
	{
		printf("hSock=%d, set SO_KEEPALIVE failed, errno=%d, errstr=%s\n", hSock, errno, strerror(errno));
		return -FAILURE;
	}
	if (setsockopt(hSock,SOL_TCP,TCP_KEEPIDLE,(void *)&keepIdle,sizeof(keepIdle)) == -1)
	{
		printf("hSock=%d, set TCP_KEEPIDLE failed\n", hSock);
		return -FAILURE; 
	}
	if (setsockopt(hSock,SOL_TCP,TCP_KEEPINTVL,(void *)&keepInterval,sizeof(keepInterval)) == -1)
	{
		printf("hSock=%d, set TCP_KEEPINTVL failed\n", hSock);
		return -FAILURE;
	}
	if (setsockopt(hSock,SOL_TCP,TCP_KEEPCNT,(void *)&keepCount,sizeof(keepCount)) == -1)
	{
		printf("hSock=%d, set TCP_KEEPCNT failed\n", hSock);
	 	return -FAILURE;
	}

	return SUCCESS;
}

int SetSockIOTimeout(int hSock, int seconds)
{
	int ret = SUCCESS;
	struct timeval timeo;

	timeo.tv_sec = seconds;
	timeo.tv_usec = 0;
	
	ret = setsockopt(hSock, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
	if(ret < 0)	
	{    	
		printf("SOCK-ERROR: set send timeout failed\n");
		return -FAILURE;
	}
	//set receive timeout    
	ret = setsockopt(hSock, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
	if(ret < 0)	
	{
		printf("SOCK-ERROR: set rece timeout failed\n");
		return -FAILURE;
	}

	return SUCCESS;
}

VD_BOOL isSockIOErr(int sock)
{
	int status = 0;
	socklen_t slen = sizeof(status);

	if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (void *)&status, &slen))
	{
		return TRUE;
	}

	if (status != 0)
	{
		DBG_PRINT("socker error(%d, %s)\n", status, strerror(status));
		return TRUE;
	}
	
	return FALSE;
}

//ntp 时间同步
#define NTP_PORT				123
#define TIME_PORT				37
//#define NTP_SERVER_IP			"202.120.2.101"
#define NTP_PORT_STR			"123"
#define NTPV1					"NTP/V1"
#define NTPV2					"NTP/V2"
#define NTPV3					"NTP/V3"
#define NTPV4					"NTP/V4"
#define TIME					"TIME/UDP"

#define NTP_PCK_LEN 48
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

#define JAN_1970				0x83aa7e80
#define NTPFRAC(x)				(4294 * (x) + ((1981 * (x)) >> 11))
#define USEC(x)					(((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

//int construct_packet(char *packet);
//int get_ntp_time(int sk, struct addrinfo *addr, struct ntp_packet *ret_time);
//int set_local_time(struct ntp_packet * pnew_time_packet);

typedef struct _ntp_time
{
	unsigned int coarse;
	unsigned int fine;
} ntp_time;

struct ntp_packet
{
	unsigned char leap_ver_mode;
	unsigned char startum;
	char poll;
	char precision;
	int root_delay;
	int root_dispersion;
	int reference_identifier;
	ntp_time reference_timestamp;
	ntp_time originage_timestamp;
	ntp_time receive_timestamp;
	ntp_time transmit_timestamp;
};

static char protocol[32] = {0};


int construct_packet(char *packet)
{
	char version = 1;
	long tmp_wrd;
	int port;
	time_t timer;
	
	strcpy(protocol, NTPV4);
	
	if(!strcmp(protocol, NTPV1)||!strcmp(protocol, NTPV2) || !strcmp(protocol, NTPV3) || !strcmp(protocol, NTPV4))
	{
		memset(packet, 0, NTP_PCK_LEN);
		port = NTP_PORT;
		version = protocol[5]-0x30;
		tmp_wrd = htonl((LI << 30)|(version << 27) |(MODE << 24)|(STRATUM << 16)|(POLL << 8)|(PREC & 0xff));
		memcpy(packet, &tmp_wrd, sizeof(tmp_wrd));
		tmp_wrd = htonl(1<<16);
		memcpy(&packet[4], &tmp_wrd, sizeof(tmp_wrd));
		memcpy(&packet[8], &tmp_wrd, sizeof(tmp_wrd));
		time(&timer);
		tmp_wrd = htonl(JAN_1970 + (long)timer);
		memcpy(&packet[40], &tmp_wrd, sizeof(tmp_wrd));
		tmp_wrd = htonl((long)NTPFRAC(timer));
		memcpy(&packet[44], &tmp_wrd, sizeof(tmp_wrd));
		return NTP_PCK_LEN;
	}
	else if(!strcmp(protocol, TIME))/* "TIME/UDP" */
	{
		port = TIME_PORT;
		memset(packet, 0, 4);
		return 4;
	}
	return 0;
} 

int get_ntp_time(int sk, struct addrinfo *addr, struct ntp_packet *ret_time, int timeout)
{
	fd_set pending_data;
	struct timeval block_time;
	char data[NTP_PCK_LEN * 8];
	int packet_len, data_len = addr->ai_addrlen, count = 0, result;
	
	if(!(packet_len = construct_packet(data)))
	{
		return 0;
	}
 	
	if((result = sendto(sk, data, packet_len, 0, addr->ai_addr, data_len)) < 0)
	{
		perror("sendto");
		return 0;
	}
	
	FD_ZERO(&pending_data);
	FD_SET(sk, &pending_data);
	block_time.tv_sec = (timeout <= 0) ? 10 : timeout;
	block_time.tv_usec = 0;
	if(select(sk + 1, &pending_data, NULL, NULL, &block_time) > 0)
	{
		if((count = recvfrom(sk, data, NTP_PCK_LEN * 8, 0, addr->ai_addr, (socklen_t*)&data_len)) < 0)
		{
			perror("recvfrom");
			return 0;
		}
		
		if(strcmp(protocol, TIME) == 0)
		{
			memcpy(&ret_time->transmit_timestamp, data, 4);
			return 1;
		}
		else if(count < NTP_PCK_LEN)
		{
			return 0;
		}
		
		ret_time->leap_ver_mode = ntohl(data[0]);
		ret_time->startum = ntohl(data[1]);
		ret_time->poll = ntohl(data[2]);
		ret_time->precision = ntohl(data[3]);
		ret_time->root_delay = ntohl(*(int*)&(data[4]));
		ret_time->root_dispersion = ntohl(*(int*)&(data[8]));
		ret_time->reference_identifier = ntohl(*(int*)&(data[12]));
		ret_time->reference_timestamp.coarse = ntohl(*(int*)&(data[16]));
		ret_time->reference_timestamp.fine = ntohl(*(int*)&(data[20]));
		ret_time->originage_timestamp.coarse = ntohl(*(int*)&(data[24]));
		ret_time->originage_timestamp.fine = ntohl(*(int*)&(data[28]));
		ret_time->receive_timestamp.coarse = ntohl(*(int*)&(data[32]));
		ret_time->receive_timestamp.fine = ntohl(*(int*)&(data[36]));
		ret_time->transmit_timestamp.coarse = ntohl(*(int*)&(data[40]));
		ret_time->transmit_timestamp.fine = ntohl(*(int*)&(data[44]));
		
		return 1;
	} /* end of if select */
	
	return 0;
} 

int set_local_time(struct ntp_packet *pnew_time_packet, int timezoneoffset)
{
	struct timeval tv;
	tv.tv_sec = pnew_time_packet->transmit_timestamp.coarse - JAN_1970 + timezoneoffset;
	tv.tv_usec = USEC(pnew_time_packet->transmit_timestamp.fine);
	return settimeofday(&tv, NULL);
}

int get_ntp_tv(struct ntp_packet *pnew_time_packet, int timezoneoffset, struct timeval *tv)
{
	if(tv == NULL)
	{
		return -1;
	}
	
	tv->tv_sec = pnew_time_packet->transmit_timestamp.coarse - JAN_1970 + timezoneoffset;
	tv->tv_usec = USEC(pnew_time_packet->transmit_timestamp.fine);
	
	return 0;
}


int GetTimeFromNtpserver(const char *ntpserveradress, int timezoneoffset, int timeout, struct timeval *tv)
{
	int sockfd, rc;
	struct addrinfo hints, *res = NULL;
	struct ntp_packet new_time_packet;
	
	if(tv == NULL)
	{
		return -1;
	}
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	rc = getaddrinfo(ntpserveradress, NTP_PORT_STR, &hints, &res);
	if(rc != 0)
	{
		perror("getaddrinfo");
		return -1;
	}
	
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}
	
	int ret = -1;
	
	if(get_ntp_time(sockfd, res, &new_time_packet, timeout))
	{
		//if(!set_local_time(&new_time_packet, timezoneoffset))
		if(!get_ntp_tv(&new_time_packet, timezoneoffset, tv))
		{
			printf("NTP client success!\n");
			ret = 0;
		}
	}
	
	close(sockfd);
	
	freeaddrinfo(res);
	
	return ret;
} 



