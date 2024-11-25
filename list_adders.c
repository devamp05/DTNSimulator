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

/*declaring global variables*/
extern int MAXNODES;

extern int NODEINDEX;

extern NODE *nodePool;

extern LIST FREENODES;

int ListAdd(LIST *list, void *item)
{
	if(NODEINDEX < (MAXNODES-1))
	{
		/*assign item to the new Node and increment the counter*/
		nodePool[NODEINDEX].data = item;

		if(list->first == NULL)
		{
			/*new node is the first node of the list*/
			nodePool[NODEINDEX].next = NULL;
			nodePool[NODEINDEX].prev = NULL;
			list->first = &nodePool[NODEINDEX];
			list->last = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}
		else
		{
				/*first set the next of new node to next of curr*/
				nodePool[NODEINDEX].next = list->curr->next;

				/*set the prev*/
				nodePool[NODEINDEX].prev = list->curr;

				if(list->curr->next != NULL)
				{
					/*set the prev of next node to point at new node*/
					list->curr->next->prev = &nodePool[NODEINDEX];	
				}
				/*set the next node of the current node to be new node*/
				list->curr->next = &nodePool[NODEINDEX];
				

				/*check if the last node is on the previous current node*/
				if(list->curr == list->last)
				{
					list->last = &nodePool[NODEINDEX];
				}

				/*change the cursor*/
				list->curr = &nodePool[NODEINDEX];

			
		}
		NODEINDEX++;
		list->count++; /*increment the counter*/
		return 0;
	}

	/*check if there are any free nodes in our free list*/
	else if(FREENODES.first != NULL)
	{
		/*assign a pointer temporarily*/
		NODE *tempHolder;

		tempHolder = FREENODES.first;

		/*move the first of our freelist to next free node or NULL*/
		FREENODES.first = FREENODES.first->next;

		tempHolder->data = item;	


		if(list->first == NULL)
		{
			/*new node is the first node of the list*/
			tempHolder->next = NULL;
			tempHolder->prev = NULL;
			list->first = tempHolder;
			list->last = tempHolder;
			list->curr = tempHolder;
		}
		else
		{
	
				/*first set the next of new node to next of curr*/
				tempHolder->next = list->curr->next;


				/*set the prev*/
				tempHolder->prev = list->curr;


				if(list->curr->next != NULL)
				{
					/*set the prev of next node to point at new node*/
					list->curr->next->prev = tempHolder;	
				}


				/*set the next node of the current node to be new node*/
				list->curr->next = tempHolder;
				
				
				/*check if the last node is on the current node*/
				if(list->curr == list->last)
				{
					list->last = tempHolder;
				}

				/*change the cursor*/
				list->curr = tempHolder;



			
		}
		list->count++; /*increment the counter*/
		return 0;
	}

	else
	{
		return -1;
	}
	

}


int ListInsert(LIST *list, void *item)
{
	/*for this procedure I will try to copy paste my code from 
	 *my ListAdd() procedure and change the logic to behave
	 as ListInsert() */


	if(NODEINDEX < (MAXNODES-1))
	{
		/*assign item to the new Node and increment the counter*/
		nodePool[NODEINDEX].data = item;

		if(list->first == NULL)
		{
			/*new node is the first node of the list*/
			nodePool[NODEINDEX].next = NULL;
			nodePool[NODEINDEX].prev = NULL;
			list->first = &nodePool[NODEINDEX];
			list->last = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}
		else
		{

				/*set the prev*/
				nodePool[NODEINDEX].prev = list->curr->prev;	

				/*first set the next of new node to be curr*/
				nodePool[NODEINDEX].next = list->curr;

				if(list->curr->prev != NULL)
				{

					/*set the next of prev to new node*/
					list->curr->prev->next = &nodePool[NODEINDEX];
				}

				/*set the prev node of the current node to be new node*/
				list->curr->prev = &nodePool[NODEINDEX];
				

				

				/*check if the first node is on the previous current node*/
				if(list->curr == list->first)
				{
					list->first = &nodePool[NODEINDEX];
				}
				/*change the cursor*/
				list->curr = &nodePool[NODEINDEX];
			
		}
		NODEINDEX++;
		list->count++; /*increment the counter*/
		return 0;
	}


	/*check if there are any free nodes in our free list*/
	else if(FREENODES.first != NULL)
	{
		/*assign a pointer temporarily*/
		NODE *tempHolder;

		tempHolder = FREENODES.first;

		/*move the first of our freelist to next free node or NULL*/
		FREENODES.first = FREENODES.first->next;

		tempHolder->data = item;	


		if(list->first == NULL)
		{
			/*new node is the first node of the list*/
			tempHolder->next = NULL;
			tempHolder->prev = NULL;
			list->first = tempHolder;
			list->last = tempHolder;
			list->curr = tempHolder;
		}
		else
		{
			
				/*set the prev*/
				tempHolder->prev = list->curr->prev;	

				/*first set the next of new node to curr*/
				tempHolder->next = list->curr;
				if(list->curr->prev != NULL)
				{
					/*set the next of prev to new node*/
					list->curr->prev->next = tempHolder;
				}	

				/*set the next node of the current node to be new node*/
				list->curr->prev = tempHolder;
				

				

				/*check if the first node is on the previous current node*/
				if(list->curr == list->first)
				{
					list->first = tempHolder;
				}

				/*change the cursor*/
				list->curr = tempHolder;



			
		}
		list->count++; /*increment the counter*/
		return 0;
	}



	else
	{	
		return -1;
	}
}


