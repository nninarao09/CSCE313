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
#define NUM_BINS 10
#define MAX_WORKER_THREADS 126

//struct timeval start;
//struct timeval end;
//long total_time;
string people[] = {"Joe", "Jane", "Jill"};
int histogram[NUM_PEOPLE][NUM_BINS];


/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void *request_threads_func(void *args);
void *worker_thread_func(void *args);
void *stat_thread_func(void *args);
void print_hist(string name, int hist[]);
PCBuffer *lookup(string req, PCBuffer **holder);


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

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	
	
  int request_per_person = 0; // -n
  int buff_size = 0;  //-b
  int number_worker_threads = 0; //-w

  int c = 0;
  while((c = getopt (argc, argv, "n:w:b:")) != -1){
	switch(c){
        case 'n':
            request_per_person = atoi(optarg);
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
  
  clock_t begin = clock();

  //start timer
  //gettimeofday(&start, NULL);

  /*
  std::string reply1 = chan.send_request("hello");
  std::cout << "Reply to request 'hello' is '" << reply1 << "'" << std::endl;
  */

  
  // create and run req threads
  RTFARGS *reqp[NUM_PEOPLE];
  
  PCBuffer *work_buff = new PCBuffer(buff_size);
    
  // Joe
  cout << "Creating request thread for: " << people[0] << endl;
  reqp[0] = new RTFARGS{
	  request_per_person,  // the number of worker threads
	  people[0],
	  work_buff
  };  
  pthread_create(&request_threads[0], NULL, request_threads_func, (void*) reqp[0]);

  
  //jane
  cout << "Creating request thread for: " << people[1] << endl;
  reqp[1] = new RTFARGS{
	  request_per_person,  // the number of worker threads
	  people[1],
	  work_buff
  };  
  pthread_create(&request_threads[1], NULL, request_threads_func, (void*) reqp[1]);
  
  //jill
  cout << "Creating request thread for: " << people[2] << endl;
  reqp[2] = new RTFARGS{
	  request_per_person,  // the number of worker threads
	  people[2],
	  work_buff
  };  
  pthread_create(&request_threads[2], NULL, request_threads_func, (void*) reqp[2]);
	

  
  
  
  PCBuffer *stat_buff[NUM_PEOPLE];
  // create stat threads
  STATARGS *stat[NUM_PEOPLE];
  for(int i=0; i<NUM_PEOPLE; ++i){
	cout << "Creating statistic thread for: " << people[i] << endl;
	stat_buff[i] = new PCBuffer(request_per_person);
	stat[i] = new STATARGS{
		request_per_person,
		i,
		stat_buff[i]
	};
	
	pthread_create(&stat_threads[i], NULL, stat_thread_func, (void*) stat[i]);
  }
  /*
  //still need to build histagram func to replace the request thread func
  cout << "Creating statistic thread for: " << people[0] << endl;
  pthread_create(&stat_threads[0], NULL, stat_thread_func, (void*) stat[0]);
	
  cout << "Creating statistic thread for: " << people[1] << endl;
  pthread_create(&stat_threads[1], NULL, stat_thread_func, (void*) stat[1]);
	
  cout << "Creating statistic thread for: " << people[2] << endl;
  pthread_create(&stat_threads[2], NULL, stat_thread_func, (void*) stat[2]);
  
  */
  
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

  /* -- Start sending a sequence of requests */


/*
  std::string reply2 = chan.send_request("data Joe Smith");
  std::cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << std::endl;

  std::string reply3 = chan.send_request("data Jane Smith");
  std::cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << std::endl;

  for(int i = 0; i < 5; i++) { // change the values to record the points
    std::string request_string("data TestPerson" + int2string(i));
    std::string reply_string = chan.send_request(request_string);
    std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }
*/

  // wait for the threads to finish their jobs
  pthread_join(request_threads[0], NULL);
  pthread_join(request_threads[1], NULL);
  pthread_join(request_threads[2], NULL);

  for(int i=0; i<number_worker_threads; ++i){
	  work_buff->Deposit("quit");
  }
  
  for(int i=0; i<number_worker_threads; ++i){
	  pthread_join(worker_threads[i], NULL);
  }
  
  pthread_join(stat_threads[0], NULL);
  pthread_join(stat_threads[1], NULL);
  pthread_join(stat_threads[2], NULL);
  
  clock_t end = clock();
  double total_time = double(end-begin) / CLOCKS_PER_SEC;
  // end timer
  //gettimeofday(&end, NULL);
  
  std::string reply4 = chan.send_request("quit");
  std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;
  
  
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
	  delete channel[i];
	  
  }

  
  
  wait(NULL);
  
  
  // print histograms and data
  print_hist(people[0], histogram[0]);
  print_hist(people[1], histogram[1]);
  print_hist(people[2], histogram[2]);
  
  cout << "Total number of request is: " << request_per_person << endl;
  cout << "The buffer size is : " << buff_size << endl;
  cout << "Total number of worker threads: " << number_worker_threads << endl;
  cout << "Total time taken is: " << total_time << endl;


  


  usleep(1000000);
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
		cout <<"Response to: " << req << " - " << "the reply was: " << reply << endl;
		if(req == "quit"){
			break;
		}
		PCBuffer *sbb = lookup(req, wtfa->holder);
        sbb->Deposit(reply);
	}
	return 0;
}

PCBuffer *lookup(string req, PCBuffer **holder){
	for(int i=0; i<NUM_PEOPLE; ++i){
		size_t found = req.find(people[i]);
		if(found != std::string::npos){
			cout << "Depositing " << req << " into stat buffer for " << people[i] << endl;
            return holder[i];
		}
	}
}

void *stat_thread_func(void *args){  // makes the histogram
	STATARGS *print = (STATARGS*) args;
	string item;
	cout << "Making Histogram for: " << people[print->patient_index] << endl;
	int bin, index;
	for(int i=0; i<print->n_req; ++i){
		item = print->buff->Remove();
		index = atoi(item.c_str());
		bin = (int) (floor(index/10));
		histogram[print->patient_index][bin] = histogram[print->patient_index][bin] + 1;
	}
	
	cout << "Statistics func is finished" << endl;
	return 0;
}

void print_hist(string people, int hist[]){
	
	cout << "Printing histogram for " << people << endl;
	//cout << "---------------------------------------------------------"<< endl;
	for(int i=0; i<100; i+=10){
		int count =0;
		cout << "[" << setw(2) << setfill('0') << i << ", " << setw(2) <<  setfill('0') <<i+9<< "]: " << setfill(' ');
		for( int j=0; j<hist[i/10]; ++j){
			count++;
		}
		cout << count;
		cout << endl;
	}
	cout << "---------------------------------------------------------"<< endl;
}
