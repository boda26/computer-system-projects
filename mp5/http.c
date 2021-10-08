#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>


#include "http.h"


/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  printf("%d\n", 111);
  ssize_t ret = 0;
  req->action = malloc(20);
  req->path = malloc(20);
  req->version = malloc(20);

  char* split_line;
  char* saveptr1, saveptr2;
  char* copy1 = malloc(buffer_len + 1);
  char* copy2;
  strcpy(copy1, buffer);

  char* delim1 = "\r\n";
  char* delim2 = ": ";
  
  int count_lines = 0;
  split_line = strtok_r(copy1, delim1, &saveptr1);
  while (split_line != NULL) {
    ret += strlen(split_line);
    req->buffer_array[count_lines] = malloc(strlen(split_line) + 1);
    strcpy(req->buffer_array[count_lines], split_line);
    split_line = strtok_r(NULL, delim1, &saveptr1);
    count_lines++;
    ret += strlen(delim1);
  }
  free(copy1);
  printf("%d\n", 222);

  //parse the header
  char* saveptr3;
  char* header = malloc(strlen(req->buffer_array[0]) + 1);
  memcpy(header, req->buffer_array[0], strlen(req->buffer_array[0]));
  //printf("%s\n", header);
  char* split_header = strtok_r(header, " ", &saveptr3);
  int headcount = 0;
  while (split_header != NULL) {
    //printf("%s\n", split_header);
    if (headcount == 0) {
      strcpy(req->action, split_header);
    } else if (headcount == 1) {
      strcpy(req->path, split_header);
    } else if (headcount == 2) {
      strcpy(req->version, split_header);
    }
    split_header = strtok_r(NULL, " ", &saveptr3);
    headcount++;
  }
  free(header);
  printf("%d\n", 333);

  int j;
  char* split_colonsp;
  int keyOrValue = 0;
  int index = 0;
  req->head = NULL;
  
  for (j = 0; j < count_lines; j++) {
    if (strstr(req->buffer_array[j], delim2)) {   //find a line with delim
      //printf("%s\n", req->buffer_array[j]);
      char* temp1 = malloc(strlen(req->buffer_array[j]) + 1);
      strcpy(temp1, req->buffer_array[j]);
      split_colonsp = strtok_r(temp1, delim2, &saveptr2);
      keyOrValue = 0;

      Pair* cur = malloc(sizeof(Pair));

      while (split_colonsp != NULL) {
        //printf("%s\n", split_colonsp);

        if (keyOrValue == 0) {
          //req->keys[index] = malloc(strlen(split_colonsp) + 1);
          //memcpy(req->keys[index], split_colonsp, strlen(split_colonsp) + 1);
          //printf("%s\n", req->keys[index]);

          cur->key = malloc(strlen(split_colonsp) + 1);
          strcpy(cur->key, split_colonsp);
          //printf("%s\n", cur->key);

        } else if (keyOrValue == 1) {
          //req->values[index] = malloc(strlen(split_colonsp) + 1);
          //memcpy(req->values[index], split_colonsp, strlen(split_colonsp) + 1);
          //printf("%s\n", req->values[index]);

          cur->value = malloc(strlen(split_colonsp) + 1);
          strcpy(cur->value, split_colonsp);
          break;
          //printf("%s\n", cur->value);
        }

        split_colonsp = strtok_r(NULL, delim2, &saveptr2);
        keyOrValue++;
      }
      cur->next = NULL;
      //printf("%s\n", cur->key);
      //printf("%s\n", cur->value);

      if (req->head) {
        cur->next = req->head;
        req->head = cur;
      } else {
        req->head = cur;
      }

      //free(temp1);
      index++;
    }
  }
  //req->keyslength = index;
  printf("%d\n", 444);
  return ret;
}


/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  int sz;
  ioctl(sockfd,FIONREAD,&sz);
  char* buffer = malloc(sz + 1);
  read(sockfd, buffer, sz);
  //printf("%s\n", buffer);
  char* bf = malloc(sz);
  memcpy(bf, buffer, sz);
  httprequest_parse_headers(req, bf, sz);
  //printf("%d\n", a);
  Pair* cur = req->head;
  char* saveptr;
  while (cur) {
    if (strcmp(cur->key, "Content-Length") == 0) {
      char* split1 = strtok_r(buffer, "\r\n\r\n", &saveptr);
      
      //split1 = strtok_r(NULL, "\r\n\r\n", &saveptr);
      //printf("%s\n", split1);
      req->payload = malloc(atoi(cur->value));
      memcpy(req->payload, buffer + sz - atoi(cur->value), atoi(cur->value));
      //printf("%s\n", req->payload);

      return sz;
    }
    cur = cur->next;
  }
  req->payload = NULL;
  return sz;
}


/**
 * httprequest_get_action
 * 
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req) {
  return req->action;
}


/**
 * httprequest_get_action
 * 
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  Pair* cur = req->head;
  while (cur != NULL) {
    if (strcmp(cur->key, key) == 0) {
      return cur->value;
    }
    cur = cur->next;
  }
  return NULL;
}


/**
 * httprequest_get_action
 * 
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req) {
  return req->path;
}


/**
 * httprequest_get_action
 * 
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req) {
  
}