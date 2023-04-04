#ifndef SERVER_UTIL_C
#define SERVER_UTIL_C


#include "ServerUtil.h"

#define N 3 // number of nodes in the network
#define ip "127.0.0.1"
#define TABLESIZE 100

// global variables

struct serverUtility serverUtil;
hashTableEntry hashTable[tableSize]; 
int NODE_ID = -1;


//--------------------------------hashTable functions --------------------------------


void initializeHashTable()
{
	int i=0;

	for(i=0;i<TABLESIZE;i++)
	{

		hashTable[i].data=0;
  		hashTable[i].link=NULL;
 	}
}

static void appendNode(struct node *start,int data)
{
		while(start->link!=NULL) start = start->link;	
		start->link = (struct node *)malloc(sizeof(struct node));
		start->link->link = NULL;
		start->link->data = data;
}

int addToHashTable(int key, int value, int nodeId)
{
	int maxKeyLimit = (TABLESIZE-1)*N+nodeId;
	int nodeIndex, ret;
	
	if(key%N == nodeId && key <= maxKeyLimit && key > -1) // key is intended to be added in the current node's hashtable
	{
	   nodeIndex = (key-nodeId)/N;
	   // if hashtable is empty
	   if(hashTable[nodeIndex].data == 0)
	   {
	   	hashTable[nodeIndex].data = value;
	   	ret = 1;
	   }
	   // if hash table has only exactly one entry for the given key
	   else if(hashTable[nodeIndex].link == NULL)
	   {
	   	hashTable[nodeIndex].link = (struct node *)malloc(sizeof(struct node));
    		hashTable[nodeIndex].link->data = value;
    		hashTable[nodeIndex].link->link = NULL;
    		ret = 1;
	   }
	   // if hash table has multiple entries for the given key
	   else
	   {
	   	appendNode(hashTable[nodeIndex].link, value);
	   	ret = 1;
	   }
	}
	else
	{
  		printf("\nERROR:KEY = %d,VALUE = %d CANNOT ADD IN TABLE, MAX KEY LIMIT = %d\nENTER NEW GET/PUT 				REQUEST :",key,value,maxKeyLimit);
  		
		ret=0;
 	}
 	return ret;

}

int fetchValueFromHashTable(int key, int nodeId)		
{
	
	int relativeIndex = (key-nodeId)/N;
	int return_value;	

	if(hashTable[relativeIndex].link == NULL)					//first value in the list
		return_value =  hashTable[relativeIndex].data;

  	else
	{
    		struct node *start = hashTable[relativeIndex].link;
     		while(start->link!=NULL)					
      			start = start->link;					// progess pointer to last element
     		return_value =  start->data;					//assign last element 
   	}   
	return return_value;
}

void displayHashTable(int nodeId)
{

	int from_key = nodeId;
	int to_key = (TABLESIZE-1)*N + nodeId;

	if(from_key > to_key)
	{
		
		int temp = from_key;
		from_key = to_key;
		to_key = temp ;
	}

	printf("\n-----Hash Table Contents(%d--%d)------------\n",from_key,to_key);

	if(from_key % N == nodeId && to_key % N == nodeId)
	{

 		int i,fetchValue,key;
 		int from=(from_key - nodeId)/N;
 		int to = (to_key - nodeId)/N;

 		for(i=from;i<=to;i++)
		{

			key = i*N+nodeId;
			fetchValue = fetchValueFromHashTable(key, nodeId);
  			if(fetchValue !=0)
  				printf("\nkey : %d ===== value : %d\n",key,fetchValueFromHashTable(key, nodeId));
 		}
	}
 	else
		printf("invalid keys , hash table cannot be displayed\n");

 	printf("-----------------------------------------------------\nENTER NEW GET/PUT REQUEST:");
}


//----------------------------------hashTable functions ----------------------------------


static void itoa(int n,char buff[])
{
	int i =0,j;

	if(n==0)
	{
		
		buff[0] = '0';
		buff[1] = '\0';
  		i=1;
 	}

 	while(n>0)
	{
		  		
		int rem = n %10;
  		n=n/10;
  		buff[i]=rem+48;			
		i++;
	} 

	buff[i]='\0';

	for(j=0;j<i/2;j++)
	{

 		char temp = buff[j];
 		buff[j] = buff[i-j-1];
 		buff[i-j-1] = temp;
		
	}

}

static char *extractIpaddress(char *buff,char a,char b)
{
	
	char *ptr = (char *)malloc(20);
	int i=0,j=0;

	while(buff[i]!=a)
	{
		i++;
	}
 	i=i+1;

  	while(buff[i]!=b)
	{
    		
		ptr[j++]=buff[i];
    		i++;
  	}
 	ptr[j]='\0';
 	return ptr;
}


