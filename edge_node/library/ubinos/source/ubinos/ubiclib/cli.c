/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubiclib.h"

#if !(UBINOS__UBICLIB__EXCLUDE_CLI == 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define CLI_SLEEP_TICK 1
#define CLI_CMD_SIZE_MAX 255
#define CLI_PROMPT_SIZE_MAX 31
#define CLI_PROMPT__DEFAULT	"cli> "

#if !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1)

extern volatile int _ubiclib_logm_level[LOGM_CATEGORY__END];
extern const char *_ubiclib_logm_categoryname[LOGM_CATEGORY__END];
extern const char *_ubiclib_logm_levelname[LOGM_LEVEL__END];

#endif /* !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1) */

char _cli_cmd_buf[CLI_CMD_SIZE_MAX + 1] = { 0, };
char _cli_initial_cmd_buf[CLI_CMD_SIZE_MAX + 1] = { 0, };
char _cli_prompt_buf[CLI_PROMPT_SIZE_MAX + 1] = { 0, };

cli_hookfunc_ft _cli_hookfunc = NULL;
void *_cli_hookarg = NULL;

cli_helphookfunc_ft _cli_helphookfunc = NULL;

static int cli_rootfunc(char *str, int len, void *arg);

static int cli_cmdfunc__help(char *str, int len, void *arg);
static int cli_cmdfunc__set(char *str, int len, void *arg);
static int cli_cmdfunc__show(char *str, int len, void *arg);

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)
static int cli_cmdfunc__heap(char *str, int len, void *arg);
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */

#if (INCLUDE__UBINOS__UBIK == 1)
static int cli_cmdfunc__date_set(char *str, int len, void *arg);
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

int cli_sethookfunc(cli_hookfunc_ft hookfunc, void *arg) {
	_cli_hookfunc = hookfunc;
	_cli_hookarg = arg;

	return 0;
}

int cli_sethelphookfunc(cli_helphookfunc_ft helphookfunc) {
	_cli_helphookfunc = helphookfunc;

	return 0;
}

int cli_setprompt(char *prompt) {
	int r = -1;

	if (prompt != NULL) {
		strncpy(_cli_prompt_buf, prompt, CLI_PROMPT_SIZE_MAX);
		r = 0;
	}

	return r;
}

int cli_set_initial_cmd(char * cmd)
{
	int r = -1;

	if (cmd != NULL) {
		strncpy(_cli_initial_cmd_buf, cmd, CLI_CMD_SIZE_MAX);
		r = 0;
	}

	return r;
}

void cli_main(void *arg) {
	int r = -1;
	int len;
	int is_first = 1;

	if (strlen(_cli_prompt_buf) == 0) {
		strncpy(_cli_prompt_buf, CLI_PROMPT__DEFAULT, CLI_PROMPT_SIZE_MAX);
	}

	printf("\n%s", _cli_prompt_buf);
	fflush(stdout);
	for (;;) {
		if (is_first)
		{
			len = strnlen(_cli_initial_cmd_buf, CLI_CMD_SIZE_MAX);
			if (len > 0)
			{
				strncpy(_cli_cmd_buf, _cli_initial_cmd_buf, CLI_CMD_SIZE_MAX);
			}
			else
			{
				len = dtty_gets(_cli_cmd_buf, CLI_CMD_SIZE_MAX);
			}
			is_first = 0;
		}
		else
		{
			len = dtty_gets(_cli_cmd_buf, CLI_CMD_SIZE_MAX);
		}
		if (0 < len) {
			printf("\n%s\n", _cli_cmd_buf);

			r = cli_rootfunc(_cli_cmd_buf, len, NULL);
			if (0 != r) {
				if (NULL != _cli_hookfunc) {
					r = _cli_hookfunc(_cli_cmd_buf, len, _cli_hookarg);
				}
			}
		} else {
			printf("\n");
			r = -1;
		}

		if (0 != r) {
			cli_cmdfunc__help(_cli_cmd_buf, len, NULL);
			if (NULL != _cli_helphookfunc) {
				_cli_helphookfunc();
			}
		}

		printf("\n%s", _cli_prompt_buf);
		fflush(stdout);
	}
}

static int cli_rootfunc(char *str, int len, void *arg) {
	int r = -1;
	char *tmpstr;
	int tmplen;
	char *cmd = NULL;
	int cmdlen;

	tmpstr = str;
	tmplen = len;

	do {
		cmd = "set ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			printf("\n");

			r = cli_cmdfunc__set(tmpstr, tmplen, arg);
			break;
		}

		cmd = "show ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			printf("\n");

			r = cli_cmdfunc__show(tmpstr, tmplen, arg);
			break;
		}

#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
		cmd = "mi";
		cmdlen = strlen(cmd);
		if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			printf("\n");
			heap_printheapinfo(NULL);

			r = 0;
			break;
		}
