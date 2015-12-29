/*
 * pub.h
 *
 *  Created on: 2013年12月17日
 *      Author: zhujy
 */

#ifndef PUB_H_
#define PUB_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>

#define BUFSIZE 32768  //32k

void catch_Signal(int Sign);
int signal1(int signo, void (*func)(int));
void init_socket_client();
void deliver(int index, const char *buf, ssize_t len);
void socket_work(int index);
void *socket_contrl(void *arg);
int socket_create(uint16_t port);
void socket_accept(int st);
void sockaddr_toa(const struct sockaddr_in *addr, char *IPAddr);

#endif /* PUB_H_ */
