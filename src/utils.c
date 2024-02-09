#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

char* ReadFile(char* uri, int* buf_size){
	char* buf = NULL;
	FILE* fp = NULL;
	char tmp[128] = ".";
       	strcat(tmp, uri);
	fp = fopen(tmp, "rb");
	if(fp){
		fseek(fp, 0, SEEK_END);
		*buf_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buf = malloc(*buf_size);
		if(buf){
			fread(buf, 1, *buf_size, fp);
		}
		fclose(fp);
	}
	return buf;	
}
