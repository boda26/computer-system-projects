#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wallet.h"

/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Implement
  //printf("%d\n", 111);
  pthread_mutex_init(&wallet->lock, NULL);
  pthread_cond_init(&wallet->cond, NULL);
  wallet->head = NULL;
  //printf("%d\n", 111);
}



/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  // Implement this
  //printf("%d\n", 222);
  wallet_resource* cur = wallet->head;
  while (cur) {
    if (strcmp(resource, cur->resource_name) == 0) {
      return cur->amount;
    }
    cur = cur->next;
  }
  //printf("%d\n", 222);
  return 0;
}

/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  // Implement this
  //printf("%d\n", 333);
  pthread_mutex_lock(&wallet->lock);
  wallet_resource* cur = wallet->head;
  while (cur) {
    if (strcmp(resource, cur->resource_name) == 0) {
      if (cur->amount + delta >= 0) {
        cur->amount = cur->amount + delta;
        pthread_cond_broadcast(&wallet->cond);
        pthread_mutex_unlock(&wallet->lock);
        return;
      } else {
        while (1) {
          while (cur->amount + delta < 0) {
            //printf("%d\n", cur->amount + delta);
            pthread_cond_wait(&wallet->cond, &wallet->lock);
          }
          cur->amount = cur->amount + delta;
          pthread_cond_broadcast(&wallet->cond);
          pthread_mutex_unlock(&wallet->lock);
          return;
        }
      }
    }
    cur = cur->next;
  }

  if (delta >= 0) {
    cur = malloc(sizeof(wallet_resource));
    cur->resource_name = malloc(sizeof(char));
    strcpy(cur->resource_name, resource);
    cur->amount = delta;
    cur->next = NULL;
    if (wallet->head) {
      cur->next = wallet->head;
      wallet->head = cur;
    } else {
      wallet->head = cur;
    }
    pthread_cond_broadcast(&wallet->cond);
    pthread_mutex_unlock(&wallet->lock);
    return;
  } else {
    while (1) {
      cur = malloc(sizeof(wallet_resource));
      cur->resource_name = malloc(sizeof(char));
      strcpy(cur->resource_name, resource);
      cur->amount = 0;
      cur->next = NULL;
      if (wallet->head) {
        cur->next = wallet->head;
        wallet->head = cur;
      } else {
        wallet->head = cur;
      }

      while (cur->amount + delta < 0) {
        //printf("%d\n", cur->amount + delta);
        pthread_cond_wait(&wallet->cond, &wallet->lock);
      }

      cur->amount = cur->amount + delta;
      pthread_cond_broadcast(&wallet->cond);
      pthread_mutex_unlock(&wallet->lock);
      return;
    }
  }
}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement this
  wallet_resource *cur = wallet->head;
  while (cur) {
    wallet_resource *next = cur->next;
    free(cur);
    cur = next;
  }
}