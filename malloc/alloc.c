/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *startOfHeap = NULL;

typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
  //void* prevFree;
  //void* nextFree;
} metadata_t;

void* freeHead = NULL;

void print_heap(size_t size) {
  printf("Inside: malloc(%lu):\n", size);
  // If we have not recorded the start of the heap, record it:
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
    // sbrk(0) returns the current end of our heap, without increasing it.
  }
  // Print out data about each metadata chunk:
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  printf("-- Start of Heap (%p) --\n", startOfHeap);
  while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  printf("-- End of Heap (%p) --\n\n", endOfHeap);
}


/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc:
    void *ptr = malloc(num * size);
    if (!ptr) {
      return NULL;
    }
    memset(ptr, 0 , num*size);
    return ptr;
}


/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void *malloc(size_t size) {
  // implement malloc
  //print_heap(size);
  
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
    // sbrk(0) returns the current end of our heap, without increasing it.
  }
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  while ((void *)curMeta < endOfHeap) {
    if (curMeta->isUsed == 0 && curMeta->size == size) {
      curMeta->isUsed = 1;
      void* ptr = (void *)curMeta + sizeof(metadata_t);
      return ptr;
    } else if (curMeta->isUsed == 0 && curMeta->size > size) {
      size_t oldsize = curMeta->size;
      curMeta->isUsed = 1;
      curMeta->size = size;
      void* curptr = (void *)curMeta + sizeof(metadata_t);
      metadata_t* nextMeta = (void *)curptr + size;
      nextMeta->isUsed = 0;
      nextMeta->size = oldsize - sizeof(metadata_t) - size;
      return curptr;
    } else {
      //do nothing
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }

  metadata_t *meta = sbrk( sizeof(metadata_t) );
  if (!meta) {
    return NULL;
  }
  meta->size = size;
  meta->isUsed = 1;

  // Allocate heap memory for the requested memory:
  void *ptr = sbrk( size );

  if (!ptr) {
    return NULL;
  }

  // Return the pointer for the requested memory:
  return ptr;
}


/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
  // implement free
  metadata_t *meta = ptr - sizeof( metadata_t );
  meta->isUsed = 0;

  //traverse
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
    // sbrk(0) returns the current end of our heap, without increasing it.
  }
  // Print out data about each metadata chunk:
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  metadata_t* nextMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  while ((void *)nextMeta < endOfHeap) {   // While we're before the end of the heap...
    if (curMeta->isUsed == 0 && nextMeta->isUsed == 0) {
      size_t totalsize = curMeta->size + nextMeta->size + 2 * sizeof(metadata_t);
      curMeta->size = curMeta->size + nextMeta->size + sizeof(metadata_t);
      nextMeta = (void *)curMeta + totalsize;
    } else {
      metadata_t* temp = nextMeta;
      nextMeta = (void *)nextMeta + nextMeta->size + sizeof(metadata_t);
      curMeta = temp;
    }
  }
}


/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    ptr = malloc(size);
    if (!ptr) {
      return NULL;
    }
    return ptr;
  }
  if (size == 0) {
    free(ptr);
    return NULL;
  }
  metadata_t* meta = ptr - sizeof(metadata_t);
  if (size <= meta->size) {
    meta->size = size;
    return ptr;
  } else {
    void* newptr = malloc(size);
    if (!newptr) {
      return NULL;
    }
    memcpy(newptr, ptr, meta->size);
    free(ptr);
    return newptr;
  }
}