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
  // You may need to initialize more members here!
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

// create the split function
SegmentHeader* SegmentHeader::Split(size_t _len){
	SegmentHeader* seg_new = new((void*)((char*)this+_len)) SegmentHeader(this->length-_len);
	this->length = _len;
	return seg_new;
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
}

FreeList::~FreeList() {
  // You may need to add code here.
}

bool FreeList::Add(SegmentHeader * _segment) {
  //assert(false); // This implementation does nothing, other than abort.

  
  if(_segment == nullptr){
	  return false;
  }
  
  _segment->next = nullptr;
  _segment->prev = nullptr;
  
  if(head != nullptr){
	 head->prev= _segment;
	 _segment->next = head;
	 head = _segment;
  }
  else{
	  head = _segment;
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
	head = _segment->next;
  }
  if(_segment->next != NULL){
	  _segment->next->prev = _segment->prev;
  }
  if(_segment->prev != NULL){
	  _segment->prev->next = _segment->next;
  }
  
  _segment->is_free = false;
  return true;
}
