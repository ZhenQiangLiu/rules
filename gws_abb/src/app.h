/*
 * app.h
 *
 *  Created on: Jul 25, 2016
 *  Updated on: 2016.11.11
 *  Updated on: Jan 31, 2017
 *      Author: Qige Zhao <qige@6harmonics.com>
 */

#ifndef APP_H_
#define APP_H_

// data types
typedef unsigned int			uint;
typedef unsigned char			uchar;
typedef char					byte;
typedef unsigned short 			ushort;

//#define USE_GETOPT
#define USE_GETOPT_LONG


#define APP_DESC				("GWS Analog Baseband")
#define APP_VERSION				("v0.0.310117. Buildtime: "__DATE__", "__TIME__)

#define ABB_IFNAME_LENGTH		16

typedef struct {
	struct {
		int version;
		int help;
		int debug;
		int daemon;
	} flag;
	char ifname[ABB_IFNAME_LENGTH];
} APP_CONF;

#endif /* APP_H_ */
