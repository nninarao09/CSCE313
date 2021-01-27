/* 
    File: free_list.cpp

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class FreeList.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include "free_list.hpp"

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
/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/

SegmentHeader::SegmentHeader(size_t _length, bool _is_free) {
  length = _length;
  is_free = _is_free;
  cookie = COOKIE_VALUE;
  next = nullptr;
  prev = nullptr;
  buddy_type = 0;
  index = 0;
}


SegmentHeader::~SegmentHeader() {
  // You may need to add code here.
}

size_t SegmentHeader::GetLength(){
	return length;
}

SegmentHeader* SegmentHeader::GetNext(){
	return next;
}

bool SegmentHeader::GetIsFree(){
	return is_free;
}

int SegmentHeader::GetBuddyType(){
	return buddy_type;
}

int SegmentHeader::GetIndex(){
	return index;
}

/*
void SegmentHeader::SetIndex(int idx){
	index = idx;
}

*/

SegmentHeader* SegmentHeader::Split(int sizetoA){   // said split does not need an arguement
	
	//SegmentHeader* new_seg = new((void*)((char*)this+_len)) SegmentHeader(this->length-_len);
	//cout << "this length before" << this->length << endl;
	SegmentHeader* new_seg = new(((char*)this+sizetoA))SegmentHeader(this->length-sizetoA, false);
	this->length = sizetoA;


	
	
	// define the buddy type can be an integer 0 is left 1 is right
	
	if(this->length > new_seg->length){
		buddy_type = 0; 
	}
	else{
		buddy_type = 1;
	}
	

	return new_seg;
	
}


void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    //assert(false);
    // You will need to check with the debugger to see how we got into this
    // predicament.
  }
}

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/

FreeList::FreeList() {
	head = nullptr;
	
	idx = 0;
	sizetoA = 0;
}

FreeList::~FreeList() {
  // You may need to add code here.
}


bool FreeList::Add(SegmentHeader * _segment) {
  //assert(false); // This implementation does nothing, other than abort.

  
  if(_segment == nullptr){
     //cout << "here 1" << endl;

	  return false;
  }
  
  _segment->next = nullptr;
  _segment->prev = nullptr;
  
  if(head != nullptr){
	 //cout << "here 2" << endl;
	 //cout << "seg before"  << head << endl;
	//cout << "head before"  << head << endl;
	//cout << "here 1" << endl;

	 head->prev= _segment;
	//cout << "head-prev after"  <<  head->prev->length << endl;

	 _segment->next = head;
	 	 
	//cout << "add: segment->next" <<  _segment->next << endl;

	 
	 head = _segment;
	 
	//cout << "add: head" <<  head << endl;
	 
	 head->prev = nullptr;
	 	//cout << "head "  <<  head << endl;
		//cout << "head next  "  <<  head->next << endl;

  }
  else{
	  //cout << "add: head1" <<  head << endl;
//cout << "here 3" << endl;
	  
	  head = _segment;
	  head->prev = nullptr;
	  
	  //cout << "add: head2" <<  head << endl;

	  //cout << "head address " << head << endl;

	  //cout << "head length " << head->length << endl;
  }
  _segment->is_free = true;
  return true;
}

bool FreeList::Remove(SegmentHeader * _segment) {
  //assert(false); // This implementation does nothing, other than abort.
  
  if(head == nullptr && _segment == nullptr){
	  return false;
  }
  if(head == _segment){
	//cout << "First: " << head->length << endl;
	//cout << "Segment length" << _segment->length << endl;
	if(_segment->next == nullptr){
		head == nullptr;
		return false;
	}
	head = _segment->next;

	//cout << "Second: " << head->length << endl;
  }
  if(_segment->next != NULL){
	  _segment->next->prev = _segment->prev;
	     //cout << "here 1" << endl;

  }
  if(_segment->prev != NULL){
	  _segment->prev->next = _segment->next;
//	     cout << "here 3" << endl;

  }
   //cout << "here 4" << endl;
  
  _segment->is_free = false;
  return true;
}
