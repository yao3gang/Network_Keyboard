#include "types.h"
#include "object.h"
#include "singleton.h"
#include "c_lock.h"
#include "ctrlprotocol.h"
#include "cthread.h"
#include "ctimer.h"
#include "net.h"
#include "biz.h"
#include "biz_net.h"
#include "biz_config.h"
#include "ccircular_buffer_lockfree.h"



#define SEARCHPORT						6666
#define ACKSEARCHPORT					SEARCHPORT+1
#define MULTICASTGROUP					"224.0.1.2"


/*************************************************************************
模块功能:
1、网络依据配置参数初始化
2、响应CMS 设备网络连接及各种命令操作
*************************************************************************/
const u8 gc_protocolGUID[16] = {0x61,0x78,0xDA,0xB5,0xD3,0x8E,0x43,0xdb,0x9E,0xD7,0xF2,0x20,0x78,0x36,0x18,0x79};


#define g_biz_net_manager (*CBizNetManager::instance())

class CBizNetManager : public CObject
{
public:
	PATTERN_SINGLETON_DECLARE(CBizNetManager);
	~CBizNetManager();
	int Init(void);
	//给threadWriteFlash 发消息
	int WriteMsg2Thread(EM_MSG_TYPE msg_type);
	VD_BOOL isNetworkOk();
	int GetNetParam(SConfigNetParam &snet_param);
	int SetNetParam(SConfigNetParam &snet_param);
	
private:
	explicit CBizNetManager();
	explicit CBizNetManager(CBizNetManager &)
	{

	}
	void FreeSrc();//释放资源
	int networkInit(SConfigNetParam &snet_param);
	void networkUnInit();
	ifly_cp_t *AddCPLink(int hSock,u8 conntype, u32 ip, u16 port, u16 *pwErrorCode);
	void threadNetworkAck(uint param);//
	
private:
	VD_BOOL b_inited;
	VD_BOOL b_network_ok;
	C_Lock *plock4param;//mutex
	//ifly_DeviceInfo_t dev_info;
	Threadlet m_threadlet_netack;//网络回应线程
	int sock_search_svr;//组播回应
	int sock_search_ack;
	int sock_svr;
	ifly_cp_t *hCPLink;//[CTRL_PROTOCOL_MAXLINKNUM];
	CcircularBufferLockfree *pmsg_queue;
	SConfigNetParam cur_net_param;
	SConfigNetParam	new_net_param;
	
};

PATTERN_SINGLETON_IMPLEMENT(CBizNetManager);

CBizNetManager::CBizNetManager()
: b_inited(FALSE)
, b_network_ok(FALSE)
, plock4param(NULL)
, sock_search_svr(INVALID_SOCKET)
, sock_search_ack(INVALID_SOCKET)
, sock_svr(INVALID_SOCKET)
, hCPLink(NULL)
, pmsg_queue(NULL)
{
	//memset(&dev_info, 0, sizeof(ifly_DeviceInfo_t));
}

