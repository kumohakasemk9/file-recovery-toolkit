#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

void main(int argc, char* argv[]) {
	int i,s;
	char *ptn;
	FILE *f;
	char buffer[8192];
	if(argc < 3) {
		printf("Usage: %s filename pattern.\n",argv[0]);
		return;
	}
	if(strlen(argv[2]) < 2) {
		printf("Pattern format: sABCDE --> string \"ABCDE\" b00010203 --> Binary 0x0 0x1 0x2 0x3\n");
		return;
	}
	if(argv[2][0] == 's') {
		s = strlen(argv[2]) - 1;
		ptn = malloc(s);
		if(ptn == NULL) {
			printf("Heap fault!\n");
			return;
		}
		memcpy(ptn,&argv[2][1],s);
	} else if(argv[2][0] == 'b') {
		if(strlen(argv[2]) % 2 == 0) {
			printf("Binary pattern length is invalid.\n");
			return;
		}
		s = (strlen(argv[2]) - 1) / 2;
		ptn = malloc(s);
		if(ptn == NULL) {
			printf("Heap fault!\n");
			return;
		}
		for(i = 0; i < s; i++) {
			char *er;
			char buf[3];
			memcpy(buf,&argv[2][i * 2 + 1],2);
			ptn[i] = (char)strtol(buf,&er,16);
			if(strlen(er) != 0) {
				printf("Invalid format detected in binary pattern!\n");
				free(ptn);
				return;
			}
		}
	} else {
		printf("Pattern format prefix: s or b\n");
		return;
	}
	f = fopen(argv[1],"rb");
	if(f == NULL) {
		perror("File open failed");
		free(ptn);
		return;
	}
	printf("Searching %s...\n", argv[1]);
	unsigned long long a = 0;
	int mc = 0;
	while(feof(f) == 0) {
		int rs = fread(buffer,1,sizeof(buffer),f);
		if(rs == 0) {break;}
		for(i = 0; i < rs; i++) {
			if(buffer[i] == ptn[mc]) {
				if(mc < s - 1) {
					mc++;
				} else {
					// pattern match
					unsigned long long pos = ftell(f);
					unsigned long long offs = a + i - (s - 1);
					unsigned long long dumpaddr;
					char buffer2[32 + s];
					char buffer3[17];
					if(offs < 16) {
						dumpaddr = 0;
					} else {
						dumpaddr = offs - 16;
					}
					fseek(f,dumpaddr,SEEK_SET);
					int rs2 = fread(buffer2,1,sizeof(buffer2),f);
					fseek(f,pos,SEEK_SET);
					printf("Match found in 0x%llx\n",offs);
					printf("Address          | +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +a +b +c +d +e +f | 0123456789ABCDEF\n");
					int k;
					for(int j = 0;j < rs2; j++) {
						if((j % 16) == 0) {
							printf("%16llx | ",dumpaddr + j);
							memset(buffer3,0,17);
							k = 0;
						}
						printf("%02x ",(unsigned char)buffer2[j]);
						char t = buffer2[j];
						if(isgraph(t)) {buffer3[j % 16] = t;} else {buffer3[j % 16] = '.';}
						if(k == 15) {
							printf("| %s\n",buffer3);
						}
						k++;
					}
					if(k < 16) {
						for(int j = 0; j < 16 - k;j++) {printf("   ");}
						printf("| %s\n",buffer3);
					}
					printf("==============================================================================\n");
					mc = 0;
				}
			} else {
				mc = 0;
			}
		}
		a += rs;
	}
	free(ptn);
	fclose(f);
}
