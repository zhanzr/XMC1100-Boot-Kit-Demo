#ifndef __SLIST_H__
#define	__SLIST_H__

#include <stdint.h>

/*******************************************************************************
 * DATA STRUCTURES, this structure must be exposed to user code
 *******************************************************************************/
typedef struct strSList
{
  void* pVal;
  struct strSList *next;
}SList;

typedef void *XMC_LIST_t;

/*
 *
 */
void SLIST_Init(XMC_LIST_t *list);

/*
 *
 */
void SLIST_Add(XMC_LIST_t *list, SList *const item);

/*
 *
 */
void SLIST_Remove(XMC_LIST_t *list, SList *const item);

/*
 *
 */
uint32_t SLIST_GetLength(XMC_LIST_t *list);

/*
 *
 */
void *SLIST_GetHead(XMC_LIST_t *list);

/*
 *
 */
void *SLIST_GetTail(XMC_LIST_t *list);

/*
 *
 */
void SLIST_Insert(XMC_LIST_t *list, void *prev_item, SList *new_item);


#endif
