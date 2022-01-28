#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	// opening the file with RDONLY
	int fd = open(argv[1], O_RDONLY);
	char ch;
	int line_count = 0;
	while( read(fd, &ch, 1) != 0 ){
		if( ch == '\n' ){
			line_count++;
		}
	}
	printf("Total number of lines in the given file : %s is %d\n", argv[1], line_count);
	return 0;
}

