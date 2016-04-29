#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

#define SA      struct sockaddr
#define MAXLINE 4096
#define MAXSUB  200


#define LISTENQ 1024

void http_connect(int ecg, int bpm);

extern int h_errno;

ssize_t process_http(int sockfd, char *host, char *page, char *poststr)
{
	char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
	ssize_t n;
	snprintf(sendline, MAXSUB,
		 "POST %s HTTP/1.0\r\n"
		 "Host: %s\r\n"
		 "Content-type: application/x-www-form-urlencoded\r\n"
		 "Content-length: %d\r\n\r\n"
		 "%s", page, host, (int)strlen(poststr), poststr);

	write(sockfd, sendline, strlen(sendline));
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = '\0';
		printf("%s", recvline);
	}
	return n;

}
int main(void)
{
	
    http_connect(100, 70);

}


void http_connect(int ecg, int bpm){
    

    
    int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	//********** You can change. Puy any values here *******
	char *hname = "mikronexus.com";
	char *page = "/ecg/ecg.php";
    
    char ecg_str = (char)ecg; char bpm_str = (char)bpm;
	//printf("data -> %c", ecg_str);
    
	char src[10]; char src2[10]; char src_temp[2];
	char buf[10];
	snprintf(buf, 10, "%d", ecg);
	//src_temp[0] = ecg_str;
	//strcpy(src, ecg_str);
	strcpy(src2, "ecg=");
	/*char postst_1 =*/ strcat(src2, buf);
    /*char *postst_2 = strcat("&bpm=", bpm_str);
    char *postst_3 = strcat("&password=", "test");
    
    char *postst = strcat(postst_1, postst_2);
         *postst = strcat(postst, postst_3);*/
    
    
    
   
	//*******************************************************

	char str[50];
	struct hostent *hptr;
	if ((hptr = gethostbyname(hname)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s",
			hname, hstrerror(h_errno));
		exit(1);
	}
	printf("hostname: %s\n", hptr->h_name);
	if (hptr->h_addrtype == AF_INET
	    && (pptr = hptr->h_addr_list) != NULL) {
		printf("address: %s\n",
		       inet_ntop(hptr->h_addrtype, *pptr, str,
				 sizeof(str)));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_pton(AF_INET, str, &servaddr.sin_addr);

	connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
	process_http(sockfd, hname, page, src2);
	close(sockfd);
	
    
}