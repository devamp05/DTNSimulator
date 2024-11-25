/*
 * Devam Punitbhai Patel
 * dns682
 * 11316715
 *
 * Preet Jain
 * pvj636
 * 11270494
 */

#ifndef __LIST_H__
#define __LIST_H__         /* To avoid multiple copies*/

/* A Structure for NODE */
typedef struct node
{
	void *data;
	struct node *prev;
	struct node *next;
	
}NODE;


/* A structure for LIST */
typedef struct list
{
	NODE *first;
	NODE *last;
	NODE *curr;    /* Cursor*/
	int count;
} LIST;

LIST *ListCreate();

int ListCount(LIST *list);

void *ListFirst(LIST *list);

void *ListLast(LIST *list);

void *ListNext(LIST *list);

void *ListPrev(LIST *list);

void *ListCurr(LIST *list);

int ListAdd(LIST *list, void *item);

int ListInsert(LIST *list, void *item);

int ListAppend(LIST *list, void *item);

int ListPrepend(LIST *list, void *item);

void *ListRemove(LIST *list);

void ListConcat(LIST *list1, LIST *list2);

typedef void (*freeRoutine)(void *itemToBeFreed);

void ListFree(LIST *list, freeRoutine itemFree);    /*itemFree is a routine*/

void *ListTrim(LIST *list);

typedef int (*COMP)(void *comp1, void *comp2);

void *ListSearch(LIST *list, COMP comparator, void* comparisonArg);

#endif
