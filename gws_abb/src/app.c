/*
 * app.c
 *
 *  Created on: Jul 25, 2016
 *  Updated on: Aug 6, 2016
 *  Updated on: Jan 31, 2017
 *      Author: Qige Zhao <qige@6harmonics.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <getopt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h>

#include "app.h"
#include "task.h"


static void app_version(void);
static void app_help(const char *app);
static void app_daemon(void);

/*
 * read user input
 * parse & save to app_conf
 * call main task()
 */
int main(int argc, char **argv)
{
	// release before interrupt/quit/terminated
	signal(SIGINT, 	(__sighandler_t) task_prepare_exit); //+ "^C"
	signal(SIGQUIT, (__sighandler_t) task_prepare_exit); //+ "^\"
	signal(SIGTERM,	(__sighandler_t) task_prepare_exit); //+ "kill", not "kill -9"

	// release zombies
	// comment next line if "wait()/waitpid()"
	signal(SIGCHLD, SIG_IGN);


	int ret = 0;
	char opt = 0;
	APP_CONF app_conf;
	memset(&app_conf, 0, sizeof(app_conf));

	while((opt = getopt(argc, argv, "Dvhdi:")) != -1) {
		switch(opt) {
		case 'i':
			snprintf(app_conf.ifname, ABB_IFNAME_LENGTH, "%s", optarg);
			break;
		case 'h':
			app_conf.flag.help = 1;
			break;
		case 'v':
			app_conf.flag.version = 1;
			break;
		case 'd':
			app_conf.flag.debug = 1;
			break;
		case 'D':
			app_conf.flag.daemon = 1;
			break;
		default:
			break;
		}
	}

	if (app_conf.flag.help) {
		//app_version();
		app_help(argv[0]);
		return 0;
	}

	if (app_conf.flag.version) {
		app_version();
		return 0;
	}

	if (app_conf.flag.daemon) {
		app_daemon();
	}

	ret = task(&app_conf);
	return ret;
}

// detach terminal
// run in backgrund
static void app_daemon(void)
{
	int fr=0;

	fr = fork();
	if( fr < 0 ) {
		fprintf(stderr, "fork() failed\n");
		exit(1);
	}
	if ( fr > 0 ) {
		exit(0);
	}

	if( setsid() < 0 ) {
		fprintf(stderr, "setsid() failed\n");
		exit(1);
	}

	fr = fork();
	if( fr < 0 ) {
		fprintf(stderr, "fork() failed\n");
		exit(1);
	}
	if ( fr > 0 ) {
		fprintf(stderr, "forked to background (%d)\n", fr);
		exit(0);
}

	umask(0);

	chdir("/");
	close(0);
	close(1);
	close(2);

	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
}

static void app_version(void)
{
	printf("%s\n(%s)\n", APP_DESC, APP_VERSION);
}

static void app_help(const char *app)
{
	//printf("  usage: %s [-D|--daemon] [-i|--ifname wlan0]\n", app);
	//printf("         %s [-d|--debug] [-v|--version|--ver] [-h|--help]\n", app);
	printf("  usage: %s [-D] [-i wlan0]\n", app);
	printf("         %s [-d] [-v] [-h]\n", app);
}
