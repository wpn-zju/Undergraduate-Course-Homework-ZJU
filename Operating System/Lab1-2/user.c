#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	system("dmesg -c");
	system("insmod displayinfo.ko");
	system("rmmod displayinfo");
	system("dmesg > output");
	return 0;
}