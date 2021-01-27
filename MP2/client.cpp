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


#include <errno.h>
#include <unistd.h>

#include "reqchannel.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
  std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


//local execution of generate data function
std::string generate_data() {
  // Generate the data to be returned to the client.
  return int2string(rand() % 100);
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  struct timeval start_single, end_single, start_multiple, end_multiple;
  long time_for_sreq_server, time_for_mreq_server;

  if(fork() == 0){
	std::cout << "Starting data server" << std::endl;
	execv("dataserver", NULL); // ask about exec function
  }



  std::cout << "CLIENT STARTED:" << std::endl;

  std::cout << "Establishing control channel... " << std::flush;
  RequestChannel chan("control", RequestChannel::Side::CLIENT);
  std::cout << "done." << std::endl;


  /* -- Start sending a sequence of requests */

  //gettimeofday(&start_single, NULL); // locally
  std::string reply1 = chan.send_request("hello");
  std::cout << "Reply to request 'hello' is '" << reply1 << "'" << std::endl;
  //gettimeofday(&end_single, NULL);

  //gettimeofday();
  std::string reply2 = chan.send_request("data Joe Smith");
  std::cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << std::endl;
  //gettimeofday();

  //gettimeofday();
  std::string reply3 = chan.send_request("data Jane Smith");
  std::cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << std::endl;
  //gettimeofday();

  gettimeofday(&start_multiple, NULL);
  for(int i = 0; i < 5; i++) { // change the values to record the points
    std::string request_string("data TestPerson" + int2string(i));
    std::string reply_string = chan.send_request(request_string);
    std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }
  gettimeofday(&end_multiple, NULL);


  for(int i = 0; i < 5; i++) { // change the values to record the points
  // use generate data function
	generate_data();
    //std::string request_string("data TestPerson" + int2string(i));
    //std::string reply_string = chan.send_request(request_string);
    //std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }


  //gettimeofday();
  std::string reply4 = chan.send_request("quit");
  std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;
  //gettimeofday();


  //Timings for part 2 of MP2

  //time_for_sreq_server = end_single.tv_usec - start_single.tv_usec;
  time_for_mreq_server = end_multiple.tv_usec - start_multiple.tv_usec;


  std::cout << std::endl;
  std::cout << "-> Timings for PART 2" << std::endl;
  //std::cout << "-> Total time for single request locally in microseconds: " << time_for_sreq_server << std::endl;
  std::cout << "-> Total time for multiple request locally in microseconds: " << time_for_mreq_server << std::endl;
  std::cout << std::endl;




  usleep(1000000);
}