int ListAppend(LIST *list, void *item)
{
	if(NODEINDEX < (MAXNODES-1))
	{
		/*if there are any nodes left in the pool*/

		nodePool[NODEINDEX].data = item;

		if(list->last == NULL)
		{
			/*if it's an empty list*/
			nodePool[NODEINDEX].next = NULL;
			nodePool[NODEINDEX].prev = NULL;
			list->first = &nodePool[NODEINDEX];
			list->last = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}
		else
		{
			nodePool[NODEINDEX].prev = list->last;
			nodePool[NODEINDEX].next = NULL;
			list->last->next = &nodePool[NODEINDEX];

			list->last = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}

		/*increment the node index*/
		NODEINDEX++;

		list->count++;
		return 0;
	}


	/*check if there are any free nodes in our free list*/
	else if(FREENODES.first != NULL)
	{
		/*assign a pointer temporarily*/
		NODE *tempHolder;

		tempHolder = FREENODES.first;

		/*move the first of our freelist to next free node or NULL*/
		FREENODES.first = FREENODES.first->next;

		tempHolder->data = item;


		if(list->last == NULL)
		{
			/*if it's an empty list*/
			tempHolder->next = NULL;
			tempHolder->prev = NULL;
			list->first = tempHolder;
			list->last = tempHolder;
			list->curr = tempHolder;
		}
		else
		{
			tempHolder->prev = list->last;
			tempHolder->next = NULL;
			list->last->next = tempHolder;

			list->last = tempHolder;
			list->curr = tempHolder;
		}

		list->count++;
		return 0;
	}

	else
	{
		return -1;
	}

}

int ListPrepend(LIST *list, void *item)
{
	if(NODEINDEX < (MAXNODES-1))
	{
		/*if there are any nodes left in the pool*/

		nodePool[NODEINDEX].data = item;

		if(list->first == NULL)
		{
			/*if it's an empty list*/
			nodePool[NODEINDEX].next = NULL;
			nodePool[NODEINDEX].prev = NULL;
			list->first = &nodePool[NODEINDEX];
			list->last = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}
		else
		{
			nodePool[NODEINDEX].prev = NULL;
			nodePool[NODEINDEX].next = list->first;
			list->first->prev = &nodePool[NODEINDEX];

			list->first = &nodePool[NODEINDEX];
			list->curr = &nodePool[NODEINDEX];
		}

		/*increment the node index*/
		NODEINDEX++;

		list->count++;
		return 0;
	}


	/*check if there are any free nodes in our free list*/
	else if(FREENODES.first != NULL)
	{
		/*assign a pointer temporarily*/
		NODE *tempHolder;

		tempHolder = FREENODES.first;

		/*move the first of our freelist to next free node or NULL*/
		FREENODES.first = FREENODES.first->next;

		tempHolder->data = item;


		if(list->last == NULL)
		{
			/*if it's an empty list*/
			tempHolder->next = NULL;
			tempHolder->prev = NULL;
			list->first = tempHolder;
			list->last = tempHolder;
			list->curr = tempHolder;
		}
		else
		{
			tempHolder->prev = NULL;
			tempHolder->next = list->first;
			list->first->prev = tempHolder;

			list->first = tempHolder;
			list->curr = tempHolder;
		}

		list->count++;
		return 0;
	}

	else
	{
		return -1;
	}

}

