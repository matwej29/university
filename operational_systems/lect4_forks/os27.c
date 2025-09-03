#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
int volatile done = 0;
void handler(int sig, siginfo_t *siginfo, void *context){
	printf("\nsignal %d was caught\n", sig );
	printf("your UID is %d\n", siginfo->si_uid);
	printf("your PID is %d\n", siginfo->si_pid);
	done = 1;
}
int main(){
	int pid = getpid();
	printf("Ok, let’s go − kill me (%d) and I’ll tell you who you are.\n", pid);
	//we ’ re using the more elaborated sigaction handler
    struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
//    sa.sa_handler = handler;
	sa.sa_sigaction = handler;
	if (sigaction(SIGINT, &sa, NULL)!=0){
		return(1);
	}

	while(!done){
	}
	printf("Told you so!\n");
	return(0);
}
