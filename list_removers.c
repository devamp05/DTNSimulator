/*
 * Names: Preet Jain / Devam Punitbhai Patel
 * NSID: pvj636 / dns682
 * Student Numbers: 11270494 / 11316715
 */

#include<stdio.h>
#include<stdlib.h>
#include<list.h>

extern LIST FREENODES;

void *ListRemove(LIST *list)
{
	/*to maintain standards*/
	NODE *currNode;

	void *currItem;
	
	/*first check that there is some item at curr*/
	if(list->curr == NULL)
	{
		printf("current item is NULL!\n");
		return NULL;
	}
	/*else*/

	/*store the current node*/
	currNode = list->curr;

	/*check if the current node is the first node in the list*/
	if(list->curr == list->first)
	{
		/*now check if it is the only item in the list*/
		if(list->last == list->curr)
		{
			/*remove the node and set first, last and curr
			to NULL in this case*/
			list->first = NULL;
			list->last = NULL;
			list->curr = NULL;
		}
		else
		{
			/*else there is another node in the list*/
			/*in this case, remove the current node and make the next node i
			 * curr
			and change the first node to point at next node*/

			/*first set curr's next to be curr*/
			list->curr = list->curr->next;

			/*first to point at new curr*/
			list->first = list->curr;

			/*new curr's prev to null*/
			list->curr->prev = NULL;

			/*we will change the old current's next in FREENODES list anyways
			so not changing it here*/
		}
	}

	/*else check if the current node is the last node*/
	else if(list->last == list->curr)
	{
		/*remove the node and set curr to NULL*/

		/*set last to last's prev*/
		list->last = list->last->prev;

		/*set last's next to null*/
		list->last->next = NULL;

		/*set curr to new last*/
		list->curr = list->last;

	}
	else
	{

		/*else curr is some middle node*/
	/*no problem in this case, simply remove curr and adjust it's neighbours*/
	
		/*set the current's prev's next to curr's next*/
		list->curr->prev->next =  list->curr->next;

		/*set the curr's next's prev to curr'r prev*/
		list->curr->next->prev = list->curr->prev;

		/*change curr to point at curr's next*/
		list->curr = list->curr->next;
	}

	/*set the node at it's appropriate position in the freenodes list
	 * similarly to listappend but in the freelist and return the item*/

	 /*we have the removed node stored in currNode here*/

	 /*first get it's item*/
	 currItem = currNode->data;

	/*set it's data to null*/
	 currNode->data = NULL;

	 /*set it in the freenodes list*/

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

		/*set the free nodes next to null because it will be the new lastnode*/
		currNode->next = NULL;

		/*freenodes prev to point at the current last in the freenodes list*/
		currNode->prev = FREENODES.last;

		/*change freenodes list's last's next to currentnode*/
		(FREENODES.last)->next = currNode;

		/*change last to point at currNode*/
		FREENODES.last = currNode;
	 }

	 /*increment FREENODES's counter*/
	 FREENODES.count++;

	 /*decrement list's counter*/
	 list->count--;

	 /*return item*/
	 return currItem;

}


void *ListTrim(LIST *list)
{
	/*to maintain standards*/
	NODE *currNode;

	void *currItem;

	/*check if the list is empty and return null if it is*/
	if(list->count == 0)
	{
		printf("Error!, ListTrim() called on empty list\n");
		return NULL;
	}

	/*first store the last node*/
	currNode = list->last;

	/*check if the last item is the only item in the list
	in this case will have to change first item as well*/
	if(list->count == 1)
	{
		list->last = NULL;
		list->first = NULL;
		list->curr = NULL;
	}

	/*remove the last item like we did in ListRemove but only for
	last item this time*/
	else
	{
		/*set last's prev's next to null*/
		list->last->prev->next = NULL;

		/*set last to last's prev*/
		list->last = list->last->prev;

		/*change curr to last*/
		list->curr = list->last;
	}

	/*if not simply remove it*/

	/*now we will have the last node stored in some node which we will have
	to add in the freenodes list 
	give it the same name so that we can copy paste from ListRemove()
	the code to add new node to the freenodes list*/



	 /*we have the removed node stored in currNode here*/

	 /*first get it's item*/
	 currItem = currNode->data;

	/*set it's data to null*/
	 currNode->data = NULL;

	 /*set it in the freenodes list*/

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

		/*set the free nodes next to null because it will be the new lastnode*/
		currNode->next = NULL;

		/*freenodes prev to point at the current last in the freenodes list*/
		currNode->prev = FREENODES.last;

		/*change freenodes list's last's next to currentnode*/
		(FREENODES.last)->next = currNode;

		/*change last to point at currNode*/
		FREENODES.last = currNode;
	 }

	 /*increment FREENODES's counter*/
	 FREENODES.count++;

	 /*decrement list's counter*/
	 list->count--;

	 /*return item*/
	 return currItem;

}
