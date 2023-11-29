/* @file debug_function.h
 * @author wangzefeng
 */


// usage: copy file to your system and include abs dir to your source code
// e.g #include "/home/user/debug_function.h"
#define __LOCAL_DEBUG__ // this is compile switch

#ifdef __LOCAL_DEBUG__

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>

#include <time.h>
#include <stdio.h>

#define RED_B	"\033[41m"
#define BLACK	"\033[30m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define PURPLE	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"
#define CLR		"\033[0m"

#define MSG			PURPLE
#define FATAL		RED_B
#define ERROR		RED
#define WARN		YELLOW
#define DEBUG		GREEN
#define NOTE		CYAN
#define INFO		BLUE

#define OUT2FILE 1
#define OUT2TERM 0

#define LOGFILEPATH "/tmp/wps.log"

#define LOGOUT(level, mode, fmt, args...) \
{\
	FILE* fp = NULL;\
	if (mode == OUT2FILE)\
		fp = fopen(LOGFILEPATH, "a+"); \
	else\
		fp = stdout;\
	pid_t pid_debug = getpid();\
	pid_t tid = syscall(SYS_gettid);\
	struct timeval tv;\
	struct timezone tz;\
	time_t tm;\
	struct tm stm;\
	suseconds_t tn;\
	gettimeofday(&tv, &tz);\
	tm = tv.tv_sec;\
	tn = tv.tv_usec;\
	localtime_r(&tm, &stm);\
	fprintf(fp, "%s[%04d-%02d-%02d %02d:%02d:%02d.%03ld.%03ld][pid:%d][tid:%d][%s:%d]<%s>%s " #fmt "\n",\
	mode? #level : level,\
	stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour,stm.tm_min,\
	stm.tm_sec, tn/1000, tn%1000, pid_debug, tid, __FILE__,__LINE__,__FUNCTION__,\
	mode? "" : CLR, ##args);\
	fflush(fp);\
	if (mode == OUT2FILE)\
		fclose(fp);\
	fp = NULL;\
}

#define LOG2FILE(level, fmt, args...)\
		LOGOUT(#level, OUT2FILE, fmt, ##args);

#define LOG2TERM(level, fmt, args...)\
		LOGOUT(level, OUT2TERM, fmt, ##args);

#define LOG(level, fmt, args...)\
		LOG2TERM(level, fmt, ##args);\
		LOG2FILE(#level, fmt, ##args);




#define START_TIME(str) struct timeval tv_begin_##str;\
		gettimeofday(&(tv_begin_##str), NULL);

#define STOP_TIME(str) struct timeval tv_end_##str;\
		gettimeofday(&(tv_end_##str), NULL);

#define ELAPSED_SEC(str)  ((long int)((tv_end_##str.tv_sec)-(tv_begin_##str.tv_sec)))
#define ELAPSED_MSEC(str)  ((long int)(((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))/1000 >=0? \
		((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))/1000:\
		((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))/1000+1000 ))
#define ELAPSED_USEC(str)  ((long int)(((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))%1000 >=0?\
		((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))%1000:\
		((tv_end_##str.tv_usec)-(tv_begin_##str.tv_usec))%1000+1000))

#define BEGINLOG(level, str) \
		START_TIME(str)\
		LOG2TERM(level,"begin:%s",#str);\
		LOG2FILE(#level,"begin:%s",#str);

#define ENDLOG(level, str) \
		STOP_TIME(str)\
		LOG2TERM(level, "end:%s:\t%ld.%03ld.%03ld", #str, ELAPSED_SEC(str), ELAPSED_MSEC(str), ELAPSED_USEC(str));\
		LOG2FILE(#level, "end:%s:\t%ld.%03ld.%03ld", #str, ELAPSED_SEC(str), ELAPSED_MSEC(str), ELAPSED_USEC(str));

#endif //__LOCAL_DEBUG__
