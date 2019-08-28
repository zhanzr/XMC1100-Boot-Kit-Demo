#ifndef __SLIST_H__
#define	__SLIST_H__

#include <stdint.h>

/*******************************************************************************
 * DATA STRUCTURES, this structure must be exposed to user code
 *******************************************************************************/
typedef struct strSList {
	//next member must be the first member
  struct strSList* next;
  void* pVal;
}SList;


#endif