#endif

#if ( (INCLUDE__UBINOS__UBIK == 1) && !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1) )
		cmd = "ki";
		cmdlen = strlen(cmd);
		if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			printf("\n");
			ubik_printkernelinfo();

			r = 0;
			break;
		}
#endif

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)
		cmd = "heap ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			printf("\n");

			r = cli_cmdfunc__heap(tmpstr, tmplen, arg);
			break;
		}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */

#if (INCLUDE__UBINOS__UBIK == 1)
		cmd = "date";
		cmdlen = strlen(cmd);
		if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			struct timeval tv;
			struct tm* tm_ptr;
		    char time_string[40];

			gettimeofday(&tv, NULL);
			tm_ptr = localtime(&tv.tv_sec);
			strftime(time_string, sizeof (time_string), "%a %b %d %H:%M:%S %Y", tm_ptr);
			printf(time_string);

			r = 0;
			break;
		}

		cmd = "date ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			printf("\n");

			r = cli_cmdfunc__date_set(tmpstr, tmplen, arg);
			break;
		}
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

		break;
	} while (1);

	return r;
}

static int cli_cmdfunc__help(char *str, int len, void *arg) {
	printf("\n");
	printf("h                                       : help\n");
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
	printf("\n");
	printf("mi                                      : memory information\n");
#endif
#if ( (INCLUDE__UBINOS__UBIK == 1) && !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1) )
	printf("\n");
	printf("ki                                      : kernel information\n");
#endif
	printf("\n");
	printf("set echo <on|off>                       : set echo on/off\n");
	printf("\n");
	printf("set autocr <on|off>                     : set auto carriage return on/off\n");
#if !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1)
	printf("\n");
	printf("set logm <category id> <level id>       : set log message level\n");
	printf("    category id\n");
	printf("        %4d : [ALL    ]\n", -1);
	for (int i = 0; i < LOGM_CATEGORY__END; i++) {
		printf("        %4d : %s\n", i, _ubiclib_logm_categoryname[i]);
	}
	printf("    level id\n");
	for (int i = 0; i < LOGM_LEVEL__END; i++) {
		printf("        %4d : %s\n", i, _ubiclib_logm_levelname[i]);
	}
	printf("\n");
	printf("show logm                               : show log message level\n");
#endif
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)
	printf("\n");
	printf("heap param <algorithm> <size> (m)       : calculate heap parameters\n");
	printf("    algorithm: heap algorithm\n");
	printf("        group    : group system\n");
	printf("        pgroup   : pure group system (without tailing) \n");
	printf("        bbuddy   : binary buddy system\n");
	printf("        wbuddy   : weighted buddy system\n");
	printf("        bestfit  : best fit\n");
	printf("        firstfit : first fit\n");
	printf("        nextfit  : next fit\n");
	printf("    size: heap size \n");
	printf("    m: M value for group system and pure group system\n");
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
#if (INCLUDE__UBINOS__UBIK == 1)
	printf("\n");
	printf("date [MMDDhhmm[YYYY][.ss]]              : print or set the system date and time\n");
#endif
	printf("\n");
	return 0;
}

static int cli_cmdfunc__set(char *str, int len, void *arg) {
	int r = -1;
	char *tmpstr;
	int tmplen;
	char *cmd = NULL;
	int cmdlen;

	tmpstr = str;
	tmplen = len;

	do {
		cmd = "echo ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			cmd = "on";
			cmdlen = strlen(cmd);
			if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				r = dtty_setecho(1);
				if (0 == r) {
					printf("    success\n");
				} else {
					printf("    fail\n");
				}
				break;
			}

			cmd = "off";
			cmdlen = strlen(cmd);
			if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				r = dtty_setecho(0);
				if (0 == r) {
					printf("    success\n");
				} else {
					printf("    fail\n");
				}
				break;
			}
		}

		cmd = "autocr ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			cmd = "on";
			cmdlen = strlen(cmd);
			if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				r = dtty_setautocr(1);
				if (0 == r) {
					printf("    success\n");
				} else {
					printf("    fail\n");
				}
				break;
			}

			cmd = "off";
			cmdlen = strlen(cmd);
			if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				r = dtty_setautocr(0);
				if (0 == r) {
					printf("    success\n");
				} else {
					printf("    fail\n");
				}
				break;
			}
		}

#if !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1)
		cmd = "logm ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			int id;
			int level;

			sscanf(tmpstr, "%d %d", &id, &level);
			printf("    category id : %d\n", id);
			printf("    level id    : %d\n\n", level);

			r = logm_setlevel(id, level);
			if (0 == r) {
				printf("        success\n");
			} else {
				printf("        fail\n");
			}

			r = 0;
		}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1) */

		break;
	} while (1);

	return r;
}

