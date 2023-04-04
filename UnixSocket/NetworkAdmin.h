#ifndef NETWORK_ADMIN_H
#define NETWORK_ADMIN_H


#define IP_ADDR_LEN 16
#define OIF_LEN 32
#define SOCKET_NAME "NetworkAdminSocket"

//routing model operations
typedef enum {
    CREATE,
    UPDATE,
    DELETE,
    SHOW,
    SHUTDOWN
} OPCODE;

//message content
typedef struct _contents {
    char dest[IP_ADDR_LEN];
    unsigned short mask;
    char gw[IP_ADDR_LEN];
    char oif[OIF_LEN];
} contents_t;


typedef struct _sync_msg {
    OPCODE op_code;
    contents_t contents;
} sync_msg_t;

// routing table ds to store the data (sll)
typedef struct routing_table_entry_ {
    contents_t *contents;
    struct routing_table_entry_ *next;
    struct routing_table_entry_ *prev;
} routing_table_entry_t;

typedef struct routing_table_{
    routing_table_entry_t *head;
} routing_table_t;

void init_routing_table(routing_table_t *table);
void display(const routing_table_t *table);
void process_sync_mesg(routing_table_t *table, const sync_msg_t *sync_msg);
void deinit_routing_table(routing_table_t *table);

#endif
