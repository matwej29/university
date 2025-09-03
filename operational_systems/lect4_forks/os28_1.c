#define _GNU_SOURCE // to define REG_RIP
#include <stdio.h>
#include <signal.h>
#include <sys/ucontext.h>
// #include <unistd.h>

static void handler(int sig_no, siginfo_t *info, void *cntx){
	ucontext_t *context = (ucontext_t *) cntx;
	// unsigned long pc = context->uc_mcontext.gregs[REG_RIP];
	unsigned long pc = context->uc_mcontext->__ss.__pc;
	printf("IP: %lx value 0x%x\n", pc, *(int*)pc);
}

int main(){
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	printf("Let's go!\n");
    for (;;);
    return 0;
}
