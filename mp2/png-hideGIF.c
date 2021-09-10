#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include "lib/crc32.h"

int png_hideGIF(const char *png_filename, const char *gif_filename) {
  PNG *png = PNG_open(png_filename, "r+");
  if (!png) { return ERROR_INVALID_FILE; }

  printf("PNG Header written.\n");
  int written = 0;

  // Read chunks until reaching "IEND" or in invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }

    if (written == 1) {
      int pos = ftell(png->ff);
      fseek(png->ff, 0, SEEK_END);
      int end = ftell(png->ff);
      char* store = malloc(end - pos);
      fseek(png->ff, pos, SEEK_SET);
      fread(store, 1, end - pos, png->ff);
      fseek(png->ff, pos, SEEK_SET);

      FILE* g = fopen(gif_filename, "r");
      fseek(g, 0, SEEK_END);
      uint32_t length = ftell(g);
      fseek(g, 0, SEEK_SET);
      printf("%d\n", length);
      
      PNG_Chunk gifchunk;
      gifchunk.len = length; //len
      char tp[5] = {'u', 'i', 'u', 'c', '\x0'};
      strcpy(gifchunk.type, tp);  //type
      printf("%s\n", gifchunk.type);

      gifchunk.data = malloc(length);
      fread(gifchunk.data, 1, length, g); //data

      gifchunk.crc = 0;

      PNG_write(png, &gifchunk);

      fwrite(store, 1, end - pos, png->ff);

      free(store);
      free(gifchunk.data);
      fclose(g);
      written = 0;
      png->pos += length + 12;
    }
    // Report data about the chunk to the command line:
    //bytesWritten = PNG_write(out, &chunk);
    //printf("PNG chunk %s written (%lu bytes)\n", chunk.type, bytesWritten);

    if (strcmp(chunk.type, "IHDR") == 0) {
      written = 1;
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
}

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_hideGIF(argv[1], argv[2]);
}

