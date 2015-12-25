#define SOCKET int

void getfilename(const char *filename, char *name);

int init_socket();

SOCKET socket_connect(const char *hostname, int port);

int send_work(const char *hostname, int port, const char*filename);

SOCKET socket_create(int port);

SOCKET socket_accept(SOCKET listen_st);

int recv_work(int port);