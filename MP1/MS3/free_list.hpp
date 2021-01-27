/* 
    File: free_list.hpp

    Author: <your name>
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:

*/

#ifndef _free_list_hpp_                   // include file only once
#define _free_list_hpp_


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
//#include "my_allocator.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/
class FreeList;

class SegmentHeader {

friend FreeList;

private:

  static const unsigned COOKIE_VALUE = 0xBADB00;
  unsigned int cookie; /* To check whether this is a genuine header! */
  size_t length;
  bool is_free;
  SegmentHeader* next;
  SegmentHeader* prev;
  int buddy_type;
  int index;
  
public:

  SegmentHeader(size_t _length, bool _is_free = true);
  
  ~SegmentHeader();
  /* We probably won't need the destructor. */
  
  size_t GetLength();
  
  SegmentHeader* GetNext();
  
  bool GetIsFree();
  
  int GetBuddyType();
  
  int GetIndex();
  
  //void SetIndex(int idx);

  SegmentHeader* Split(int sizetoA);
  
  void CheckValid();
  /* Check if the cookie is valid. */
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/

class FreeList {

 private:

  /* Here you add whatever private members you need...*/
  

public:

  SegmentHeader* head;
  
  int idx;
  
  int sizetoA; // fib * basic block size 
  
  
  FreeList(); 
  /* This function initializes a new free-list. */

  ~FreeList(); 
  /* We probably don't need a destructor. */ 
  
  //bool Empty();
  
  bool Remove(SegmentHeader * _segment); 
  /* Remove the given segment from the given free list. 
     Returns true if the function succeeds.
  */ 

  bool Add(SegmentHeader * _segment); 
  /* Add the segment to the given free list. */
  
};

#endif 
