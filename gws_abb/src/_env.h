/*
 * _env.h
 *
 *  Created on: Jan 31, 2017
 *      Author: Qige Zhao <qige@6harmonics.com>
 */

#ifndef ENV_H_
#define ENV_H_

// AND: &&
// OR: ||
#if (!defined(USE_GETOPT) && !defined(USE_GETOPT_LONG))
#define USE_GETOPT
#endif

// determine radio platform
#if (!defined(_HW_GWS3K) && !defined(_HW_GWS4K) && !defined(_HW_GWS5K))
#define _HW_GWS5K
#endif

#endif /* ENV_H_ */
