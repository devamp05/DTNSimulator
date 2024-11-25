/*
 * Devam Punitbhai Patel
 * dns682
 * 11316715
 *
 * Preet Jain
 * pvj636
 * 11270494
 */

#include <stdio.h>
#include <stdlib.h>
#include <list.h>

/*defining gloabal variables*/
extern int MAXNODES;

int MAXNODES = 10000;

extern int MAXLISTS;

int MAXLISTS = 1100;

extern int NODEINDEX;

int NODEINDEX = 0;

extern int LISTINDEX;

int LISTINDEX = 0;

/*LIST LISTPOOL[5];*/   /*trying to do the bonus part so commenting this out*/

extern LIST *listPool;

LIST *listPool;

extern NODE *nodePool;

NODE *nodePool;

extern LIST FREENODES;

LIST FREENODES;		/*A list to keep track of nodes that have been removed*/

LIST **FREELISTS;	/*create an array of list pointers to store empty lists
						after list concat*/

int FREELISTSINDEX;	/*to store index of where we are in free lists*/

LIST *ListCreate()
{
	/*To check if a pool of nodes has already been assigned*/

	if (nodePool == NULL)
	{
		nodePool = (NODE *) malloc(sizeof(NODE)*MAXNODES);
		
		/*check if memory is allocated*/
		if(nodePool == NULL)
		{
			printf("Nodepool not assigned");
			return NULL;
		}

	}

	/*check if pool of lists has already been assigned*/

	if (listPool == NULL)
	{
		listPool = (LIST *) malloc(sizeof(LIST)*MAXLISTS);
		
		/*if memory allocation fails return NULL*/
		if(listPool == NULL)
		{
			printf("Listpool not assigned");
			return NULL;
		}
		/*to store list pointers of the free lists that remain after List 
		 * concat*/
		FREELISTS = (LIST **) malloc(sizeof(LIST*)*MAXLISTS);
	}

	LISTINDEX++;
	if (LISTINDEX >= MAXLISTS)
	{
		/*check if we have any freed lists in FREELISTS*/
		if(FREELISTSINDEX > 0)
		{
			/*it means there are FREELISTS*/

			/*decrement the FREELISTSINDEX*/
			FREELISTSINDEX--;

			/*return a list pointer from the FREELISTS*/
			return FREELISTS[FREELISTSINDEX];
		}
		else
		{
			/*return null if there are no lists left in the pool or in 
			 * freelists because list realloc has not been implemented yet*/
			return NULL;
		}
	}
	/*not working*
	else
	{
		*this means we are at the end of our pool so double the pool*
		MAXLISTS *= 2;
		listPool = (LIST *) realloc(&listPool[0], sizeof(LIST)*MAXLISTS);
	}
	*/

	/*shrinking memory*/
	/*
	if (LISTINDEX < (MAXLISTS/2))
	{
		MAXLISTS /= 2;
		firstList = (LIST *) realloc(firstList, sizeof(LIST)*MAXLISTS);
	}*/

	return &listPool[(LISTINDEX-1)];
}

int ListCount(LIST *list)
{
	return list->count;	
}

void ListConcat(LIST *list1, LIST *list2)
{
	/*check if list1 is empty*/
	if(list1->count == 0)
	{
		/*I am testing this not sure if it will work
		but if it does I will have answers for implementing 
		my bonus part*/
		/*trying some fancy trick but not sure if it will work*/
		/**(LIST**)list1 = list2;*/
		/*it doesn't work*/
		list1->first = list2->first;
		list1->last = list2->last;
		list1->curr = list2->curr;
		list1->count = list2->count;

	}
	else
	{
		/*connect list1's last to list2's first*/
		list1->last->next = list2->first;

		list2->first->prev = list1->last;

		/*change list1's last to be list2's last*/
		list1->last = list2->last;


		list1->count += list2->count;
		list2->count = 0;
	}
		/*change everything in list2 to point at null*/
		list2->first = NULL;
		list2->last = NULL;
		list2->curr = NULL;

		list2->count = 0;

		/*store list 2 in FREELISTS*/
		FREELISTS[FREELISTSINDEX] = list2;

		FREELISTSINDEX++;


	/*store NULL in list2*/
/*	list2 = NULL;*/
}



void ListFree(LIST *list, freeRoutine itemFree)
{
	NODE *currNode;

	/*go to the first element*/
	list->curr = list->first;	/*not going to save previous first because
									the list is gonna be deleted anyways*/

	/*iterate untill end*/
	while(list->curr != NULL)
	{
		/*call itemFree on every item*/
		(*itemFree)(list->curr->data);

		/*get the previous node and delete it*/
		currNode = list->curr;

		/*set the previious to NULL*/
		list->curr->prev = NULL;

		list->curr = list->curr->next;

		 /*check if FREENODES list is empty*/
		 if(FREENODES.count == 0)
		 {
		 	currNode->next = NULL;
			currNode->prev = NULL;
			FREENODES.first = currNode;
			FREENODES.last = currNode;
		 }
		 /*else there are nodes in the freenodes list*/
		 else
		 {
			/*implement it such that we always add new node at the end*/
			/*like ListAppend()*/

			/*set the free nodes next to null because it will be the new 
			 * lastnode*/
			currNode->next = NULL;

			/*freenodes prev to point at the current 
			 * last in the freenodes list*/
			currNode->prev = FREENODES.last;

			/*change freenodes list's last's next to currentnode*/
			(FREENODES.last)->next = currNode;

			/*change last to point at currNode*/
			FREENODES.last = currNode;
		 }

		 /*increment FREENODES's counter*/
		 FREENODES.count++;

		 /*decrement the list counter*/
		 list->count--;
	}


	/*check the list counteer*/
	if(list->count != 0)
	{
		/*there is an error in freeing nodes somewhere*/
		printf("ListFree() error, not freeing internal nodes properly\n");
	}

	else
	{
		/*set the lists first and last to null*/
		list->first = NULL;

		list->last = NULL;


		/*now free the list*/

		FREELISTS[FREELISTSINDEX] = list;

		FREELISTSINDEX++;
	}

	
}

void *ListSearch(LIST *list, COMP comparator, void* comparisonArg)
{
	/*int flag to determine if a match was found or not*/
	int flag = 0;

	/*if list is empty return NULL*/
	if(list->count == 0)
	{
		return NULL;
	}

	/*iterate untill end*/
	while(list->curr != NULL)
	{
		if((*comparator)(list->curr->data, comparisonArg) == 1)
		{
			flag = 1;
			break;
		}
		list->curr = list->curr->next;
	}


	/*return item if found or return null*/
	if(flag == 1)
	{
		return list->curr->data;
	}

	/*else*/
	return NULL;
	
}
