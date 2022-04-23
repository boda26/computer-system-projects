#pragma once

#ifdef __cplusplus
extern "C" {
#endif

const char *emoji_favorite();
int emoji_count(char *utf8str);
void emoji_invertChar(char *utf8str);
void emoji_invertAll(char *utf8str);
char *emoji_random_alloc();      

#ifdef __cplusplus
}
#endif