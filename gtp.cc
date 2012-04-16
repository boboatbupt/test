#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sys/timeb.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <syslog.h>
using namespace std;
#define FIFO_NAME_ONE "/tmp/my_fifo"
#define FIFO_NAME_TWO "/tmp/my_fifo1"
#define FIFO_NAME_THREE "/tmp/my_fifo2"
#define FIFO_NAME_FOUR "/tmp/my_fifo3"
#define FIFO_NAME_FIVE "/tmp/my_fifo4"
#define BUFFER_SIZE 2048 
//#include "http.cc"
#include "full_main_test.cpp"
//#include "full_main_test1.cpp"
using namespace std;
long sum=0;
long sum1=0;
long sum2=0;
long call=0;
long call1wap=0;
long call1http=0;
long call2wap=0;
long call2http=0;
long read1=0;
long read2=0;
long write1=0;
long write2=0;
long reloop=4;
int  insertnum=0;
int shmid;
time_t start,end;
int pipe_fd[4];
int pipe_fd_c;
pid_t pid[4];
pid_t mainpid;
int killid=0;
int killid1=0;
VOID ProcessPackets(TC_PACKETS_BUFFER buffer);

void handler_sigusr1(int signo);

void daemonize(const char *cmd);

int main(int argc, char* argv[])
{
		/* 让进程变成daemon进程 */
		daemonize("main process");

        PTC_PORT        portList;
        ULONG           numPorts;
        TC_STATUS       status;
        ULONG           i;
        ULONG           numPort;
        TC_PORT port;
        TC_INSTANCE instance;
        UNUSED(argc);
        UNUSED(argv);
	char *addr;
        int error1,error2;
        start = time(NULL);
        mkdirTime_1.tv_sec=0;
        mkdirTime_2.tv_sec=0;
        mkdirTime_3.tv_sec=0;
        mkdirTime_4.tv_sec=0;
    	gettimeofday(&firstTime, NULL);
    	gettimeofday(&firstTime2, NULL);
    	gettimeofday(&firstTime3, NULL);
    	gettimeofday(&firstTime4, NULL);
    	gettimeofday(&mainfirstTime, NULL);
	gettimeofday(&mainfirstTime_1, NULL);
/*	
	if(MainMap.empty())  MainMap.clear();
	if(MainTEID.empty())  MainTEID.clear();
	if(CallIDMap.empty()) CallIDMap.clear();
	if(InfoMap.empty()) InfoMap.clear();
	if(CallIDMap2.empty()) CallIDMap2.clear();
	if(InfoMap2.empty()) InfoMap2.clear();
	if(RTHttp_1.empty())  RTHttp_1.clear();
	if(RTHttpPage_1.empty())  RTHttpPage_1.clear();
	if(RTPDPLinks_1.empty())  RTPDPLinks_1.clear();
	if(RTHttp_2.empty())  RTHttp_2.clear();
	if(RTHttpPage_2.empty())  RTHttpPage_2.clear();
	if(RTPDPLinks_2.empty())  RTPDPLinks_2.clear();
	if(Sta_map.empty())  Sta_map.clear();
	if(WAP_SDR_map_1.empty())  WAP_SDR_map_1.clear();
	if(Sta_map2.empty())  Sta_map2.clear();
	if(WAP_SDR_map_2.empty())  WAP_SDR_map_2.clear();
	if(MMS_SDR_map_1.empty())  MMS_SDR_map_1.clear();
	if(MMS_SDR_map_2.empty())  MMS_SDR_map_2.clear();
*/
	unlink(FIFO_NAME_ONE); 
	unlink(FIFO_NAME_TWO); 
	unlink(FIFO_NAME_THREE); 
	unlink(FIFO_NAME_FOUR); 
	unlink(FIFO_NAME_FIVE); 
int res;
 if(access(FIFO_NAME_ONE,F_OK)==-1){
        res=mkfifo(FIFO_NAME_ONE,0777);
  }
 if(access(FIFO_NAME_TWO,F_OK)==-1){
        res=mkfifo(FIFO_NAME_TWO,0777);
  }
 if(access(FIFO_NAME_THREE,F_OK)==-1){
        res=mkfifo(FIFO_NAME_THREE,0777);
  }
 if(access(FIFO_NAME_FOUR,F_OK)==-1){
        res=mkfifo(FIFO_NAME_FOUR,0777);
  }
 if(access(FIFO_NAME_FIVE,F_OK)==-1){
        res=mkfifo(FIFO_NAME_FIVE,0777);
  }
pipe_fd[0]= open(FIFO_NAME_ONE,O_RDWR);
if(pipe_fd[0] ==-1)
        printf("pipe1 failed ...\n");
printf("pipe_fd1 is %d\n",pipe_fd[0]);
pipe_fd[1]= open(FIFO_NAME_TWO,O_RDWR);
if(pipe_fd[1] ==-1)
        printf("pipe2 failed ...\n");
printf("pipe_fd2 is %d\n",pipe_fd[1]);
pipe_fd[2]= open(FIFO_NAME_THREE,O_RDWR);
if(pipe_fd[2] ==-1)
        printf("pipe3 failed ...\n");
printf("pipe_fd3 is %d\n",pipe_fd[2]);
pipe_fd[3]= open(FIFO_NAME_FOUR,O_RDWR);
if(pipe_fd[3] ==-1)
        printf("pipe4 failed ...\n");
printf("pipe_fd4 is %d\n",pipe_fd[3]);
pipe_fd_c= open(FIFO_NAME_FIVE,O_RDWR|O_NONBLOCK,0);
if(pipe_fd_c ==-1)
        printf("pipe5 failed ...\n");
printf("pipe_fd5 is %d\n",pipe_fd_c);

// 向/var/run/gtp.pid中写入自己的pid
pid_t mainpid = getpid();
char strmainpid[10] = {0};
int runfd = open("/var/run/gtp.pid", O_CREAT|O_TRUNC|O_RDWR);
if (runfd == -1) {
	syslog(LOG_ERR, "write pid failed\n");
	exit(1);
}
sprintf(strmainpid, "%u", mainpid);
write(runfd, strmainpid, strlen(strmainpid));
syslog(LOG_ERR, "write pid %d OK\n", mainpid);
close(runfd);   

// 绑定信号监听函数
if (signal(SIGUSR1, handler_sigusr1) == SIG_ERR) {
	perror("signal");
	exit(1);
}

for(int i=0;i<4;i++)
{
if((pid[i]=fork())==0)
{
	int bindId;
       	cpu_set_t mask;
        bindId = i+1;
        CPU_ZERO(&mask);
        CPU_SET(bindId,&mask);
        if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
        {       printf("warning: could not set thread1 affinity, continuing...\n");}
		long num_start=getNumFromTime(i,NODE);	
			switch(i)
			{
				case 0:
					PageSSN_1 = num_start;
					HttpSSN_1 = num_start;
					tempPdpSSN1= num_start;
					tempCallID1= num_start;
					WAPSSN_1_1= num_start;
					WAPPAGESSN_1_1= num_start;
					MMS_SSN_1_1= num_start;
				case 1:
					PageSSN_2 = num_start;
					HttpSSN_2 = num_start;
					tempPdpSSN2= num_start;
					tempCallID2= num_start;
					WAPSSN_1_2= num_start;
					WAPPAGESSN_1_2= num_start;
					MMS_SSN_1_2= num_start;
				case 2:
					PageSSN_3 = num_start;
					HttpSSN_3 = num_start;
					tempPdpSSN3= num_start;
					tempCallID3= num_start;
					WAPSSN_2_1= num_start;
					WAPPAGESSN_2_1= num_start;
					MMS_SSN_2_1= num_start;
				case 3:
					PageSSN_4 = num_start;
					HttpSSN_4 = num_start;
					tempPdpSSN4= num_start;
					tempCallID4= num_start;
					WAPSSN_2_2= num_start;
					WAPPAGESSN_2_2= num_start;
					MMS_SSN_2_2= num_start;
				default: break;
			}
      	u_char buffer[BUFFER_SIZE+1];
   	if(pipe_fd[i]!=-1)
	{
                printf("child %d is handling ...\n",i);
        	do{
               		memset(buffer,0,BUFFER_SIZE+1);
		 	do{
				res = read(pipe_fd[i],(char*)buffer,BUFFER_SIZE);
			}while(res<=0);
			switch(i)
			{
				case 0:
					GtpPacketHandle_1(buffer);break;
				case 1:
					GtpPacketHandle_2(buffer);break;
				case 2:
					GtpPacketHandle_3(buffer);break;
				case 3:
					GtpPacketHandle_4(buffer);break;
				default: break;
			}
        	}while(res>0);
  	}
	printf("child  go out\n");
 	exit(0);
}
}
if((mainpid=fork())==0)
{
	
        int bindId;
        cpu_set_t mask;
        bindId = 0;
        CPU_ZERO(&mask);
        CPU_SET(bindId,&mask);
        if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
        {        printf("warning: could not set main thread affinity, continuing...\n");}

        status = TcQueryPortList(&portList, &numPorts);

        if (status != TC_SUCCESS)
        {
                printf("Cannot retrieve port list, %s\n", TcStatusGetString(status));
                return -1;
        }

        if (numPorts == 0)
	{
                printf("No ports available.\n");
                return 0;
        }
/*
        do
        {
                for (i = 0; i < numPorts; i++)
                {
                        printf("%u) %s (%s)\n", i, TcPortGetDescription(portList[i]), TcPortGetName(portList[i]));
                }

                printf("\nPort number? ");
        }
        while((scanf_s("%u", &numPort) == 0 ) || (numPort >= numPorts) );
*/
        port = portList[2];
//	printf("You have select the Port %u\n",numPort);

        status = TcInstanceOpenByPort(port, &instance);

        TcFreePortList(portList);

        if (status != TC_SUCCESS)
        {
                printf("Cannot open port, %08x\n", status);
                return -1;
        }

        //
        // Enable reception
        //
        status = TcInstanceSetFeature(instance, TC_INST_FT_RX_STATUS, 1);
        if (status != TC_SUCCESS)
        {
                printf("Error, cannot enable reception: %s (%08x)", TcStatusGetString(status), status);

                (VOID)TcInstanceClose(instance);
        return -1;
        }
       	ULONG bufferLength;
        ULONG bufferEffectiveLength;
 	do
        {
                TC_PACKETS_BUFFER hPacketsBuffer;
                status = TcInstanceReceivePackets(instance, &hPacketsBuffer);
                if (status != TC_SUCCESS) break;
                if (hPacketsBuffer == NULL) 
		{
	//		printf("there is no packet,please check your network\n");
			continue;
		}
                ProcessPackets(hPacketsBuffer);
                TcPacketsBufferDestroy(hPacketsBuffer);
        }while(TRUE);
        TcInstanceClose(instance);
        return 0;
}

//	if(maintimeTemp.tv_sec-mainfirstTime.tv_sec>=60)
	do{
		//
		int switch_time = 60 * 60 * 2; // 60s x 60min * 4hr
		sleep(switch_time);
		// 使用sigsuspend等待脚本发送杀死进程的信号
		/*
		sigset_t zeromask;
		sigemptyset(&zeromask);

		if (sigsuspend(&zeromask) != -1) {
			perror("sigsuspend");
			exit(1);
		}
		*/
		int res=0;
		killid=(killid+1)%2;
		char buf[4];
		buf[0]=killid;
		res=write(pipe_fd_c,buf,8);	
		//printf("killid has been written into the pipe %d\n",buf[0]);
		

		for(int i=0;i<2;i++)
		{
		int stat;	
		kill(pid[killid*2+i], SIGKILL);
		wait(&stat);
		if(WIFSIGNALED(stat)){printf("Child process received signal (%d).\n", WTERMSIG(stat));}
	//	++reloop;
		long num=getNumFromTime(killid*2+i,NODE);
		switch(killid*2+i)
		{
			case 0: PageSSN_1 = num;
				HttpSSN_1 = num;
				tempPdpSSN1= num;
				tempCallID1= num;
				WAPSSN_1_1= num;
				WAPPAGESSN_1_1= num;
				MMS_SSN_1_1= num;
    				gettimeofday(&firstTime, NULL);
				break;
			case 1: PageSSN_2 = num;
                                HttpSSN_2 = num;
				tempPdpSSN2= num;
				tempCallID2= num;
				WAPSSN_1_2= num;
                                WAPPAGESSN_1_2=num;
                                MMS_SSN_1_2= num;
    				gettimeofday(&firstTime2, NULL);
				break;
			case 2: PageSSN_3 = num;
                                HttpSSN_3 = num;
				tempPdpSSN3= num;
				tempCallID3= num;
				WAPSSN_2_1= num;
                                WAPPAGESSN_2_1= num;
                                MMS_SSN_2_1=num;
    				gettimeofday(&firstTime3, NULL);
				break;
			case 3: PageSSN_4 = num;
                                HttpSSN_4 = num;
				tempPdpSSN4=num;
				tempCallID4=num;
				WAPSSN_2_2=num;
                                WAPPAGESSN_2_2=num;
                              //  MMS_SSN_2_2=1000000000*reloop;
                                MMS_SSN_2_2=num;
    				gettimeofday(&firstTime4, NULL);
				break;
			default: break;
		}
		pid[killid*2+i]=fork();
		if(pid[killid*2+i]<0)
		{
			perror("fork");
		}
		else if(pid[killid*2+i]==0)
		{
			//printf("new fork  is running\n");
        		int bindId;
       			cpu_set_t mask;
        		bindId = i+1;
        		CPU_ZERO(&mask);
        		CPU_SET(bindId,&mask);
        		if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
        		{        printf("warning: could not set thread1 affinity, continuing...\n");}
      			u_char buffer[BUFFER_SIZE+1];
   			if(pipe_fd[killid*2+i]!=-1)
			{
                		//printf("child1 is handling ...\n");
        			do{
               				memset(buffer,0,BUFFER_SIZE+1);
                			do{
						res = read(pipe_fd[killid*2+i],(char*)buffer,BUFFER_SIZE);
                			}while(res<=0);
					switch(killid*2+i)
					{
						case 0:
                        				GtpPacketHandle_1(buffer);
							break;
						case 1:
                        				GtpPacketHandle_2(buffer);
							break;
						case 2:
                        				GtpPacketHandle_3(buffer);
							break;
						case 3:
                        				GtpPacketHandle_4(buffer);
							break;
						default:
							break;
					}
        			}while(res>0);
  			}
   			exit(0);
		}
		}
	}while(true);
}

VOID ProcessPackets(TC_PACKETS_BUFFER buffer)
{
        TC_PACKET_HEADER header;
        PVOID packet;
	u_char *packet_content=NULL;
	do
        {
                if (TcPacketsBufferQueryNextPacket(buffer, &header, &packet) != TC_SUCCESS)
                        break;
/*
		if(((++sum)%50000000)==0)
		{
                        printf("the packet number is %ld\n",sum);
		}
  */
				//      if(sum==2000000)
          //     {
          //              end = time(NULL);
          //              printf("The pause used %f seconds.\n",difftime(end,start));

          //      }
	//	printf("the number is %ld\n",sum);
		packet_content=(u_char*)(packet);		
		
		PacketHandle(packet_content);	
        }while(TRUE);
}
void PacketHandle(u_char *packet_content)
{
    struct udp_header *udp_protocol;
    u_short source_port;
    u_short destination_port;
    u_short length;
    udp_protocol = (struct udp_header*)(packet_content+ 14+20);
    source_port = ntohs(udp_protocol->udp_source_port);
    destination_port = ntohs(udp_protocol->udp_destination_port);
    length = ntohs(udp_protocol->udp_length);
    //printf("----------  UDP Protocol  (Transport  Layer)  ----------\n");
   // printf("Source port:%d\n", source_port);
   // printf("Destination port:%d\n", destination_port);
    switch (destination_port)
    {
        case 2152:
        case 2123:
        case 3386:
		GtpPacketMainHandle(packet_content);
                break;
        default:
            break; 
    }
}




