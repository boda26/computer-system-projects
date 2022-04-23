#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Modify the string `s` to capitalize the first letter of the string if the first letter of `s`
// is lower case.  Otherwise, do nothing.
void capitalize(char *s) {
  if (s[0] >= 97 && s[0] <= 122) {
    s[0] = s[0] - 32;
  }
}

// Modify the string `s` to capitalize the ALL of the lower case letters in `s`.  If the letter
// is not lower case, do nothing.  (You can assume you will be given only ASCII characters.)
void capitalizeAll(char *s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    if (s[i] >= 97 && s[i] <= 122) {
      s[i] = s[i] - 32;
    }
  }
}

// Return a new string (allocated in heap memory that you allocate) that transformed `s` in
// the same way as `capitalizeAll`.  (In this function `s` cannot be modified directly and
// must be done in the new memory you will allocate.)
char *capitalizeAll_alloc(const char *s) {
  char* a = malloc(100);
  strcpy(a, s);
  capitalizeAll(a);
  //printf("%s\n", s);
  //printf("%s\n", a);
  return a;
}
