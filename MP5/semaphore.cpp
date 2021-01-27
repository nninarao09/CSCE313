/*
    File: 

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

#include "semaphore.hpp"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

#define SEMAPHORE_TEST

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

Semaphore::Semaphore(int _val){

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
	value = _val;

}

Semaphore::~Semaphore(){
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
}

int Semaphore::P(){
	
	pthread_mutex_lock(&m);
    

    if (value < 0){
        pthread_cond_wait(&c, &m);
	}
	value--;
    pthread_mutex_unlock(&m);
    return 0;
}

int Semaphore::V(){
	pthread_mutex_lock(&m);
    

    if (value <= 0){
        pthread_cond_signal(&c);
	}
	
	value++;
    pthread_mutex_unlock(&m);
    return 0;

}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


