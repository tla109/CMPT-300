#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h> //for assertions


//array of lists and nodes 
static struct List_s head_pool[LIST_MAX_NUM_HEADS]; //array of list structs
static struct Node_s node_pool[LIST_MAX_NUM_NODES]; //array of node structs
static int nextHeadIndex = 0; //tracks which head available in the pool
static int nextNodeIndex = 0; //tracks which node available in pool


// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create() {
	if (nextHeadIndex < LIST_MAX_NUM_HEADS) {
		List* newList = &head_pool[nextHeadIndex]; 
		//printf("nextheadindex: %d and max head %d\n", nextHeadIndex, LIST_MAX_NUM_HEADS);
		nextHeadIndex++; //increment index 
		newList->curr = NULL; //initialize current pointer 
		newList->tail = NULL;
		newList->elementCount = 0;
		return newList; //return reference
	} else {
		//printf("no more available heads");
		return NULL; //no more heads available 
	}
}

// Returns the number of items in pList.
int List_count(List* pList) {
	assert(pList != NULL);
	return pList->elementCount;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
	assert(pList != NULL);
	void* firstItem = NULL;
	if (pList->elementCount == 0) { //empty list case 
		pList->curr = NULL;
	} else {
		Node* pCurrent = pList->head; 
		firstItem = pCurrent->item;
		pList->curr = pCurrent;
	}
	return firstItem;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList) {
	assert(pList != NULL);
	void* lastItem = NULL;
	if (pList->elementCount == 0) { //empty list case 
		pList->curr = NULL;
	} else {
		Node* pCurrent = pList->tail;
		lastItem = pCurrent->item;
		pList->curr = pCurrent;
	}
	return lastItem;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList) {
	assert(pList != NULL);
	void* nextItem = NULL;
	if (pList->curr->next != NULL) { //not null
		pList->curr = pList->curr->next; //move curr pointer 
		nextItem = 	pList->curr->item; //pointer to new current item
	} else { //beyond end of pList
		//set current item 
		pList->curr = NULL;
		pList->bounds = LIST_OOB_END;
	}
	return nextItem;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {
	assert(pList != NULL);
	void* prevItem = NULL;
	
	if (pList->curr->prev != NULL) { //not null
		pList->curr = pList->curr->prev; //move curr pointer 
		prevItem = 	pList->curr->item; //pointer to new current item
	} else { //beyond start of pList
		//set current item 
		pList->curr = NULL;
		pList->bounds = LIST_OOB_START;
	}
	return prevItem;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
	assert(pList != NULL);
	return pList->curr->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem, size_t size) {
	assert(pList != NULL);
		
	//i have to check node count to see if there is any left 
	if (nextNodeIndex >= LIST_MAX_NUM_NODES) {
		//printf("sorry no more nodes!");
		return LIST_FAIL;
	}
	
	//need to create a node for the pitem
	Node* anItem = &node_pool[nextNodeIndex]; 
	nextNodeIndex++; //update index
	anItem->item = pItem;
    anItem->size = size;
	
	//if the only element 
	if (pList->elementCount == 0) {
		pList->head = anItem;
		pList->tail = anItem;
		anItem->next = NULL;
		anItem->prev = NULL;
	} else if ((pList->curr == NULL) && (pList->bounds == LIST_OOB_START)) { //add it to after current item 
		//printf("START OOB\n");
		anItem->next = pList->head; //attach to head
		anItem->prev = NULL; //update prev
		pList->head = anItem; //update head
	
	} else if ((pList->curr == NULL) && (pList->bounds == LIST_OOB_END)) {// { //or its last item
		//printf("end OOB IN THE INSERT AFTER FUNCTION\n");
		//add it to end 
		pList->tail-> next = anItem; //add it to tail
		anItem->prev = pList->tail; //update prev 
		pList->tail = anItem; //update tail 
	
	} else if (pList->curr->next != NULL) {
		//printf("normal case?\n");
		//normal case 
		Node* tmp = pList->curr->next;
		anItem->next = pList->curr->next; //save the next 
		pList->curr->next = anItem; //add new item 
		anItem->prev = pList->curr; //update prev for the new item 
		tmp->prev = anItem;// update the prev the followed the new item ***

	}
	pList->curr = anItem; // update curr
	pList->elementCount++; //update count	 
	return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem, size_t size) {
	//i have to check node count to see if there is any left 
	if (nextNodeIndex >= LIST_MAX_NUM_NODES) {
		//printf("sorry no more nodes!\n");
		return LIST_FAIL;
	}
	//need to create a node for the pitem
	Node* anItem = &node_pool[nextNodeIndex]; 
	nextNodeIndex++; //update index
	anItem->item = pItem;
    anItem->size = size;

	//add it before current item 
	if ((pList->bounds == LIST_OOB_START) && (pList->curr == NULL)) {
		//printf("START OOB???????????????????????????????\n");
		pList->head = anItem; //update head 
		pList->tail = anItem; //update tail
		pList->head->next = NULL;
		pList->tail->next = NULL;
	} else if (pList->curr->prev == NULL) { // first item 
		//printf("first item\n");
		anItem->next = pList->head; //attach to head
		anItem->prev = NULL; //update prev
		pList->head = anItem; //update head
	
	} else if ((pList->bounds == LIST_OOB_END) && (pList->curr == NULL)) { //or its last item
		//printf("end OOB>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		//add it to end 
		pList->tail-> next = anItem; //add it to tail
		anItem->prev = pList->tail; //update prev 
		pList->tail = anItem; //update tail 
	
	} else if (pList->curr != NULL) {
		//printf("normal case?***************************\n");
		//normal case 
		Node* tmp = pList->curr->prev; //save the prev node
		anItem->next = pList->curr; //attach to curr node
		pList->curr->prev = anItem; //update prev link
		tmp->next = anItem; //update tmps next 
		anItem->prev = tmp; //update new items prev
	}

	pList->curr = anItem; // update curr
	pList->elementCount++; //update count	 
	return LIST_SUCCESS;
		
}
// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem, size_t size) {
	assert(pList != NULL);
	
	//i have to check node count to see if there is any left 
	if (nextNodeIndex >= LIST_MAX_NUM_NODES) {
		//printf("sorry no more nodes!");
		return LIST_FAIL;
	}
	
	//need to create a node for the pitem
	Node* anItem = &node_pool[nextNodeIndex]; 
	nextNodeIndex++; //update index
	anItem->item = pItem;
    anItem->size = size;

	//add it to end of LL
	if (pList->elementCount == 0) {
		pList->head = anItem; //update head
		//printf("only item\n");
	} else {
		//regular case
		//printf("%d is at the tail", pList->tail->item);
		pList->tail->next = anItem; //attach to tail
		//printf("regular case\n");
	}

	anItem->prev = pList->tail; //update prev
	anItem->next = NULL; //update next
	pList->tail = anItem; //need to update tail 
	pList->curr = anItem; //update curr
	pList->elementCount++; //update count			
		 
	return LIST_SUCCESS;		
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem, size_t size) {
	assert(pList != NULL);
	
	//i have to check node count to see if there is any left 
	if (nextNodeIndex >= LIST_MAX_NUM_NODES) {
		//printf("sorry no more nodes!");
		return LIST_FAIL;
	}
	//need to create a node for the pitem
	Node* anItem = &node_pool[nextNodeIndex]; 
	nextNodeIndex++; //update index
	anItem->item = pItem;
    anItem->size = size;

	//add it to front of LL
	if (pList->elementCount == 0) {
		pList->tail = anItem; //if its the first item, need to update tail 
	} else { //normal case 
		//printf("normal case\n);
		anItem->next = pList->head; //attach to head
		pList->head->prev = anItem; //update old heads prev
		
	}
	//regular case
	anItem->prev = NULL; //update prev
	pList->head = anItem; //update head 
	pList->curr = anItem; //update curr
	pList->elementCount++; //update count			
		 
	return LIST_SUCCESS;
}
// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList) {
	assert(pList != NULL);
	
	void* curItem = NULL;
	//check if out of bounds 
	if ((pList->curr == NULL) && ((pList->bounds == LIST_OOB_START) || (pList->bounds == LIST_OOB_END))) {
		//printf("OOB");
		return  curItem;
	}
	//if the curr is the first element -> move curr OOB
	if (pList->elementCount == 1) {
		curItem = pList->curr->item;
		pList->curr = NULL; //update null 
		pList->bounds = LIST_OOB_START;
	} else {
		//if the curr is the first element -> need to also update head 
		if (pList->curr == pList->head) {
			//printf("the first element is being removed!!!\n");
			//printf("curr == head");
			curItem = pList->curr->item; 
			pList->head = pList->head->next; //update head 
			pList->curr = pList->head; //update curr
			pList->curr->prev = NULL; //remove link
			pList->head->prev = NULL; //remove link
		} else {
			//normal case 
			//printf("normal case of remove\n");
			curItem = pList->curr->item; //save cur item into curitem
			Node* tmp = pList->curr->prev; //save the prev item 
			pList->curr = pList->curr->next; //update curr 
			tmp->next = pList->curr; //update next link
			pList->curr->prev = tmp; //update prev link
		}
	}
	pList->elementCount--; //update elementcount 
	nextNodeIndex--; //update node pool
	return curItem;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
	assert(pList != NULL);
	
	void* curItem = NULL;
	//check if out of bounds 
	if ((pList->curr == NULL) && ((pList->bounds == LIST_OOB_START) || (pList->bounds == LIST_OOB_END))) {
		//printf("OOB");
		return  curItem;
	}
	//if the curr is the first element -> move cuest append 6
	if (pList->elementCount == 1) {
		curItem = pList->tail->item;
		pList->curr = NULL; //update null 
		pList->bounds = LIST_OOB_START;
	} else {
		//normal case 
		curItem = pList->tail->item; //save tail item into curitem
		Node* tmp = pList->tail->prev; //save the prev item 
		pList->tail = tmp; //update tail 
		pList->tail->next = NULL; //remove old link
		pList->curr = tmp; //update curr
	}
	pList->elementCount--; //update elementcount 
	nextNodeIndex++; //update node pool
	return curItem;
}
// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2) {
	assert(pList1 != NULL);
	assert(pList2 != NULL);
	
	//if p1 is empty -> curr->next is p2 and free p2 head 
	if (pList1->curr == NULL) {
		//printf("case that p1 is empty\n");
		pList1->head = pList2->head; //attach p2 to p1
		pList2->head->prev = pList1->head; //update prev link
		pList1->elementCount = pList2->elementCount; //update element count to p2
		pList1->curr = NULL; //keep it null
		pList1->bounds = LIST_OOB_START; //out of bounds
	} else if (pList2->curr == NULL) {	//if p2 is empty -> free p2 head 
		//printf("case that p2 but not p1 is empty \n");
		//	nextHeadIndex--;//free p2 head 
	} else if ((pList1 != NULL) && (pList2 != NULL)) { //normal case 
		pList1->tail->next = pList2->head;//tail of p1 the next should point to head of p2
		pList2->head->prev = pList1->tail; //update prev link
		pList1->elementCount = pList1->elementCount + pList2->elementCount; //add the elementcounts 
	}
	//if p1 or p2 is empty; p1 empty, still current is null
	nextHeadIndex--;//free p2 head regardless of case   
	return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
