#ifndef SERVER_UTIL_H
#define SERVER_UTIL_H

#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

// ---------------------------hashTable ----------------------------


#define tableSize 100

struct node 
{
	int data;
	struct node *link;
};

typedef struct htEntry
{
	int data;
	struct node *link;
	
}hashTableEntry;


void initializeHashTable();
int addToHashTable(int key, int value, int nodeId);
int fetchFromHashTable(int key, int nodeId);
void displayHashTable(int nodeId);


// ------------------hashTable--------------------------------------------


struct machine_id 
{
	char *ip_address;
	int tcpportno;
	int udpportno;
};

struct serverUtility
{
	struct machine_id node[20];    // contains info about current and next node in the network
	struct sockaddr_in server_addr_tcp,server_addr_udp,client_addr;
	int masterSocket;
	int udpSocket;
	int opt;
	int TransferValue;
	char buffer[1025],replyBuffer[1024]; 
	fd_set readfds;
	
};




int extractKeyFromPutRequest(char request[]);
int extractValueFromPutRequest(char request[]);
int extractKeyFromGetRequest(char request[]);
void forwardUDP(int destination_node, char sendString[]);
int isRequestTypeGet(char requestBuffer[]);
int isRequestForCurrentNode(int nodeId , char requestString[]);
int init_server(int node_id);
void setupServer();
void deinit_server();


#endif
