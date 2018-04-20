#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mhDefs.h"
#include "stuDefs.h"


int main()
{
  
  HeapType *heap;
  void  *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7, *ptr8, *ptr9;

  heap = (HeapType *) malloc(sizeof(HeapType));
  mh_init(heap);

  ptr1 = mh_alloc(heap, 5*sizeof(int), "ints");
  ptr2 = mh_alloc(heap, 10*sizeof(double), "doubles");
  ptr3 = mh_alloc(heap, 8*sizeof(char), "chars");
  ptr4 = mh_alloc(heap, 12*sizeof(StudentType), "Students");

  printf("\nDUMP 1, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  mh_dealloc(heap, ptr1);
  mh_dealloc(heap, ptr2);
  mh_dealloc(heap, ptr3);

  printf("\nDUMP 2, byte count = %d\n", mh_count(heap));
  mh_dump(heap);
  mh_collect(heap);
  
  printf("\nDUMP 3, byte count = %d\n", mh_count(heap));
  mh_dump(heap);


  //allocating the 5 data structures and print the byte count and dump the heap
  ptr5 = mh_alloc(heap, 5*sizeof(SeaSponges), "Sea Sponges");
  printf("\nDUMP 4, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  ptr6 = mh_alloc(heap, 3*sizeof(Placoderms), "Placoderms");
  printf("\nDUMP 5, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  ptr7 = mh_alloc(heap, 10*sizeof(Amphibians), "Amphibians");
  printf("\nDUMP 6, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  ptr8 = mh_alloc(heap, 5*sizeof(Mammals), "Mammals");
  printf("\nDUMP 7, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  ptr9 = mh_alloc(heap, 2*sizeof(HomoSapians), "Homo Sapians");
  printf("\nDUMP 8, byte count = %d\n", mh_count(heap));
  mh_dump(heap);  


  //deallocating the 5 data structures and print the byte count and dump the heap
  mh_dealloc(heap,ptr5);
  printf("\nDUMP 9, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  mh_dealloc(heap,ptr6);
  printf("\nDUMP 10, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  mh_dealloc(heap,ptr7);
  printf("\nDUMP 11, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  mh_dealloc(heap,ptr8);
  printf("\nDUMP 12, byte count = %d\n", mh_count(heap));
  mh_dump(heap);

  mh_dealloc(heap,ptr9);
  printf("\nDUMP 13, byte count = %d\n", mh_count(heap));
  mh_dump(heap);
  printf("\n\n");

  mh_cleanup(heap);
  free(heap);

  return 0;
}

/*set the number of blocks in heap to 0	      
  allocate memory in blocks for MAX_BLK*/
void mh_init(HeapType *heap){
  heap->head = (NodeType*)malloc(sizeof(NodeType));
  heap->head->next = NULL;
}

/*free all the blocks allocated in mh_init*/
void mh_cleanup(HeapType *heap){
  free(heap->head);
}

/*allocates memory of size n
  saves the address of the alocated memory in block
  alongside with its size and lable
  increaments numBlocks
  setes the block to reserved
  returns the address of the allocated memory*/
void *mh_alloc(HeapType *heap, int n, char *label){
  NodeType *curr, *temp;
  curr = heap->head;
  while (curr->next !=NULL){
    curr = curr->next;
  }
  temp = (NodeType*)malloc(sizeof(NodeType));
  temp->data.size = n;
  temp->data.rsv = C_TRUE;
  strcpy(temp->data.tag , label);
  temp->data.addr = malloc(n);
  
  curr->next = temp;
  temp->next = NULL;
  return temp->data.addr;
}

/*compares the input address to all the addresses in blocks
  if found, check to see if the block is reserved
  if reserved, set the block to not reserved
  and deallocate the memory associated with pointer addr */
void mh_dealloc(HeapType *heap, void *addr){
  NodeType *temp, *prev, *now;
  prev = NULL;
  now = NULL;
  temp = heap->head->next;
  while (temp->next !=NULL){
    if(temp->data.addr == addr && temp->data.rsv == C_TRUE){ 
      if(prev == NULL){
        temp->data.rsv = C_FALSE;
        free(temp->data.addr);
        heap->head->next = temp->next;
        now = temp;
        free(now);
        return;
      }
      else if(prev != NULL){
        temp->data.rsv = C_FALSE;
        free(temp->data.addr);
        prev->next = temp->next;
        now = temp;
        free(now);
        return;
      }
    }
    else{
      prev = temp;
      temp = temp->next;
    }
  } 
  if(temp->next == NULL){
     if(temp->data.addr == addr && temp->data.rsv == C_TRUE){
       temp->data.rsv = C_FALSE;
       free(temp->data.addr);
       now = temp;
       heap->head->next = NULL;
       free(now); 
       return;
     } 
   }
}

/*check if the block is reserved
  if reserved, add the size of the block to the sum*/
int mh_count(HeapType *heap){
  int sum = 0;
  NodeType *temp;
  temp = heap->head;
  while (temp->next !=NULL){
    temp = temp->next;
    if(temp->data.rsv == C_TRUE)
      sum += temp->data.size;
  }
  return sum;
}

/*check if the block is reserved
  if reserved, print the label, size and the address kept in the block */
void mh_dump(HeapType *heap){
  NodeType *temp;
  temp = heap->head;
  while (temp->next !=NULL){
    temp = temp->next;
    if(temp->data.rsv == C_TRUE)
      printf("%50s : %20d bytes stored at %p\n", temp->data.tag, temp->data.size, temp->data.addr);
  }   
}

/*check if the block is reserved
  if reserved, set it to not reserved
  free the memory pointed to by the addr in that block*/
void mh_collect(HeapType *heap){
  NodeType *temp, *now;
  now = NULL;
  temp = heap->head->next;
  while (temp->next != NULL){
    if(temp->data.rsv == C_TRUE){
      temp->data.rsv = C_FALSE;
      free(temp->data.addr);
      now = temp;
      heap->head->next = temp->next;
      free(now);
    }
    temp = temp->next;
  }
  if(temp->next == NULL){
    if(temp->data.rsv == C_TRUE){
      temp->data.rsv = C_FALSE;
      free(temp->data.addr);
      now = temp;
      heap->head->next = NULL;
      free(now); 
    } 
  } 
}


















