//Name: Yash Moondhra
//Email: ymoondhra@gmail.com
//ID: 123456789

#include "SortedList.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
/**
 * SortedList (and SortedListElement)
 *
 *	A doubly linked list, kept sorted by a specified key.
 *	This structure is used for a list head, and each element
 *	of the list begins with this structure.
 *
 *	The list head is in the list, and an empty list contains
 *	only a list head.  The next pointer in the head points at
 *      the first (lowest valued) elment in the list.  The prev
 *      pointer in the list head points at the last (highest valued)
 *      element in the list.  Thus, the list is circular.
 *
 *      The list head is also recognizable by its NULL key pointer.
 *
 */

/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *	The specified element will be inserted in to
 *	the specified list, which will be kept sorted
 *	in ascending order based on associated keys
 *
 * @param SortedList_t *list ... header for the list
 * @param SortedListElement_t *element ... element to be added to the list
 */
void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
  if(list == NULL) {
    list = element;
    return;
  }

  if(opt_yield & INSERT_YIELD) sched_yield();

  SortedListElement_t* ptr = list;
  SortedListElement_t* nextt = ptr->next;
  while(nextt != NULL && nextt != list) {
    if(nextt->key != NULL && strcmp(element->key, nextt->key) < 0) {
      element->next = nextt;
      element->prev = ptr;
      nextt->prev = element;
      ptr->next = element;
      return;
    }
    else {
      ptr = nextt;
      nextt = nextt->next;
    }
  }

  ptr->next = element;
  element->prev = ptr;
  element->next = list;
  list->prev = element;
  return;
}
/**
DONE
 * SortedList_delete ... remove an element from a sorted list
 *
 *	The specified element will be removed from whatever
 *	list it is currently in.
 *
 *	Before doing the deletion, we check to make sure that
 *	next->prev and prev->next both point to this node
 *
 * @param SortedListElement_t *element ... element to be removed
 *
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int SortedList_delete( SortedListElement_t *element) {
  if(element == NULL) {
    return 1;
  }
  SortedList_t* previous = element->prev;
  SortedList_t* next = element->next;
  
  if(previous->next != element) {
    return 1;
  }
  
  if(next->prev != element)
    return 1;

  if(opt_yield & DELETE_YIELD) sched_yield();
  previous->next = element->next;
  next->prev = element->prev;
  
  return 0;
}

/** 
 * SortedList_lookup ... search sorted list for a key
 *
 *	The specified list will be searched for an
 *	element with the specified key.
 *
 * @param SortedList_t *list ... header for the list
 * @param const char * key ... the desired key
 *
 * @return pointer to matching element, or NULL if none is found
 */
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  if(list == NULL)
    return NULL;
  //search forward
  SortedListElement_t* ptr = list->next;
  while(ptr != list) {
    if(strcmp(ptr->key, key) == 0)
      return ptr;
    if(opt_yield & LOOKUP_YIELD) sched_yield();
    ptr = ptr->next;
  }
  //could not find key
  return NULL;
  }

/**
 * SortedList_length ... count elements in a sorted list
 *	While enumeratign list, it checks all prev/next pointers
 *
 * @param SortedList_t *list ... header for the list
 *
 * @return int number of elements in list (excluding head)
 *	   -1 if the list is corrupted
 */

int SortedList_length(SortedList_t *list) {
  if(list == NULL)
    return -1;

  SortedList_t* ptr = list->next;
  int count = 0;
  if(list == ptr) {
    return 0;
  }
  while(ptr != list) {
    if(ptr->prev->next != ptr)
      return -1;
    if(ptr->next->prev != ptr)
      return -1;
    if(opt_yield & LOOKUP_YIELD) sched_yield();
    ptr = ptr->next;
    count++;
  }
  return count;
}
