# include <sys/time.h>
# include<unistd.h>
# include <stdio.h>
int millisecond(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec/1000;
}
