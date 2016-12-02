//
// Created by nillyr on 01/12/16.
//

#ifndef PUYOPLAYER_SERVER_H
#define PUYOPLAYER_SERVER_H

#include <curl/curl.h>

#ifdef DEBUG
#define DEBUG_VAL 1
#else
#define DEBUG_VAL 0
#endif // DEBUG

#define DEBUGPRINT(fmt, ...) \
do { if (DEBUG_VAL) fprintf(stderr, fmt, ##__VA_ARGS__); } while(0)

#define URL "https://puyopuyo.vermeille.fr/"

#define GAME_NAME "name="
#define GAME_MOVE "&move="

#define ACTION_NEW "new?"
#define ACTION_TURN "turn?"

CURL *curl;
CURLcode res;

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct {
  char *url;
  char *name;
  char *request;
  struct curl_slist *chunk;
  struct MemoryStruct mem_chunk;
} Server_t;

int create_new_game(Server_t *server, char *name);
int send_move(Server_t *server, char *action);

#endif //PUYOPLAYER_SERVER_H
