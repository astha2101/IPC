# DistributedTransparentMemory
DistributedTransparentMemory

This is the implementation of Distributed transparent memory in C, based on socket programming project. A distributed memory - adding up the main memory of several connected machines together to create a one big unified chunk of storage for the user - the locality information of the data is completely hidden from the user. Data stored by the user is hashed to the memory of of one of the connected backend node and resides there. Memory appear as local memory to the end user, though memory is distributed across several machines in the backend. User specify the necessary keys to fetch the data, the backend automatically finds the machine which hosts the requested data and fetch it to user. This is how Hadoop HDFS is designed. The exact problem statement is here.

Here we are using two types of sockets, UDP and TCP socket
UDP socket is used for forwarding requests between the nodes in the network
TCP socket is the master socket or the entry point for the other nodes to be connected to each other and communicate to each other.
