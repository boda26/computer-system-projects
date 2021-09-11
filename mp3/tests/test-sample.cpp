#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/catch.hpp"
#include "lib/mstats-utils.h"

// BASIC MEMORY ALLOCATOR
TEST_CASE("Test Sample1 - Malloc Assigns Memory", "[weight=2][part=1]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample1 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 1700);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample2 - Malloc Assigns Memory", "[weight=2][part=1]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample2 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 10000);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample3 - Malloc Assigns Memory", "[weight=2][part=1]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample3 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 10000);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample7 - Calloc Assigns Memory", "[weight=2][part=1]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample7 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample8 - Realloc Reassigns Memory", "[weight=2][part=1]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample8 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 6000);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

// BLOCK SPLITTING
TEST_CASE("Test Sample1 - Block Splitting", "[weight=10][part=2]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample1 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 1100);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

// MEMORY COALESCING
TEST_CASE("Test Sample2 - Memory Coalescing", "[weight=5][part=3]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample2 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 7500);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample3 - Memory Coalescing", "[weight=5][part=3]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample3 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 7500);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample4 - Memory Coalescing", "[weight=5][part=3]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample4 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 10000);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}

// FREE LISTS
TEST_CASE("Test Sample6 - Free Lists", "[weight=5][part=4]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample6 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 3000000);
  REQUIRE(result->max_heap_used > 0);
  REQUIRE(result->time_taken < 3);
  system("rm mstats_result.txt");
}

TEST_CASE("Test Sample8 - Realloc Reuses Memory", "[weight=5][part=4]") {
  system("make -s");
  system("./mstats tests/samples_exe/sample8 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  REQUIRE(result->status == 1);
  REQUIRE(result->max_heap_used < 2000);
  REQUIRE(result->max_heap_used > 0);
  system("rm mstats_result.txt");
}
