/*
 * task.c
 *
 *  Created on: Jul 25, 2016
 *  Updated on: Aug 6, 2016
 *  Updated on: Nov 11, 2016
 *  Updated on: Jan 22, 2017
 *      Author: Qige Zhao <qige@6harmonics.com>
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <syslog.h>

#include "_env.h"
#include "app.h"
#include "task.h"


int FLAG_SIG_EXIT = 0;

static void task_init(void);
int (*task_core)(const void *);
void (*task_exit)(void);


// TODO: main logic/algorithm
// make your own algorithm here
// re-organize your calls here
int  task(APP_CONF *app_conf)
{
	task_init();

	int i = 0;
	for(i = 0; i < 10; i ++) {
		//(*task_core)(app_conf);
		sleep(1);
	}

	//(*task_exit)();

	return TASK_OK;
}


// set function pointer
static void task_init(void)
{
#if defined(_RADIO_MODEL) && (_RADIO_MODEL == GWS5K)
	//task_core = &gws5k_run;
	//task_idle = &gws5k_idle;
#endif
}


// mark FLAG_SIG_EXIT
void task_prepare_exit(void)
{
	printf("\n* SIGNAL caught, prepare to exit!\n");

	// TODO: syslog() signal exit
	LOG("signal(SIGINT, SIGQUIT, SIGTERM) detected, exiting...\n");
	FLAG_SIG_EXIT = 1;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
