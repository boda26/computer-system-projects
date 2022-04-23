#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../capitalize.h"
#include "lib/catch.hpp"

TEST_CASE("`capitalize` will capitalize a lowercase letter", "[weight=2][part=2]") {
  char *s = malloc(100);
  strcpy(s, "hello!");
  capitalize(s);
  REQUIRE(strcmp(s, "Hello!") == 0);
  free(s);
}

TEST_CASE("`capitalize` will not modify an already capitalized letter", "[weight=2][part=2]") {
  char *s = malloc(100);
  strcpy(s, "HI!");
  capitalize(s);
  REQUIRE(strcmp(s, "HI!") == 0);
  free(s);
}

TEST_CASE("`capitalizeAll` will fully capitalize a lowercase string of letters", "[weight=2][part=2]") {
  char *s = malloc(100);
  strcpy(s, "hello");
  capitalizeAll(s);
  REQUIRE(strcmp(s, "HELLO") == 0);
  free(s);
}

TEST_CASE("`capitalizeAll` will not modify numbers when capitalizing a string", "[weight=2][part=2]") {
  char *s = malloc(100);
  strcpy(s, "hi cs240");
  capitalizeAll(s);
  REQUIRE(strcmp(s, "HI CS240") == 0);
  free(s);
}

TEST_CASE("`capitalizeAll_alloc` will return newly allocated memory", "[weight=1][part=2]") {
  char *s = malloc(100);
  strcpy(s, "the quick brown fox jumped over the lazy red dog.");
  char *ptr = capitalizeAll_alloc(s);
  REQUIRE(ptr != NULL);
  REQUIRE(ptr != s);
  free(ptr);
  free(s);
}

TEST_CASE("`capitalizeAll_alloc` will capitalizing a lowercase string of letters", "[weight=2][part=2]") {
  char *s = malloc(100);
  strcpy(s, "the quick brown fox jumped over the lazy red dog.");
  char *ptr = capitalizeAll_alloc(s);
  REQUIRE(ptr != NULL);
  REQUIRE(strcmp(ptr, "THE QUICK BROWN FOX JUMPED OVER THE LAZY RED DOG.") == 0);
  free(ptr);
  free(s);
}
