#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include <errno.h>

int png_extractGIF(const char *png_filename, const char *gif_filename) {
  PNG *png = PNG_open(png_filename, "r");
  if (!png) {
    return ERROR_INVALID_FILE;
  }
  //printf("PNG Header: OK\n");

  // Read chunks until reaching "IEND" or an invalid chunk:
  while (1) {

    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }

    // Report data about the chunk to the command line:
    //printf("Chunk: %s (%d bytes of data)\n", chunk.type, chunk.len);

    if (strcmp(chunk.type, "uiuc") == 0) {
      FILE* f = fopen(gif_filename, "w");
      char* towrite = malloc(chunk.len);
      memcpy(towrite, chunk.data, chunk.len);
      fwrite(towrite, 1, chunk.len, f);
      free(towrite);
      fclose(f);
      //printf("Chunk: %s (%d bytes of data)\n", chunk.type, chunk.len);
      
    }

    // Check for the "IEND" chunk to exit:
    if ( strcmp(chunk.type, "IEND") == 0 ) {
      PNG_free_chunk(&chunk);
      break;  
    }

    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }

  PNG_close(png);
  return 0;

  //return 255; Change the to a zero to indicate success, when your implementaiton is complete.
}


int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF Name>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_extractGIF(argv[1], argv[2]);
}
