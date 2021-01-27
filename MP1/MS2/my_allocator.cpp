/* 
 File: my_allocator.cpp
 
 Author: Nina Rao
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
#include "free_list.hpp"
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
	blocksize = _basic_block_size; 
	SegmentHeader* seg1 = new(start)SegmentHeader(_size);
	free_list = new(start) FreeList();
	start = (char*)start + sizeof(FreeList);
	free_list->Add(seg1); 
}

MyAllocator::~MyAllocator() {
    std::free((char*)start + sizeof(FreeList)); //std::free(start)
}

size_t MyAllocator::GetBasicBlockSize(){
	return blocksize;
}


void* MyAllocator::Malloc(size_t _length) {
		
	//round up to next multiple of blocksizes

	size_t len = _length+sizeof(SegmentHeader); 
	
	size_t round = GetBasicBlockSize();		
	if(len%round != 0){
		while(len > round){
			round = round + GetBasicBlockSize();
		}
		len = round;
	}


	SegmentHeader* seg = free_list->head;
	
	
	while(seg != nullptr && seg->GetLength() < len){
		seg = seg->GetNext();
	}
	
	if(seg == nullptr){
		return nullptr;
	}
	
	free_list->Remove(seg);
	
	if(seg->GetLength() > len){
		SegmentHeader* seg2 = seg->Split(len);
		free_list->Add(seg2);

	}
	
	void* ptr = (void*)((char*)seg + sizeof(SegmentHeader));
	return ptr;
	
	
	
    // This empty implementation just uses C standard library malloc
    cout << "MyAllocator::Malloc called with length = " << _length << endl;
}

bool MyAllocator::Free(void* _a) {
	 // map _a to seg node
	 //add seg to freelist
	 SegmentHeader* seg = (SegmentHeader*)((char*)_a - sizeof(SegmentHeader));
	 free_list->Add(seg);
	
    // This empty implementation just uses C standard library free
    cout << "MyAllocator::Free called" << endl;
    //std::free(_a);
    return true;
}

