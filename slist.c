#include <stdio.h>
#include <stdint.h>
#include "slist.h"

/*
 *
 */
void SLIST_Init(XMC_LIST_t *list)
{
  *list = NULL;
}

/*
 *
 */
void SLIST_Add(XMC_LIST_t *list, SList *const item)
{
  SList *tail;
  
  ((SList *)item)->next = NULL;
  tail = (SList *)SLIST_GetTail(list);
  
  if (tail == NULL) 
  {
    *list = item;
  } 
  else 
  {
    tail->next = (SList *)item;
  }
}

/*
 *
 */
void SLIST_Remove(XMC_LIST_t *list, SList *const item)
{
  SList *right, *left;
  
  if (*list != NULL) 
  {
    left = NULL;
    for(right = (SList *)*list; right != NULL; right = right->next)
    {
      if(right == item) 
      {
        if(left == NULL) 
        {
          /* First on list */
          *list = right->next;
        } 
        else 
        {
          /* Not first on list */
          left->next = right->next;
        }
        right->next = NULL;
        break;
      }
      left = right;
    }
  }
}

/*
 *
 */
uint32_t SLIST_GetLength(XMC_LIST_t *list)
{
  uint32_t len = 0;
  
  SList *tail;
  
  if (*list == NULL) 
  {
    ;
  }
  else
  {
		len = 1;
    for (tail = (SList *)*list; tail->next != NULL; tail = tail->next, ++len)
    {
      /* Loop through the list */
    }  
  }
  
  return len;  
}

/*
 *
 */
void *SLIST_GetHead(XMC_LIST_t *list)
{
  return *list;
}

/*
 *
 */
void *SLIST_GetTail(XMC_LIST_t *list)
{
  SList *tail;
  
  if (*list == NULL) 
  {
    tail = NULL;
  }
  else
  {
    for (tail = (SList *)*list; tail->next != NULL; tail = tail->next)
    {
      /* Loop through the list */
    }  
  }
  
  return tail;    
}

/*
 *
 */
void SLIST_Insert(XMC_LIST_t *list, void *prev_item, SList *new_item)
{
  if (prev_item == NULL) 
  {
	((SList *)new_item)->next = (SList *)*list;
	*list = new_item;
  } 
  else 
  {
    ((SList *)new_item)->next = ((SList *)prev_item)->next;
    ((SList *)prev_item)->next = (SList *)new_item;
  }
}


