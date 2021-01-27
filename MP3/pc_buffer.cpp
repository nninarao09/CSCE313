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
#include <queue>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "pc_buffer.hpp"
#include "semaphore.hpp"


using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

#define PCBUFFER_TEST

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

PCBuffer::PCBuffer(int _size){
	size = _size;
    empty = new Semaphore(size);
	lock = new Semaphore(1);
	full = new Semaphore(0);

}

PCBuffer::~PCBuffer(){
	delete lock;
	delete full;
	delete empty;

}

void PCBuffer::Deposit(string _item){
	empty->P();
    lock->P(); 
    buff.push(_item);
    lock->V();
    full->V();
    
}

string PCBuffer::Remove(){
	full->P();
    lock->P();
    string poppedItem = buff.front(); 
    buff.pop(); 
    lock->V();
    empty->V();
	return poppedItem;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


