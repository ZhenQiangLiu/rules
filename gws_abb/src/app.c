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

#include "_env.h"

#include "app.h"
#include "task.h"


static void app_version(void);
static void app_help(const char *app);
static void app_daemon(void);

/*
 * read user input
 * parse & save to app_conf
 * call main task()
 *
 * version 1.1.310117
 * todo: getopt(), getopt_long()
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


	// read & parse user cli input
	int ret = 0;
	APP_CONF app_conf;
	memset(&app_conf, 0, sizeof(app_conf));

#ifdef USE_GETOPT
	// verified by Qige @ 2017.01.31
	int c = 0;
	while((c = getopt(argc, argv, "Dvhdi:")) != -1) {
		switch(c) {
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
#endif

#ifdef USE_GETOPT_LONG
	// todo: verify in gdbserver
	for(;;) {
		int option_index = 0, c = 0;
		static struct option long_options[] = {
				{ "h", 			no_argument, 0, 0 },
				{ "help", 		no_argument, 0, 0 },
				{ "v", 			no_argument, 0, 0 },
				{ "version", 	no_argument, 0, 0 },
				{ "d", 			no_argument, 0, 0 },
				{ "debug", 		no_argument, 0, 0 },
				{ "D", 			no_argument, 0, 0 },
				{ "daemon", 	no_argument, 0, 0 },
				{ "i", 			required_argument, 0, 0 },
				{ "ifname", 	required_argument, 0, 0 },
				{ 0, 			no_argument, 0, 0 }
		};

		//c = getopt_long(argc, argv, "", long_options, &option_index);
		c = getopt_long_only(argc, argv, "", long_options, &option_index);

		// no more params
		if (c == -1) break;

		// unknown param
		if (c == '?') continue;

		// handle param
		switch(option_index) {
		case 0:
		case 1:
			app_conf.flag.help = 1;
			return 0;
			break;
		case 2:
		case 3:
			app_conf.flag.version = 1;
			return 0;
			break;
		case 4:
		case 5:
			app_conf.flag.debug = 1;
			break;
		case 6:
		case 7:
			app_conf.flag.daemon = 1;
			break;
		case 9:
		case 10:
			snprintf(app_conf.ifname, ABB_IFNAME_LENGTH, "%s", optarg);
			break;
		default: // running with default values
			break;
		}
	}
#endif


	if (app_conf.flag.help) {
		//app_version();
		app_help(argv[0]);
		return 0;
	}

	if (app_conf.flag.version) {
		app_version();
		return 0;
	}

	if (app_conf.flag.debug) {
		app_version();
	}

	if (app_conf.flag.daemon) {
		app_daemon();
	}

	ret = task(&app_conf);
	return ret;
}

// todo: verify with gdbserver
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
#ifdef USE_GETOPT
	printf("  usage: %s [-D] [-i ifname]\n", app);
	printf("         %s [-d] [-v] [-h]\n", app);
#endif
#ifdef USE_GETOPT_LONG
	printf(" usage: %s [-D|--daemon] [-i|--ifname ifname]\n", app);
	printf("        %s [-d|--debug] [-v|--version|--ver] [-h|--help]\n", app);
#endif
}
