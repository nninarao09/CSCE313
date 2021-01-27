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

#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "mutex.hpp"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

#define MUTEX_TEST

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

Mutex::Mutex(){

	pthread_mutex_init(&mu, NULL);
	cout << "Mutex is created " << endl;

}

Mutex::~Mutex(){
	pthread_mutex_destroy(&mu);
	cout << "Mutex is destroyed " << endl;

}

void Mutex::Lock(){
	cout << "Mutex is locking " << endl;
	pthread_mutex_lock(&mu);
	cout << "Mutex Lock function is complete " << endl;

}

void Mutex::Unlock(){
	cout << "Mutex is unlocking " << endl;
	pthread_mutex_unlock(&mu);
	cout << "Mutex Unlock function is complete " << endl;


}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


