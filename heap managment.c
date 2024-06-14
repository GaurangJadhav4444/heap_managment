#include <stdio.h>
#include <stdlib.h>
#define HEAPSIZE 10000
char Heap[HEAPSIZE];
typedef struct MetaData_Tag
{
    int size;
    char status; 
    struct MetaData_Tag *next;
} metadata;

metadata *heap_ptr = (void *)Heap;


void Initialize()
{
    heap_ptr->size = (HEAPSIZE - (sizeof(metadata)));
    printf("Heap size is %d bytes \n", heap_ptr->size);
    printf("Size of Metadata structure is %d bytes \n", sizeof(metadata));
    heap_ptr->status = 'f';
    heap_ptr->next = NULL;
    printf("Address of starting of heap: %d \n", 0);
    printf("Ending address: %d\n", HEAPSIZE);
    printf("Heap block initialized \n\n");
}

void *Allocate(int given_size)
{
    metadata *prev, *lead;
    void *ret_ptr;
    prev = heap_ptr;
    lead = heap_ptr;
    ret_ptr = heap_ptr;

    int start = 0;
    while ((lead->size < given_size) || ((lead->status == 'a') && (lead != NULL)))
    {
        prev = lead;
        lead = lead->next;
        start = start + (prev->size + sizeof(metadata));
    }

    if ((lead->next == NULL) && (lead->size < given_size))
    {
        printf("No free space left to allocate %d bytes of memory \n", given_size);
        ret_ptr = NULL;
    }
    else
    {
        if (lead->size == given_size)
        {
            lead->status = 'a';
            printf("Allocated %d bytes of memory at Index %d - %d \n", given_size, start, start + given_size - 1);
            ret_ptr = lead;
        }
        else
        {
            metadata *newnode = (void *)(((void *)lead) + sizeof(metadata) + given_size);
            newnode->size = ((lead->size) - given_size - sizeof(metadata));
            newnode->next = lead->next;
            lead->size = given_size;
            printf("Allocated %d bytes of memory at Index %d - %d \n", given_size, start, start + given_size - 1);
            lead->next = newnode;
            lead->status = 'a';
            ret_ptr = newnode;
        }
    }
    return ret_ptr;
}

void Merge()
{
    metadata *merge_ptr, *temp;
    merge_ptr = heap_ptr;

    while (merge_ptr->next != NULL)
    {
        if ((merge_ptr->status == 'f') && ((merge_ptr->next)->status == 'f'))
        {
            merge_ptr->size = merge_ptr->size + ((merge_ptr->next)->size) + sizeof(metadata);
            merge_ptr->next = (merge_ptr->next)->next;
            printf("two free blocks are merged \n");
        }
        else
        {
            temp = merge_ptr;
            merge_ptr = merge_ptr->next;
        }
    }
    printf("Merge function done \n\n");
}

void Free(void *free_ptr)
{
    if (((heap_ptr) <= (free_ptr)) && ((heap_ptr + 10000)) >= (free_ptr))
    {
        metadata *ptr = heap_ptr;
        int index = 0;

        while (ptr->next != free_ptr)
        {
            index = index + (ptr->size + sizeof(metadata));
            ptr = ptr->next;
        }

        printf("Freed location at Index %d \n", index);
        ptr->status = 'f';
        printf("Freed \n");
        Merge();
    }
    else
    {
        printf("Entered block of address to be freed is invalid \n");
    }
}

void DisplayHeap()
{
    metadata *traverse_ptr = (void *)Heap;
    int index = 0;

    while (traverse_ptr != NULL)
    {
        printf("Index %d - %d  ", index, index + traverse_ptr->size - 1);
        printf("%c\n", traverse_ptr->status);

        index = index + traverse_ptr->size;
        traverse_ptr = traverse_ptr->next;
    }
    printf("\n\n");
}

int main()
{

    Initialize();
    int *memory1 = (int *)Allocate(150 * sizeof(int));
    DisplayHeap();
    long int *memory2 = (long int *)Allocate(150 * sizeof(long int));
    DisplayHeap();
    char *memory3 = (char *)Allocate(100 * sizeof(char));
    DisplayHeap();
    
    Free(memory2);
    printf("Heap memory status : \n");
    DisplayHeap();

    Free(memory3);
    printf("Heap memory status : \n");
    DisplayHeap();

    Free(memory1);
    printf("Heap memory status : \n");
    DisplayHeap();

    return 0;
}  