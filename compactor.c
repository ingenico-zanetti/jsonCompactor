// Parse an indented JSON stream to one-line JSON by removing all CR/LF except for the lines containing only a }
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, const char *argv[]){
	FILE *output = stdout;
	if(argc < 2){
		fprintf(stderr, "Usage %s <input file name> [<output file name>]" "\n", argv[0]);
	}else{
		FILE *input = fopen(argv[1], "rb");
		fprintf(stderr, "using [%s] as input" "\n", argv[1]);
		if(argc > 2){
			output = fopen(argv[2], "rb");
			fprintf(stderr, "using [%s] as output" "\n", argv[2]);
		}
		if((NULL != input) && (NULL != output)){
			unsigned char buffer[1024];
			int startOfLine = true;
			int outputNewLine = false;
			for(;;){
				int lus = fread(buffer, 1, sizeof(buffer), input);
				if(0 < lus){
					unsigned char octet;
					int length = lus;
					unsigned char *p = buffer;
					while(length-- > 0){
						unsigned char octet = *p++;
						if(('\n' == octet) || ('\r' == octet)){
							if(outputNewLine){
								// fprintf(stderr, "new line character and outputNewLine TRUE" "\n");
								fputc(octet, output);
								fflush(output);
							}
							// fprintf(stderr, "startOfLine now TRUE" "\n");
							startOfLine = true;
						}else{
							fputc(octet, output);
							if(startOfLine && ('}' == octet)){
								// fprintf(stderr, "startOfLine && '}' TRUE" "\n");
								outputNewLine = true;
							}else{
								outputNewLine = false;
							}
							startOfLine = false;
						}
					}

				}else{
					// fprintf(stderr, "EOF or error detected, closing the file" "\n");
					fclose(input);
					for(;;){
						input = fopen(argv[1], "rb");
						if(NULL != input){
							break;
						}
						perror(argv[1]);
						// fprintf(stderr, "%s successfully reopened" "\n", argv[1]);
						sleep(1);
					}
				}
			}

		}
	}
	return(0);
}

