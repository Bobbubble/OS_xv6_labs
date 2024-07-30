#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
	int p[2], temp[100];
	// 打开管道，并写入2-35数字 
	pipe(p);
	for (int i = 2; i < 36; i++){
		if (write(p[1],&i,4) != 4)
			exit(1);
	}
	while(1)
	{
		//子进程当中 
		if(fork() == 0){
			close(p[1]);
			int a, prime, ptr,rest;
			a = prime = ptr = rest = 0;
			a = read(p[0],&prime,4);//将第一个质数读出 
			if(a == 0)
				exit(0);
			else if(a<0)
				exit(1); 
			else
				printf("prime %d\n",prime);
			
			for(int i = 0; a>0; i++){
				a = read(p[0], &rest, 4);
				if(a<0)
					exit(1);
				//如果读出来的数是指数，则储存在temp当中 
				if(rest % prime){
					temp[ptr] = rest;
					ptr++;
				}
			} 
			close(p[0]);
			pipe(0);//重新开启一个子管道 
			for(int i = 0; i<ptr; i++){
				//将保存的质数写入子管道当中 
				if(write(p[1],&temp[i],4)!=4)
					exit(1);
			}
		} 
		//父进程当中 
		else{
			close(p[0]);
			close(p[1]);
			wait(0);
			exit(0);
		}
	}
} 
