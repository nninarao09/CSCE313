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

#include "mutex_guard.hpp"
#include "mutex.hpp"  // change 

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

#define MUTEX_GUARD_TEST

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


MutexGuard::MutexGuard(Mutex &m){

	this->m = &m;
	
	cout << "System is locking" << endl;

	this->m->Lock();
	
	cout << "System is locked" << endl;
	

}

MutexGuard::~MutexGuard(){
	cout << "MutexGaurd destructor is working" << endl;
	m->Unlock();
	cout << "MutexGaurd destructor is has completed" << endl;

}



/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


