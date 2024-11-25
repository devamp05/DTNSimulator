/*
 * Names: Preet Jain / Devam Punitbhai Patel
 * NSID: pvj636 / dns682
 * Student Numbers: 11270494 / 11316715
 */

#include<stdio.h>
#include<stdlib.h>
#include<list.h>

void *ListFirst(LIST *list)
{
	if(list->count == 0)
	{
		/*return NULL if the list is empty*/
		return NULL;
	}
	list->curr = list->first;
	return list->curr->data;
}


void *ListLast(LIST *list)
{
	if(list->count == 0)
	{
		/*return NULL if the list is empty*/
		return NULL;
	}
	
	list->curr = list->last;
	return list->curr->data;
}


void *ListNext(LIST *list)
{
	if(list->count == 0)
	{
		/*return NULL if the list is empty*/
		return NULL;
	}
	

	/*this takes care of case when 
	there is only one item in the list as well*/
	if(list->curr == list->last)
	{
		list->curr = NULL;
		return NULL;
	}

	if(list->curr == NULL)
	{
		return NULL;
	}

	list->curr = list->curr->next;
	return list->curr->data;
}


void *ListPrev(LIST *list)
{
	if(list->count == 0)
	{
		/*return NULL if the list is empty*/
		return NULL;
	}
	
	
	if(list->curr == list->first)
	{
		return NULL;
	}

	list->curr = list->curr->prev;
	return list->curr->data;
}


void *ListCurr(LIST *list)
{
	if(list->curr == NULL)
	{
		/*return NULL if the list is empty*/
		return NULL;
	}

	return list->curr->data;
}
