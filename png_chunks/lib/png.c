#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;


/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  FILE* f = fopen(filename, mode);
  if (f == NULL) {
    return NULL;
  }
  if (strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0) {
    char* sig = malloc(8);
    fread(sig, 1, 8, f);
    if (sig[0] != -119 || sig[1] != 80 || sig[2] != 78 || sig[3] != 71 || sig[4] != 13 || sig[5] != 10 || sig[6] != 26 || sig[7] != 10) {
      free(sig);
      return NULL;
    } else {
      PNG* p = malloc(sizeof(PNG*));
      p->ff = f;
      p->pos = 8;
      p->isEnd = 0;
      free(sig);
      return p;
    }
  } else if (strcmp(mode, "w") == 0) {
    char* sig = malloc(8);
    sig[0] = -119;  //137
    sig[1] = 80;
    sig[2] = 78;
    sig[3] = 71;
    sig[4] = 13;
    sig[5] = 10;
    sig[6] = 26;
    sig[7] = 10;
    fwrite(sig, 1, 8, f);
    
    char* sss = malloc(8);
    fread(sss, 1, 8, f);
    //printf("%s\n", sss);
    PNG* p = malloc(sizeof(PNG*));
    p->ff = f;
    p->pos = 8;
    p->isEnd = 0;
    free(sig);
    return p;
  } else {
    return NULL;
  }
}


/*
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
  if (png->ff == NULL) {
    return 0;
  }
  if (png->isEnd == 1) {
    return 0;
  }
  fseek(png->ff, png->pos, SEEK_SET);
  fread(&chunk->len, 1, 4, png->ff);
  chunk->len = swap32(chunk->len);
  fread(chunk->type, 1, 4, png->ff);
  chunk->type[4]='\x0';
  if (chunk->type[0] == 73 && chunk->type[1] == 69 && chunk->type[2] == 78 && chunk->type[3] == 68) {
    png->isEnd = 1;
  }
  chunk->data = malloc(chunk->len);
  fread(chunk->data, 1, chunk->len, png->ff);
  fread(&chunk->crc, 1, 4, png->ff);
  chunk->crc = swap32(chunk->crc);
  png->pos = ftell(png->ff);
  return chunk->len + 12;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written.   As part of writing the `chunk`, you must calculate the CRC
 * based on the other data in the `chunk`; do not assume the CRC given has been calculated for you.
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  if (png->ff == NULL) {
    return 0;
  }
  fseek(png->ff, png->pos, SEEK_SET);
  uint32_t l1 = swap32(chunk->len);
  fwrite(&l1, 1, 4, png->ff);
  chunk->type[4] = '\x0';
  fwrite(&chunk->type, 1, 4, png->ff);
  fwrite(chunk->data, 1, chunk->len, png->ff);

  char* buffer = malloc(chunk->len + 4);
  strcpy(buffer, chunk->type);
  strcat(buffer, chunk->data);
  //printf("%s\n", buffer);
  uint32_t crc = 0;
  crc32(buffer, chunk->len + 4, &crc);
  //printf("%d\n", crc);
  crc = swap32(crc);
  //printf("%d\n", crc);
  fwrite(&crc, 1, 4, png->ff);

  /*
  fseek(png->ff, 8, SEEK_SET);
  fread(&chunk->len, 1, 4, png->ff);
  chunk->len = swap32(chunk->len);
  //printf("%d\n", chunk->len);
  fread(chunk->type, 1, 4, png->ff);
  //printf("%s\n", chunk->type);
  chunk->data = malloc(chunk->len);
  fread(chunk->data, 1, chunk->len, png->ff);
  fread(&chunk->crc, 1, 4, png->ff);
  */
  //printf("%d\n", chunk->crc);
  free(buffer);
  png->pos = ftell(png->ff);
  return chunk->len + 12;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  free(chunk->data);
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  fclose(png->ff);
}

uint32_t swap32(uint32_t k)
{
  return ((k << 24) |
    ((k & 0x0000FF00) << 8) |
    ((k & 0x00FF0000) >> 8) |
    (k >> 24)
  );
}
