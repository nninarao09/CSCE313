#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <cassert>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "netreqchannel.hpp"
using namespace std;
int MAX_MSG = 255;


struct sockaddr_in sock_in;

NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no){
	
	stringstream ss;
	ss << _port_no;
	string port_no = ss.str();
	
	memset(&sock_in, 0, sizeof(sock_in));
	sock_in.sin_family = AF_INET;
	
	if(struct servent *pse = getservbyname(port_no.c_str(), "tcp")){
		sock_in.sin_port = pse->s_port;
	}
	
	if(struct hostent *h = gethostbyname(_server_host_name.c_str())){
		memcpy(&sock_in.sin_addr, h->h_addr, h->h_length);
	}
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(fd, (struct sockaddr*) &sock_in, sizeof(sock_in)) < 0){
	}
		
}

NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, int backlog ,void * (*connection_handler) (void *)){
	
	stringstream ss;
	ss << _port_no;
	string port_no = ss.str();
	
	memset(&sock_in, 0, sizeof(sock_in));
	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = INADDR_ANY;
	
	if(struct servent* pse = getservbyname(port_no.c_str(), "tcp")){
		sock_in.sin_port = pse->s_port;
	}
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Not able to Create Socket\n";
    }

    if (bind(fd, (struct sockaddr *) &sock_in, sizeof(sock_in)) < 0) {
        cerr << "Not able to Bind\n";
    }
	
	
	listen(fd, backlog);
	
	for(;;){
		pthread_t thread;
		int *wor = new int;
		
		//*wor = accept(fd, (struct sockaddr*)&sock_in, (socklen_t *)&(sizeof(sock_in)));
		
		if(wor < 0){
			delete wor;
		}
		if(errno = EINTR){
			continue;
		}
		
		pthread_create(&thread, NULL, connection_handler, (void*)wor);
	}
	
	cout << "connection complete" << endl;
}

NetworkRequestChannel::~NetworkRequestChannel(){
	close(fd);
}




string send_request(string _request){
	//cwrite(_request);
	//string s = cread()
	//return s;
}

string cread(){
	char arr[MAX_MSG];
	
	read(fd, arr, MAX_MSG);
	
	string str = arr;
	
	return str;

}

int cwrite(string _msg){
	write(fd, _msg.c_str(), strlen(_msg.c_str()) +1);
	return 0;
}

