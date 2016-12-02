//
// Created by nillyr on 01/12/16.
//

#include "server.h"

#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <zconf.h>


static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

static noreturn void print_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

static void set_request(Server_t *server, char *action, char *pf_move) {
  if (!strcmp(ACTION_NEW, action)) {
    size_t length = strlen(server->url) + strlen(action) + strlen(GAME_NAME) +
                    strlen(server->name);
    server->request = malloc(sizeof(char) * length);
    if (!server->request)
      print_error("Error: Request failed\n");

    sprintf(server->request, "%s%s%s%s", server->url, action, GAME_NAME,
            server->name);

  } else if (!strcmp(ACTION_TURN, action)) {
    size_t length = strlen(server->url) + strlen(action) + strlen(GAME_NAME) +
                    strlen(server->name) + strlen(GAME_MOVE) + strlen(pf_move);
    server->request = malloc(sizeof(char) * length);
    if (!server->request)
      print_error("Error: Request failed\n");

    sprintf(server->request, "%s%s%s%s%s%s", server->url, action, GAME_NAME,
            server->name, GAME_MOVE, pf_move);
  }
  DEBUGPRINT("Request: \"%s\"\n", server->request);
}

int send_request(Server_t *server) {
  curl_easy_setopt(curl, CURLOPT_URL, server->request);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&(server->mem_chunk));


  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    fprintf(stderr, "Request Failed: %s\n", curl_easy_strerror(res));
    return -1;
  }

  curl_easy_cleanup(curl);
  return 0;
}

static void init(Server_t *server, char *name) {
  // Set the Server URL
  server->url = malloc(sizeof(char) * strlen(URL));
  if (!server->url)
    print_error("ERROR: server initialization failed\n");

  memcpy(server->url, URL, strlen(URL));
  DEBUGPRINT("Server url: \"%s\"\n", server->url);

  // Set the Game name
  server->name = malloc(sizeof(char) * strlen(name));
  if (!server->name)
    print_error("ERROR: server initialization failed\n");

  sprintf(server->name, "%s", name);
  DEBUGPRINT("Game name: \"%s\"\n", server->name);

  server->chunk = NULL;

  server->mem_chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  server->mem_chunk.size = 0;    /* no data at this point */

  curl = curl_easy_init();
  if (curl) {
    server->chunk = curl_slist_append(server->chunk, "Accept: text/plain");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, server->chunk);
  }
}

int create_new_game(Server_t *server, char *name) {
  if (!name)
    print_error("ERROR: Something went wrong with your game's name\n");

  init(server, name);
  set_request(server, ACTION_NEW, name);
  send_request(server);
  return 0;
}


