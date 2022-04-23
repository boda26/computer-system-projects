// Tests Updated (v2) on Sunday, Aug. 29
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../emoji.h"
#include "lib/catch.hpp"


/**
 * Returns `true` if string `s` starts with an emoji.
 * 
 * This is a crude function that returns `false` when the string `s` absolutely does not start
 * with an emoji.  When this function returns `true`, `s` starts with something that is within
 * the range of valid emoji characters.  However, this range is sparse and this function does
 * not check if the emoji is a defined emoji.
 * 
 * See Unicode's Emoji List: https://www.unicode.org/Public/UCD/latest/ucd/emoji/emoji-data.txt
 */
int isEmoji(const char *s) {
  unsigned int val = 0;
  for(int i=0;i<strlen(s);i++)
  {
    val = (val << 8) | ((unsigned int)(s[i]) & 0xFF);
  }
  return
  (
    (val >= 14844092 /* U+203C */ && val <= 14912153 /* U+3299 */) ||
    (val >= 4036984960 /* U+1F000 */ && val <= 4036995737 /* U+1FA99 */ )
  );
}


TEST_CASE("`emoji_favorite` returns a valid emoji", "[weight=1][part=3]") {
  const char *s = emoji_favorite();
  REQUIRE(strcmp(s, "") != 0);
  REQUIRE(isEmoji(s) != 0);
}

TEST_CASE("`emoji_count` counts one emoji", "[weight=1][part=3]") {
  char *s = malloc(100);
  strcpy(s, "hi\xF0\x9F\x8E\x89");
  int r = emoji_count(s);
  REQUIRE(r == 1);
  free(s);
}

TEST_CASE("`emoji_count` counts multiple emoji", "[weight=3][part=3]") {
  char *s = malloc(100);
  strcpy(s, "\xF0\x9F\x92\x96 \xF0\x9F\x92\xBB \xF0\x9F\x8E\x89");
  int r = emoji_count(s);
  REQUIRE(r == 3);
  free(s);
}

TEST_CASE("`emoji_invertChar` inverts smiley face into another emoji", "[weight=1][part=3]") {
  char *s = malloc(100);
  strcpy(s, "\xF0\x9F\x98\x8A");
  emoji_invertChar(s);
  REQUIRE(strcmp(s, "\xF0\x9F\x98\x8A") != 0);
  free(s);
}

TEST_CASE("`emoji_invertChar` inverts at least six total emojis", "[weight=3][part=3]") {
  int emoji_invert_count = 0;

  unsigned int i;
  for (i = 4036984960; i <= 4036995737; i++) {
    char emoji[5];
    /* memcpy() results depends on machine architecture and endian-ness */
    emoji[0] = (i >> 24) & 0xFF;
    emoji[1] = (i >> 16) & 0xFF;
    emoji[2] = (i >> 8) & 0xFF;
    emoji[3] = i & 0xFF;
    emoji[4] = '\0';
    emoji_invertChar(emoji);

    int* val = (int *)emoji;
    if (*val != i) {
      emoji_invert_count++;
      if (emoji_invert_count >= 6) { break; }
    }
  }

  REQUIRE( emoji_invert_count >= 6 );
}

TEST_CASE("`emoji_invertAll` inverts a string of emojis", "[weight=3][part=3]") {
  char *s = malloc(100);
  strcpy(s, "\xF0\x9F\x92\x96 \xF0\x9F\x92\xBB \xF0\x9F\x8E\x89 \xF0\x9F\x98\x8A");
  emoji_invertAll(s);
  char *testing_emoji = malloc(20);
  strcpy(testing_emoji, s + 15);
  testing_emoji[4] = '\0';
  REQUIRE(strcmp(testing_emoji, "\xF0\x9F\x98\x8A") != 0);
  free(s);
}

TEST_CASE("`emoji_random_alloc` allocates new memory", "[weight=1][part=3]") {
  char *s1 = emoji_random_alloc();
  char *s2 = emoji_random_alloc();
  REQUIRE(s1 != s2);
  free(s2);
  free(s1);
}

TEST_CASE("`emoji_random_alloc` allocates valid emoji", "[weight=3][part=3]") {
  const int total_emojis = 100;
  int valid_emoji = 0;

  for (int i = 0; i < total_emojis; i++) {
    char *s = emoji_random_alloc();
    if (s != NULL && isEmoji(s)) { valid_emoji++; }
  }

  REQUIRE( valid_emoji == total_emojis );
}

TEST_CASE("`emoji_random_alloc` allocates random emoji", "[weight=3][part=3]") {
  const int total_emojis = 1000;
  int unique_emoji = 0;

  char **emojis = malloc(total_emojis * sizeof(char *));
  for (int i = 0; i < total_emojis; i++) {
    emojis[i] = emoji_random_alloc();
    if (emojis[i] == NULL) { break; }

    int isUnique = 1;
    for (int j = 0; j < i; j++) {
      if (strcmp(emojis[i], emojis[j]) == 0) {
        isUnique = 0; break;
      }
    }

    if (isUnique) {
      unique_emoji++;
    }
  }

  // There will randomly be some non-unique emoji.  However, in the range of over ~2,816,
  // the chance of having less than 500 unique emoji when randomly drawn is vanishingly small.
  REQUIRE( unique_emoji > total_emojis * 0.5 );
}

