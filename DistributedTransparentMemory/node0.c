#include<stdio.h>
#include "ServerUtil.h"

int main()
{

int ret;
int node_id = 0;
ret = init_server(0);
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
