#include <stdio.h>
#include "list.h"

List* List_create()
{
    assert(num_head<LIST_MAX_NUM_HEADS);
    num_head++;
    return &head_arr[num_head-1];
};

int List_count(List* pList)
{
    int count = 0;
    Node *p = pList-> head;
    while (p != NULL)
    {
        p = p->next;
        count++;
    }
    return count;
};

void* List_first(List* pList)
{
    pList->curr = pList->head->next;
    return pList->curr;
};

void* List_last(List* pList)
{
    Node *p = pList-> head;
    while (p->next != NULL)
    {
        p = p->next;
    }
    pList->curr = p;
    return pList->curr;
};

void* List_next(List* pList)
{
    pList->curr = pList->curr->next;
    return pList->curr;
};

void* List_prev(List* pList)
{
    pList->curr = pList->curr->prev;
    return pList->curr;
};

void* List_curr(List* pList)
{
    return &pList->curr;
};

// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem);

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem);

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem);

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem);

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList);

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList);

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2);
