#include <stdio.h>
#include <stdint.h>

#include <xmc_common.h>

#include "slist.h"

/*
 *
 */
uint32_t XMC_LIST_GetLength(XMC_LIST_t *list) {
  uint32_t len = 0;
 
  SList *tail;
 
  if (*list == NULL) {
    ;
  } else {    
		len = 1;
    for (tail = (SList *)*list; tail->next != NULL; tail = tail->next, ++len) {
      /* Loop through the list */
    }  
  }
 
  return len;  
}
