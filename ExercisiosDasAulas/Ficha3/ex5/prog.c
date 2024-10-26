#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 

int main () 
{
int a = 10;

	if (fork() == 0) 
  		a++;
	else 
		a--;
	printf("A = %d\n" , a);
return 0; 
}
