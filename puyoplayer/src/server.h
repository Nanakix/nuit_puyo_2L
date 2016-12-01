//
// Created by nillyr on 01/12/16.
//

#ifndef PUYOPLAYER_SERVER_H
#define PUYOPLAYER_SERVER_H

#ifdef DEBUG
#define DEBUG_VAL 1
#else
#define DEBUG_VAL 0
#endif // DEBUG

#define DEBUGPRINT(fmt, ...) \
do { if (DEBUG_VAL) fprintf(stderr, fmt, __VA_ARGS__); } while(0)

#define URL "https://puyopuyo.vermeille.fr/"

typedef struct {
  char *url;
  char *name;
} Server_t;

void init_server(Server_t *server);
int create_new_game(Server_t *server, char *name);

#endif //PUYOPLAYER_SERVER_H
