#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

void child_handler(int signo)
{
	printf("Child handler!\n");
}

void foo(void)
{
	for(int i = 0; i < 5; ++i)
	{
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	int ret;
	pid_t pid;
	sigset_t set;

	signal(SIGCHLD, child_handler);

	ret = sigemptyset(&set);

	if(ret < 0)
	{
		perror("sigemptyset()");
		return 1;
	}

	ret = sigaddset(&set, SIGCHLD);
	
	if(ret < 0)
	{
		perror("sigaddset()");
		return 1;
	}
	
	ret = sigprocmask(SIG_BLOCK, &set, nullptr);

	if(ret < 0)
	{
		perror("sigprocmask()");
		return 1;
	}

	pid = fork();

	if(pid < 0)
	{
		perror("fork()");
		return 1;
	}

	if(pid == 0)
	{
		ret = execl("child", "./child", nullptr);
		if(ret < 0)
		{
			perror("execl()");
			return 1;
		}
	}

	foo();

	ret = sigprocmask(SIG_UNBLOCK, &set, nullptr);

	if(ret < 0)
	{
		perror("sigprocmask()");
		return 1;
	}

	return 0;
}