static int extractNodeno(char * buff)
{

	char *nodeno=extractIpaddress(buff,')','[');
 	return atoi(nodeno);
}


int extractKeyFromPutRequest(char request[])
{
	char *b = strstr(request, "("),*c = strstr(request, ",");
	int position = b - request,position1 = c - request,k;
	char to[4];
	strncpy(to, request + position+1, position1 - position);   //for extracting the first integer no. k of put(k,x)
 
	k = atoi(to);		//converting to int
	fflush(stdout);
	return k;
}

int extractValueFromPutRequest(char request[])
{
	char *b = strstr(request, ","),*c = strstr(request, ")");
	int position = b - request,position1 = c - request,k;
	char to[16];
	memset(to, 0 , 16);
	strncpy(to, request + position+1, position1 - position);   //for extracting the first integer no. k of put(k,x) 
	k = atoi(to);
	return k;
}

int extractKeyFromGetRequest(char request[])
{
	char *b = strstr(request, "("),*c = strstr(request, ")");
	int position = b - request,position1 = c - request,k;
	char to[4];
	strncpy(to, request + position+1, position1 - position);   //for extracting the first integer no. k of put(k,x) 
	k = atoi(to);
	fflush(stdout);
	return k;
}



int isRequestTypeGet(char requestBuffer[])
{
	if(requestBuffer[0] == 'g' || requestBuffer[0] == 'G')
  		return 1;
	else
  		return 0;
}

int isRequestForCurrentNode(int nodeId, char requestString[])
{
	
	int k;
	if(isRequestTypeGet(requestString)==1)				//check whether request is get or put

		k = extractKeyFromGetRequest(requestString);			
 	else
 		k=extractKeyFromPutRequest(requestString);	
 		
	if((k % N) == nodeId)			

 		return 1;					//return 1 if for current node
 	else
 		return 0;
}


static char *forwardedData(char inputbuff[],char flag)			//Called to prepare data to be forwaded to next node
{
	
	int key,tcpportno;
	char keybuff[5],portbuff[5],fl[2];
	fl[0]=flag;
	fl[1]='\0';
	char *outputbuff=(char *)malloc(sizeof(char)*40),nodebuff[3];

 	if(isRequestTypeGet(inputbuff)==1)
  		key = extractKeyFromGetRequest(inputbuff);		//extract key from data get before preparing sending data
 	else
		key = extractKeyFromPutRequest(inputbuff);		//extract key from data get before preparing sending data

	tcpportno=serverUtil.node[NODE_ID].tcpportno;		

	itoa(tcpportno,portbuff);					//convert integer port number to character array

	itoa(key,keybuff);						//convert integer key to character array

	strcpy(outputbuff,"xxx(");   					//start forwarding data with xxx( 

	strcat(outputbuff,keybuff);					

	strcat(outputbuff,",");						

	strcat(outputbuff,portbuff);					

	strcat(outputbuff,")");   					

	itoa(NODE_ID,nodebuff);				//convert integer node number to character array

	strcat(outputbuff,nodebuff);					 

	strcat(outputbuff,"[");						

	strcat(outputbuff,serverUtil.node[NODE_ID].ip_address);	

	strcat(outputbuff,",");						

	strcat(outputbuff,fl);						

	strcat(outputbuff,"]");						

	outputbuff[strlen(outputbuff)+1] = '\0';			//end 

	return outputbuff;					//return the address of data in memory which is to be forwaded
}




void forwardUDP(int destination_node, char sendString[])
{
	destination_node = destination_node % N;		//destination node to which data is to be forwaded
	int sock;
	struct sockaddr_in server_addr;
				
	struct hostent *host;			//hostent predefined structure use to store info about host
	host = (struct hostent *) gethostbyname(serverUtil.node[destination_node].ip_address);//gethostbyname returns a pointer to hostent 
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{

		perror("socket");
		exit(1);
	}

	//destination address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serverUtil.node[destination_node].udpportno);		
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);	//host->h_addr gives address of host
	bzero(&(server_addr.sin_zero),8);
	sendto(sock, sendString, strlen(sendString), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

	//sendto() function shall send a message through a connectionless-mode socket. 
	printf("\nFORWARD REQUEST : '%s' has been forwarded to node ---->%d\n",sendString,destination_node);  
	close(sock);
}

