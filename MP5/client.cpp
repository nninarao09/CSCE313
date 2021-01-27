/*
    File: simpleclient.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2019/09/23

    Simple client main program for MP2 in CSCE 313
*/
//hello
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iomanip>      // for the hist
#include <errno.h>
#include <unistd.h>

#include "reqchannel.hpp"
#include "pc_buffer.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

typedef struct work_thread_params {
    PCBuffer *buff;
    RequestChannel *RC;
	PCBuffer **holder;
} WTFARGS;

typedef struct request_thread_params {
	int n_req;
	string patient_name;
	PCBuffer *buff;
} RTFARGS;

typedef struct statistic_params {
	int n_req;
	int patient_index;
	PCBuffer *buff;
} STATARGS;


/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/
#define NUM_PEOPLE 3
// worker threads should not go over 126



string people[] = {"Joe", "Jane", "Jill"};

int joeHist[10];
int janeHist[10];
int jillHist[10];


/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
  std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

void enableDataServer(){
	if(fork()==0){
		std::cout << "Starting data server" << std::endl;
		execv("dataserver", NULL);
	}

}


void *request_threads_func(void *args){
	RTFARGS *rtfa = (RTFARGS*) args;
	for(int i=0; i<rtfa->n_req; ++i){ 
		string req = "data " + rtfa->patient_name;
		cout << "Depositing the request: " << req << endl;
		rtfa->buff->Deposit(req);
	}
	cout << "Request thread is finished for: " << rtfa->patient_name << endl;
	return 0;
}


void *worker_thread_func(void *args){
	WTFARGS * wtfa = (WTFARGS*) args;
	for(;;){
		string req = wtfa->buff->Remove();
		cout << " requesting string  = " << req << endl;
		string reply = wtfa->RC->send_request(req);
		
		
		if(req == "quit"){
			break;
		}

		PCBuffer *temp;
		//check each case
		if(req.find(people[0]) != string::npos){
			temp = wtfa->holder[0];
		}
		if(req.find(people[1]) != string::npos){
			temp = wtfa->holder[1];
		}
		if(req.find(people[2]) != string::npos){
			temp = wtfa->holder[2];
		}
		
		temp->Deposit(reply);

	}
	return 0;
}

 
void *stat_thread_func(void *args){  // makes the histogram
	STATARGS *print = (STATARGS*) args;
	cout << "Making Histogram for: " << people[print->patient_index] << endl;
	string item;
	int other;
	int index;
	for(int i=0; i<print->n_req; ++i){
		item = print->buff->Remove();
		index = atoi(item.c_str());
		other = (index/10);
		if(people[print->patient_index] == "Joe"){
			joeHist[other] = joeHist[other] + 1;
		}
		else if(people[print->patient_index] == "Jane"){
			janeHist[other] = janeHist[other] + 1;
		}
		else if(people[print->patient_index] == "Jill"){
			jillHist[other] = jillHist[other] + 1;
		}
	}
	
	cout << "Statistics func is finished" << endl;
	return 0;
}

