/*
 * pub.h
 *
 *  Created on: 2014-3-10
 *      Author: zhujy
 */

#ifndef PUB_H_
#define PUB_H_

int send_work(const char *hostname, int port, const char *filename);
int recv_work(int port);

#endif /* PUB_H_ */
