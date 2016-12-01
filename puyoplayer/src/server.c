//
// Created by nillyr on 01/12/16.
//

#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_server(Server_t *server) {
  server->url = malloc(sizeof(char) * strlen(URL));
  memcpy(server->url, URL, strlen(URL));
  DEBUGPRINT("Server url: %s\n", server->url);
}

int create_new_game(Server_t *server, char *name) {
  if (!name) {
    fprintf(stderr, "ERROR: Something went wrong with your game's name\n");
    return -1;
  }

  server->name = malloc(sizeof(char) * strlen(name));
  memcpy(server->name, name, strlen(name));
  DEBUGPRINT("Server name: %s\n", server->name);

  return 0;
}
