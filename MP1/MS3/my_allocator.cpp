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
int MyAllocator::fib(int idx){
	int fibi[idx+1];
	int i; 
	
	fibi[0]= 1;
	fibi[1] = 2;
	
	for(i = 2; i<=idx; ++i){
		fibi[i] = fibi[i-1]+fibi[i-2];
	}
	
	return fibi[idx];
}

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
	
	start = std::malloc(_size);
	blocksize = _basic_block_size; 
	//SegmentHeader* seg1 = new(start)SegmentHeader(_size);
	start = (char*)start + sizeof(FreeList);
	
	//cout << "starrt " << start << endl;
	
	idx = 0;
  	int i = 0;
	
	while(fib(i)*blocksize <= _size){
		++i;
	}
	sizeofArray = i;
	//bl = new int[i];
	//large enough to hold all the segments 
	fl = new FreeList[i];
		
	// allocate the freelists
	
	//run a loop to assign idx and size
	for(int j=0; j<i; ++j){
		fl[j].sizetoA = fib(j)*blocksize;
		fl[j].idx = j;
		
		//cout << "fl[j]: " << fl[j] < endl;
		
	}
	
	// push segment into the freelist and start from the top
	
	size_t remaining_size = _size;
	char* temp_start = (char*)start;
	
	for(int k=sizeofArray-1; k>=0; --k){
		//cout << " fib k block: " << fib(k)*blocksize << endl;
		//cout << " checker for size: " << remaining_size - fib(k)*blocksize<< endl;
		int x = remaining_size-fib(k)*blocksize;
		//cout << "x: " << x << endl;
		
		if(x >= 0){
			
			SegmentHeader* seg = new(temp_start)SegmentHeader(fib(k)*blocksize);
			fl[k].Add(seg); // = fib(k)*blocksize;
			//seg = fib(k)*blocksize;
			//cout << "remaining size1: " << remaining_size << endl;

			remaining_size = remaining_size - fib(k)*blocksize;
			temp_start = temp_start + fib(k)*blocksize;
			
			//cout << "fl[k]: "<< fl[k].sizetoA << " and k is " << k << endl;
			//cout << "remaining size: " << remaining_size << endl << endl;

		}


	}
}

MyAllocator::~MyAllocator() {
    std::free((char*)start + sizeof(FreeList)); //std::free(start)
}

	
size_t MyAllocator::GetBasicBlockSize(){
	return blocksize;
}


/*
FreeList[] MyAllocator::GetFreeListArray(){
	return fl;
}
*/

void* MyAllocator::Malloc(size_t _length) {
	
	//cout << "IAM in MALLOC" << endl;

	size_t	len = _length+sizeof(SegmentHeader); 
	

	int i = 0;
	size_t round = fib(i)*GetBasicBlockSize();	 // changed from fib(i) to fib(0)	
	
	//cout << "len: " << len << endl;
	//cout << "round: "  << round << endl;

	if(len<round){
		len = round;
	}
	else{	
		if(len%round != 0){
			//size_t round = fib(i)*GetBasicBlockSize();	
			while(len > round){
				round = round + fib(i)*GetBasicBlockSize();
				cout << "round: "  << round << endl;
				++i; // possibly remove
			}
			len = round;
		}
	}
	
	
	//cout << "len : " << len << endl;
	//cout << "size of array " << sizeofArray << endl;
	
	// idx less than size of freelist array 
	//idx<fl.size() && (fl[idx].empty() && fl[idx].sizetoA < len
	idx = 0;
	while( idx<sizeofArray && (fl[idx].head == nullptr && fib(idx) < len ) ){
		idx++;
	}
	//cout << "idx " << idx << endl; 
	
	//idx == fl->sizetoA
	if(idx == sizeofArray){
		return nullptr;
	}
	//cout << "len before remove: " << fl[idx].head->GetLength() << endl;
	
	
	SegmentHeader* seg = fl[idx].head;
	fl[idx].Remove(seg);
	
	//cout << "fl idx " << fl[idx].sizetoA << endl;
	//cout << "seg lentgh: " << seg->GetLength() << endl;
	
	if(seg->GetLength() == len){
		//cout << "THis weird size to allocate if" << fl[idx].sizetoA << endl;

		void* ptr = (void*)((char*)seg + sizeof(SegmentHeader));
		return ptr;
	}
	else{
		//seg.sizetoA = fl[i-1];  // decrease the seg length to the previous number
		//cout << "THis weird size to allocate else " << fl[idx].sizetoA << endl;
		
		//cout <<  "before split" << seg->GetLength() << endl;
		
		SegmentHeader* seg2 = seg->Split(fl[idx-1].sizetoA);
		
		//cout << "after split" << seg->GetLength() << endl;
		//cout << seg->GetBuddyType()<< endl;

		if(seg->GetLength() == seg2->GetLength()){
			//cout << "fl[idx] " << fl[idx].sizetoA << endl;
			//cout << "fl[idx-1] " << fl[idx - 1].sizetoA << endl;

			fl[idx-1].Add(seg);
			//cout <<"succesfull add seg 1 if "<< seg->GetLength() << endl;
			//cout << "seg buddy " << seg->GetBuddyType() << endl;
			fl[idx-1].Add(seg2);
			//cout <<"succesfull add seg 2 if "<< seg2->GetLength() << endl;
			//cout << "seg2 buddy " << seg2->GetBuddyType() << endl;
			//cout << "test " << seg->GetNext()->GetLength() << endl;

		}
		else{
			fl[idx-1].Add(seg);
			//cout <<"succesfull add seg 1 else  "<< seg->GetLength() << endl;
			fl[idx-2].Add(seg2);
			//cout <<"succesfull add seg 2 else "<< seg2->GetLength() << endl;
		}
		
	}

	
    // This empty implementation just uses C standard library malloc
    cout << "MyAllocator::Malloc called with length = " << _length << endl;
}

bool MyAllocator::Free(void* _a) {
	cout << "I am in free" << endl;
	 // map _a to seg node
	 //add seg to freelist
	 //cout <<"index at free function" << GetIndex() << endl
	 SegmentHeader* seg = (SegmentHeader*)((char*)_a - sizeof(SegmentHeader));
	 
	 //fl[GetIndex()].Add(seg);
	 
	 //1. check if seg is left or right buddy
		//case 1 -  seg is left buddy
		//case 2 - seg is right buddy
		//find the starting address of the other buddy
		
		// case 3 - seg is largest possible segment

	if(seg->GetBuddyType() == 0){
		fl[idx].Add(seg);
		
	}
	else if(seg->GetBuddyType() == 1){
		fl[idx].Add(seg);
	}
	else if(seg->GetIndex() == sizeofArray-1){
		fl[sizeofArray-1].Add(seg);
		return true;
	}

		
	 
	 //2. check if size of buddy is correct
	 
	 //3. check if buddy is free
	 
	 // 4. glue segments back together
	 //seg->length = fib(i+2);
	 

	
    // This empty implementation just uses C standard library free
    cout << "MyAllocator::Free called" << endl;
    //std::free(_a);
    return true;
}

