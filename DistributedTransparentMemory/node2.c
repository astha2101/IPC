#include<stdio.h>
#include "ServerUtil.h"

int main()
{

int ret;
int node_id = 2;
ret = init_server(node_id);
if (!ret)
{
	printf("Unaable to initilaize Node !!");
}
else
{
printf("Initialize successful %d" , ret);
}

setupServer();



return 0;
}
