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
  void* prev_free;
  void* next_free;
  void* prev_mem;
  void* next_mem;
} metadata_t;

void* free_head = NULL;
void* mem_tail = NULL;

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

void insert_block_into_free_list(void *block) {
  metadata_t *meta      = (metadata_t *) block;
  metadata_t *cur = (metadata_t *) free_head;

  if (free_head == NULL) {
    free_head = (void*)meta;
    return;
  }

  while (cur->next_free) {
    cur = cur->next_free;
  }
  cur->next_free = meta;
  meta->prev_free = cur;
  meta->next_free = NULL;
}

void remove_block_from_free_list(void *block) {
  metadata_t *meta = (metadata_t *) block;

  metadata_t *prev_meta = (metadata_t *) meta->prev_free;
  metadata_t *next_meta = (metadata_t *) meta->next_free;
  meta->prev_free = NULL;
  meta->next_free = NULL;

  // REMOVE BLOCK FROM LIST
  if (prev_meta != NULL) {
    prev_meta->next_free = next_meta;
  }

  if (next_meta != NULL) {
    next_meta->prev_free = prev_meta;
  }

  // CHANGE HEAD IF NEEDED
  if (block == free_head) {
    free_head = (void*)next_meta;
  }
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

  metadata_t* cur = (metadata_t*)free_head;
  while (cur) {
    if (cur->size == size) {
      cur->isUsed = 1;
      void* ptr = (void*)cur + sizeof(metadata_t);
      remove_block_from_free_list((void*)cur);
      return ptr;
    } else if (cur->size > size) {
      metadata_t* next_block = cur->next_mem;
      size_t oldsize = cur->size;
      cur->isUsed = 1;
      cur->size = size;

      metadata_t* curPrev = cur->prev_free;
      metadata_t* curNext = cur->next_free;
      remove_block_from_free_list(cur);

      void* curptr = (void *)cur + sizeof(metadata_t);
      metadata_t* newBlock = (void *)curptr + size;
      newBlock->isUsed = 0;
      newBlock->size = oldsize - sizeof(metadata_t) - size;
      newBlock->next_mem = next_block;
      if (next_block) {
        next_block->prev_mem = newBlock;
      }
      cur->next_mem = newBlock;
      newBlock->prev_mem = cur;
      
      //insert new block into free list
      newBlock->prev_free = curPrev;
      newBlock->next_free = curNext;
      if (curPrev) {
        curPrev->next_free = newBlock;
      }
      if (curNext) {
        curNext->prev_free = newBlock;
      }
      if (!newBlock->prev_free) {
        free_head = (void*)newBlock;
      }

      return curptr;
    } else {
      //do nothing
    }
    cur = cur->next_free;
  }

  //malloc new block
  metadata_t *meta = sbrk( sizeof(metadata_t) );
  if (!meta) {
    return NULL;
  }
  meta->size = size;
  meta->isUsed = 1;
  void *ptr = sbrk( size );
  if (!ptr) {
    return NULL;
  }
  meta->next_mem = NULL;
  meta->prev_mem = mem_tail;
  meta->next_free = NULL;
  meta->prev_free = NULL;

  if (mem_tail) {
    metadata_t* tail = (metadata_t*)mem_tail;
    tail->next_mem = meta;
  }
  mem_tail = (void*)meta;
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
  if (!ptr) {
    return;
  }
  metadata_t* meta = ptr - sizeof(metadata_t);
  metadata_t* nextMeta = meta->next_mem;
  metadata_t* prevMeta = meta->prev_mem;

  int prev_cont = (prevMeta) && ((void*)prevMeta + sizeof(metadata_t) + prevMeta->size == meta);
  int next_cont = (nextMeta) && ((void*)meta + sizeof(metadata_t) + meta->size == nextMeta);

  //merge two sides case
  if (prevMeta && nextMeta && prevMeta->isUsed == 0 && nextMeta->isUsed == 0 && prev_cont && next_cont) {
    remove_block_from_free_list((void*)nextMeta);
    metadata_t* next_next = nextMeta->next_mem;
    if (next_next) {
      next_next->prev_mem = prevMeta;
    }
    prevMeta->next_mem = next_next;

    if ((void*)nextMeta == mem_tail) {
      mem_tail = (void*)prevMeta;
    }

    prevMeta->size += meta->size + nextMeta->size + 2 * sizeof(metadata_t);
    return;
  }

  if (prevMeta && prevMeta->isUsed == 0 && prev_cont) {
    if (nextMeta) {
      nextMeta->prev_mem = prevMeta;
    }
    prevMeta->next_mem = nextMeta;
    if (mem_tail == (void*)meta) {
      mem_tail = (void*)prevMeta;
    }
    prevMeta->size += meta->size + sizeof(metadata_t);
    remove_block_from_free_list(meta);
    return;
  }

  if (nextMeta && nextMeta->isUsed == 0 && next_cont) {
    metadata_t* nextMeta_prev = nextMeta->prev_free;
    metadata_t* nextMeta_next = nextMeta->next_free;
    remove_block_from_free_list((void*)nextMeta);
    metadata_t* next_next = nextMeta->next_mem;
    if (next_next) {
      next_next->prev_mem = meta;
    }
    meta->next_mem = next_next;
    meta->size += nextMeta->size + sizeof(metadata_t);
    meta->isUsed = 0;
    meta->prev_free = nextMeta_prev;
    meta->next_free = nextMeta_next;
    if (nextMeta_prev) {
      nextMeta_prev->next_free = meta;
    }
    if (nextMeta_next) {
      nextMeta_next->prev_free = meta;
    }
    if (!meta->prev_free) {
      free_head = (void*)meta;
    }
    return;
  }
  meta->isUsed = 0;
  insert_block_into_free_list(meta);
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