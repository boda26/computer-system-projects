#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

/*
You must read an HTTP request from the fd (use your httprequest_read).
You must create an HTTP response to respond to the request.
  If the requested path is /, you should process that request as if the path is /index.html.
  If the file does not exist in your static directory (excluding the /), you must respond with a 404 Not Found response.
  If the file requested does exist, you will respond with a 200 OK packet and:
    Return the contents of the file as the payload,
    If the file name ends in .png, the Content-Type header must be set to image/png.
    If the file name ends in .html, the Content-Type header must be set to text/html.

    curl http://localhost:24003/ -H 'Host: localhost'
*/

void *client_thread(void *vptr) {
  int fd = *((int *)vptr);
  HTTPRequest* request = malloc(sizeof(HTTPRequest));
  httprequest_read(request, fd);

  //printf("%s\n", 111);
  //change request path
  if (strcmp(request->path, "/") == 0) {
    request->path = "/index.html";
  }

  //try to open the file
  //printf("%d\n", 111);
  char* newpath = malloc(7 + strlen(request->path));
  strcat(newpath, "static");
  strcat(newpath, request->path);
  FILE* f = fopen(newpath, "r");
  //printf("%d\n", 222);
  if (f == NULL) {    //file name does not exist
    char* response1 = "HTTP/1.0 404 Not Found\r\n";
    write(fd, response1, strlen(response1) + 1);
    close(fd);
    //printf("%d\n", 333);
    return;
  } else {    //file name exist
    //printf("%d\n", 444);
    char* response2 = malloc(4096);
    strcat(response2, "HTTP/1.0 200 OK\r\n");
    char* getdot = strchr(request->path, '.');
    //printf("%s\n", getdot);
    //printf("%d\n", 555);
    if (strcmp(getdot, ".png") == 0) {
      //printf("%d\n", 333);
      strcat(response2, "Content-Length: ");
      fseek(f, 0, SEEK_END);
      int end = ftell(f);
      fseek(f, 0, SEEK_SET);
      char* size = malloc(sizeof(char));
      sprintf(size, "%d", end);
      strcat(response2, size);
      strcat(response2, "\r\n");
      strcat(response2, "Content-Type: ");
      strcat(response2, "image/png");
      strcat(response2, "\r\n\r\n");
      write(fd, response2, strlen(response2));
      void* payload = malloc(end);
      fread(payload, 1, end, f);
      write(fd, payload, end);
      close(fd);
      return;
    } else if (strcmp(getdot, ".html") == 0) {
      //printf("%d\n", 333);
      strcat(response2, "Content-Length: ");
      fseek(f, 0, SEEK_END);
      int end = ftell(f);
      fseek(f, 0, SEEK_SET);
      char* size = malloc(sizeof(char));
      sprintf(size, "%d", end);
      strcat(response2, size);
      strcat(response2, "\r\n");
      strcat(response2, "Content-Type: ");
      strcat(response2, "text/html");
      strcat(response2, "\r\n\r\n");
      write(fd, response2, strlen(response2));
      void* payload = malloc(end);
      fread(payload, 1, end, f);
      write(fd, payload, end);
      close(fd);
      //printf("%d\n", 333);
      return;
    }

  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);
  printf("Binding to port %d. Visit http://localhost:%d/ to interact with your server!\n", port, port);

  // socket:
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // bind:
  struct sockaddr_in server_addr, client_address;
  memset(&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);  
  bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

  // listen:
  listen(sockfd, 10);

  // accept:
  socklen_t client_addr_len;
  while (1) {
    int *fd = malloc(sizeof(int));
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    pthread_create(&tid, NULL, client_thread, fd);
    pthread_detach(tid);
  }

  return 0;
}