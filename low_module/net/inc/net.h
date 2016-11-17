#ifndef  __NET_H__
#define  __NET_H__

#ifdef __cplusplus
extern "C" {
#endif

//IP 地址比较，用于排序
int lessIP_u32(u32 ipa, u32 ipb);

VD_BOOL CheckNetLink();
int createMulticastSocket(u32 multicast_addr, u16 port);
int createServerSocket(u16 port, u32 listen_len);
int SetKeepAlive(int hSock);
int SetSockIOTimeout(int hSock, int seconds);


int looprecv(SOCKET s, char * buf, unsigned int rcvsize);
int loopsend(SOCKET s, char * buf, unsigned int sndsize);


//get
int GetHWAddr(char *pBuf);
u32 GetLocalIp();
u32 GetNetMask();
u32 GetDefaultGateway();


//set
int SetHWAddr(const char *pBuf);
int SetLocalIp(const u32 dwIp);
int SetNetMask(const u32 dwIp);
int SetDefaultGateway(const u32 dwIp);
int	SetDNSServer(const u32 dwIp);
int AddDNSServer(const u32 dwIp);

//sock 对应的网络通信是否有错误产生
VD_BOOL isSockIOErr(int sock);

//ntp 时间同步
int GetTimeFromNtpserver(const char *ntpserveradress, int timezoneoffset, int timeout, struct timeval *tv);


#ifdef __cplusplus
}
#endif

#endif