int init_server(int node_id)
{
	
	initializeHashTable();
	NODE_ID = node_id;
			
	serverUtil.node[NODE_ID].ip_address = ip;					
	serverUtil.node[NODE_ID].tcpportno = 2000+NODE_ID*2;			
	serverUtil.node[NODE_ID].udpportno = serverUtil.node[NODE_ID].tcpportno+1;		
	serverUtil.node[(NODE_ID+1)%N].ip_address = ip;					
	serverUtil.node[(NODE_ID+1)%N].tcpportno = 2000+((NODE_ID+1)%N)*2;			
	serverUtil.node[(NODE_ID+1)%N].udpportno = serverUtil.node[(NODE_ID+1)%N].tcpportno+1;	
	serverUtil.opt = 1;
	serverUtil.TransferValue = 0;
	
	
	if ((serverUtil.udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)		//Creating a UDP Socket
	{		 

            perror("Socket");						
            exit(1);
        }
        else printf("udp socket created successfully\n");
	//current node address
	serverUtil.server_addr_udp.sin_family = AF_INET;  		//AF_INET represents the address family INET for IPv4 protocol
	serverUtil.server_addr_udp.sin_port = htons(serverUtil.node[NODE_ID].udpportno);//convert port in host byte order to N/W byte order
	serverUtil.server_addr_udp.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serverUtil.server_addr_udp.sin_zero),8);	//bzero() sets all values in a buffer to zero.

	if (bind(serverUtil.udpSocket,(struct sockaddr *)&serverUtil.server_addr_udp, sizeof(struct sockaddr)) == -1)//Binding UDP socket
	{		
            perror("Bind");	
            exit(1);
        }
        else printf("udp socket bind successful\n");
        
	// working with TCP
	if ((serverUtil.masterSocket = socket(AF_INET,SOCK_STREAM,0))==0) 		//Create TCP Socket
	{				
		perror("Create master_socket");
    		exit(EXIT_FAILURE);
  	}
  	else printf("tcp socket created successfully\n");

	//set master socket to allow multiple connections 
  	if (setsockopt(serverUtil.masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&serverUtil.opt, sizeof(int))<0)
	{
		
		
    		perror("setsockopt");
    		exit(EXIT_FAILURE);
  	}
	//current node address for tcp
	serverUtil.server_addr_tcp.sin_family = AF_INET;			
  	serverUtil.server_addr_tcp.sin_addr.s_addr = INADDR_ANY;		
	serverUtil.server_addr_tcp.sin_port = htons(serverUtil.node[NODE_ID].tcpportno);		

	if (bind(serverUtil.masterSocket, (struct sockaddr *)&serverUtil.server_addr_tcp, sizeof(struct sockaddr)) < 0)	//bind the socket to port 
	{
    		perror("bind");
    		exit(EXIT_FAILURE);
  	}
  	else printf("tcp socket bind successful\n");
  	return 1;
				
	// server is in listening mode and ready to  the requests
}