void print_hist(string people, int hist[]){
	int k = 0;
	cout << "Printing histogram for " << people << endl;
	//cout << "---------------------------------------------------------"<< endl;
	for(int i=0; i<10; i++){
		int count =0;
		cout << "("  << k << "-"  << k+9<< "): ";
		for( int j=0; j<hist[k/10]; ++j){
			count++;
		}
		cout << count;
		if(k<90){
			cout << " -> ";
		}
		k=k+10;
	}
	cout << endl;
	cout << "---------------------------------------------------------"<< endl;
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	
	
  int requests_for_each = 0; // -n
  int buff_size = 0;  //-b
  int number_worker_threads = 0; //-w

  int c = 0;
  while((c = getopt (argc, argv, "n:w:b:")) != -1){
	switch(c){
        case 'n':
            requests_for_each = atoi(optarg);
            break;
        case 'w':
             number_worker_threads = atoi(optarg);
            break;
        case 'b':
            buff_size = atoi(optarg);
            break;

        default:
			break;
    }
  }
  
  
  // initialize the threads
  pthread_t request_threads[NUM_PEOPLE];
  pthread_t stat_threads[NUM_PEOPLE];
  pthread_t worker_threads[number_worker_threads];

  // start connection to data server
  enableDataServer(); 
 
  
  std::cout << "CLIENT STARTED:" << std::endl;

  std::cout << "Establishing control channel... " << std::flush;
  RequestChannel chan("control", RequestChannel::Side::CLIENT);
  std::cout << "done." << std::endl;
  
  timeval t1, t2;
  gettimeofday(&t1, NULL);
  
  // create and run req threads
  RTFARGS *reqp[NUM_PEOPLE];
  
  PCBuffer *work_buff = new PCBuffer(buff_size);
    
  // Joe
  //cout << "Creating request thread for: " << people[0] << endl;
  reqp[0] = new RTFARGS{
	  requests_for_each,  // the number of worker threads
	  people[0],
	  work_buff
  };  
  pthread_create(&request_threads[0], NULL, request_threads_func, (void*) reqp[0]);

  
  //jane
  //cout << "Creating request thread for: " << people[1] << endl;
  reqp[1] = new RTFARGS{
	  requests_for_each,  // the number of worker threads
	  people[1],
	  work_buff
  };  
  pthread_create(&request_threads[1], NULL, request_threads_func, (void*) reqp[1]);
  
  //jill
 // cout << "Creating request thread for: " << people[2] << endl;
  reqp[2] = new RTFARGS{
	  requests_for_each,  // the number of worker threads
	  people[2],
	  work_buff
  };  
  pthread_create(&request_threads[2], NULL, request_threads_func, (void*) reqp[2]);
	

  
  
  // creating stat threads
  PCBuffer *stat_buff[NUM_PEOPLE];
  STATARGS *stat[NUM_PEOPLE];
  //joe
  //cout << "Creating statistic thread for: " << people[0] << endl;
  stat_buff[0] = new PCBuffer(requests_for_each);
  stat[0] = new STATARGS{
	requests_for_each,
	0,
	stat_buff[0]
  };
  pthread_create(&stat_threads[0], NULL, stat_thread_func, (void*) stat[0]);
 
 //jane
  cout << "Creating statistic thread for: " << people[1] << endl;
  stat_buff[1] = new PCBuffer(requests_for_each);
  stat[1] = new STATARGS{
	requests_for_each,
	1,
	stat_buff[1]
  };
  pthread_create(&stat_threads[1], NULL, stat_thread_func, (void*) stat[1]);
  
  //jill
  cout << "Creating statistic thread for: " << people[2] << endl;
  stat_buff[2] = new PCBuffer(requests_for_each);
  stat[2] = new STATARGS{
	requests_for_each,
	2,
	stat_buff[2]
  };
  pthread_create(&stat_threads[2], NULL, stat_thread_func, (void*) stat[2]);

	
  
  
  // create the worker threads
  RequestChannel *channel[number_worker_threads];
  WTFARGS *workerThread[number_worker_threads];
  
  for(int i=0; i<number_worker_threads; ++i){
	cout << "Creating worker thread for: " << people[i] << endl;
	string reply5 = chan.send_request("newthread");
	cout << "reply for request new thread is '" << reply5 << "'" << endl;
	channel[i] = new RequestChannel(reply5, RequestChannel::Side::CLIENT);
	workerThread[i] = new WTFARGS{
		work_buff,
		channel[i],
		stat_buff
	};
	pthread_create(&worker_threads[i], NULL, worker_thread_func, (void*) workerThread[i]);
  }


  // wait for the threads to finish their jobs
  pthread_join(request_threads[0], NULL);
  pthread_join(request_threads[1], NULL);
  pthread_join(request_threads[2], NULL);

  for(int i=0; i<number_worker_threads; ++i){
	  work_buff->Deposit("quit");
  }
  
  for(int i=0; i<number_worker_threads; ++i){  // need to send message to kill the worker threads
	  pthread_join(worker_threads[i], NULL);
  }
  
  pthread_join(stat_threads[0], NULL);
  pthread_join(stat_threads[1], NULL);
  pthread_join(stat_threads[2], NULL);
  
  // end timer
  gettimeofday(&t2, NULL);
  
  std::string reply4 = chan.send_request("quit");
  //std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;
  
  
  //STEP 5 Cleanly terminate all threads

  for(int i=0; i<NUM_PEOPLE; ++i){
	  delete reqp[i];
  }
  for( int i=0; i<NUM_PEOPLE ; ++i){
	  delete stat_buff[i];
  }
  for(int i=0; i<NUM_PEOPLE; ++i){
	  delete stat[i];
  }
  
	  
  for(int i=0; i< number_worker_threads; ++i){
	  delete workerThread[i];
	  
  }
  
  for(int i=0; i< number_worker_threads; ++i){
	  if(i>0){
		//reply4 = chan.send_request("quit");
	  }
	  delete channel[i];
	  
	  
  }
  reply4 = chan.send_request("quit");
  
  
  wait(0);
  
  print_hist(people[0], joeHist); //joe
  print_hist(people[1], jillHist); // jane
  print_hist(people[2], janeHist);  // jill
  
  cout << "Total number of request is: " << requests_for_each<< endl;
  cout << "The buffer size is : " << buff_size << endl;
  cout << "Total number of worker threads: " << number_worker_threads << endl;
  cout << "Total other timer taken is: " << t2.tv_usec-t1.tv_usec << endl;


  usleep(1000000);
}








