#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>

#include "lib/catch.hpp"
#include "../degree.h"
#include "../lib/wallet.h"

// Test 1
TEST_CASE("Test Degree Program - Successfully Completes","[weight=10][part=1]") {
  // Run multiple iterations of degree.
  for(int it=0; it<2; it++) {
    wallet_t wallet;
    wallet_init(&wallet);
    srand (time(NULL));

    // Require initial zero.
    REQUIRE(wallet_get(&wallet,"degree!") == 0);
    pthread_t tids[10];
    pthread_create(&tids[0], NULL, job_graduation, (void *) &wallet);
    pthread_create(&tids[1], NULL, job_combine_research, (void *)&wallet);
    pthread_create(&tids[2], NULL, job_research_blue, (void *)&wallet);
    pthread_create(&tids[3], NULL, job_research_orange, (void *)&wallet);
    pthread_create(&tids[4], NULL, job_research_green, (void *)&wallet);
    pthread_create(&tids[5], NULL, job_dna, (void *)&wallet);
    pthread_create(&tids[6], NULL, job_workshop, (void *)&wallet);
    pthread_create(&tids[7], NULL, job_clover_patch, (void *)&wallet);
    pthread_create(&tids[8], NULL, job_orchard, (void *)&wallet);
    const int totalJobs = 9;
    for (int i = 0; i < totalJobs; i++) {
      pthread_join(tids[i], NULL);
    }
    REQUIRE(wallet_get(&wallet,"degree!") == 1);
    wallet_destroy(&wallet); 
  }
  return 0;
}