void setupServer()
{
	int maxfd;
	int newSocket;
	char buffer[1025],replyBuffer[1024];
	int valread;
	
	system("clear");
	printf ("       INSTRUCTIONS \n\n  =================NODE %d=======================\n",NODE_ID);
	puts("   1.'put' request format : PUT(<integer>,<integer>)\n");
	puts("   2.'get' request format : GET(<integer>)\n");
	puts("   3.To print Hash Table : 'r'\n");
	puts("-----------------------------------\n\nENTER GET/PUT REQUEST :");
	
	
	if (listen(serverUtil.masterSocket, 5) < 0)		//Specify maximum of 5 pending connections for the master socket 
	{
    		perror("listen");
    		exit(EXIT_FAILURE);
  	}
  	else printf("tcp socket listen successful\n");
  	
  	// server is in listening mode and ready to accept the requests
	
	// setting up server
	while(1)
	{
		FD_ZERO(&(serverUtil.readfds));	//clears out the fd_set-readfds, so that it doesn't contain any file descriptors. 


   		FD_SET(serverUtil.udpSocket, &(serverUtil.readfds));	//adds file descriptor "sock_udp" to the fd_set,select will return 


   		FD_SET(serverUtil.masterSocket, &(serverUtil.readfds));	//adds file descriptor "master_socket" to the fd_set,select will return



   		FD_SET(0,&(serverUtil.readfds)); 			// to read from standard input
   		
   		if(serverUtil.masterSocket > serverUtil.udpSocket)
    			maxfd = serverUtil.masterSocket;
   		else
    			maxfd = serverUtil.udpSocket;
    			
   		select(maxfd+1, &(serverUtil.readfds), NULL, NULL, NULL);
   		
   		if (FD_ISSET(serverUtil.masterSocket, &serverUtil.readfds))
		{

      
			if (newSocket = accept(serverUtil.masterSocket, (struct sockaddr *)&serverUtil.server_addr_tcp, sizeof(serverUtil.server_addr_tcp)) < 0)
			{

				/* if accept failed to return a socket descriptor, display error and exit */
        			perror("accept1");
        			exit(EXIT_FAILURE);
       			}

			/* inform user of socket number - used in send and receive commands */
          
      			if(serverUtil.TransferValue != 0)  // means put request was made
      			{

          			// means put request was made on this node , and this node need to supply the value 'TransferValue' back to client
           			
				itoa(serverUtil.TransferValue,replyBuffer);// as sooon as conection is made, TCP server send value to client
            			send(newSocket,replyBuffer,strlen(replyBuffer), 0);   
            			read(newSocket, buffer, 1024);
            			printf("\n%s\n",buffer);
	   			serverUtil.TransferValue = 0 ;
           			close(newSocket);
        		}
        		else
			{
	          		// means get request was made and tcp server is only need to recieve value from client
            			if ((valread = read(newSocket, buffer, 1024)) < 0) 
            				close(newSocket);
        			
				else
				 {
					/* set the terminating NULL byte on the end of the data read */
        				buffer[valread] = 0;
					printf("\n%s\n",buffer); //m
        			}
        			close(newSocket);
			}
		 } 
		 
		 if(FD_ISSET(serverUtil.udpSocket, &serverUtil.readfds))	/*Check udp_socket has data available to be read */
		 {
 			char rec_buff[5000];
 			
			int len = recvfrom(serverUtil.udpSocket,rec_buff,5000,0,(struct sockaddr *)&serverUtil.client_addr, sizeof(struct sockaddr));
			rec_buff[len] = '\0';
			
			printf("\n--------\nUDP PACKET RECIEVED FROM (IP ADDRESS : %s , PORT NO : %d , NODE NO : %d)  : ",
						inet_ntoa(serverUtil.client_addr.sin_addr),ntohs(serverUtil.client_addr.sin_port),extractNodeno(rec_buff));

			//inet_ntoa() converts a n/w address in a struct in_addr to a dots-and-numbers format string		
			printf("%s\n",rec_buff);
			

			// Check whether the req is to be forwarded or to be processed by the current node itself

      			if(isRequestForCurrentNode(NODE_ID, rec_buff) == 0)
			{

         			forwardUDP(NODE_ID+1,rec_buff); // forward the request as it is to the next node
	 			puts("\n---------------------------------------\nENTER NEW GET/PUT REQUEST:");     
        		}
			else
			{
				printf("\nPROCESSING THE REQUEST ON THE CURRENT NODE : \n")
;
   				// this is the node where get/put request is to be processed, so fetching
				// extracting key from request

				int key = extractKeyFromPutRequest(rec_buff);
				int nodeno = extractNodeno(rec_buff);
		 
				// establish TCP conectiom with server whether it is get Or put message

        			int sock, bytes_recieved;  
        			char send_data[1024],recv_data[1024];
        			char flag = rec_buff[strlen(rec_buff)-2];	//extracted flag value from request
        			struct hostent *host;
        			struct sockaddr_in server_addr;  
        			host = gethostbyname(extractIpaddress(rec_buff,'[',','));//extracting originator IP from request 

        			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				{

            				perror("Socket");
            				exit(1);
        			}
				//To which connection is to be made
				server_addr.sin_family = AF_INET;     
        			server_addr.sin_port = htons(extractValueFromPutRequest(rec_buff)); 
					//same format put(key,value) initially
					//now format put(key,port)  
        			server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        			bzero(&(server_addr.sin_zero),8); 

        			if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) //Make Connec
				{

            				perror("Connect");
            				exit(1);
        			}        
              			if(flag == 's')		//Get request and the value is to be fetched from hash table
				{

               				int valuefetched = fetchValueFromHashTable(key,NODE_ID);
               				if(valuefetched !=0)
					{

               					strcpy(send_data,"value = ");
               					char valuebuff[6],nodebuff[4];
               					itoa(valuefetched,valuebuff);
               					strcat(send_data,valuebuff);
               					strcat(send_data,". Value retrieved from node no : ");
               					itoa(NODE_ID,nodebuff);
               					strcat(send_data,nodebuff);
						strcat(send_data,"\n-------------------\nENTER NEW GET/PUT REQUEST:");
                			}
               				else
					{
               					strcpy(send_data,"RESULT: No hash entry to this key on node no : ");
               					char nodebuff[4];
               					itoa(NODE_ID,nodebuff);
               					strcat(send_data,nodebuff);
      	       					strcat(send_data,"\n--------------------\nENTER NEW GET/PUT REQUEST :");

              				}
              				 send(sock,send_data,strlen(send_data), 0); //Send the fetched value
                                       printf("\n KEY RECIEVED %d ,\nREQUEST ORIGINALLY INVOKED ON NODE  %d , flag recieved = %c . VALUE SUPPLIED BACK ON TCP CONNECTION.\nENTER NEW GET/PUT REQUEST:",key,nodeno,rec_buff[strlen(rec_buff)-2]);
              			}
              			else
				{
              				bytes_recieved=recv(sock,recv_data,1024,0);	//Receiving Put value
              				recv_data[bytes_recieved] = '\0';
                                         printf("\n KEY RECIEVED IS: %d ,\nREQUEST ORIGINALLY INVOKED ON NODE  %d , flag recieved = %c\n",key,nodeno,rec_buff[strlen(rec_buff)-2]); 
              				
					printf("\nVALUE RECIEVED (ON TCP CONNECTION) FROM NODE NO %d = %s " ,nodeno, recv_data);
					// now insert the key,value in hash table of this node and send confirmation message 						back to parent node
					// on which the request was originallly invoked by the  user

               				if(addToHashTable(key,atoi(recv_data),NODE_ID))
               					strcpy(send_data,"RESULT: put operation has been done successfully.  Value added on node no :");
	       				else
               					strcpy(send_data,"RESULT: put operation failed. Maximum key limit Exceeded on node number  ");
               				char nodebuff[4];
               				itoa(NODE_ID,nodebuff);
               				strcat(send_data,nodebuff);
	       				strcat(send_data,".\n------------------------------------\nENTER NEW GET/PUT REQUEST :");
               				send(sock,send_data,strlen(send_data), 0); 

				}	
        			close(sock);
        			fflush(stdout);
			} // else ends
		}
		
		//  working with console 


		if (FD_ISSET(0, &serverUtil.readfds))//FD_ISSET()Returns a non-zero value if the bit for the file descriptor 0' is set in the file descriptor set pointed to by readfds, and 0 otherwise.
		{
		
        		char rec_buff[5000];
			gets(rec_buff);		


 			if(rec_buff[0] == 'r' || rec_buff[0] == 'R')
			{

        			displayHashTable(NODE_ID);	
       			}

       			else if(isRequestForCurrentNode(NODE_ID, rec_buff) == 0) //fun return 1 if for you;return 0 if not for you
			{							
         			char outputbuff[40],*out,flag;
         			int i =0;

           			if(isRequestTypeGet(rec_buff) == 0)		//fun() return 1 for get, 0 for put
				{   
					//value from put to be transferred at last
            				serverUtil.TransferValue = extractValueFromPutRequest(rec_buff);

            				flag = 'r';	 // indicates that last node has to receive a value
          			}
          			else
            				flag = 's';	//indicates that last node will send a value
     				
        			out = forwardedData(rec_buff,flag);     //fun() to prepare the data to be forwaded
        			
				
				for(i=0;i<strlen(out);i++)
        				outputbuff[i] = *(out+i);	//data to be forwaded is assigned to outputbuff
 
        			outputbuff[i] = '\0';			

        			forwardUDP(NODE_ID+1,outputbuff);	//forwading method called
        			free(out);                    
		        }
       
			else
			{
				printf("\nPROCESSING THE REQUEST HERE:\n-------------------\n");

				

    				if(isRequestTypeGet(rec_buff)==1)	//fun() return 1 for get, 0 for put 
				{
					
					// extract key from get request
     					int key = extractKeyFromGetRequest(rec_buff),value;
					int maxkeylimit=(TABLESIZE-1)*N+NODE_ID;  //Compute maximum key limit

     					if(key <= maxkeylimit)
					{

   						value = fetchValueFromHashTable(key, NODE_ID);	//call fun() to fetch value from hash table

     						if(value == 0)
      							
							printf("\nError - No value in Hash table for this key on Current node. 								\n---------------------\nEnter NEW GET/PUT REQUEST :"); 
     						
						else
      							printf("\n key = %d, value = %d on same node \n----------------------- 								-\nEnter GET/PUT REQUEST :",key,value);
      					}
     					else
					{
        					printf("\n Result :  Error - value cannot be fetched , maximum key is %d 							\n----------------------\nEnter NEW GET/PUT REQUEST :",maxkeylimit);
      					}
    				}
    				else
				{
      					// processing put request on the same node
       					addToHashTable(extractKeyFromPutRequest(rec_buff),extractValueFromPutRequest(rec_buff),NODE_ID);
       				}

			} //else ends
			fflush(stdout);
                
		} // read from console ends here

     

   		
   		
	} // while ends
	
	deinit_server();
}


#endif
