#include <stdio.h>
#include <unistd.h>

int main(){
	if(fork() > 0){
		printf("PARENT: this parent's process\n");
		printf("PARENT: my PID %d\n", getpid());
		printf("PARENT: I wait child\n");
		wait();
		printf("PARENT: success\n");

	}
	else{
		printf("CHILD: this child's process\n");
		printf("CHILD: my PID%d\n", getpid());
		return 0;
	}
	return 0;
}