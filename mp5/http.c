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
  char *copy1 = malloc(strlen(buffer) + 1);
  strcpy(copy1, buffer);
  char *saveptr1;
  char *saveptr2;
  char *saveptr3;
  char *split_line = strtok_r(copy1, "\r\n", &saveptr1);
  char *split_space;
  char *split_colon;
  char *copy2;
  char *copy3;
  int i = 0;
  int count_header = 0;
  int keyOrValue = 0;
  Pair* cur = NULL;
  while (split_line) {
    //printf("%s\n", split_line);
    if (i == 0) {   //populate the header
      copy2 = malloc(strlen(split_line) + 1);
      strcpy(copy2, split_line);
      split_space = strtok_r(copy2, " ", &saveptr2);
      count_header = 0;
      while (split_space != NULL) {
        if (count_header == 0) {
          req->action = malloc(strlen(split_space) + 1);
          strcpy(req->action, split_space);
        } else if (count_header == 1) {
          req->path = malloc(strlen(split_space) + 1);
          strcpy(req->path, split_space);
        } else if (count_header == 2) {
          req->version = malloc(strlen(split_space) + 1);
          strcpy(req->version, split_space);
        } else {
        }
        split_space = strtok_r(NULL, " ", &saveptr2);
        count_header++;
      }
    }

    if (strstr(split_line, ": ")) {
      copy3 = malloc(strlen(split_line) + 1);
      strcpy(copy3, split_line);
      split_colon = strtok_r(copy3, ": ", &saveptr3);
      keyOrValue = 0;
      cur = malloc(sizeof(Pair*));
      while (split_colon != NULL) {
        if (keyOrValue == 0) {
          cur->key = malloc(strlen(split_colon) + 1);
          strcpy(cur->key, split_colon);
        } else if (keyOrValue == 1) {
          cur->value = malloc(strlen(split_colon) + 1);
          strcpy(cur->value, split_colon);
          break;
        }
        keyOrValue++;
        split_colon = strtok_r(NULL, ": ", &saveptr3);
      }
      cur->next = NULL;
      if (req->head) {
        cur->next = req->head;
        req->head = cur;
      } else {
        req->head = cur;
      }
    }

    split_line = strtok_r(NULL, "\r\n", &saveptr1);
    i++;
  }
  return 1;
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
      //printf("%s\n", cur->value);
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