void GtpPacketMainHandle(u_char *packet_content)
{
	struct timeval maintimeTemp;
//	struct timeval maintimeTemp_1;
//    	gettimeofday(&maintimeTemp, NULL);
	gettimeofday(&maintimeTemp, NULL);
	std::map<u_int64_t,long>::iterator iter_main;
	std::map<long,Teidid_t>::iterator iter_id;
	pair<std::map<u_int64_t,long>::iterator,bool> testinsert;
	if(maintimeTemp.tv_sec-mainfirstTime_1.tv_sec>=2)
	{
		mainfirstTime_1=maintimeTemp;
		iter_id=MainTEID.begin();
		int loopNum=0;
		while((loopNum<300)&&(iter_id!=MainTEID.end())){
			if(maintimeTemp.tv_sec-iter_id->second.mainlasttime.tv_sec>=180)
			{
			MainMap_C.erase(iter_id->second.teid_t.Uplink_TEID_C);
			MainMap_D.erase(iter_id->second.teid_t.Uplink_TEID_D);
			MainMap_C.erase(iter_id->second.teid_t.Downlink_TEID_C);
			MainMap_D.erase(iter_id->second.teid_t.Downlink_TEID_D);
			MainTEID.erase(iter_id);
			iter_id=MainTEID.begin();
			}
			else 
				++iter_id;
			++loopNum;
                }
		iter_main=MainMap_C.begin();
		int mainLoopNum_c=0;
		while((mainLoopNum_c<100)&&(iter_main!=MainMap_C.end())){
			iter_id=MainTEID.find(iter_main->second);
			if(iter_id==MainTEID.end())
			{
				MainMap_C.erase(iter_main);
				iter_main=MainMap_C.begin();
			}
			else
				++iter_main;
			++mainLoopNum_c;
		}
		iter_main=MainMap_D.begin();
		int mainLoopNum_d=0;
		while((mainLoopNum_d<100)&&(iter_main!=MainMap_D.end())){
			iter_id=MainTEID.find(iter_main->second);
			if(iter_id==MainTEID.end())
			{
				MainMap_D.erase(iter_main);
				iter_main=MainMap_D.begin();
			}
			else
				++iter_main;
			++mainLoopNum_d;
		}
	}	


//	if((sum%12500000)==0)
//	printf("the size of map is %d\n",MainTEID.size());
	u_int32_t sourceIp,destinationIp;
        struct ip_hdr *ipOverGtp;
        ipOverGtp=(struct ip_hdr*)(packet_content+14);
        sourceIp=ntohl(ipOverGtp->saddr);
        destinationIp=ntohl(ipOverGtp->daddr);
	u_int64_t keyMap=0;
	struct gtpHeader *gtp_protocol;
	gtp_protocol = (struct gtpHeader*)(packet_content+14+20+8);
	int len=0;
	int lgtp=0;
	if(gtp_protocol->flags==0x30)
	{	lgtp=8;
//		printf("i got 1\n");
	}
	else if(gtp_protocol->flags==0x32)
	{	lgtp=12;
//		printf("i got 2\n");
	}
	else 
	{
//		printf("i can't process this type %02x\n",gtp_protocol->flags);
		return;
	}
	len=ntohs(gtp_protocol->length);
	if(gtp_protocol->flags==0x32)  len=len-4;
	switch((int)(gtp_protocol->type))
	{
		case 255:	
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_main=MainMap_D.find(keyMap);
			if(iter_main!=MainMap_D.end())
                	{
				iter_id=MainTEID.find(iter_main->second);
				if(iter_id!=MainTEID.end())
				{
				iter_id->second.mainlasttime=maintimeTemp;
				int divdata=iter_id->second.proid;
				int res;
				switch(divdata)
                		{
                			case 1:
                        			res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        			break;
                			case 2:
                        			res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        			break;
                			case 3:
                        			res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        			break;
                			case 4:
                        			res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        			break;
					default:
						break;
                		}
				}
			}
			break;
		case 16:
			struct Create_Req s;
			s=iedecaps(packet_content,len);
			keyMap=convert2key(s.teid_c,s.sgsn_s);
			iter_main=MainMap_C.find(keyMap);
			if(iter_main==MainMap_C.end())
			{
				int res;
				char buffer[4];
				Teidid_t teidid_t;
				long id=mainCallID++;
			 	res = read(pipe_fd_c,(char*)buffer,8);	
				if(res>0)
				{
					//printf("packet capture process has read the killid %d\n",buffer[0]);
					killid1=(killid1+1)%2;
				}
			//	teidid_t.proid=id%2+1;
				teidid_t.proid=killid1*2+id%2+1;	
	//			teidid_t.proid=id%4+1;
				teidid_t.mainlasttime=maintimeTemp;
				teidid_t.seq_create=gtp_protocol->seq;
				teidid_t.teid_t.Downlink_TEID_C=convert2key(s.teid_c,s.sgsn_s);
				teidid_t.teid_t.Downlink_TEID_D=convert2key(s.teid_d,s.sgsn_u);
				switch(teidid_t.proid)
                		{
                			case 1:
                        			res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        			break;
                			case 2:
                        			res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        			break;
                			case 3:
                        			res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        			break;
                			case 4:
                        			res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        			break;
					default:
						break;
                		}
				
				MainTEID.insert(pair<long,Teidid_t>(id,teidid_t));
				testinsert=MainMap_C.insert(pair<u_int64_t,long>(teidid_t.teid_t.Downlink_TEID_C,id));
				if(testinsert.second==0) 
				{
					//printf("can't insert into the map,request,control\n");
					return;
				}
					
				testinsert=MainMap_D.insert(pair<u_int64_t,long>(teidid_t.teid_t.Downlink_TEID_D,id));
				if(testinsert.second==0) 
				{	//printf("can't insert into the map,request,data\n");
					return;
				}
			}
			break;
		case 17:
			struct Create_Res t;
			t=iedecaps1(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_main=MainMap_C.find(keyMap);
			if(iter_main!=MainMap_C.end())
			{
				iter_id=MainTEID.find(iter_main->second);
				if(iter_id!=MainTEID.end())
				{
					int divcreatres=iter_id->second.proid;
					int res;
					switch(divcreatres)
                			{
                				case 1:
                        				res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        				break;
                				case 2:
                        				res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        				break;
                				case 3:
                        				res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        				break;
                				case 4:
                        				res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        				break;
						default:
							break;
                			}
						if((iter_id->second.seq_create!=gtp_protocol->seq)) return;
						if(int(t.cause)==128)
						{
							u_int64_t keyC=0,keyD=0;
							keyC=convert2key(t.teid_c,t.ggsn_s);
							keyD=convert2key(t.teid_d,t.ggsn_u);
							iter_id->second.teid_t.Uplink_TEID_C=keyC;
                                       			iter_id->second.teid_t.Uplink_TEID_D=keyD;
							iter_id->second.mainlasttime=maintimeTemp;
							testinsert=MainMap_C.insert(pair<u_int64_t,long>(keyC,iter_main->second));
							if(testinsert.second==0) 
							{
							//	if((++insertnum)%100==0) 
							//	printf("insert into the map,response,control  %d\n",insertnum);	
								return;
							}
                               				testinsert=MainMap_D.insert(pair<u_int64_t,long>(keyD,iter_main->second));
							if(testinsert.second==0)
							{
								return;
							//	printf("can't insert into the map,response,data\n");
							//	printf("the error teid is %llx\n",keyD);
							//	printf("the teid already in map is %llx\n",(testinsert.first)->first);
							 	std::map<u_int64_t,long>::iterator iter_maintest;
								std::map<long,Teidid_t>::iterator iter_idtest;
								iter_maintest=MainMap_D.find(keyD);
								if(iter_maintest!=MainMap_D.end())
								{
								//	printf("the callid is %ld\n",iter_maintest->second);
									iter_idtest=MainTEID.find(iter_maintest->second);
									if(iter_idtest!=MainTEID.end())
									{
										long a=iter_maintest->second;
                                                				MainMap_C.erase(iter_idtest->second.teid_t.Downlink_TEID_C);
                                                				MainMap_C.erase(iter_idtest->second.teid_t.Uplink_TEID_C);
                                                				MainMap_D.erase(iter_idtest->second.teid_t.Downlink_TEID_D);
                                                				MainMap_D.erase(iter_idtest->second.teid_t.Uplink_TEID_D);
                                                				MainTEID.erase(a);	
									/*
										printf("uplink teid control is %llx\n",iter_idtest->second.teid_t.Uplink_TEID_C);
										printf("uplink teid data is %llx\n",iter_idtest->second.teid_t.Uplink_TEID_D);
										printf("downlink teid control is %llx\n",iter_idtest->second.teid_t.Downlink_TEID_C);
										printf("downlink teid data is %llx\n",iter_idtest->second.teid_t.Downlink_TEID_D);
									*/
									}
								}
							/*	else printf("main map can't find the teid\n");	
								printf("the callid at this time is %ld\n",iter_main->second);
								printf("uplink teid control is %llx\n",iter_id->second.teid_t.Uplink_TEID_C);
								printf("uplink teid data is %llx\n",iter_id->second.teid_t.Uplink_TEID_D);
								printf("downlink teid control is %llx\n",iter_id->second.teid_t.Downlink_TEID_C);
								printf("downlink teid data is %llx\n",iter_id->second.teid_t.Downlink_TEID_D);
							*/
							MainMap_D.insert(pair<u_int64_t,long>(keyD,iter_main->second));
							}
						}
						else
						{
							long callid=iter_main->second;
							MainMap_C.erase(iter_id->second.teid_t.Downlink_TEID_C);
							MainMap_D.erase(iter_id->second.teid_t.Downlink_TEID_D);
							MainTEID.erase(callid);
						}
					}
			}
			break;
		case 18:
			struct Update_Req u;
			u=iedecaps4(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_main=MainMap_C.find(keyMap);
			if(iter_main!=MainMap_C.end())
			{	iter_id=MainTEID.find(iter_main->second);
				if(iter_id!=MainTEID.end())
				{	
				int res;
				switch(iter_id->second.proid)
                		{
                			case 1:
                        			res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        			break;
                			case 2:
                        			res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        			break;
                			case 3:
                        			res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        			break;
                			case 4:
                        			res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        			break;
					default:
						break;
                		}
					u_int64_t k,l;
					k=convert2key(u.teid_c,u.sgsn_s);
					l=convert2key(u.teid_d,u.sgsn_u);
					testinsert=MainMap_C.insert(pair<u_int64_t,long>(l,iter_main->second));
					if(testinsert.second==0)
					{
						//printf("can not insert update c \n");
						return;

					}
					testinsert=MainMap_D.insert(pair<u_int64_t,long>(k,iter_main->second));
					if(testinsert.second==0)
					{
						//printf("can not insert update d \n");
						return;

					}
					MainMap_D.erase(iter_id->second.teid_t.Downlink_TEID_D);
					MainMap_C.erase(iter_id->second.teid_t.Downlink_TEID_C);
					iter_id->second.teid_t.Downlink_TEID_C=k;
					iter_id->second.teid_t.Downlink_TEID_D=l;
					iter_id->second.mainlasttime=maintimeTemp;
					iter_id->second.seq_update=gtp_protocol->seq;

				}	
			}
			break;
			
			case 19:
				struct Update_Res v;
				v=iedecaps5(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_main=MainMap_C.find(keyMap);
				if(iter_main!=MainMap_C.end())
				{
					iter_id=MainTEID.find(iter_main->second);
					if(iter_id!=MainTEID.end())
					{
						int res;
						switch(iter_id->second.proid)
                				{
                					case 1:
                        					res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        					break;
                					case 2:
                        					res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        					break;
                					case 3:
                        					res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        					break;
                					case 4:
                        					res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        					break;
							default:
								break;
                				}
						if(iter_id->second.seq_update!=gtp_protocol->seq) return;
						if(int(v.cause)==128)
						{
							u_int64_t m;
							m=convert2key(v.teid_d,v.ggsn_u);
						testinsert=MainMap_D.insert(pair<u_int64_t,long>(m,iter_main->second));
					if(testinsert.second==0)
					{
						//printf("can not insert update d \n");
						return;

					}
						MainMap_D.erase(iter_id->second.teid_t.Uplink_TEID_D);
							iter_id->second.teid_t.Uplink_TEID_D=m;
							iter_id->second.mainlasttime=maintimeTemp;
						}	
					}
				}
				break;
			case 20:
                                struct Delete_Req w;
                                w=iedecaps2(packet_content,len);
                                keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
                                iter_main=MainMap_C.find(keyMap);
                                if(iter_main!=MainMap_C.end())
                                {
                                        iter_id=MainTEID.find(iter_main->second);
                                        if(iter_id!=MainTEID.end())
                                        {
						int res;
						switch(iter_id->second.proid)
						{
                					case 1:
                        					res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        					break;
                					case 2:
                        					res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        					break;
                					case 3:
                        					res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        					break;
                					case 4:
                        					res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        					break;
                                                        default:
                                                                break;
						}
                                                if(w.Teardown_ID!=0)
                                                {
                                                        iter_id->second.Delreq=1;
                                                        iter_id->second.mainlasttime=maintimeTemp;
							iter_id->second.seq_delete=gtp_protocol->seq;
                                                }
                                        }
                                }
	
			case 21:
				struct Delete_Res x;
				x=iedecaps3(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_main=MainMap_C.find(keyMap);
				if(iter_main!=MainMap_C.end())
				{
                                	iter_id=MainTEID.find(iter_main->second);
					if(iter_id!=MainTEID.end())
					{
						//int divdel=iter_id->second.proid;
						int res;
						switch(iter_id->second.proid)
                				{
                					case 1:
                        					res = write(pipe_fd[0],packet_content,BUFFER_SIZE);
                        					break;
                					case 2:
                        					res = write(pipe_fd[1],packet_content,BUFFER_SIZE);
                        					break;
                					case 3:
                        					res = write(pipe_fd[2],packet_content,BUFFER_SIZE);
                        					break;
                					case 4:
                        					res = write(pipe_fd[3],packet_content,BUFFER_SIZE);
                        					break;
							default:
								break;
                				}
						if(iter_id->second.seq_delete!=gtp_protocol->seq) return;
					//	if(iter_id->second.Delreq==1)
					//	{
						if(int(x.cause)==128)
						{
							
						long a=iter_main->second;	
						MainMap_C.erase(iter_id->second.teid_t.Downlink_TEID_C);
						MainMap_C.erase(iter_id->second.teid_t.Uplink_TEID_C);
						MainMap_D.erase(iter_id->second.teid_t.Downlink_TEID_D);
						MainMap_D.erase(iter_id->second.teid_t.Uplink_TEID_D);
						MainTEID.erase(a);
						}
					//	}
					}	
				}
				break;
			default:
				break;
		
		}


}

void GtpPacketHandle_1(u_char *packet_content)
{
std::map<u_int64_t,long>::iterator iter_callid;
std::map<long,GTP_Call_Info_t>::iterator iter_val;
pair<std::map<u_int64_t,long>::iterator,bool> testcallid;
	struct timeval timeTemp1;
    gettimeofday(&timeTemp1, NULL);
	if(timeTemp1.tv_sec-mkdirTime_1.tv_sec>=600)
		mkdirTime_1=timeTemp1;
/*
if(((++sum1)%1000000)==0)
{
printf("the size of fork 1 map is %d\n",InfoMap.size());
}
*/
 if(timeTemp1.tv_sec-firstTime.tv_sec>=2)
        {
                firstTime=timeTemp1;
                int loopNum=0;
                iter_val=InfoMap.begin();
                while((loopNum<100)&&(iter_val!=InfoMap.end())){
                        if(timeTemp1.tv_sec-iter_val->second.lasttime.tv_sec>=180)
                        {
                        iter_val->second.sdr_time.Stop_Time=timeTemp1;
                        GPRS_Pkt_info_t gprs_pkt_info;
                        gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                        gprs_pkt_info.timeStamp=timeTemp1;
                        gprs_pkt_info.gtp_call_info=iter_val->second;
                        if((strncmp(iter_val->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val->second.pdp_status.APN,"cmwap",5))==0)
                        {
                                wap_protocol_packet_callback(&(gprs_pkt_info),iter_val->second.GPRS_PDP_SSN,1,1);
                                //writeCDRFile(&(iter_val->second),1);
                                writeSDRFile(&(iter_val->second),1);
                        }
                        else if((strncmp(iter_val->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val->second.pdp_status.APN,"cmnet",5))==0)
                        {
                                http_protocol_packet_callback_one(&(gprs_pkt_info),iter_val->second.GPRS_PDP_SSN);
                                //writeCDRFile(&(iter_val->second),1);
                                writeSDRFile(&(iter_val->second),1);
                        }
                        CallIDMap_C.erase(iter_val->second.teid_t.Uplink_TEID_C);
                        CallIDMap_D.erase(iter_val->second.teid_t.Uplink_TEID_D);
                        CallIDMap_C.erase(iter_val->second.teid_t.Downlink_TEID_C);
                        CallIDMap_D.erase(iter_val->second.teid_t.Downlink_TEID_D);
                        InfoMap.erase(iter_val);
                        iter_val=InfoMap.begin();
                        }
                        else
                                ++iter_val;
                        ++loopNum;
                }
	}







	u_int32_t sourceIp,destinationIp;
        struct ip_hdr *ipOverGtp;
        ipOverGtp=(struct ip_hdr*)(packet_content+14);
        sourceIp=ntohl(ipOverGtp->saddr);
  //   	 printf("***********************%02x\n",sourceIp);
        destinationIp=ntohl(ipOverGtp->daddr);
  //  	printf("***********************%02x\n",destinationIp);
	u_int64_t keyMap=0;
	struct gtpHeader *gtp_protocol;
	gtp_protocol = (struct gtpHeader*)(packet_content+14+20+8);
	int len=0;
	int lgtp=0;
	if(gtp_protocol->flags==0x30)
	{	lgtp=8;
	}
	else if(gtp_protocol->flags==0x32)
	{	lgtp=12;
	}
	else 
	{
		return;
	}
	len=ntohs(gtp_protocol->length);
	if(gtp_protocol->flags==0x32)  len=len-4;
	switch((int)(gtp_protocol->type))
	{
		case 255:
			
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid=CallIDMap_D.find(keyMap);
			if(iter_callid!=CallIDMap_D.end())
                	{
				iter_val=InfoMap.find(iter_callid->second);
				if(iter_val!=InfoMap.end())
				{
					GPRS_Pkt_info_t gprs_pkt_info;
                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content+14+20+8+lgtp);
					iter_val->second.lasttime=timeTemp1;
					gprs_pkt_info.timeStamp=timeTemp1;
					gprs_pkt_info.packetLength=len;
					if(keyMap==iter_val->second.teid_t.Downlink_TEID_D)
					{	iter_val->second.GPRS_DownBytes+=len;
						iter_val->second.linkFlag=0;
						//printf("the DownBytes is %ld\n",iter_d->second.GPRS_DownBytes);
					}
					else if(keyMap==iter_val->second.teid_t.Uplink_TEID_D)
					{	 iter_val->second.GPRS_UpBytes+=len;
						 iter_val->second.linkFlag=1;
						//printf("the UpBytes is %ld\n",iter_d->second.GPRS_UpBytes);
					}
					else return;
					gprs_pkt_info.gtp_call_info=iter_val->second;
					if((strncmp(iter_val->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val->second.pdp_status.APN,"cmwap",5))==0) 
                        		{
					//	printf("the call1wap is %ld\n",++call1wap);
					//	++call1wap;
						wap_protocol_packet_callback(&(gprs_pkt_info),0,1,1);
					
                        		}
                        		else if((strncmp(iter_val->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val->second.pdp_status.APN,"cmnet",5))==0)
                        		{
			//			printf("the call1http is %ld\n",++call1http);
					//	++call1http;
						http_protocol_packet_callback_one(&(gprs_pkt_info),0);
                        		}
				}
			}
			break;
		case 16:
			struct Create_Req s;
			memset(s.imsi,0,8);
                        memset(s.imei,0,8);           
                        memset(s.msisdn,0,8);           
                        memset(s.apn,0,6);
                        s.teid_d=0;           
                        s.teid_c=0;
			s.sgsn_s=0;
			s.sgsn_u=0;
			s=iedecaps(packet_content,len);
			keyMap=convert2key(s.teid_c,s.sgsn_s);
			iter_callid=CallIDMap_C.find(keyMap);
			if(iter_callid==CallIDMap_C.end())
			{
                		GTP_Call_Info_t gtpCallInfo_t;
				gtpCallInfo_t.GPRS_Call_ID=tempCallID1++;
				gtpCallInfo_t.teid_t.Downlink_TEID_C=convert2key(s.teid_c,s.sgsn_s);
				gtpCallInfo_t.teid_t.Downlink_TEID_D=convert2key(s.teid_d,s.sgsn_u);
			testcallid=CallIDMap_C.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_C,gtpCallInfo_t.GPRS_Call_ID));
				if(testcallid.second==0) return;
				testcallid=CallIDMap_D.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_D,gtpCallInfo_t.GPRS_Call_ID));
				if(testcallid.second==0) return;
				memset(gtpCallInfo_t.pdp_status.APN,0,6);
				memset(gtpCallInfo_t.MT_IMSI,0,8);
                                memset(gtpCallInfo_t.MT_MSISDN,0,8);
				memset(gtpCallInfo_t.MT_IMEI,0,8);
				memcpy(gtpCallInfo_t.pdp_status.APN,s.apn,6);
				memcpy(gtpCallInfo_t.MT_IMSI,s.imsi,8);
				memcpy(gtpCallInfo_t.MT_IMEI,s.imei,8);
				memcpy(gtpCallInfo_t.MT_MSISDN,s.msisdn,8);
				gtpCallInfo_t.LAC=s.lac;
				gtpCallInfo_t.CellID=s.ci;
				gtpCallInfo_t.rat=s.rat;
				gtpCallInfo_t.GPRS_Intface=2;
				gtpCallInfo_t.pdp_status.seq_create=gtp_protocol->seq;
				tempTime1=timeTemp1;
				gtpCallInfo_t.GPRS_DownBytes=0;
				gtpCallInfo_t.GPRS_UpBytes=0;
				gtpCallInfo_t.lasttime=timeTemp1;
				gtpCallInfo_t.pdp_status.Delreq=0;	
				InfoMap.insert(pair<long,GTP_Call_Info_t>(gtpCallInfo_t.GPRS_Call_ID,gtpCallInfo_t));
			}
			break;
		case 17:
			struct Create_Res t;
			t.cause=0;
			t.teid_d=0;
			t.teid_c=0;
			t.enduserIP=0;
			t.ggsn_s=0;
			t.ggsn_u=0;
			t=iedecaps1(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid=CallIDMap_C.find(keyMap);	
                        if(iter_callid!=CallIDMap_C.end())
			{
				iter_val=InfoMap.find(iter_callid->second);
				if(iter_val!=InfoMap.end())
				{
					if(iter_val->second.pdp_status.seq_create!=gtp_protocol->seq) return;
					if(int(t.cause)==128)
					{
						u_int64_t keyC=0,keyD=0;
						keyC=convert2key(t.teid_c,t.ggsn_s);
						keyD=convert2key(t.teid_d,t.ggsn_u);
						iter_val->second.teid_t.Uplink_TEID_C=keyC;
                                        	iter_val->second.teid_t.Uplink_TEID_D=keyD;
                                        	testcallid=CallIDMap_C.insert(pair<u_int64_t,long>(keyC,iter_callid->second));
						if(testcallid.second==0) return;
                                        	testcallid=CallIDMap_D.insert(pair<u_int64_t,long>(keyD,iter_callid->second));
						if(testcallid.second==0)
						{
							return;
							std::map<u_int64_t,long>::iterator iter_callidtest;
							std::map<long,GTP_Call_Info_t>::iterator iter_valtest;
							iter_callidtest=CallIDMap_D.find(keyD);
							if(iter_callidtest!=CallIDMap_D.end())
							{
								iter_valtest=InfoMap.find(iter_callidtest->second);
								if(iter_valtest!=InfoMap.end())
								{
								iter_valtest->second.sdr_time.Stop_Time=timeTemp1;
								GPRS_Pkt_info_t gprs_pkt_info;
                                				gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                				gprs_pkt_info.timeStamp=timeTemp1;
								gprs_pkt_info.packetLength=len;
                                				gprs_pkt_info.gtp_call_info=iter_valtest->second;
							//printf("the call1 is %ld\n",++call1);
								if((strncmp(iter_valtest->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmwap",5))==0) 
                                				{
						//		printf("the call1wap is %ld\n",++call1wap);
									
							//	++call1wap;
									wap_protocol_packet_callback(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN,1,1);
                                				}
                                				if((strncmp(iter_valtest->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmnet",5))==0)
                                				{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
									http_protocol_packet_callback_one(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN);
								}
								//writeCDRFile(&(iter_valtest->second),1);
								writeSDRFile(&(iter_valtest->second),1);
					//		u_int64_t a=iter_callid->second;
								CallIDMap_C.erase(iter_valtest->second.teid_t.Uplink_TEID_C);
								CallIDMap_D.erase(iter_valtest->second.teid_t.Uplink_TEID_D);
								CallIDMap_C.erase(iter_valtest->second.teid_t.Downlink_TEID_C);
								CallIDMap_D.erase(iter_valtest->second.teid_t.Downlink_TEID_D);
								InfoMap.erase(iter_valtest);
								}
							}
                                        	CallIDMap_D.insert(pair<u_int64_t,long>(keyD,iter_callid->second));
						}	
						iter_val->second.pdp_status.RspCause=t.cause;
						iter_val->second.pdp_status.enduserIP=t.enduserIP;
						iter_val->second.GPRS_PDP_SSN=tempPdpSSN1++;
						iter_val->second.pdp_status.AttachSuccess='Y';
						iter_val->second.sdr_time.Start_Time=timeTemp1;
						iter_val->second.lasttime=timeTemp1;
						if(timeTemp1.tv_sec>=tempTime1.tv_sec)
                                                {
                                                iter_val->second.gprs_status.AttachDelay=(timeTemp1.tv_sec-tempTime1.tv_sec)*1000000+timeTemp1.tv_usec-tempTime1.tv_usec;
                                                iter_val->second.pdp_status.pdpReqDelay=(timeTemp1.tv_sec-tempTime1.tv_sec)*1000000+timeTemp1.tv_usec-tempTime1.tv_usec;
                                                }
					}
				
					else
					{
					    // 失败，写回
						iter_val->second.pdp_status.AttachSuccess='N';
						iter_val->second.pdp_status.RspCause=t.cause;
						iter_val->second.pdp_status.enduserIP=t.enduserIP;
						iter_val->second.GPRS_PDP_SSN=tempPdpSSN1++;
						iter_val->second.sdr_time.Start_Time=timeTemp1;
						iter_val->second.sdr_time.Stop_Time=timeTemp1;
						iter_val->second.lasttime=timeTemp1;
						if(timeTemp1.tv_sec>=tempTime1.tv_sec)
                                                {
                                                iter_val->second.gprs_status.AttachDelay=(timeTemp1.tv_sec-tempTime1.tv_sec)*1000000+timeTemp1.tv_usec-tempTime1.tv_usec;
                                                iter_val->second.pdp_status.pdpReqDelay=(timeTemp1.tv_sec-tempTime1.tv_sec)*1000000+timeTemp1.tv_usec-tempTime1.tv_usec;
                                                }
						writeSDRFile(&(iter_val->second),1);
						
						u_int64_t teid_c=iter_val->second.teid_t.Downlink_TEID_C;
						u_int64_t teid_d=iter_val->second.teid_t.Downlink_TEID_D;
						InfoMap.erase(iter_callid->second);
						CallIDMap_C.erase(teid_c);
						CallIDMap_D.erase(teid_d);
					}
				}
			}
			break;
		case 18:
			struct Update_Req u;
			u=iedecaps4(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid=CallIDMap_C.find(keyMap);
			if(iter_callid!=CallIDMap_C.end())
			{	
				iter_val=InfoMap.find(iter_callid->second);
				if(iter_val!=InfoMap.end())
				{
					u_int64_t k,l;
					k=convert2key(u.teid_c,u.sgsn_s);
					l=convert2key(u.teid_d,u.sgsn_u);
				testcallid=CallIDMap_C.insert(pair<u_int64_t,long>(k,iter_callid->second));
			if(testcallid.second==0) return;
				testcallid=CallIDMap_D.insert(pair<u_int64_t,long>(l,iter_callid->second));
			if(testcallid.second==0) return;
					CallIDMap_D.erase(iter_val->second.teid_t.Downlink_TEID_D);
					CallIDMap_C.erase(iter_val->second.teid_t.Downlink_TEID_C);
					iter_val->second.teid_t.Downlink_TEID_C=k;
					iter_val->second.teid_t.Downlink_TEID_D=l;
					iter_val->second.LAC=u.lac;
					iter_val->second.CellID=u.ci;
					iter_val->second.rat=u.rat;
					iter_val->second.lasttime=timeTemp1;
					iter_val->second.pdp_status.seq_update=gtp_protocol->seq;
				}
			}
			break;
		case 19:
				struct Update_Res v;
				v=iedecaps5(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid=CallIDMap_C.find(keyMap);
				if(iter_callid!=CallIDMap_C.end())
				{
					iter_val=InfoMap.find(keyMap);
					if(iter_val!=InfoMap.end())
					{
						if(iter_val->second.pdp_status.seq_update!=gtp_protocol->seq) return;
						if(int(v.cause)==128)
						{
							u_int64_t m;
							m=convert2key(v.teid_d,v.ggsn_u);
						testcallid=CallIDMap_D.insert(pair<u_int64_t,long>(m,iter_callid->second));
			if(testcallid.second==0) return;
							CallIDMap_D.erase(iter_val->second.teid_t.Uplink_TEID_D);
							iter_val->second.teid_t.Uplink_TEID_D=m;
							iter_val->second.lasttime=timeTemp1;
						}	
					}
				}
				break;
			case 20:
                                struct Delete_Req w;
                                w=iedecaps2(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
                              	iter_callid=CallIDMap_C.find(keyMap);
                                if(iter_callid!=CallIDMap_C.end())
                                {
					iter_val=InfoMap.find(iter_callid->second);
					if(iter_val!=InfoMap.end())
					{
						if(w.Teardown_ID!=0)
						{
							iter_val->second.pdp_status.Delreq=1;
							iter_val->second.lasttime=timeTemp1;
							iter_val->second.pdp_status.seq_delete=gtp_protocol->seq;
						}
					}
                                }
	
			case 21:
				struct Delete_Res x;
				x=iedecaps3(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid=CallIDMap_C.find(keyMap);
                if(iter_callid!=CallIDMap_C.end())
				{
					iter_val=InfoMap.find(iter_callid->second);
					if(iter_val!=InfoMap.end())
					{
						if(iter_val->second.pdp_status.seq_delete!=gtp_protocol->seq) 
						    return;
					
						if(int(x.cause)==128)
						{
							iter_val->second.sdr_time.Stop_Time=timeTemp1;
							GPRS_Pkt_info_t gprs_pkt_info;
                                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                			gprs_pkt_info.timeStamp=timeTemp1;
							gprs_pkt_info.packetLength=len;
                                			gprs_pkt_info.gtp_call_info=iter_val->second;
							//printf("the call1 is %ld\n",++call1);
							if((strncmp(iter_val->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val->second.pdp_status.APN,"cmwap",5))==0) 
                                			{
						//		printf("the call1wap is %ld\n",++call1wap);
							//	++call1wap;
								wap_protocol_packet_callback(&(gprs_pkt_info),iter_val->second.GPRS_PDP_SSN,1,1);
                                			}
                                			if((strncmp(iter_val->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val->second.pdp_status.APN,"cmnet",5))==0)
                                			{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
								http_protocol_packet_callback_one(&(gprs_pkt_info),iter_val->second.GPRS_PDP_SSN);
							}
								//writeCDRFile(&(iter_val->second),1);
								writeSDRFile(&(iter_val->second),1);
					//		u_int64_t a=iter_callid->second;
							CallIDMap_C.erase(iter_val->second.teid_t.Uplink_TEID_C);
							CallIDMap_D.erase(iter_val->second.teid_t.Uplink_TEID_D);
							CallIDMap_C.erase(iter_val->second.teid_t.Downlink_TEID_C);
							CallIDMap_D.erase(iter_val->second.teid_t.Downlink_TEID_D);
							InfoMap.erase(iter_val);
						}else {
							// 失败，写回
							//iter_val->second.pdp_status.AttachSuccess='N';
							//writeSDRFile(&(iter_val->second),1);
					    }	
				    }
				}
				break;
			default:
				break;
		
		}	
}
void GtpPacketHandle_2(u_char *packet_content)
{
std::map<u_int64_t,long>::iterator iter_callid2;
std::map<long,GTP_Call_Info_t>::iterator iter_val2;
pair<std::map<u_int64_t,long>::iterator,bool> testcallid2;
	struct timeval timeTemp2;
    	gettimeofday(&timeTemp2, NULL);
	if(timeTemp2.tv_sec-mkdirTime_2.tv_sec>=600)
		mkdirTime_2=timeTemp2;
/*
if(((++sum2)%1000000)==0)
{
//printf("the value of sum2 is %ld\n",sum2);
printf("the size of fork 2 map is %d\n",InfoMap2.size());
}
*/
        if(timeTemp2.tv_sec-firstTime2.tv_sec>=2)
        {
                firstTime2=timeTemp2;
                iter_val2=InfoMap2.begin();
                int loopNum=0;
                while((loopNum<100)&&(iter_val2!=InfoMap2.end())){
                        if(timeTemp2.tv_sec-iter_val2->second.lasttime.tv_sec>=180)
                        {
                        iter_val2->second.sdr_time.Stop_Time=timeTemp2;
                        GPRS_Pkt_info_t gprs_pkt_info;
                        gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                        gprs_pkt_info.timeStamp=timeTemp2;
                        gprs_pkt_info.gtp_call_info=iter_val2->second;
                        if((strncmp(iter_val2->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmwap",5))==0)
                        {
                                wap_protocol_packet_callback(&(gprs_pkt_info),iter_val2->second.GPRS_PDP_SSN,1,2);
                                //writeCDRFile(&(iter_val2->second),2);
                                writeSDRFile(&(iter_val2->second),2);
                        }
                        else if((strncmp(iter_val2->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmnet",5))==0)
                        {
                                http_protocol_packet_callback_two(&(gprs_pkt_info),iter_val2->second.GPRS_PDP_SSN);
                                //writeCDRFile(&(iter_val2->second),2);
                                writeSDRFile(&(iter_val2->second),2);
                        }
                        CallIDMap2_C.erase(iter_val2->second.teid_t.Uplink_TEID_C);
                        CallIDMap2_D.erase(iter_val2->second.teid_t.Uplink_TEID_D);
                        CallIDMap2_C.erase(iter_val2->second.teid_t.Downlink_TEID_C);
                        CallIDMap2_D.erase(iter_val2->second.teid_t.Downlink_TEID_D);
                        InfoMap2.erase(iter_val2);
                        iter_val2=InfoMap2.begin();
                        }
                        else
                                ++iter_val2;
                        ++loopNum;
                }	
       /*  
	       int loopNum1=0;
                iter_callid2=CallIDMap2.begin();
                do{
                        iter_val2=InfoMap2.find(iter_callid2->second);
                        if(iter_val2==InfoMap2.end())
                        {
                                CallIDMap2.erase(iter_callid2);
                                iter_callid2=CallIDMap2.begin();
                        }
                        else
                                ++iter_callid2;
                        ++loopNum1;
                }while((loopNum1<100)&&(iter_callid2!=CallIDMap2.end()));
        */
	}



	u_int32_t sourceIp,destinationIp;
        struct ip_hdr *ipOverGtp;
        ipOverGtp=(struct ip_hdr*)(packet_content+14);
        sourceIp=ntohl(ipOverGtp->saddr);
  //   	 printf("***********************%02x\n",sourceIp);
        destinationIp=ntohl(ipOverGtp->daddr);
  //  	printf("***********************%02x\n",destinationIp);
	u_int64_t keyMap=0;
	struct gtpHeader *gtp_protocol;
	gtp_protocol = (struct gtpHeader*)(packet_content+14+20+8);
	int len=0;
	int lgtp=0;
	if(gtp_protocol->flags==0x30)
	{	lgtp=8;
	}
	else if(gtp_protocol->flags==0x32)
	{	lgtp=12;
	}
	else 
	{
		return;
	}
	len=ntohs(gtp_protocol->length);
	if(gtp_protocol->flags==0x32)  len=len-4;
	switch((int)(gtp_protocol->type))
	{
		case 255:
			
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid2=CallIDMap2_D.find(keyMap);
			if(iter_callid2!=CallIDMap2_D.end())
                	{
				iter_val2=InfoMap2.find(iter_callid2->second);
				if(iter_val2!=InfoMap2.end())
				{
					GPRS_Pkt_info_t gprs_pkt_info;
                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content+14+20+8+lgtp);
					iter_val2->second.lasttime=timeTemp2;
					gprs_pkt_info.timeStamp=timeTemp2;
					gprs_pkt_info.packetLength=len;
					if(keyMap==iter_val2->second.teid_t.Downlink_TEID_D)
					{	iter_val2->second.GPRS_DownBytes+=len;
						iter_val2->second.linkFlag=0;
						//printf("the DownBytes is %ld\n",iter_d->second.GPRS_DownBytes);
					}
					else if(keyMap==iter_val2->second.teid_t.Uplink_TEID_D)
					{	 iter_val2->second.GPRS_UpBytes+=len;
						 iter_val2->second.linkFlag=1;
						//printf("the UpBytes is %ld\n",iter_d->second.GPRS_UpBytes);
					}
				//	else return;
					gprs_pkt_info.gtp_call_info=iter_val2->second;
					if((strncmp(iter_val2->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmwap",5))==0)  
                        		{
						wap_protocol_packet_callback(&(gprs_pkt_info),0,1,2);
					
                        		}
                        		else if((strncmp(iter_val2->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmnet",5))==0)
                        		{
						http_protocol_packet_callback_two(&(gprs_pkt_info),0);
                        		}
				}
			}
			break;
		case 16:
			struct Create_Req s;
			memset(s.imsi,0,8);
                        memset(s.imei,0,8);           
                        memset(s.msisdn,0,8);           
                        memset(s.apn,0,6);
                        s.teid_d=0;           
                        s.teid_c=0;
			s.sgsn_s=0;
			s.sgsn_u=0;
			s=iedecaps(packet_content,len);
			keyMap=convert2key(s.teid_c,s.sgsn_s);
			iter_callid2=CallIDMap2_C.find(keyMap);
			if(iter_callid2==CallIDMap2_C.end())
			{
                		GTP_Call_Info_t gtpCallInfo_t;
				gtpCallInfo_t.GPRS_Call_ID=tempCallID2++;
				gtpCallInfo_t.teid_t.Downlink_TEID_C=convert2key(s.teid_c,s.sgsn_s);
				gtpCallInfo_t.teid_t.Downlink_TEID_D=convert2key(s.teid_d,s.sgsn_u);
			testcallid2=CallIDMap2_C.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_C,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid2.second==0) return;
			testcallid2=CallIDMap2_D.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_D,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid2.second==0) return;
				memset(gtpCallInfo_t.pdp_status.APN,0,6);
				memset(gtpCallInfo_t.MT_IMSI,0,8);
                                memset(gtpCallInfo_t.MT_MSISDN,0,8);
				memset(gtpCallInfo_t.MT_IMEI,0,8);
				memcpy(gtpCallInfo_t.pdp_status.APN,s.apn,6);
				memcpy(gtpCallInfo_t.MT_IMSI,s.imsi,8);
				memcpy(gtpCallInfo_t.MT_IMEI,s.imei,8);
				memcpy(gtpCallInfo_t.MT_MSISDN,s.msisdn,8);
				gtpCallInfo_t.LAC=s.lac;
				gtpCallInfo_t.CellID=s.ci;
				gtpCallInfo_t.rat=s.rat;
				gtpCallInfo_t.GPRS_Intface=2;
				gtpCallInfo_t.pdp_status.seq_create=gtp_protocol->seq;
				tempTime2=timeTemp2;
				gtpCallInfo_t.GPRS_DownBytes=0;
				gtpCallInfo_t.GPRS_UpBytes=0;
				gtpCallInfo_t.lasttime=timeTemp2;
				gtpCallInfo_t.pdp_status.Delreq=0;	
				InfoMap2.insert(pair<long,GTP_Call_Info_t>(gtpCallInfo_t.GPRS_Call_ID,gtpCallInfo_t));
			}
			break;
		case 17:
			struct Create_Res t;
			t.cause=0;
			t.teid_d=0;
			t.teid_c=0;
			t.enduserIP=0;
			t.ggsn_s=0;
			t.ggsn_u=0;
			t=iedecaps1(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid2=CallIDMap2_C.find(keyMap);	
                        if(iter_callid2!=CallIDMap2_C.end())
			{
				iter_val2=InfoMap2.find(iter_callid2->second);
				if(iter_val2!=InfoMap2.end())
				{
					if((iter_val2->second.pdp_status.seq_create!=gtp_protocol->seq)) return;
					if(int(t.cause)==128)
					{
						u_int64_t keyC=0,keyD=0;
						keyC=convert2key(t.teid_c,t.ggsn_s);
						keyD=convert2key(t.teid_d,t.ggsn_u);
						iter_val2->second.teid_t.Uplink_TEID_C=keyC;
                                        	iter_val2->second.teid_t.Uplink_TEID_D=keyD;
                                        	testcallid2=CallIDMap2_C.insert(pair<u_int64_t,long>(keyC,iter_callid2->second));
			if(testcallid2.second==0) return;
                                        	testcallid2=CallIDMap2_D.insert(pair<u_int64_t,long>(keyD,iter_callid2->second));
						if(testcallid2.second==0)
						{
							return;
							std::map<u_int64_t,long>::iterator iter_callidtest;
							std::map<long,GTP_Call_Info_t>::iterator iter_valtest;
							iter_callidtest=CallIDMap2_D.find(keyD);
							if(iter_callidtest!=CallIDMap2_D.end())
							{
								iter_valtest=InfoMap2.find(iter_callidtest->second);
								if(iter_valtest!=InfoMap2.end())
								{
								iter_valtest->second.sdr_time.Stop_Time=timeTemp2;
								GPRS_Pkt_info_t gprs_pkt_info;
                                				gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                				gprs_pkt_info.timeStamp=timeTemp2;
								gprs_pkt_info.packetLength=len;
                                				gprs_pkt_info.gtp_call_info=iter_valtest->second;
							//printf("the call1 is %ld\n",++call1);
								if((strncmp(iter_valtest->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmwap",5))==0) 
                                				{
						//		printf("the call1wap is %ld\n",++call1wap);
									
							//	++call1wap;
									wap_protocol_packet_callback(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN,1,2);
                                				}
                                				if((strncmp(iter_valtest->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmnet",5))==0)
                                				{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
									http_protocol_packet_callback_two(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN);
								}
								//writeCDRFile(&(iter_valtest->second),2);
								writeSDRFile(&(iter_valtest->second),2);
					//		u_int64_t a=iter_callid->second;
								CallIDMap2_C.erase(iter_valtest->second.teid_t.Uplink_TEID_C);
								CallIDMap2_D.erase(iter_valtest->second.teid_t.Uplink_TEID_D);
								CallIDMap2_C.erase(iter_valtest->second.teid_t.Downlink_TEID_C);
								CallIDMap2_D.erase(iter_valtest->second.teid_t.Downlink_TEID_D);
								InfoMap2.erase(iter_valtest);
								}
							}
                                        		CallIDMap2_D.insert(pair<u_int64_t,long>(keyD,iter_callid2->second));
						}	
						iter_val2->second.pdp_status.RspCause=t.cause;
						iter_val2->second.pdp_status.enduserIP=t.enduserIP;
						iter_val2->second.GPRS_PDP_SSN=tempPdpSSN2++;
						iter_val2->second.pdp_status.AttachSuccess='Y';
						iter_val2->second.sdr_time.Start_Time=timeTemp2;
						iter_val2->second.lasttime=timeTemp2;
						if(timeTemp2.tv_sec>=tempTime2.tv_sec)
                                                {
                                                iter_val2->second.gprs_status.AttachDelay=(timeTemp2.tv_sec-tempTime2.tv_sec)*1000000+timeTemp2.tv_usec-tempTime2.tv_usec;
                                                iter_val2->second.pdp_status.pdpReqDelay=(timeTemp2.tv_sec-tempTime2.tv_sec)*1000000+timeTemp2.tv_usec-tempTime2.tv_usec;
                                                }
					}
				
					else
					{
					    
					    // 失败，写回
						iter_val2->second.pdp_status.AttachSuccess='N';
						iter_val2->second.pdp_status.RspCause=t.cause;
						iter_val2->second.pdp_status.enduserIP=t.enduserIP;
						iter_val2->second.GPRS_PDP_SSN=tempPdpSSN2++;
						iter_val2->second.sdr_time.Start_Time=timeTemp2;
						iter_val2->second.sdr_time.Stop_Time=timeTemp2;
						iter_val2->second.lasttime=timeTemp2;
						if(timeTemp2.tv_sec>=tempTime2.tv_sec)
                                                {
                                                iter_val2->second.gprs_status.AttachDelay=(timeTemp2.tv_sec-tempTime2.tv_sec)*1000000+timeTemp2.tv_usec-tempTime2.tv_usec;
                                                iter_val2->second.pdp_status.pdpReqDelay=(timeTemp2.tv_sec-tempTime2.tv_sec)*1000000+timeTemp2.tv_usec-tempTime2.tv_usec;
                                                }
						writeSDRFile(&(iter_val2->second),2);
						
						u_int64_t teid_c=iter_val2->second.teid_t.Downlink_TEID_C;
						u_int64_t teid_d=iter_val2->second.teid_t.Downlink_TEID_D;
						InfoMap2.erase(iter_callid2->second);
						CallIDMap2_C.erase(teid_c);
						CallIDMap2_D.erase(teid_d);
					    
					    
					}
				}
			}
			break;
		case 18:
			struct Update_Req u;
			u=iedecaps4(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid2=CallIDMap2_C.find(keyMap);
			if(iter_callid2!=CallIDMap2_C.end())
			{	
				iter_val2=InfoMap2.find(iter_callid2->second);
				if(iter_val2!=InfoMap2.end())
				{
					u_int64_t k,l;
					k=convert2key(u.teid_c,u.sgsn_s);
					l=convert2key(u.teid_d,u.sgsn_u);
				testcallid2=CallIDMap2_C.insert(pair<u_int64_t,long>(k,iter_callid2->second));
			if(testcallid2.second==0) return;
				testcallid2=CallIDMap2_D.insert(pair<u_int64_t,long>(l,iter_callid2->second));
			if(testcallid2.second==0) return;
					CallIDMap2_D.erase(iter_val2->second.teid_t.Downlink_TEID_D);
					CallIDMap2_C.erase(iter_val2->second.teid_t.Downlink_TEID_C);
					iter_val2->second.teid_t.Downlink_TEID_C=k;
					iter_val2->second.teid_t.Downlink_TEID_D=l;
					iter_val2->second.LAC=u.lac;
                                        iter_val2->second.CellID=u.ci;
					iter_val2->second.rat=u.rat;
					iter_val2->second.lasttime=timeTemp2;
					iter_val2->second.pdp_status.seq_update=gtp_protocol->seq;
				}
			}
			break;
		case 19:
				struct Update_Res v;
				v=iedecaps5(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid2=CallIDMap2_C.find(keyMap);
				if(iter_callid2!=CallIDMap2_C.end())
				{
					iter_val2=InfoMap2.find(keyMap);
					if(iter_val2!=InfoMap2.end())
					{
					 	if(iter_val2->second.pdp_status.seq_update!=gtp_protocol->seq) return;
						if(int(v.cause)==128)
						{
							u_int64_t m;
							m=convert2key(v.teid_d,v.ggsn_u);
						testcallid2=CallIDMap2_D.insert(pair<u_int64_t,long>(m,iter_callid2->second));
			if(testcallid2.second==0) return;
							CallIDMap2_D.erase(iter_val2->second.teid_t.Uplink_TEID_D);
							iter_val2->second.teid_t.Uplink_TEID_D=m;
							iter_val2->second.lasttime=timeTemp2;
						}	
					}
				}
				break;
			case 20:
                                struct Delete_Req w;
                                w=iedecaps2(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
                              	iter_callid2=CallIDMap2_C.find(keyMap);
                                if(iter_callid2!=CallIDMap2_C.end())
                                {
					iter_val2=InfoMap2.find(iter_callid2->second);
					if(iter_val2!=InfoMap2.end())
					{
						if(w.Teardown_ID!=0)
						{
							iter_val2->second.pdp_status.Delreq=1;
							iter_val2->second.lasttime=timeTemp2;
							iter_val2->second.pdp_status.seq_delete=gtp_protocol->seq;
						}
					}
                                }
	
			case 21:
				struct Delete_Res x;
				x=iedecaps3(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid2=CallIDMap2_C.find(keyMap);
                                if(iter_callid2!=CallIDMap2_C.end())
				{
					iter_val2=InfoMap2.find(iter_callid2->second);
					if(iter_val2!=InfoMap2.end())
					{
 						if(iter_val2->second.pdp_status.seq_delete!=gtp_protocol->seq) return;
				//		if(iter_val2->second.pdp_status.Delreq==1)
				//		{
						if(int(x.cause)==128)
						{
							iter_val2->second.sdr_time.Stop_Time=timeTemp2;
							GPRS_Pkt_info_t gprs_pkt_info;
                                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                			gprs_pkt_info.timeStamp=timeTemp2;
							gprs_pkt_info.packetLength=len;
                                			gprs_pkt_info.gtp_call_info=iter_val2->second;
							//printf("the call1 is %ld\n",++call1);
							if((strncmp(iter_val2->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmwap",5))==0) 
                                			{
						//		printf("the call1wap is %ld\n",++call1wap);
							//	++call1wap;
								wap_protocol_packet_callback(&(gprs_pkt_info),iter_val2->second.GPRS_PDP_SSN,1,2);
                                			}
                                			if((strncmp(iter_val2->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val2->second.pdp_status.APN,"cmnet",5))==0)
                                			{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
								http_protocol_packet_callback_two(&(gprs_pkt_info),iter_val2->second.GPRS_PDP_SSN);
							}
								//writeCDRFile(&(iter_val2->second),2);
								writeSDRFile(&(iter_val2->second),2);
							CallIDMap2_C.erase(iter_val2->second.teid_t.Uplink_TEID_C);
							CallIDMap2_D.erase(iter_val2->second.teid_t.Uplink_TEID_D);
							CallIDMap2_C.erase(iter_val2->second.teid_t.Downlink_TEID_C);
							CallIDMap2_D.erase(iter_val2->second.teid_t.Downlink_TEID_D);
							InfoMap2.erase(iter_val2);
						}else {
							// 失败，写回
							//iter_val2->second.pdp_status.AttachSuccess='N';
							//writeSDRFile(&(iter_val2->second),2);
						}
					//	}
					}	
				}
				break;
			default:
				break;
		
		}	
}
void GtpPacketHandle_3(u_char *packet_content)
{
std::map<u_int64_t,long>::iterator iter_callid3;
std::map<long,GTP_Call_Info_t>::iterator iter_val3;
 pair<std::map<u_int64_t,long>::iterator,bool> testcallid3;
	struct timeval timeTemp3;
    	gettimeofday(&timeTemp3, NULL);
	if(timeTemp3.tv_sec-mkdirTime_3.tv_sec>=600)
		mkdirTime_3=timeTemp3;


        if(timeTemp3.tv_sec-firstTime3.tv_sec>=2)
        {
                firstTime3=timeTemp3;
                iter_val3=InfoMap3.begin();
                int loopNum=0;
                while((loopNum<100)&&(iter_val3!=InfoMap3.end())){
                        if(timeTemp3.tv_sec-iter_val3->second.lasttime.tv_sec>=180)
                        {
                        iter_val3->second.sdr_time.Stop_Time=timeTemp3;
                        GPRS_Pkt_info_t gprs_pkt_info;
                        gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                        gprs_pkt_info.timeStamp=timeTemp3;
                        gprs_pkt_info.gtp_call_info=iter_val3->second;
                        if((strncmp(iter_val3->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmwap",5))==0)
                        {
                                wap_protocol_packet_callback(&(gprs_pkt_info),iter_val3->second.GPRS_PDP_SSN,2,1);
                                //writeCDRFile(&(iter_val3->second),3);
                                writeSDRFile(&(iter_val3->second),3);
                        }
                        else if((strncmp(iter_val3->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmnet",5))==0)
                        {
                                http_protocol_packet_callback_three(&(gprs_pkt_info),iter_val3->second.GPRS_PDP_SSN);
                                //writeCDRFile(&(iter_val3->second),3);
                                writeSDRFile(&(iter_val3->second),3);
                        }
                        CallIDMap3_C.erase(iter_val3->second.teid_t.Uplink_TEID_C);
                        CallIDMap3_D.erase(iter_val3->second.teid_t.Uplink_TEID_D);
                        CallIDMap3_C.erase(iter_val3->second.teid_t.Downlink_TEID_C);
                        CallIDMap3_D.erase(iter_val3->second.teid_t.Downlink_TEID_D);
                        InfoMap3.erase(iter_val3);
                        iter_val3=InfoMap3.begin();
                        }
                        else
                                ++iter_val3;
                        ++loopNum;
                }
	}






	u_int32_t sourceIp,destinationIp;
        struct ip_hdr *ipOverGtp;
        ipOverGtp=(struct ip_hdr*)(packet_content+14);
        sourceIp=ntohl(ipOverGtp->saddr);
  //   	 printf("***********************%02x\n",sourceIp);
        destinationIp=ntohl(ipOverGtp->daddr);
  //  	printf("***********************%02x\n",destinationIp);
	u_int64_t keyMap=0;
	struct gtpHeader *gtp_protocol;
	gtp_protocol = (struct gtpHeader*)(packet_content+14+20+8);
	int len=0;
	int lgtp=0;
	if(gtp_protocol->flags==0x30)
	{	lgtp=8;
	}
	else if(gtp_protocol->flags==0x32)
	{	lgtp=12;
	}
	else 
	{
		return;
	}
	len=ntohs(gtp_protocol->length);
	if(gtp_protocol->flags==0x32)  len=len-4;
	switch((int)(gtp_protocol->type))
	{
		case 255:
			
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid3=CallIDMap3_D.find(keyMap);
			if(iter_callid3!=CallIDMap3_D.end())
                	{
				iter_val3=InfoMap3.find(iter_callid3->second);
				if(iter_val3!=InfoMap3.end())
				{
					GPRS_Pkt_info_t gprs_pkt_info;
                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content+14+20+8+lgtp);
					iter_val3->second.lasttime=timeTemp3;
					gprs_pkt_info.timeStamp=timeTemp3;
					gprs_pkt_info.packetLength=len;
					if(keyMap==iter_val3->second.teid_t.Downlink_TEID_D)
					{	iter_val3->second.GPRS_DownBytes+=len;
						iter_val3->second.linkFlag=0;
						//printf("the DownBytes is %ld\n",iter_d->second.GPRS_DownBytes);
					}
					else if(keyMap==iter_val3->second.teid_t.Uplink_TEID_D)
					{	 iter_val3->second.GPRS_UpBytes+=len;
						 iter_val3->second.linkFlag=1;
						//printf("the UpBytes is %ld\n",iter_d->second.GPRS_UpBytes);
					}
					else return;
					gprs_pkt_info.gtp_call_info=iter_val3->second;
					if((strncmp(iter_val3->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmwap",5))==0)  
                        		{
					//	printf("the call1wap is %ld\n",++call1wap);
					//	++call1wap;
						wap_protocol_packet_callback(&(gprs_pkt_info),0,2,1);
					
                        		}
                        		else if((strncmp(iter_val3->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmnet",5))==0)
                        		{
			//			printf("the call1http is %ld\n",++call1http);
					//	++call1http;
						http_protocol_packet_callback_three(&(gprs_pkt_info),0);
                        		}
				}
			}
			break;
		case 16:
			struct Create_Req s;
			memset(s.imsi,0,8);
                        memset(s.imei,0,8);           
                        memset(s.msisdn,0,8);           
                        memset(s.apn,0,6);
                        s.teid_d=0;           
                        s.teid_c=0;
			s.sgsn_s=0;
			s.sgsn_u=0;
			s=iedecaps(packet_content,len);
			keyMap=convert2key(s.teid_c,s.sgsn_s);
			iter_callid3=CallIDMap3_C.find(keyMap);
			if(iter_callid3==CallIDMap3_C.end())
			{
                		GTP_Call_Info_t gtpCallInfo_t;
				gtpCallInfo_t.GPRS_Call_ID=tempCallID3++;
				gtpCallInfo_t.teid_t.Downlink_TEID_C=convert2key(s.teid_c,s.sgsn_s);
				gtpCallInfo_t.teid_t.Downlink_TEID_D=convert2key(s.teid_d,s.sgsn_u);
			testcallid3=	CallIDMap3_C.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_C,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid3.second==0) return;
			testcallid3=	CallIDMap3_D.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_D,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid3.second==0) return;
				memset(gtpCallInfo_t.pdp_status.APN,0,6);
				memset(gtpCallInfo_t.MT_IMSI,0,8);
                                memset(gtpCallInfo_t.MT_MSISDN,0,8);
				memset(gtpCallInfo_t.MT_IMEI,0,8);
				memcpy(gtpCallInfo_t.pdp_status.APN,s.apn,6);
				memcpy(gtpCallInfo_t.MT_IMSI,s.imsi,8);
				memcpy(gtpCallInfo_t.MT_IMEI,s.imei,8);
				memcpy(gtpCallInfo_t.MT_MSISDN,s.msisdn,8);
				gtpCallInfo_t.LAC=s.lac;
				gtpCallInfo_t.rat=s.rat;
				gtpCallInfo_t.CellID=s.ci;
				gtpCallInfo_t.GPRS_Intface=2;
				gtpCallInfo_t.pdp_status.seq_create=gtp_protocol->seq;
				tempTime3=timeTemp3;
				gtpCallInfo_t.GPRS_DownBytes=0;
				gtpCallInfo_t.GPRS_UpBytes=0;
				gtpCallInfo_t.lasttime=timeTemp3;
				gtpCallInfo_t.pdp_status.Delreq=0;	
				InfoMap3.insert(pair<long,GTP_Call_Info_t>(gtpCallInfo_t.GPRS_Call_ID,gtpCallInfo_t));
			}
			break;
		case 17:
			struct Create_Res t;
			t.cause=0;
			t.teid_d=0;
			t.teid_c=0;
			t.enduserIP=0;
			t.ggsn_s=0;
			t.ggsn_u=0;
			t=iedecaps1(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid3=CallIDMap3_C.find(keyMap);	
                        if(iter_callid3!=CallIDMap3_C.end())
			{
				iter_val3=InfoMap3.find(iter_callid3->second);
				if(iter_val3!=InfoMap3.end())
				{
					if((iter_val3->second.pdp_status.seq_create!=gtp_protocol->seq)) return;
					if(int(t.cause)==128)
					{
						u_int64_t keyC=0,keyD=0;
						keyC=convert2key(t.teid_c,t.ggsn_s);
						keyD=convert2key(t.teid_d,t.ggsn_u);
						iter_val3->second.teid_t.Uplink_TEID_C=keyC;
                                        	iter_val3->second.teid_t.Uplink_TEID_D=keyD;
                                        	testcallid3=CallIDMap3_C.insert(pair<u_int64_t,long>(keyC,iter_callid3->second));
			if(testcallid3.second==0) return;
                                        	testcallid3=CallIDMap3_D.insert(pair<u_int64_t,long>(keyD,iter_callid3->second));
						if(testcallid3.second==0)
						{
							return;
							std::map<u_int64_t,long>::iterator iter_callidtest;
							std::map<long,GTP_Call_Info_t>::iterator iter_valtest;
							iter_callidtest=CallIDMap3_D.find(keyD);
							if(iter_callidtest!=CallIDMap3_D.end())
							{
								iter_valtest=InfoMap3.find(iter_callidtest->second);
								if(iter_valtest!=InfoMap3.end())
								{
								iter_valtest->second.sdr_time.Stop_Time=timeTemp3;
								GPRS_Pkt_info_t gprs_pkt_info;
                                				gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                				gprs_pkt_info.timeStamp=timeTemp3;
								gprs_pkt_info.packetLength=len;
                                				gprs_pkt_info.gtp_call_info=iter_valtest->second;
							//printf("the call1 is %ld\n",++call1);
								if((strncmp(iter_valtest->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmwap",5))==0) 
                                				{
						//		printf("the call1wap is %ld\n",++call1wap);
									
							//	++call1wap;
									wap_protocol_packet_callback(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN,2,1);
                                				}
                                				if((strncmp(iter_valtest->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmnet",5))==0)
                                				{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
									http_protocol_packet_callback_three(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN);
								}
								//writeCDRFile(&(iter_valtest->second),3);
								writeSDRFile(&(iter_valtest->second),3);
					//		u_int64_t a=iter_callid->second;
								CallIDMap3_C.erase(iter_valtest->second.teid_t.Uplink_TEID_C);
								CallIDMap3_D.erase(iter_valtest->second.teid_t.Uplink_TEID_D);
								CallIDMap3_C.erase(iter_valtest->second.teid_t.Downlink_TEID_C);
								CallIDMap3_D.erase(iter_valtest->second.teid_t.Downlink_TEID_D);
								InfoMap3.erase(iter_valtest);
								}
							}
                                        	CallIDMap3_D.insert(pair<u_int64_t,long>(keyD,iter_callid3->second));
						}	
						iter_val3->second.pdp_status.RspCause=t.cause;
						iter_val3->second.pdp_status.enduserIP=t.enduserIP;
						iter_val3->second.GPRS_PDP_SSN=tempPdpSSN3++;
						iter_val3->second.pdp_status.AttachSuccess='Y';
						iter_val3->second.sdr_time.Start_Time=timeTemp3;
						iter_val3->second.lasttime=timeTemp3;
						if(timeTemp3.tv_sec>=tempTime3.tv_sec)
                                                {
                                                iter_val3->second.gprs_status.AttachDelay=(timeTemp3.tv_sec-tempTime3.tv_sec)*1000000+timeTemp3.tv_usec-tempTime3.tv_usec;
                                                iter_val3->second.pdp_status.pdpReqDelay=(timeTemp3.tv_sec-tempTime3.tv_sec)*1000000+timeTemp3.tv_usec-tempTime3.tv_usec;
                                                }
					}
				
					else
					{
					    // 失败，写回
						iter_val3->second.pdp_status.AttachSuccess='N';
						iter_val3->second.pdp_status.RspCause=t.cause;
						iter_val3->second.pdp_status.enduserIP=t.enduserIP;
						iter_val3->second.GPRS_PDP_SSN=tempPdpSSN3++;
						iter_val3->second.sdr_time.Start_Time=timeTemp3;
						iter_val3->second.sdr_time.Stop_Time=timeTemp3;
						iter_val3->second.lasttime=timeTemp3;
						if(timeTemp3.tv_sec>=tempTime3.tv_sec)
                                                {
                                                iter_val3->second.gprs_status.AttachDelay=(timeTemp3.tv_sec-tempTime3.tv_sec)*1000000+timeTemp3.tv_usec-tempTime3.tv_usec;
                                                iter_val3->second.pdp_status.pdpReqDelay=(timeTemp3.tv_sec-tempTime3.tv_sec)*1000000+timeTemp3.tv_usec-tempTime3.tv_usec;
                                                }
						writeSDRFile(&(iter_val3->second),3);
						
						u_int64_t teid_c=iter_val3->second.teid_t.Downlink_TEID_C;
						u_int64_t teid_d=iter_val3->second.teid_t.Downlink_TEID_D;
						InfoMap3.erase(iter_callid3->second);
						CallIDMap3_C.erase(teid_c);
						CallIDMap3_D.erase(teid_d);
						
					}
				}
			}
			break;
		case 18:
			struct Update_Req u;
			u=iedecaps4(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid3=CallIDMap3_C.find(keyMap);
			if(iter_callid3!=CallIDMap3_C.end())
			{	
				iter_val3=InfoMap3.find(iter_callid3->second);
				if(iter_val3!=InfoMap3.end())
				{
					u_int64_t k,l;
					k=convert2key(u.teid_c,u.sgsn_s);
					l=convert2key(u.teid_d,u.sgsn_u);
				testcallid3=	CallIDMap3_C.insert(pair<u_int64_t,long>(k,iter_callid3->second));
			if(testcallid3.second==0) return;
				testcallid3=	CallIDMap3_D.insert(pair<u_int64_t,long>(l,iter_callid3->second));
			if(testcallid3.second==0) return;
					CallIDMap3_D.erase(iter_val3->second.teid_t.Downlink_TEID_D);
					CallIDMap3_C.erase(iter_val3->second.teid_t.Downlink_TEID_C);
					iter_val3->second.teid_t.Downlink_TEID_C=k;
					iter_val3->second.teid_t.Downlink_TEID_D=l;
					iter_val3->second.LAC=u.lac;
					iter_val3->second.rat=u.rat;
                                        iter_val3->second.CellID=u.ci;
					iter_val3->second.lasttime=timeTemp3;
					iter_val3->second.pdp_status.seq_update=gtp_protocol->seq;
				}
			}
			break;
		case 19:
				struct Update_Res v;
				v=iedecaps5(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid3=CallIDMap3_C.find(keyMap);
				if(iter_callid3!=CallIDMap3_C.end())
				{
					iter_val3=InfoMap3.find(keyMap);
					if(iter_val3!=InfoMap3.end())
					{
 						if(iter_val3->second.pdp_status.seq_update!=gtp_protocol->seq) return;
						if(int(v.cause)==128)
						{
							u_int64_t m;
							m=convert2key(v.teid_d,v.ggsn_u);
						testcallid3=	CallIDMap3_D.insert(pair<u_int64_t,long>(m,iter_callid3->second));
			if(testcallid3.second==0) return;
							CallIDMap3_D.erase(iter_val3->second.teid_t.Uplink_TEID_D);
							iter_val3->second.teid_t.Uplink_TEID_D=m;
							iter_val3->second.lasttime=timeTemp3;
						}	
					}
				}
				break;
			case 20:
                                struct Delete_Req w;
                                w=iedecaps2(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
                              	iter_callid3=CallIDMap3_C.find(keyMap);
                                if(iter_callid3!=CallIDMap3_C.end())
                                {
					iter_val3=InfoMap3.find(iter_callid3->second);
					if(iter_val3!=InfoMap3.end())
					{
						if(w.Teardown_ID!=0)
						{
							iter_val3->second.pdp_status.Delreq=1;
							iter_val3->second.lasttime=timeTemp3;
							iter_val3->second.pdp_status.seq_delete=gtp_protocol->seq;
						}
					}
                                }
	
			case 21:
				struct Delete_Res x;
				x=iedecaps3(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid3=CallIDMap3_C.find(keyMap);
                                if(iter_callid3!=CallIDMap3_C.end())
				{
					iter_val3=InfoMap3.find(iter_callid3->second);
					if(iter_val3!=InfoMap3.end())
					{
 						if(iter_val3->second.pdp_status.seq_delete!=gtp_protocol->seq) return;
					//	if(iter_val3->second.pdp_status.Delreq==1)
					//	{
						if(int(x.cause)==128)
						{
							iter_val3->second.sdr_time.Stop_Time=timeTemp3;
							GPRS_Pkt_info_t gprs_pkt_info;
                                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                			gprs_pkt_info.timeStamp=timeTemp3;
							gprs_pkt_info.packetLength=len;
                                			gprs_pkt_info.gtp_call_info=iter_val3->second;
							//printf("the call1 is %ld\n",++call1);
							if((strncmp(iter_val3->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmwap",5))==0)  
                                			{
						//		printf("the call1wap is %ld\n",++call1wap);
							//	++call1wap;
								wap_protocol_packet_callback(&(gprs_pkt_info),iter_val3->second.GPRS_PDP_SSN,2,1);
                                			}
                                			if((strncmp(iter_val3->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val3->second.pdp_status.APN,"cmnet",5))==0)
                                			{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
								http_protocol_packet_callback_three(&(gprs_pkt_info),iter_val3->second.GPRS_PDP_SSN);
							}
								//writeCDRFile(&(iter_val3->second),3);
								writeSDRFile(&(iter_val3->second),3);
							CallIDMap3_C.erase(iter_val3->second.teid_t.Uplink_TEID_C);
							CallIDMap3_D.erase(iter_val3->second.teid_t.Uplink_TEID_D);
							CallIDMap3_C.erase(iter_val3->second.teid_t.Downlink_TEID_C);
							CallIDMap3_D.erase(iter_val3->second.teid_t.Downlink_TEID_D);
							InfoMap3.erase(iter_val3);
						}else {
							// 失败，写回
							//iter_val3->second.pdp_status.AttachSuccess='N';
							//writeSDRFile(&(iter_val3->second),3);
						}
					//	}
					}	
				}
				break;
			default:
				break;
		
		}	
}
void GtpPacketHandle_4(u_char *packet_content)
{
	std::map<u_int64_t,long>::iterator iter_callid4;
	std::map<long,GTP_Call_Info_t>::iterator iter_val4;
	pair<std::map<u_int64_t,long>::iterator,bool> testcallid4;
	struct timeval timeTemp4;
    	gettimeofday(&timeTemp4, NULL);
	if(timeTemp4.tv_sec-mkdirTime_4.tv_sec>=600)
		mkdirTime_4=timeTemp4;
//if(((++sum2)%1000000)==0)
//{
//printf("the size of fork 4 map is %d\n",InfoMap4.size());
//}

        if(timeTemp4.tv_sec-firstTime4.tv_sec>=2)
        {
                firstTime4=timeTemp4;
                iter_val4=InfoMap4.begin();
                int loopNum=0;
                while((loopNum<100)&&(iter_val4!=InfoMap4.end())){
                        if(timeTemp4.tv_sec-iter_val4->second.lasttime.tv_sec>=180)
                        {
                        iter_val4->second.sdr_time.Stop_Time=timeTemp4;
                        GPRS_Pkt_info_t gprs_pkt_info;
                        gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                        gprs_pkt_info.timeStamp=timeTemp4;
                        gprs_pkt_info.gtp_call_info=iter_val4->second;
                        if((strncmp(iter_val4->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmwap",5))==0)
                        {
                                wap_protocol_packet_callback(&(gprs_pkt_info),iter_val4->second.GPRS_PDP_SSN,2,2);
                                //writeCDRFile(&(iter_val4->second),4);
                                writeSDRFile(&(iter_val4->second),4);
                        }
                        else if((strncmp(iter_val4->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmnet",5))==0)
                        {
                                http_protocol_packet_callback_four(&(gprs_pkt_info),iter_val4->second.GPRS_PDP_SSN);
                                //writeCDRFile(&(iter_val4->second),4);
                                writeSDRFile(&(iter_val4->second),4);
                        }
                        CallIDMap4_C.erase(iter_val4->second.teid_t.Uplink_TEID_C);
                        CallIDMap4_D.erase(iter_val4->second.teid_t.Uplink_TEID_D);
                        CallIDMap4_C.erase(iter_val4->second.teid_t.Downlink_TEID_C);
                        CallIDMap4_D.erase(iter_val4->second.teid_t.Downlink_TEID_D);
                        InfoMap4.erase(iter_val4);
                        iter_val4=InfoMap4.begin();
                        }
                        else
                                ++iter_val4;
                        ++loopNum;
                }
	}

	u_int32_t sourceIp,destinationIp;
        struct ip_hdr *ipOverGtp;
        ipOverGtp=(struct ip_hdr*)(packet_content+14);
        sourceIp=ntohl(ipOverGtp->saddr);
  //   	 printf("***********************%02x\n",sourceIp);
        destinationIp=ntohl(ipOverGtp->daddr);
  //  	printf("***********************%02x\n",destinationIp);
	u_int64_t keyMap=0;
	struct gtpHeader *gtp_protocol;
	gtp_protocol = (struct gtpHeader*)(packet_content+14+20+8);
	int len=0;
	int lgtp=0;
	if(gtp_protocol->flags==0x30)
	{	lgtp=8;
	}
	else if(gtp_protocol->flags==0x32)
	{	lgtp=12;
	}
	else 
	{
		return;
	}
	len=ntohs(gtp_protocol->length);
	if(gtp_protocol->flags==0x32)  len=len-4;
	switch((int)(gtp_protocol->type))
	{
		case 255:
			
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid4=CallIDMap4_D.find(keyMap);
			if(iter_callid4!=CallIDMap4_D.end())
                	{
				iter_val4=InfoMap4.find(iter_callid4->second);
				if(iter_val4!=InfoMap4.end())
				{
					GPRS_Pkt_info_t gprs_pkt_info;
                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content+14+20+8+lgtp);
					iter_val4->second.lasttime=timeTemp4;
					gprs_pkt_info.timeStamp=timeTemp4;
					gprs_pkt_info.packetLength=len;
					if(keyMap==iter_val4->second.teid_t.Downlink_TEID_D)
					{	iter_val4->second.GPRS_DownBytes+=len;
						iter_val4->second.linkFlag=0;
						//printf("the DownBytes is %ld\n",iter_d->second.GPRS_DownBytes);
					}
					else if(keyMap==iter_val4->second.teid_t.Uplink_TEID_D)
					{	 iter_val4->second.GPRS_UpBytes+=len;
						 iter_val4->second.linkFlag=1;
						//printf("the UpBytes is %ld\n",iter_d->second.GPRS_UpBytes);
					}
					else return;
					gprs_pkt_info.gtp_call_info=iter_val4->second;
					if((strncmp(iter_val4->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmwap",5))==0)  
                        		{
					//	printf("the call1wap is %ld\n",++call1wap);
					//	++call1wap;
						wap_protocol_packet_callback(&(gprs_pkt_info),0,2,2);
					
                        		}
                        		else if((strncmp(iter_val4->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmnet",5))==0)
                        		{
			//			printf("the call1http is %ld\n",++call1http);
					//	++call1http;
						http_protocol_packet_callback_four(&(gprs_pkt_info),0);
                        		}
				}
			}
			break;
		case 16:
			struct Create_Req s;
			memset(s.imsi,0,8);
                        memset(s.imei,0,8);           
                        memset(s.msisdn,0,8);           
                        memset(s.apn,0,6);
                        s.teid_d=0;           
                        s.teid_c=0;
			s.sgsn_s=0;
			s.sgsn_u=0;
			s=iedecaps(packet_content,len);
			keyMap=convert2key(s.teid_c,s.sgsn_s);
			iter_callid4=CallIDMap4_C.find(keyMap);
			if(iter_callid4==CallIDMap4_C.end())
			{
                		GTP_Call_Info_t gtpCallInfo_t;
				gtpCallInfo_t.GPRS_Call_ID=tempCallID4++;
				gtpCallInfo_t.teid_t.Downlink_TEID_C=convert2key(s.teid_c,s.sgsn_s);
				gtpCallInfo_t.teid_t.Downlink_TEID_D=convert2key(s.teid_d,s.sgsn_u);
			testcallid4=	CallIDMap4_C.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_C,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid4.second==0) return;
			testcallid4=CallIDMap4_D.insert(pair<u_int64_t,long>(gtpCallInfo_t.teid_t.Downlink_TEID_D,gtpCallInfo_t.GPRS_Call_ID));
			if(testcallid4.second==0) return;
				memset(gtpCallInfo_t.pdp_status.APN,0,6);
				memset(gtpCallInfo_t.MT_IMSI,0,8);
                                memset(gtpCallInfo_t.MT_MSISDN,0,8);
				memset(gtpCallInfo_t.MT_IMEI,0,8);
				memcpy(gtpCallInfo_t.pdp_status.APN,s.apn,6);
				memcpy(gtpCallInfo_t.MT_IMSI,s.imsi,8);
				memcpy(gtpCallInfo_t.MT_IMEI,s.imei,8);
				memcpy(gtpCallInfo_t.MT_MSISDN,s.msisdn,8);
				gtpCallInfo_t.LAC=s.lac;
				gtpCallInfo_t.rat=s.rat;
				gtpCallInfo_t.CellID=s.ci;
				gtpCallInfo_t.GPRS_Intface=2;
				gtpCallInfo_t.pdp_status.seq_create=gtp_protocol->seq;
				tempTime4=timeTemp4;
				gtpCallInfo_t.GPRS_DownBytes=0;
				gtpCallInfo_t.GPRS_UpBytes=0;
				gtpCallInfo_t.lasttime=timeTemp4;
				gtpCallInfo_t.pdp_status.Delreq=0;	
				InfoMap4.insert(pair<long,GTP_Call_Info_t>(gtpCallInfo_t.GPRS_Call_ID,gtpCallInfo_t));
			}
			break;
		case 17:
			struct Create_Res t;
			t.cause=0;
			t.teid_d=0;
			t.teid_c=0;
			t.enduserIP=0;
			t.ggsn_s=0;
			t.ggsn_u=0;
			t=iedecaps1(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid4=CallIDMap4_C.find(keyMap);	
                        if(iter_callid4!=CallIDMap4_C.end())
			{
				iter_val4=InfoMap4.find(iter_callid4->second);
				if(iter_val4!=InfoMap4.end())
				{
					if((iter_val4->second.pdp_status.seq_create!=gtp_protocol->seq)) return;
					if(int(t.cause)==128)
					{
						u_int64_t keyC=0,keyD=0;
						keyC=convert2key(t.teid_c,t.ggsn_s);
						keyD=convert2key(t.teid_d,t.ggsn_u);
						iter_val4->second.teid_t.Uplink_TEID_C=keyC;
                                        	iter_val4->second.teid_t.Uplink_TEID_D=keyD;
                                        	testcallid4=CallIDMap4_C.insert(pair<u_int64_t,long>(keyC,iter_callid4->second));
			if(testcallid4.second==0) return;
                                        	testcallid4=CallIDMap4_D.insert(pair<u_int64_t,long>(keyD,iter_callid4->second));
						if(testcallid4.second==0)
						{
							return;
							std::map<u_int64_t,long>::iterator iter_callidtest;
							std::map<long,GTP_Call_Info_t>::iterator iter_valtest;
							iter_callidtest=CallIDMap4_D.find(keyD);
							if(iter_callidtest!=CallIDMap4_D.end())
							{
								iter_valtest=InfoMap4.find(iter_callidtest->second);
								if(iter_valtest!=InfoMap4.end())
								{
								iter_valtest->second.sdr_time.Stop_Time=timeTemp4;
								GPRS_Pkt_info_t gprs_pkt_info;
                                				gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                				gprs_pkt_info.timeStamp=timeTemp4;
								gprs_pkt_info.packetLength=len;
                                				gprs_pkt_info.gtp_call_info=iter_valtest->second;
							//printf("the call1 is %ld\n",++call1);
								if((strncmp(iter_valtest->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmwap",5))==0) 
                                				{
						//		printf("the call1wap is %ld\n",++call1wap);
									
							//	++call1wap;
									wap_protocol_packet_callback(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN,2,2);
                                				}
                                				if((strncmp(iter_valtest->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_valtest->second.pdp_status.APN,"cmnet",5))==0)
                                				{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
									http_protocol_packet_callback_four(&(gprs_pkt_info),iter_valtest->second.GPRS_PDP_SSN);
								}
								//writeCDRFile(&(iter_valtest->second),4);
								writeSDRFile(&(iter_valtest->second),4);
					//		u_int64_t a=iter_callid->second;
								CallIDMap4_C.erase(iter_valtest->second.teid_t.Uplink_TEID_C);
								CallIDMap4_D.erase(iter_valtest->second.teid_t.Uplink_TEID_D);
								CallIDMap4_C.erase(iter_valtest->second.teid_t.Downlink_TEID_C);
								CallIDMap4_D.erase(iter_valtest->second.teid_t.Downlink_TEID_D);
								InfoMap4.erase(iter_valtest);
								}
							}
                                        	CallIDMap4_D.insert(pair<u_int64_t,long>(keyD,iter_callid4->second));
						}	
						iter_val4->second.pdp_status.RspCause=t.cause;
						iter_val4->second.pdp_status.enduserIP=t.enduserIP;
						iter_val4->second.GPRS_PDP_SSN=tempPdpSSN4++;
						iter_val4->second.pdp_status.AttachSuccess='Y';
						iter_val4->second.sdr_time.Start_Time=timeTemp4;
						iter_val4->second.lasttime=timeTemp4;
						if(timeTemp4.tv_sec>=tempTime4.tv_sec)
                                                {
                                                iter_val4->second.gprs_status.AttachDelay=(timeTemp4.tv_sec-tempTime4.tv_sec)*1000000+timeTemp4.tv_usec-tempTime4.tv_usec;
                                                iter_val4->second.pdp_status.pdpReqDelay=(timeTemp4.tv_sec-tempTime4.tv_sec)*1000000+timeTemp4.tv_usec-tempTime4.tv_usec;
                                                }
					}
				
					else
					{
					    // 失败，写回
						iter_val4->second.pdp_status.AttachSuccess='N';
						iter_val4->second.pdp_status.RspCause=t.cause;
						iter_val4->second.pdp_status.enduserIP=t.enduserIP;
						iter_val4->second.GPRS_PDP_SSN=tempPdpSSN4++;
						iter_val4->second.sdr_time.Start_Time=timeTemp4;
						iter_val4->second.sdr_time.Stop_Time=timeTemp4;
						iter_val4->second.lasttime=timeTemp4;
						if(timeTemp4.tv_sec>=tempTime4.tv_sec)
                                                {
                                                iter_val4->second.gprs_status.AttachDelay=(timeTemp4.tv_sec-tempTime4.tv_sec)*1000000+timeTemp4.tv_usec-tempTime4.tv_usec;
                                                iter_val4->second.pdp_status.pdpReqDelay=(timeTemp4.tv_sec-tempTime4.tv_sec)*1000000+timeTemp4.tv_usec-tempTime4.tv_usec;
                                                }
						writeSDRFile(&(iter_val4->second),4);
						
						u_int64_t teid_c=iter_val4->second.teid_t.Downlink_TEID_C;
						u_int64_t teid_d=iter_val4->second.teid_t.Downlink_TEID_D;
						InfoMap4.erase(iter_callid4->second);
						CallIDMap4_C.erase(teid_c);
						CallIDMap4_D.erase(teid_d);
						
					}
				}
			}
			break;
		case 18:
			struct Update_Req u;
			u=iedecaps4(packet_content,len);
			keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
			iter_callid4=CallIDMap4_C.find(keyMap);
			if(iter_callid4!=CallIDMap4_C.end())
			{	
				iter_val4=InfoMap4.find(iter_callid4->second);
				if(iter_val4!=InfoMap4.end())
				{
					u_int64_t k,l;
					k=convert2key(u.teid_c,u.sgsn_s);
					l=convert2key(u.teid_d,u.sgsn_u);
				testcallid4=	CallIDMap4_C.insert(pair<u_int64_t,long>(k,iter_callid4->second));
			if(testcallid4.second==0) return;
				testcallid4=	CallIDMap4_D.insert(pair<u_int64_t,long>(l,iter_callid4->second));
			if(testcallid4.second==0) return;
					CallIDMap4_D.erase(iter_val4->second.teid_t.Downlink_TEID_D);
					CallIDMap4_C.erase(iter_val4->second.teid_t.Downlink_TEID_C);
					iter_val4->second.teid_t.Downlink_TEID_C=k;
					iter_val4->second.teid_t.Downlink_TEID_D=l;
					iter_val4->second.LAC=u.lac;
					iter_val4->second.rat=u.rat;
                                        iter_val4->second.CellID=u.ci;
					iter_val4->second.lasttime=timeTemp4;
					iter_val4->second.pdp_status.seq_update=gtp_protocol->seq;
				}
			}
			break;
		case 19:
				struct Update_Res v;
				v=iedecaps5(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid4=CallIDMap4_C.find(keyMap);
				if(iter_callid4!=CallIDMap4_C.end())
				{
					iter_val4=InfoMap4.find(keyMap);
					if(iter_val4!=InfoMap4.end())
					{
						if(iter_val4->second.pdp_status.seq_update!=gtp_protocol->seq) return;
						if(int(v.cause)==128)
						{
							u_int64_t m;
							m=convert2key(v.teid_d,v.ggsn_u);
						testcallid4=	CallIDMap4_D.insert(pair<u_int64_t,long>(m,iter_callid4->second));
			if(testcallid4.second==0) return;
							CallIDMap4_D.erase(iter_val4->second.teid_t.Uplink_TEID_D);
							iter_val4->second.teid_t.Uplink_TEID_D=m;
							iter_val4->second.lasttime=timeTemp4;
						}	
					}
				}
				break;
			case 20:
                                struct Delete_Req w;
                                w=iedecaps2(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
                              	iter_callid4=CallIDMap4_C.find(keyMap);
                                if(iter_callid4!=CallIDMap4_C.end())
                                {
					iter_val4=InfoMap4.find(iter_callid4->second);
					if(iter_val4!=InfoMap4.end())
					{
						if(w.Teardown_ID!=0)
						{
							iter_val4->second.pdp_status.Delreq=1;
							iter_val4->second.lasttime=timeTemp4;
							iter_val4->second.pdp_status.seq_delete=gtp_protocol->seq;
						}
					}
                                }
	
			case 21:
				struct Delete_Res x;
				x=iedecaps3(packet_content,len);
				keyMap=convert2key(ntohl(gtp_protocol->teid),destinationIp);
				iter_callid4=CallIDMap4_C.find(keyMap);
                                if(iter_callid4!=CallIDMap4_C.end())
				{
					iter_val4=InfoMap4.find(iter_callid4->second);
					if(iter_val4!=InfoMap4.end())
					{
						if(iter_val4->second.pdp_status.seq_delete!=gtp_protocol->seq) return;
					//	if(iter_val4->second.pdp_status.Delreq==1)
					//	{
						if(int(x.cause)==128)
						{
							iter_val4->second.sdr_time.Stop_Time=timeTemp4;
							GPRS_Pkt_info_t gprs_pkt_info;
                                			gprs_pkt_info.gtp_pkt=(u_char*)(packet_content)+50;
                                			gprs_pkt_info.timeStamp=timeTemp4;
							gprs_pkt_info.packetLength=len;
                                			gprs_pkt_info.gtp_call_info=iter_val4->second;
							//printf("the call1 is %ld\n",++call1);
							if((strncmp(iter_val4->second.pdp_status.APN,"CMWAP",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmwap",5))==0) 
                                			{
						//		printf("the call1wap is %ld\n",++call1wap);
							//	++call1wap;
								wap_protocol_packet_callback(&(gprs_pkt_info),iter_val4->second.GPRS_PDP_SSN,2,2);
                                			}
                                			if((strncmp(iter_val4->second.pdp_status.APN,"CMNET",5)&&strncmp(iter_val4->second.pdp_status.APN,"cmnet",5))==0)
                                			{
							//	printf("the call1http is %ld\n",++call1http);
							//	++call1http;
								http_protocol_packet_callback_four(&(gprs_pkt_info),iter_val4->second.GPRS_PDP_SSN);
							}
								//writeCDRFile(&(iter_val4->second),4);
								writeSDRFile(&(iter_val4->second),4);
							CallIDMap4_C.erase(iter_val4->second.teid_t.Uplink_TEID_C);
							CallIDMap4_D.erase(iter_val4->second.teid_t.Uplink_TEID_D);
							CallIDMap4_C.erase(iter_val4->second.teid_t.Downlink_TEID_C);
							CallIDMap4_D.erase(iter_val4->second.teid_t.Downlink_TEID_D);
							InfoMap4.erase(iter_val4);
						}else {
							// 失败，写回
							//iter_val4->second.pdp_status.AttachSuccess='N';
							//writeSDRFile(&(iter_val4->second),4);
						}
					//	}
					}	
				}
				break;
			default:
				break;
		
		}	
}
struct Create_Req iedecaps(const u_char* packet_content,int len)
{
//	printf("the callid is %ld\n",tempCallID);
	int i;
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Create_Req Create_Req_t;
	memset(Create_Req_t.imsi,0,8);
	memset(Create_Req_t.imei,0,8);
	memset(Create_Req_t.msisdn,0,8);
	memset(Create_Req_t.apn,0,6);
	Create_Req_t.sgsn_s=0;
	Create_Req_t.sgsn_u=0;
	Create_Req_t.teid_d=0;
	Create_Req_t.teid_c=0;	
	end = (unsigned char*)(packet_content+14+20+8+12+len);
	p =(unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 2:	
				for(i=0;i<8;i++)
				{
					Create_Req_t.imsi[i]=SWAP8(*(p+1+i));
				//	printf("%02x",Create_Req_t.imsi[i]);
				}
			//	printf("\n");
				p=p+9;
				break;
			case 3:
				p=p+7;
				break;
			case 15:
				p=p+2;
				break;
			case 16:
				memcpy(&(Create_Req_t.teid_d),p+1,4);
				Create_Req_t.teid_d=ntohl(Create_Req_t.teid_d);
				p=p+5;
				break;
			case 17:
				memcpy(&(Create_Req_t.teid_c),p+1,4);
				Create_Req_t.teid_c=ntohl(Create_Req_t.teid_c);
				p=p+5;
				break;
			case 20:
				p=p+2;
				break;
			case 26:
				p=p+3;
				break;
			case 131:
				ielen=0;
				ielen=getlen(p);
				for(i=0;i<5;i++)
				{
					Create_Req_t.apn[i]=*(p+4+i);
				}
				Create_Req_t.apn[5]=0;
				p=p+3+ielen;
				break;
			case 128:
			case 132:
				ielen=0;
                                ielen=getlen(p);
                                p=p+3+ielen;
                                break;
			case 133:
				ielen=0;
                                ielen=getlen(p);
				if(Create_Req_t.sgsn_s==0)
				{
					memcpy(&(Create_Req_t.sgsn_s),p+3,4);
					Create_Req_t.sgsn_s=ntohl(Create_Req_t.sgsn_s);
					//printf("%lx\n",Create_Req_t.sgsn_s);
				}
				else 
				{
					memcpy(&(Create_Req_t.sgsn_u),p+3,4);
					Create_Req_t.sgsn_u=ntohl(Create_Req_t.sgsn_u);
				}
				p=p+3+ielen;
                                break;
			case 135:
				ielen=0;
				ielen=getlen(p);
				p=p+3+ielen;
				break;
			case 134:
				ielen=0;
				ielen=getlen(p);
				for(i=0;i<7;i++)
                		{ 
					Create_Req_t.msisdn[i]=SWAP8(*(p+4+i));
                		//	printf("%02x",Create_Req_t.msisdn[i]);
				}
			//	printf("\n");
				p=p+3+ielen;
				break;
			case 151:
				ielen=0;
				ielen=getlen(p);
				Create_Req_t.rat=*(p+3);
				p=p+3+ielen;
				break;
			case 152:
				ielen=0;
				ielen=getlen(p);
				memcpy(&(Create_Req_t.lac),p+7,2);
				memcpy(&(Create_Req_t.ci),p+9,2);
				Create_Req_t.lac=ntohs(Create_Req_t.lac);
				Create_Req_t.ci=ntohs(Create_Req_t.ci);
				p=p+3+ielen;
				break;	
                        case 153:
				ielen=0;
                                ielen=getlen(p);
				p=p+3+ielen;
                                break;
                        case 154:
                                ielen=0;
                                ielen=getlen(p);         
		                for(i=0;i<8;i++)
                                {
                                        
					Create_Req_t.imei[i]=SWAP8(*(p+3+i));
			//		if(Create_Req_t.imei[i]>0x99)
			//			Create_Req_t.imei[i]=0;
                        //        	printf("%02x",Create_Req_t.imei[i]);
				}
			//	printf("\n");
                                p=p+3+ielen;
                              // return Create_Req_t;
				break;
                        default:
                                p=p+len;
			//	return Create_Req_t;
				break;
			}
		}
		return Create_Req_t;
}

struct Create_Res iedecaps1(const u_char* packet_content, int len)
{	
//	printf("the createid is %ld\n",createid++); 
	int i;
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Create_Res Create_Res_t;
	Create_Res_t.cause=0;
	Create_Res_t.teid_d=0;
	Create_Res_t.teid_c=0;
	Create_Res_t.ggsn_s=0;
	Create_Res_t.ggsn_u=0;
	end = (unsigned char*) (packet_content+14+20+8+12+len);
	p =(unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 1:
				Create_Res_t.cause=*(p+1);
		//		printf("the value of cause is %d\n",Create_Res_t.cause);
				p=p+2;
				break;
			case 8:
			case 14:
				p=p+2;
				break;
			case 16:
			//	memset(&(Create_Res_t.teid_d),0,4);
				memcpy(&(Create_Res_t.teid_d),p+1,4);
				Create_Res_t.teid_d=ntohl(Create_Res_t.teid_d);
		//		printf("the value of teid_d is %08x\n",Create_Res_t.teid_d);
				p=p+5;
				break;
			case 17:
			//	memset(&(Create_Res_t.teid_c),0,4);
				memcpy(&(Create_Res_t.teid_c),p+1,4);
				Create_Res_t.teid_c=ntohl(Create_Res_t.teid_c);
		//		printf("the value of teid_c is %08x\n",Create_Res_t.teid_c);
				p=p+5;
				break;
			case 127:
				p=p+5;
				break;
			case 128:
				ielen=0;
				ielen=getlen(p);	
			//	for(i=0;i<4;i++)
                          //      {
                            //            Create_Res_t.enduserIP[i]=*(p+5+i);
		//		memset(&(Create_Res_t.enduserIP),0,4);
				memcpy(&(Create_Res_t.enduserIP),p+5,4);
				Create_Res_t.enduserIP=ntohl(Create_Res_t.enduserIP);
			//	printf("the enduser ip is %08x",Create_Res_t.enduserIP);
		//		}
				p=p+3+ielen;
				break;
			case 133:
				ielen=0;
				ielen=getlen(p); 
				if(Create_Res_t.ggsn_s==0)
                                {
                                        memcpy(&(Create_Res_t.ggsn_s),p+3,4);
                                        Create_Res_t.ggsn_s=ntohl(Create_Res_t.ggsn_s);
                                }
                                else 
                                {
                                        memcpy(&(Create_Res_t.ggsn_u),p+3,4);
                                        Create_Res_t.ggsn_u=ntohl(Create_Res_t.ggsn_u);
                                }
				p=p+3+ielen;
				break;
			case 132:
			case 135:
			case 251:
				ielen=0;
				ielen=getlen(p);
				p=p+3+ielen;
				//return Create_Res_t;
				break;
			default:
		//		printf("***************************\n");
				p=p+len;
			//	return Create_Res_t;
				break;
		}
	}	
	return Create_Res_t;
}
struct Delete_Req iedecaps2(const u_char* packet_content, int len)
{
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Delete_Req Delete_Req_t;
	Delete_Req_t.Teardown_ID=0;
	Delete_Req_t.nsapi=0;
	end = (unsigned char*)(packet_content+14+20+8+12 + len);
	p = (unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 19:
				Delete_Req_t.Teardown_ID=*(p+1);
				p=p+2;
				break;
			case 20:
				Delete_Req_t.nsapi=*(p+1);
				p=p+2;
				break;
			default:
		//		printf("***************************");
				p=p+len;
		//		return Delete_Req_t;
				break;		
		}
	}
	return Delete_Req_t;
}

struct Delete_Res iedecaps3(const u_char* packet_content, int len)
{
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Delete_Res Delete_Res_t;
	end = (unsigned char*)(packet_content+14+20+8+12 + len);
	p = (unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 1:
				Delete_Res_t.cause=*(p+1);
		//		printf("the cause is %d\n",Delete_Res_t.cause);
				p=p+2;
				break;
			default:
		//		printf("***************************\n");
				p=p+len;
	//			return Delete_Res_t;
				break;
		}
	}
		return Delete_Res_t;
}

struct Update_Req iedecaps4(const u_char* packet_content, int len)
{
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Update_Req Update_Req_t;
	Update_Req_t.teid_c=0;
	Update_Req_t.teid_d=0;
	Update_Req_t.sgsn_s=0;
	Update_Req_t.sgsn_u=0;
	end = (unsigned char*)(packet_content+14+20+8+12 + len);
	p = (unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 2:
				p=p+9;
				break;
			case 3:
				p=p+7;
				break;
			case 14:
				p=p+2;
				break;
			case 16:
		//		memset(&(Update_Req_t.teid_d),0,4);
				memcpy(&(Update_Req_t.teid_d),p+1,4);
				Update_Req_t.teid_d=ntohl(Update_Req_t.teid_d);
		//		printf("the value of teid_d is %08x\n",Update_Req_t.teid_d);
				p=p+5;
				break;
			case 17:
		//		memset(&(Update_Req_t.teid_c),0,4);
				memcpy(&(Update_Req_t.teid_c),p+1,4);
				Update_Req_t.teid_c=ntohl(Update_Req_t.teid_c);
		//		printf("the value of teid_d is %08x\n",Update_Req_t.teid_c);
				p=p+5;
				break;
			case 20:
				p=p+2;
				break;
			case 133:
				ielen=0;
                                ielen=getlen(p);
                                if(Update_Req_t.sgsn_s==0)
                                {
                                        memcpy(&(Update_Req_t.sgsn_s),p+3,4);
                                        Update_Req_t.sgsn_s=ntohl(Update_Req_t.sgsn_s);
                                }
                                else
                                {
                                        memcpy(&(Update_Req_t.sgsn_u),p+3,4);
                                        Update_Req_t.sgsn_u=ntohl(Update_Req_t.sgsn_u);
                                }
                                p=p+3+ielen;
                                break;	


			case 135:
				ielen=getlen(p);
				p=p+3+ielen;
			//	return Update_Req_t;
				break;
			
			case 151:
				ielen=getlen(p);
				Update_Req_t.rat=*(p+3);	
				p=p+3+ielen;
				break;
			case 152:
				ielen=getlen(p);
				memcpy(&(Update_Req_t.lac),p+7,2);
				memcpy(&(Update_Req_t.ci),p+9,2);
				Update_Req_t.lac=ntohs(Update_Req_t.lac);
				Update_Req_t.ci=ntohs(Update_Req_t.ci);
				p=p+3+ielen;
				break;
			case 148:
			case 153:
			case 182:
				ielen=getlen(p);
				p=p+3+ielen;
				break;
			
			default:
		//		printf("***************************");
				p=p+len;
			//	return Update_Req_t;
				break;
		}
	}
	return Update_Req_t;
}


struct Update_Res iedecaps5(const u_char* packet_content, int len)
{
	int ielen;
	unsigned char *p;
	unsigned char *end;
	struct Update_Res Update_Res_t ;
	Update_Res_t.cause=0;
	Update_Res_t.teid_d=0;
	Update_Res_t.ggsn_s=0;
	Update_Res_t.ggsn_u=0;
	end = (unsigned char*)(packet_content+14+20+8+12+ len);
	p = (unsigned char*)(packet_content+14+20+8+12);
	while(p<end)
	{
		switch(*p)
		{
			case 1:	
				Update_Res_t.cause=*(p+1);
				p=p+2;
				break;
			case 14:
				p=p+2;
				break;
			case 16:
				memcpy(&(Update_Res_t.teid_d),p+1,4);
				Update_Res_t.teid_d=ntohl(Update_Res_t.teid_d);
		//		printf("the value of teid_d is %08x\n",Update_Res_t.teid_d);
				p=p+5;
				break;
			case 127:
				p=p+5;
				break;
			case 133:
				ielen=0;
                                ielen=getlen(p);
                                if(Update_Res_t.ggsn_s==0)
                                {
                                        memcpy(&(Update_Res_t.ggsn_s),p+3,4);
                                        Update_Res_t.ggsn_s=ntohl(Update_Res_t.ggsn_s);
                                }
                                else
                                {
                                        memcpy(&(Update_Res_t.ggsn_u),p+3,4);
                                        Update_Res_t.ggsn_u=ntohl(Update_Res_t.ggsn_u);
                                }
                                p=p+3+ielen;
                                break;
			case 135:
			case 251:
				ielen=0;
				ielen=getlen(p);
				p=p+3+ielen;
				break;
			default:
				p=p+len;
			//	return Update_Res_t;
				break;
		}
	}
	return	Update_Res_t;
}


void writeSDRFile(GTP_Call_Info_t* gtpCallInfo,int id)
{
        
	char filename1[100];
	char filename2[100];
	char filename3[100];
	char filename4[100];
        struct tm tmmake1;
        struct tm tmmake2;
        struct tm tmmake3;
        struct tm tmmake4;
	  ofstream outfile;
        switch(id)
        {
                case 1:
                
                       // memcpy(&tmmake1,LOCALTIME_R(&(mkdirTime_1.tv_sec)),sizeof(struct tm));
			localtime_r(&(mkdirTime_1.tv_sec),&tmmake1);
			sprintf(filename1,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-1/RT_GPRSPDPSDR.tmp",
                	tmmake1.tm_year+1900,
                	tmmake1.tm_mon+1,
                	tmmake1.tm_mday,
               		tmmake1.tm_hour,
                	tmmake1.tm_min,
                	tmmake1.tm_sec);
        		check_make_path(filename1);
			chown(filename1,500,500);
			 outfile.open(filename1,ofstream::app);
        		if(!outfile)
                		cout<<"unable to open ..."<<endl;
			break;
               
                case 2:
                
                	//memcpy(&tmmake2,localtime_r(&(mkdirTime_2.tv_sec)),sizeof(struct tm));
			localtime_r(&(mkdirTime_2.tv_sec),&tmmake2);
        		sprintf(filename2,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-2/RT_GPRSPDPSDR.tmp",
               	 	tmmake2.tm_year+1900,
                	tmmake2.tm_mon+1,
                	tmmake2.tm_mday,
                	tmmake2.tm_hour,
                	tmmake2.tm_min,
                	tmmake2.tm_sec);
        		check_make_path(filename2);
			chown(filename2,500,500);
			outfile.open(filename2,ofstream::app);
        		if(!outfile)
                		cout<<"unable to open ..."<<endl;
			break;
                case 3:
                
                       // memcpy(&tmmake1,LOCALTIME_R(&(mkdirTime_1.tv_sec)),sizeof(struct tm));
			localtime_r(&(mkdirTime_3.tv_sec),&tmmake3);
			sprintf(filename3,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-3/RT_GPRSPDPSDR.tmp",
                	tmmake3.tm_year+1900,
                	tmmake3.tm_mon+1,
                	tmmake3.tm_mday,
               		tmmake3.tm_hour,
                	tmmake3.tm_min,
                	tmmake3.tm_sec);
        		check_make_path(filename3);
			chown(filename3,500,500);
			outfile.open(filename3,ofstream::app);
			if(!outfile)
				cout<<"unable to open..."<<endl;
			break;
                case 4:
                
			localtime_r(&(mkdirTime_4.tv_sec),&tmmake4);
			sprintf(filename4,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-4/RT_GPRSPDPSDR.tmp",
                	tmmake4.tm_year+1900,
                	tmmake4.tm_mon+1,
                	tmmake4.tm_mday,
               		tmmake4.tm_hour,
                	tmmake4.tm_min,
                	tmmake4.tm_sec);
        		check_make_path(filename4);
			chown(filename4,500,500);
			outfile.open(filename4,ofstream::app);
			if(!outfile)
				cout<<"unable to open..."<<endl;
			break;
                default:
			return;
                        break;
        }


	char strAttachSuccess[100];
	char strGPRS_Intface[100];
	char strpdpReqDelay[100];
	char strStartTime[100];
	char strStopTime[100];
	char strCallID[100];
	char strPdpSSN[100];
	char strIMSI[100];
	char strIMEI[100];
	char strMSISDN[100];
	char strAPN[100];
	char strUpBytes[100];
	char strDownBytes[100];
	char strLAC[100];
	char strCellID[100];
	char strrat[100];
	char strf8IMEI[100];
	u_int8_t endIp[100];
	char strenduserIP[100];
	struct tm tmStart; 
        struct tm tmStop;
//	ofstream outfile;
//        outfile.open(filename,ofstream::app);
//        if(!outfile)
  //              cout<<"unable to open ..."<<endl;
	sprintf(strGPRS_Intface,"%d",gtpCallInfo->GPRS_Intface);
	sprintf(strAttachSuccess,"%c",gtpCallInfo->pdp_status.AttachSuccess);
	sprintf(strLAC,"%ld",gtpCallInfo->LAC);
	sprintf(strCellID,"%ld",gtpCallInfo->CellID);
	sprintf(strrat,"%d",gtpCallInfo->rat);
//	memcpy(&tmStart,localtime_r(&(gtpCallInfo->sdr_time.Start_Time.tv_sec)),sizeof(struct tm));
        localtime_r(&(gtpCallInfo->sdr_time.Start_Time.tv_sec),&tmStart);
        sprintf(strStartTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		tmStart.tm_year+1900,
		tmStart.tm_mon+1,
		tmStart.tm_mday,
		tmStart.tm_hour,
		tmStart.tm_min,
		tmStart.tm_sec,
		int((gtpCallInfo->sdr_time.Start_Time.tv_usec)/1000));
//	memcpy(&tmStop,localtime_r(&(gtpCallInfo->sdr_time.Stop_Time.tv_sec)),sizeof(struct tm));
        localtime_r(&(gtpCallInfo->sdr_time.Stop_Time.tv_sec),&tmStop);
        sprintf(strStopTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tmStop.tm_year+1900,
                tmStop.tm_mon+1,
                tmStop.tm_mday,
                tmStop.tm_hour,
                tmStop.tm_min,
                tmStop.tm_sec,
            	int((gtpCallInfo->sdr_time.Stop_Time.tv_usec)/1000));
        
	sprintf(strpdpReqDelay,"%ld",long(gtpCallInfo->pdp_status.pdpReqDelay/1000));
	sprintf(strCallID,"%ld",gtpCallInfo->GPRS_Call_ID);
	sprintf(strUpBytes,"%ld",gtpCallInfo->GPRS_UpBytes);
	sprintf(strDownBytes,"%ld",gtpCallInfo->GPRS_DownBytes);
	sprintf(strAPN,"%s",gtpCallInfo->pdp_status.APN);
	sprintf(strPdpSSN,"%ld",gtpCallInfo->GPRS_PDP_SSN);
	sprintf(strIMSI,"%02x%02x%02x%02x%02x%02x%02x%x",
		gtpCallInfo->MT_IMSI[0],
		gtpCallInfo->MT_IMSI[1],
		gtpCallInfo->MT_IMSI[2],
		gtpCallInfo->MT_IMSI[3],
		gtpCallInfo->MT_IMSI[4],
		gtpCallInfo->MT_IMSI[5],
		gtpCallInfo->MT_IMSI[6],
		(gtpCallInfo->MT_IMSI[7]&0xF0)>>4);
	sprintf(strIMEI,"%02x%02x%02x%02x%02x%02x%02x%02x",
                gtpCallInfo->MT_IMEI[0],
                gtpCallInfo->MT_IMEI[1],
                gtpCallInfo->MT_IMEI[2],
                gtpCallInfo->MT_IMEI[3],
                gtpCallInfo->MT_IMEI[4],
                gtpCallInfo->MT_IMEI[5],
                gtpCallInfo->MT_IMEI[6],
                gtpCallInfo->MT_IMEI[7]);
/*
	sprintf(strf8IMEI,"%02x%02x%02x%02x",
                gtpCallInfo->MT_IMEI[0],
                gtpCallInfo->MT_IMEI[1],
                gtpCallInfo->MT_IMEI[2],
                gtpCallInfo->MT_IMEI[3]);
*/
	sprintf(strMSISDN,"%02x%02x%02x%02x%02x%02x%x",
                gtpCallInfo->MT_MSISDN[0],
                gtpCallInfo->MT_MSISDN[1],
		gtpCallInfo->MT_MSISDN[2],
		gtpCallInfo->MT_MSISDN[3],
		gtpCallInfo->MT_MSISDN[4],
		gtpCallInfo->MT_MSISDN[5],
		(gtpCallInfo->MT_MSISDN[6]&0xF0)>>4);
	memcpy(endIp,&(gtpCallInfo->pdp_status.enduserIP),4);
	sprintf(strenduserIP,"%d.%d.%d.%d",endIp[3],endIp[2],endIp[1],endIp[0]);
 	outfile<<strPdpSSN
		<<"\t"<<strIMSI
		<<"\t"<<strMSISDN
		<<"\t"<<strLAC
		<<"\t"<<strCellID
		<<"\t"<<strAttachSuccess
		<<"\t"<<strpdpReqDelay
		<<"\t"
		<<"\t"<<strAPN
		<<"\t"<<strenduserIP
		<<"\t"
		<<"\t"
		<<"\t"<<strUpBytes
		<<"\t"<<strDownBytes
		<<"\t"<<strIMEI
		<<"\t"<<strrat
		<<"\t"<<strStartTime
		<<"\t"<<strStopTime
		<<endl;
	outfile.close();
}

/*

void writeCDRFile(GTP_Call_Info_t* gtpCallInfo,int id)
{
        char filename1[100];
	char filename2[100];
	char filename3[100];
	char filename4[100];
        struct tm tmmake1;
	struct tm tmmake2;
	struct tm tmmake3;
	struct tm tmmake4;
	ofstream outfile;
        switch(id)
        {
                case 1:
        		localtime_r(&(mkdirTime_1.tv_sec),&tmmake1);
			sprintf(filename1,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-1/RT_GPRSCDR.tmp",
                        tmmake1.tm_year+1900,
                        tmmake1.tm_mon+1,
                        tmmake1.tm_mday,
                        tmmake1.tm_hour,
                        tmmake1.tm_min,
                        tmmake1.tm_sec);
                        check_make_path(filename1);
			chown(filename1,500,500);
			outfile.open(filename1,ofstream::app);
        		if(!outfile)
                		cout<<"unable to open ..."<<endl;	
                        break;  
               
                case 2:
                
                      //  memcpy(&tmmake2,localtime_r(&(mkdirTime_2.tv_sec)),sizeof(struct tm));
			localtime_r(&(mkdirTime_2.tv_sec),&tmmake2);
                        sprintf(filename2,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-2/RT_GPRSCDR.tmp",
                        tmmake2.tm_year+1900,
                        tmmake2.tm_mon+1,
                        tmmake2.tm_mday,
                        tmmake2.tm_hour,
                        tmmake2.tm_min,
                        tmmake2.tm_sec);
                        check_make_path(filename2);
			chown(filename2,500,500);
                        outfile.open(filename2,ofstream::app);
                        if(!outfile)
                                cout<<"unable to open ..."<<endl;
			break;
                case 3:
        		localtime_r(&(mkdirTime_3.tv_sec),&tmmake3);
			sprintf(filename3,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-3/RT_GPRSCDR.tmp",
                        tmmake3.tm_year+1900,
                        tmmake3.tm_mon+1,
                        tmmake3.tm_mday,
                        tmmake3.tm_hour,
                        tmmake3.tm_min,
                        tmmake3.tm_sec);
                        check_make_path(filename3);
			chown(filename3,500,500);
			outfile.open(filename3,ofstream::app);
        		if(!outfile)
                		cout<<"unable to open ..."<<endl;	
                        break;  
                case 4:
        		localtime_r(&(mkdirTime_4.tv_sec),&tmmake4);
			sprintf(filename4,"/var/whizteldata/tmp1_%04d-%02d-%02d_%02d-%02d-%02d-4/RT_GPRSCDR.tmp",
                        tmmake4.tm_year+1900,
                        tmmake4.tm_mon+1,
                        tmmake4.tm_mday,
                        tmmake4.tm_hour,
                        tmmake4.tm_min,
                        tmmake4.tm_sec);
                        check_make_path(filename4);
			chown(filename4,500,500);
			outfile.open(filename4,ofstream::app);
        		if(!outfile)
                		cout<<"unable to open ..."<<endl;	
                        break;  
                default:
                        break;
        }


	char strCallID[100];
	char strIMSI[100];
        char strGPRS_Intface[100];
	char strMSISDN[100];
	char strAttachSuccess[100];
	char strAttachDelay[100];
	char strStartTime[100];
        char strStopTime[100];
	u_int8_t endIp[100];
	char strenduserIP[100];
	char strLAC[100];
	char strCellID[100];
	struct tm tmStart;
        struct tm tmStop;
//	ofstream outfile;
//	outfile.open(filename,ofstream::app);
//	if(!outfile)
  //              cout<<"unable to open ..."<<endl;
	sprintf(strGPRS_Intface,"%d",gtpCallInfo->GPRS_Intface);
	sprintf(strAttachSuccess,"%c",gtpCallInfo->pdp_status.AttachSuccess);
	sprintf(strLAC,"%ld",gtpCallInfo->LAC);
	sprintf(strCellID,"%ld",gtpCallInfo->CellID);
//        memcpy(&tmStart,localtime_r(&(gtpCallInfo->sdr_time.Start_Time.tv_sec)),sizeof(struct tm));
        localtime_r(&(gtpCallInfo->sdr_time.Start_Time.tv_sec),&tmStart);
        sprintf(strStartTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tmStart.tm_year+1900,
                tmStart.tm_mon+1,
                tmStart.tm_mday,
                tmStart.tm_hour,
                tmStart.tm_min,
                tmStart.tm_sec,
                int((gtpCallInfo->sdr_time.Start_Time.tv_usec)/1000));
//        memcpy(&tmStop,localtime_r(&(gtpCallInfo->sdr_time.Stop_Time.tv_sec)),sizeof(struct tm));
        localtime_r(&(gtpCallInfo->sdr_time.Stop_Time.tv_sec),&tmStop);
        sprintf(strStopTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tmStop.tm_year+1900,
                tmStop.tm_mon+1,
                tmStop.tm_mday,
                tmStop.tm_hour,
                tmStop.tm_min,
                tmStop.tm_sec,
                int((gtpCallInfo->sdr_time.Stop_Time.tv_usec)/1000));
     	sprintf(strAttachDelay,"%ld",long(gtpCallInfo->gprs_status.AttachDelay)/1000);
    	sprintf(strCallID,"%ld",gtpCallInfo->GPRS_Call_ID);
	sprintf(strIMSI,"%02x%02x%02x%02x%02x%02x%02x%x",
                gtpCallInfo->MT_IMSI[0],
                gtpCallInfo->MT_IMSI[1],
                gtpCallInfo->MT_IMSI[2],
                gtpCallInfo->MT_IMSI[3],
                gtpCallInfo->MT_IMSI[4],
                gtpCallInfo->MT_IMSI[5],
                gtpCallInfo->MT_IMSI[6],
                (gtpCallInfo->MT_IMSI[7]&0xF0)>>4);
        sprintf(strMSISDN,"%02x%02x%02x%02x%02x%02x%x",
                gtpCallInfo->MT_MSISDN[0],
                gtpCallInfo->MT_MSISDN[1],
                gtpCallInfo->MT_MSISDN[2],
                gtpCallInfo->MT_MSISDN[3],
                gtpCallInfo->MT_MSISDN[4],
                gtpCallInfo->MT_MSISDN[5],
                (gtpCallInfo->MT_MSISDN[6]&0xF0)>>4);
	memcpy(endIp,&(gtpCallInfo->pdp_status.enduserIP),4);
        sprintf(strenduserIP,"%d.%d.%d.%d",endIp[3],endIp[2],endIp[1],endIp[0]);
	outfile<<strGPRS_Intface<<"\t"<<strCallID<<"\t"<<strIMSI<<"\t"<<strMSISDN<<"\t"<<strLAC<<"\t"<<strCellID<<"\t"<<strAttachSuccess<<"\t"<<strAttachDelay<<"\t"<<" "<<"\t"<<strStartTime<<"\t"<<strStopTime<<"\t"<<" "<<"\t"<<strenduserIP<<"\t"<<" "<<endl;
	outfile.close();
}
*/



u_int64_t convert2key(u_int32_t teid,u_int32_t ip)
{
	u_int64_t key;
 	key=ip;
 	key<<=32;
 	key|=(u_int64_t)teid&0x00000000ffffffff;
	return key;
}


void check_make_path(char file_name[]) {

    char *t = file_name;
    while(t = strchr(++t, '/')) {

        *t = 0;

        if (access(file_name, 0) != -1) {
            *t = '/';
            continue;
        }
	umask(0033);
        mkdir(file_name,0777);
        chown(file_name,500,500);

        *t = '/';
    }
}

long getNumFromTime(int proid,int node)
{
	long num = (node & 0x0FF);
	struct timeval runtime;
	gettimeofday(&runtime,NULL);
	num <<= 4;
	num |= proid;
	num <<= 36;
	num |= runtime.tv_sec;
	num <<= 20;	
	return num;
}

void handler_sigusr1(int signo)
{
	printf("Get signal USR1\n");
}

void daemonize (const char *cmd)
{
	int fd0, fd1, fd2;
	pid_t pid;
	struct sigaction sa;

	/* clear file creation mask */
	umask(0);

	/* became a session leader */
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(-1);
	} else if (pid != 0) {
		exit(0);
	}

	setsid();

	/* ensure future opens won't allocate controlling TTYs */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		perror("sigaction");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		perror("fork2");
		exit(1);
	} else if (pid != 0) {
		exit(0);
	}

	/* close stdin, stdout, stderr */
	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/* change current working directory to the root */
	if (chdir("/") < 0) {
		perror("chdir");
		exit(1);
	}

	syslog(LOG_ERR, "%s daemon OK!", cmd);
}
