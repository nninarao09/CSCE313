/* 
 File: my_allocator.cpp
 
 Author: <your name>
 Department of Computer Science
 Texas A&M University
 Date  : <date>
 
 Modified:
 
 This file contains the implementation of the class MyAllocator.
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

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
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
    
	start = std::malloc(_size);
	current_start = (char*)start;
	remaining_mem = _size;
}

MyAllocator::~MyAllocator() {
    std::free(start);
}

void* MyAllocator::Malloc(size_t _length) {
	
	
	if(_length > remaining_mem){
		return nullptr;
	}
	
	void* result = current_start;
	current_start += _length;
	remaining_mem -= _length;
	return result;
	
	
    // This empty implementation just uses C standard library malloc
    cout << "MyAllocator::Malloc called with length = " << _length << endl;
}

bool MyAllocator::Free(void* _a) {
	
	
    // This empty implementation just uses C standard library free
    cout << "MyAllocator::Free called" << endl;
    //std::free(_a);
    return true;
}