static int cli_cmdfunc__show(char *str, int len, void *arg) {
	int r = -1;
	char *tmpstr;
	int tmplen;
	char *cmd = NULL;
	int cmdlen;

	(void) tmpstr;
	(void) tmplen;
	(void) cmd;
	(void) cmdlen;

	tmpstr = str;
	tmplen = len;

	do {
#if !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1)
		cmd = "logm";
		cmdlen = strlen(cmd);
		if (tmplen == cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			    printf("    category name :    category id : level id\n");
			for (int i = 0; i < LOGM_CATEGORY__END; i++) {
				printf("    %14s: %14d : %8d\n", _ubiclib_logm_categoryname[i], i, _ubiclib_logm_level[i]);
			}

			r = 0;
		}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1) */

		break;
	} while (1);

	return r;
}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)
static int cli_cmdfunc__heap(char *str, int len, void *arg) {
	int r = -1;
	char *tmpstr;
	int tmplen;
	char *cmd = NULL;
	int cmdlen;

	tmpstr = str;
	tmplen = len;

	do {
		cmd = "param ";
		cmdlen = strlen(cmd);
		if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
			tmpstr = &tmpstr[cmdlen];
			tmplen -= cmdlen;

			unsigned int region_size;
			unsigned int region_m;
			unsigned int fbl_count;
			unsigned int fbl_memsize;
			unsigned int bitmap_memsize;

			r = -1;

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)
			cmd = "group ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				tmpstr = &tmpstr[cmdlen];
				tmplen -= cmdlen;

				sscanf(tmpstr, "%u %u", &region_size, &region_m);

				fbl_count = heap_group_calc_fblcount(region_size, region_m);
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1)
			cmd = "pgroup ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
			{
				tmpstr = &tmpstr[cmdlen];
				tmplen -= cmdlen;

				sscanf(tmpstr, "%u %u", &region_size, &region_m);

				fbl_count = heap_pgroup_calc_fblcount(region_size, region_m);
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)
			cmd = "bbuddy ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
			{
				tmpstr = &tmpstr[cmdlen];
				tmplen -= cmdlen;

				sscanf(tmpstr, "%u", &region_size);
				region_m = 2;

				fbl_count = heap_pgroup_calc_fblcount(region_size, region_m);
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)
			cmd = "wbuddy ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
			{
				tmpstr = &tmpstr[cmdlen];
				tmplen -= cmdlen;

				sscanf(tmpstr, "%u", &region_size);
				region_m = 2;

				fbl_count = heap_wbuddy_calc_fblcount(region_size, region_m);
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1)
			cmd = "bestfit ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0) {
				region_m = 2;

				fbl_count = 2;
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1)
			cmd = "firstfit ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
			{
				region_m = 2;

				fbl_count = 2;
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) */
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)
			cmd = "nextfit ";
			cmdlen = strlen(cmd);
			if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
			{
				region_m = 2;

				fbl_count = 2;
				fbl_memsize = sizeof(edlist_t) * fbl_count;
				bitmap_memsize = bitmap_getmemsize(fbl_count);

				r = 0;

				printf("    UBINOS__UBICLIB__HEAP_DIR...\n");
				printf("        algorithm                           (..._ALGORITHM)     : UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT\n");
			}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

			if (r == 0) {
				printf("        m value                             (..._M)             : %u\n", region_m);
				printf("        fbl (free block list)\n");
				printf("            fbl count minimum value         (..._FBLCOUNT)      : %u\n", fbl_count);
				printf("            fbl memory size                                     : %u bytes\n", fbl_memsize);
				printf("        fblbm (bitmap for indexing free block list)\n");
				printf("            fblbm memory size minimum value (..._FBLBM_BUFSIZE) : %u\n", bitmap_memsize);

				r = 0;
				break;
			} else {
				printf(" not supported\n");
				r = 0;
				break;
			}
		}

		break;
	} while (1);

	return r;
}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */

#if (INCLUDE__UBINOS__UBIK == 1)
static int cli_cmdfunc__date_set(char *str, int len, void *arg) {
	int r = -1;

	struct timeval tv;
	struct tm tm_data;

	int mon = 1;
	int mday = 0;
	int hour = 0;
	int min = 0;
	int year = 1970;
	int sec = 0;

	// [MMDDhhmm[YYYY][.ss]]
	sscanf(str, "%2d%2d%2d%2d%4d.%2d", &mon, &mday, &hour, &min, &year, &sec); 

	tm_data.tm_year = year - 1900;
	tm_data.tm_mon = mon - 1;
	tm_data.tm_mday = mday;
	tm_data.tm_hour = hour;
	tm_data.tm_min = min;
	tm_data.tm_sec = sec;
	tm_data.tm_isdst = -1;

	tv.tv_sec = mktime(&tm_data);
	tv.tv_usec = 0;
	settimeofday(&tv, NULL);

	r = 0;

	return r;
}
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_CLI == 1) */