//typedef void (*FREE_FN)(void* pItem); //dont think i need that here 
void List_free(List* pList, FREE_FN pItemFreeFn) {
	//basically want to traverse the LL and call pItemFreeFn on each node, and clean up 
	//clean up as in put each node back into the pool, and also the head back into the pool
	assert(pList != NULL);
	Node* tmp = pList->head; //tmp ptr
	
	for (int i = 0; i < pList->elementCount; i++) {
		//printf("gotta call free \n");
		(*pItemFreeFn)(tmp->item); //call the routine to free the node 
		tmp = tmp->next; //increment to next node 
		nextNodeIndex--; //free node 
	}
	//need to free head pointer 
	nextHeadIndex--; //free the head
	
}

// Search pList, starting at the current item, until the end is reached or a match is found. 

// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {

	assert(pList != NULL);
	void* match = NULL;
	Node* tmp = NULL;
	int found = 0; //0 for not found  
	//ensure curr is not out of bounds 
	if ((pList->curr == NULL) && (pList->bounds == LIST_OOB_END)) {
		//printf("curr is null, OOB end\n");
		return match;
	} else if ((pList->curr == NULL) && (pList->bounds == LIST_OOB_START)) {
		//if curr OOB START, start search from first node 
		tmp = pList->head;
		//printf("start formr the front\n");
	} else { //normal case 
		tmp = pList->curr;
	}
	while (tmp != NULL) {
		//printf("normal case\n");
		//call the compare 
		found = (*pComparator)(tmp->item, pComparisonArg);
		//need if statement to see if we found a match or not 
		if (found == 1) {
			//printf("i am found!\n");
			pList->curr = tmp; //update curr 
			match = tmp->item;
			return match;
		}
		tmp = tmp->next; //increment to next node 
	}
	pList->curr = NULL;
	pList->bounds = LIST_OOB_END;
	//printf("nope not found\n");
	return match;
}








