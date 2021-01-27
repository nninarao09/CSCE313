/*
    File: mutex_guard.hpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 07/14/16

*/

#ifndef _mutex_guard_H_                   // include file only once
#define _mutex_guard_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */
//#define MUTEX_GUARD_TEST


/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

//#include<pthread.h>
#include "mutex.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   M u t e x G u a r d */
/*--------------------------------------------------------------------------*/

class MutexGuard {

private:
  /* -- INTERNAL DATA STRUCTURES
     You may need to change them to fit your implementation. */

  Mutex * m;

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */
  
  MutexGuard(Mutex & m);

  ~MutexGuard();

};


#endif