int CBizNetManager::networkInit(SConfigNetParam &snet_param)
{
	struct in_addr in;
	int i;

	if (cur_net_param.mac != snet_param.mac)
	{
		if(SetHWAddr(snet_param.mac.c_str()))
		{
			ERR_PRINT("SetHWAddr %s failed!\n", snet_param.mac.c_str());
			return -FAILURE;
		}
		DBG_PRINT("set mac: %s\n", snet_param.mac.c_str());
		
		cur_net_param.mac = snet_param.mac;
	}

	if (cur_net_param.nhost_ip != snet_param.nhost_ip)
	{
		in.s_addr = snet_param.nhost_ip;
		if(SetLocalIp(snet_param.nhost_ip))
		{
			ERR_PRINT("SetLocalIpV2 failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("set IP: %s\n", inet_ntoa(in));
		
		cur_net_param.nhost_ip = snet_param.nhost_ip;
	}

	//if (cur_net_param.nsubmask != snet_param.nsubmask)
	{
		in.s_addr = snet_param.nsubmask;
		if(SetNetMask(snet_param.nsubmask))
		{
			printf("SetNetMask failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("set submask: %s\n", inet_ntoa(in));

		cur_net_param.nsubmask = snet_param.nsubmask;
	}

	//if (cur_net_param.ngateway != snet_param.ngateway)
	{
		in.s_addr = snet_param.ngateway;
		if(SetDefaultGateway(snet_param.ngateway))
		{
			ERR_PRINT("SetDefaultGateway failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("set gateway: %s\n", inet_ntoa(in));

		cur_net_param.ngateway = snet_param.ngateway;
	}

	//if (cur_net_param.ndns1 != snet_param.ndns1)
	{
		in.s_addr = snet_param.ndns1;
		if(SetDNSServer(snet_param.ndns1))
		{
			ERR_PRINT("SetDNSServer failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("set dns1: %s\n", inet_ntoa(in));

		cur_net_param.ndns1 = snet_param.ndns1;
	}

	//if (cur_net_param.ndns2 != snet_param.ndns2)
	{
		in.s_addr = snet_param.ndns2;
		if(AddDNSServer(snet_param.ndns2))
		{
			ERR_PRINT("AddDNSServer failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("set dns2: %s\n", inet_ntoa(in));

		cur_net_param.ndns2 = snet_param.ndns2;
	}

	BizConfigSetNetParam(snet_param);
#if 0
	while (!CheckNetLink())//等待设置网络完成
	{
		sleep(1);
	}


	dev_info.deviceIP = snet_param.nhost_ip;
	dev_info.devicePort = htons(snet_param.nsvr_port);
	
	strcpy(dev_info.device_name, "NetworkKeyboard");
	dev_info.nNVROrDecoder = 4;
#endif

	//Multicast
	if (INVALID_SOCKET == sock_search_svr)
	{
		sock_search_svr = createMulticastSocket(inet_addr(MULTICASTGROUP), SEARCHPORT);
		if (INVALID_SOCKET == sock_search_svr)
		{
			ERR_PRINT("createMulticastSocket failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("createMulticastSocket success\n");
	}

	if (INVALID_SOCKET == sock_search_ack)
	{
		sock_search_ack = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(INVALID_SOCKET == sock_search_ack)
		{
			ERR_PRINT("create sock_search_ack failed\n");
			return -FAILURE;
		}
		DBG_PRINT("sock_search_ack success\n");
	}

	//create server listen socket  
	if (INVALID_SOCKET == sock_svr)
	{
		sock_svr = createServerSocket(snet_param.nsvr_port, CTRL_PROTOCOL_MAXLINKNUM+STREAM_LINK_MAXNUM);
		if (INVALID_SOCKET == sock_svr)
		{
			ERR_PRINT("createServerSocket failed!\n");
			return -FAILURE;
		}
		DBG_PRINT("sock_svr success\n");
	}

	if (hCPLink)
	{
		for(i=0;i<CTRL_PROTOCOL_MAXLINKNUM;i++)
		{
			hCPLink[i].sockfd = INVALID_SOCKET;
			CleanCPHandle(&hCPLink[i]);
		}
	}
	
	return SUCCESS;
}

void CBizNetManager::networkUnInit()
{
	int i;
	
	if (INVALID_SOCKET != sock_svr)
	{
		close(sock_svr);
		sock_svr = INVALID_SOCKET;
	}

	if(INVALID_SOCKET != sock_search_ack)
	{
		close(sock_search_ack);
		sock_search_ack = INVALID_SOCKET;
	}

	if (INVALID_SOCKET != sock_search_svr)
	{
		close(sock_search_svr);
		sock_search_svr = INVALID_SOCKET;
	}

	if (hCPLink)
	{
		for(i=0;i<CTRL_PROTOCOL_MAXLINKNUM;i++)
		{
			CleanCPHandle(&hCPLink[i]);
		}
	}
}
int CBizNetManager::Init()
{	
	//mutex
	plock4param = new CMutex;
	if (NULL == plock4param)
	{
		ERR_PRINT("new CMutex failed\n");
		goto fail;
	}

	if (plock4param->Create())//FALSE
	{
		ERR_PRINT("create CMutex failed\n");
		goto fail;
	}

	hCPLink = NULL;
	hCPLink = new ifly_cp_t[CTRL_PROTOCOL_MAXLINKNUM];
	if (NULL == hCPLink)
	{
		ERR_PRINT("new hCPLink failed!\n");
		goto fail;
	}

	//net init	
	if (BizConfigGetNetParam(new_net_param))
	{
		ERR_PRINT("BizConfigGetNetParam failed\n");
		return -FAILURE;
	}	

	if (networkInit(new_net_param))
	{
		ERR_PRINT("networkInit failed\n");
		goto fail;
	}

	sleep(3);//等待设置网络参数成功
	
	pmsg_queue = new CcircularBufferLockfree(1024);
	if (NULL == pmsg_queue)
	{
		ERR_PRINT("new CcircularBufferLockfree failed\n");
		goto fail;
	}
	
	if (pmsg_queue->CreateBuffer())
	{
		ERR_PRINT("CreateBuffer CcircularBufferLockfree failed\n");
		goto fail;
	}
	
	b_inited = TRUE;
	b_network_ok = TRUE;
	
	m_threadlet_netack.run("BizNetManager-ack", this, (ASYNPROC)&CBizNetManager::threadNetworkAck, 0, 0);

	return SUCCESS;
	
fail:
	FreeSrc();
	return -FAILURE;
}

void CBizNetManager::FreeSrc()
{
	b_inited = FALSE;

	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}
	
	if (pmsg_queue)
	{
		delete pmsg_queue;
		pmsg_queue = NULL;
	}

	networkUnInit();
	/*
	if (hCPLink)
	{
		networkUnInit();
		delete []hCPLink;
		hCPLink = NULL;
	}
	*/
}

CBizNetManager::~CBizNetManager()
{
	FreeSrc();
}

int CBizNetManager::GetNetParam(SConfigNetParam &snet_param)
{
	plock4param->Lock();
	
	if (!b_inited || !b_network_ok)
	{
		plock4param->Unlock();
		return -FAILURE;
	}

	snet_param = cur_net_param;
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizNetManager::SetNetParam(SConfigNetParam &snet_param)
{
	plock4param->Lock();
	
	if (!b_inited || !b_network_ok)
	{
		plock4param->Unlock();
		return -FAILURE;
	}

	new_net_param = snet_param;
	plock4param->Unlock();
 
	WriteMsg2Thread(EM_NET_PARAM_CHANGE);

	return SUCCESS;
}

//给threadWriteFlash 发消息
int CBizNetManager::WriteMsg2Thread(EM_MSG_TYPE msg_type)
{	
	if (!b_inited)
	{
		ERR_PRINT("CBizNetManager not init\n");
		return -FAILURE;
	}
	
	if (msg_type >=  EM_MSG_TYPE_MAX)
	{
		ERR_PRINT("msg_type(%d) >=  EM_MSG_TYPE_MAX(%d)\n", msg_type, EM_MSG_TYPE_MAX);
		return -FAILURE;
	}

	if (pmsg_queue->Put((uchar *)&msg_type, sizeof(EM_MSG_TYPE)))
	{
		ERR_PRINT("pmsg_queue Put failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;

}

CPHandle CBizNetManager::AddCPLink(int hSock,u8 conntype, u32 ip, u16 port, u16 *pwErrorCode)
{
	int i;
	for(i=0;i<CTRL_PROTOCOL_MAXLINKNUM;i++)
	{
		if(hCPLink[i].sockfd == INVALID_SOCKET)
		{						
			hCPLink[i].sockfd = hSock;
			hCPLink[i].conntype = conntype;
			hCPLink[i].ip = ip;
			hCPLink[i].port = port;
			hCPLink[i].newmsgcome = 0;
			hCPLink[i].nolnkcount = 0;
			memset(&hCPLink[i].guid,0,sizeof(hCPLink[i].guid));

			DBG_PRINT("i: %d\n", i);
			
			if(pwErrorCode) *pwErrorCode = CTRL_SUCCESS;
			return &hCPLink[i];
		}
	}
	
	if(pwErrorCode) 
		*pwErrorCode = CTRL_FAILED_LINKLIMIT;
	
	return NULL;
}


#define AckRcvBuf (4096)
void CBizNetManager::threadNetworkAck(uint param)
{
	DBG_PRINT("CBizNetManager::threadNetworkAck running\n");

	int ret = SUCCESS;
	int i;
	int fd_max = 0;
	fd_set rset;
	struct timeval timeout;
	struct sockaddr_in cli;
	socklen_t len = 0;
	char *rcv_buf = NULL;
	char *ack_buf = NULL;
	EM_MSG_TYPE msg_type;
	int state = 0;
	VD_BOOL b_netlinkstate = FALSE;
	int acklen = 0;
	u16 ackret = 0;
	int sock_cli = INVALID_SOCKET;
	ifly_cp_header_t cpsndhead;
	ifly_cp_header_t cprcvhead;
	int msglen = 0;
	SConfigNetParam snet_param;
	
	
	rcv_buf = new char[AckRcvBuf];
	if (NULL == rcv_buf)
	{
		ERR_PRINT("new recv buffer failed\n");
		goto fail;
	}

	ack_buf = new char[AckRcvBuf];
	if (NULL == ack_buf)
	{
		ERR_PRINT("new ack buffer failed\n");
		goto fail;
	}
	
	while (1)
	{	
		b_netlinkstate = CheckNetLink();
		
		ret = pmsg_queue->Get((uchar *)&msg_type, sizeof(EM_MSG_TYPE));		
		if (ret != SUCCESS)
		{
			if (ret == -EDATA)
			{
				//ERR_PRINT("msg_queue is not enough valid data\n");
			}
			else
			{
				ERR_PRINT("msg_queue get data fail, ret: %d\n", ret);
				break;
			}
		}
		else	//success
		{
			//升级的时候依然可以接收网络命令
			switch (msg_type)
			{
				case EM_NET_PARAM_CHANGE:
					//网络参数改变，触发一次网络异常
					state = 0;
					b_netlinkstate = FALSE;
					
					DBG_PRINT("net param change\n");
					break;
				default:
					ERR_PRINT("msg type%d, module not support*****************\n", msg_type);
			}
		}

		//网络状态机，检查网络是否正常
		//DBG_PRINT("state: %d, b_netlinkstate: %d\n", state, b_netlinkstate);
		switch (state)
		{
			case 0:	//发现异常则去初始化
				if (!b_netlinkstate)
				{
					plock4param->Lock();
					b_network_ok = FALSE;
					snet_param = new_net_param;
					plock4param->Unlock();
					
					networkUnInit();
					state = 1;					
				}
				break;
			case 1:	//等待网络正常
				if (b_netlinkstate)	//网络正常
				{
					if (networkInit(snet_param))	//初始化失败
					{
						networkUnInit();
					}
					else	//初始化成功
					{
						state = 0;

						plock4param->Lock();
						b_network_ok = TRUE;
						plock4param->Unlock();
					}
				}
				break;
			default:
				networkUnInit();
				state = 1;
				
				ERR_PRINT("state%d, module not support*****************\n", state);				
		}

		if (state)//当前网络不正常
		{
			sleep(1);
			continue;
		}

		//数据都在本线程维护，无需锁
		//plock4param->Lock();
		
		FD_ZERO(&rset);
		
		FD_SET(sock_search_svr ,&rset);
		FD_SET(sock_svr ,&rset);

		fd_max = MAX(sock_search_svr, sock_svr);
		//DBG_PRINT("fd_max 1: %d\n", fd_max);
		
		for (i=0; i<CTRL_PROTOCOL_MAXLINKNUM; i++)
		{
			if (INVALID_SOCKET != hCPLink[i].sockfd)
			{
				//cli.sin_addr.s_addr = hCPLink[i].ip;
				//DBG_PRINT("recv cli: %s\n", inet_ntoa(cli.sin_addr));
				
				FD_SET(hCPLink[i].sockfd, &rset);
				fd_max = MAX(hCPLink[i].sockfd, fd_max);
			}
		}

		//DBG_PRINT("fd_max 2: %d\n", fd_max);
		//plock4param->Unlock();

		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
	
		ret = select(fd_max+1, &rset, NULL, NULL, &timeout);
		if (ret < 0)	//error
		{
			ERR_PRINT("select failed: %s, sleep 1s !!!\n", strerror(errno));
			sleep(1);
			continue;
		}
		else if (ret == 0)	//timeout
		{
			continue;
		}

		//hCPLink[CTRL_PROTOCOL_MAXLINKNUM].fd
		for(i=0; i<CTRL_PROTOCOL_MAXLINKNUM; i++)
		{
			sock_cli = hCPLink[i].sockfd;

			if ((INVALID_SOCKET != sock_cli) 
				&& (FD_ISSET(sock_cli, &rset)))
			{
				//cli.sin_addr.s_addr = hCPLink[i].ip;
				//DBG_PRINT("recv cli: %s\n", inet_ntoa(cli.sin_addr));
				
				ret = looprecv(sock_cli, rcv_buf, sizeof(ifly_cp_header_t));
				if (ret != SUCCESS)
				{
					DBG_PRINT("sock_cli recv ifly_cp_header_t failed\n");
					CleanCPHandle(&hCPLink[i]);
					continue;
				}
				
				hCPLink[i].newmsgcome = 1;

				memcpy(&cprcvhead, rcv_buf, sizeof(cprcvhead));
				cprcvhead.length	= ntohl(cprcvhead.length);
				cprcvhead.type		= ntohs(cprcvhead.type);
				cprcvhead.version	= ntohs(cprcvhead.version);
				cprcvhead.number	= ntohs(cprcvhead.number);
				cprcvhead.event		= ntohs(cprcvhead.event);

				cli.sin_addr.s_addr = hCPLink[i].ip;
				printf("recv from client(%s) msg: \n", inet_ntoa(cli.sin_addr));
				printf("\ttype: %d\n", cprcvhead.type);
				printf("\tevent: %d\n", cprcvhead.event);
				printf("\tlength: %d\n", cprcvhead.length);
					
				if(cprcvhead.length < sizeof(ifly_cp_header_t) || cprcvhead.type > CTRL_ACK || cprcvhead.version != CTRL_VERSION)
				{
					DBG_PRINT("sock_cli ifly_cp_header_t param error, len: %d, type: %d, version: %d\n", 
						cprcvhead.length, cprcvhead.type, cprcvhead.version);
					continue;
				}

				//有消息，目前只回应客户命令
				if(cprcvhead.length > sizeof(ifly_cp_header_t))
				{
					if (cprcvhead.length > AckRcvBuf)//出错，超出缓存区
					{
						cli.sin_addr.s_addr = hCPLink[i].ip;
						ERR_PRINT("recv from client(%s) msg too long(%d)\n", 
							inet_ntoa(cli.sin_addr), cprcvhead.length);

						int data_size = cprcvhead.length > sizeof(ifly_cp_header_t);
						int rcv_len = AckRcvBuf - (int)sizeof(ifly_cp_header_t);
						while (data_size > 0)
						{
							ret = looprecv(sock_cli, rcv_buf+sizeof(ifly_cp_header_t), rcv_len);
							if (ret != SUCCESS)
							{
								DBG_PRINT("***Error*** OUTOFMEMORY looprecv error:[%d,%s]!\n", \
									errno, strerror(errno));
								break;
							}
							
							data_size -= rcv_len;

							rcv_len = data_size>(AckRcvBuf-(int)sizeof(ifly_cp_header_t)) ? (AckRcvBuf-(int)sizeof(ifly_cp_header_t)) : data_size;					
						}

						if (data_size == 0)//接收完成
						{
							DBG_PRINT("OUTOFMEMORY looprecv finish!\n");
						}
						else	//接收出错
						{
							CleanCPHandle(&hCPLink[i]);
							ERR_PRINT("***Error*** OUTOFMEMORY looprecv failed\n");
						}
						
						continue;
					}

					
					msglen = cprcvhead.length - sizeof(ifly_cp_header_t);

					ret = looprecv(sock_cli, rcv_buf+sizeof(ifly_cp_header_t), msglen);
					if (ret != SUCCESS)
					{
						DBG_PRINT("sock_cli recv msg failed\n");
						CleanCPHandle(&hCPLink[i]);
						continue;
					}
				}

				if(cprcvhead.type == CTRL_COMMAND)
				{
					//回应客户端命令
					ackret = BizDealClientCmd(&hCPLink[i], 
												cprcvhead.event, 
												rcv_buf+sizeof(ifly_cp_header_t), 
												cprcvhead.length-sizeof(ifly_cp_header_t),
												ack_buf+sizeof(ifly_cp_header_t),
												&acklen);
					
					cpsndhead.length	= ntohl(sizeof(ifly_cp_header_t)+acklen);
					cpsndhead.type		= ntohs(CTRL_ACK);
					cpsndhead.event		= ntohs(ackret);
					cpsndhead.number	= ntohs(cprcvhead.number);
					cpsndhead.version	= ntohs(cprcvhead.version);
					memcpy(ack_buf, &cpsndhead, sizeof(ifly_cp_header_t));
					
					ret = loopsend(sock_cli, (char *)ack_buf, ntohl(cpsndhead.length));
					if (ret != SUCCESS)
					{
						DBG_PRINT("sock_cli send ack failed\n");
						CleanCPHandle(&hCPLink[i]);
						continue;
					}
				}			
			}
		}

		//sock_search_svr  接收组播包，当前只支持搜索包
		if ((INVALID_SOCKET != sock_search_svr) 
			&& (FD_ISSET(sock_search_svr, &rset)))
		{				
			memset(&cli, 0, sizeof(cli));
			len = sizeof(struct sockaddr_in);
			
			ret = recvfrom(sock_search_svr, rcv_buf, AckRcvBuf, 0, (struct sockaddr*)&cli, &len);
			if (ret < 0)
			{
				ERR_PRINT("sock_search_svr recvfrom failed\n");
			}
			else
			{
				ifly_ProtocolHead_t prohdr;
				u16 ackport = 0;
				
				if(ret < (int)sizeof(ifly_ProtocolHead_t))
				{
					ERR_PRINT("Multicast package len: %d, error\n", ret);
					continue;
				}

				memcpy(&prohdr, rcv_buf, sizeof(ifly_ProtocolHead_t));

				if(memcmp(prohdr.safeguid, gc_protocolGUID, 16))
				{
					ERR_PRINT("protocolGUID error\n");
					continue;
				}

				if(prohdr.byConnType == 0x3)	//0x3：广播搜索
				{
					ifly_DeviceInfo_t dev_info;
					
					if (BizConfigGetDevInfo(&dev_info))
					{
						ERR_PRINT("BizConfigGetDevInfo failed\n");
						continue;
					}
					//仅仅转换端口
					dev_info.devicePort = htons(dev_info.devicePort);
					
					//解析端口
					memcpy(&ackport, prohdr.reserved, sizeof(u16));
					ackport = ntohs(ackport);
					
					if(0 == ackport)//兼容当前固定端口
					{
						ackport = ACKSEARCHPORT;
					}
					
					struct sockaddr_in rem;
					memset(&rem, 0, sizeof(rem));
					rem.sin_family = AF_INET;
					rem.sin_port = htons(ackport);
					rem.sin_addr.s_addr = cli.sin_addr.s_addr;

					sendto(sock_search_ack, (char *)&dev_info, sizeof(dev_info), 0, (struct sockaddr*)&rem, sizeof(rem));					
				}
			}
		}
		
		//sock_svr  接受客户端连接
		if ((INVALID_SOCKET != sock_svr) 
			&& (FD_ISSET(sock_svr, &rset)))
		{
			memset(&cli, 0, sizeof(cli));
			len = sizeof(struct sockaddr_in);

			sock_cli = accept(sock_svr,(struct sockaddr *)&cli, &len);
			if (sock_cli < 0)
			{
				DBG_PRINT("sock_svr accept failed\n");
				continue;
			}

			if (SetSockLinkOpt(sock_cli))
			{
				close(sock_cli);
				ERR_PRINT("new client SetSockLinkOpt failed\n");
				continue;
			}

			int conntype = CheckTcpConnType(sock_cli);
			DBG_PRINT("new client conntype: %d\n", conntype);

			if (1 == conntype)	//cmdlink
			{
				if (SetKeepAlive(sock_cli))
				{
					close(sock_cli);
					ERR_PRINT("SetKeepAlive failed\n");
					continue;
				}

				if (SetSockIOTimeout(sock_cli, CTRL_SOCKET_IO_TIMEOUT))
				{
					close(sock_cli);
					ERR_PRINT("SetSockIOTimeout failed\n");
					continue;
				}
				
				if(NULL == AddCPLink(sock_cli, CTRL_CONNECTION_TCPSERVER, cli.sin_addr.s_addr, ntohs(cli.sin_port), NULL))
				{
					close(sock_cli);
					ERR_PRINT("new client AddCPLink failed\n");
				}
				else	//success
				{
					DBG_PRINT("new client accept cmdlink success, connectin from(%s,%d), sockfd=%d\n", 
						inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), sock_cli);
				}
			}
			else if (2 == conntype)
			{
				DBG_PRINT("new client accept medialink success, connectin from(%s,%d),sockfd=%d\n", 
					inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), sock_cli);
				
				ifly_TCP_Stream_Req reqhdr;
				ret = looprecv(sock_cli, (char *)&reqhdr, sizeof(ifly_TCP_Stream_Req));
				if (ret != SUCCESS)
				{
					close(sock_cli);
					DBG_PRINT("sock_cli recv ifly_TCP_Stream_Req failed\n");
					continue;
				}
				
				ret = BizDealClientDataLink(sock_cli, &reqhdr, &cli);
				if (ret != SUCCESS)
				{
					close(sock_cli);
					DBG_PRINT("BizDealClientDataLink failed\n");
				}
			}
			else
			{
				close(sock_cli);
				DBG_PRINT("connect type: %d, device not support\n", conntype);
			}
		}		
	}

	ERR_PRINT("thread exit, inconceivable\n");
	
fail:
	if (ack_buf)
	{
		delete []ack_buf;
		ack_buf = NULL;
	}
	
	if (rcv_buf)
	{
		delete []rcv_buf;
		rcv_buf = NULL;
	}
}

VD_BOOL CBizNetManager::isNetworkOk()
{
	VD_BOOL b;
	
	plock4param->Lock();
	b = b_network_ok;
	plock4param->Unlock();

	return b;
}

//外部接口
int BizNetInit(void)
{
	return g_biz_net_manager.Init();
}

VD_BOOL BizisNetworkOk(void)
{
	return g_biz_net_manager.isNetworkOk();
}

int BizNetGetNetParam(SConfigNetParam &snet_param)
{
	return g_biz_net_manager.GetNetParam(snet_param);
}

int BizNetSetNetParam(SConfigNetParam &snet_param)
{
	return g_biz_net_manager.SetNetParam(snet_param);
}


