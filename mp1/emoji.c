#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Return your favorite emoji; do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
const char *emoji_favorite() {
  return "\xF0\x9F\x90\xB6";
}


// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(char *utf8str) {
  int i;
  int count = 0;
  for (i = 0; utf8str[i] != '\0'; i++) {
    if (utf8str[i] == '\xF0') {
      count++;
    }
  }
  return count;
}


// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-simpling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {
  
  /*
  if (utf8str[3] == '\x8A') {
    utf8str[3] = '\x9E';
  } else if (utf8str[3] == '\x81') {
    utf8str[3] = '\xAD';
  } else if (utf8str[3] == '\x9C') {
    utf8str[3] = '\xA9';
  } else if (utf8str[3] == '\x8F') {
    utf8str[3] = '\x9D';
  } else if (utf8str[3] == '\x8D') {
    utf8str[3] = '\xA1';
  } else if (utf8str[3] == '\x88') {
    utf8str[3] = '\x8E';
  }
  */

  char* u = malloc(sizeof(char*));
  u = strncpy(u, utf8str, 5);
  char* a = "\xF0\x9F\x98\x8A";
  if (strcmp(u, a) == 0) {
    char* n1 = "\xF0\x9F\x98\x9E";
    strcpy(utf8str, n1);
  }
}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {
  char* c = malloc(4);
  int l;
  l = strlen(utf8str);
  int i;
  for (i = 0; i < l; i++) {
    strncpy(c, utf8str + i, 4);
    if (c[0] == '\xF0' && c[1] == '\x9F') {
      emoji_invertChar(c);
      memcpy(utf8str + i, c, 4);
    }
  }
  
  /*
    strncpy(c, utf8str + i, 5);
    if (c[0] == '\xF0' && c[1] == '\x9F') {
      emoji_invertChar(c);
    }
    if (utf8str[i] == '\xF0') {
      c[0] = utf8str[i];
      c[1] = utf8str[i+1];
      c[2] = utf8str[i+2];
      c[3] = utf8str[i+3];
      emoji_invertChar(c);
    }
  */

}

// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
  char* emj = malloc(sizeof(char*));
  emj[0] = 240;
  emj[1] = 159;
  emj[2] = rand() % 40 + 128;
  emj[3] = rand() % 64 + 128;
  emj[4] = 0;
  return emj;
}