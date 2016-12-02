//
// Created by nillyr on 01/12/16.
//

#include "server.h"

#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <unistd.h>
#include <zconf.h>

static noreturn void exit_on_fail(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t real_size = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + real_size + 1);
  if(!mem->memory) {
    /* out of memory! */
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, real_size);
  mem->size += real_size;
  mem->memory[mem->size] = 0;

  return real_size;
}

static void set_request(Server_t *server, char *action, char *pf_move) {
  if (!strcmp(ACTION_NEW, action)) {
    size_t length = strlen(server->url) + strlen(action) + strlen(GAME_NAME) +
                    strlen(server->name);
    server->request = malloc(sizeof(char) * length);
    if (!server->request)
      exit_on_fail("Error: %s Request failed\n", __func__);

    sprintf(server->request, "%s%s%s%s", server->url, action, GAME_NAME,
            server->name);

  } else if (!strcmp(ACTION_TURN, action)) {
    size_t length = strlen(server->url) + strlen(action) + strlen(GAME_NAME) +
                    strlen(server->name) + strlen(GAME_MOVE) + strlen(pf_move);
    server->request = malloc(sizeof(char) * length);
    if (!server->request)
      exit_on_fail("Error: %s Request failed\n", __func__);

    sprintf(server->request, "%s%s%s%s%s%s", server->url, action, GAME_NAME,
            server->name, GAME_MOVE, pf_move);
  }
  DEBUGPRINT("DEBUG: Request: \"%s\"\n", server->request);
}

static int send_request(Server_t *server) {
  DEBUGPRINT("DEBUG: %s \"%s\"\n", __func__, server->request);
  curl_easy_setopt(curl, CURLOPT_URL, server->request);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&(server->mem_chunk));

  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    fprintf(stderr, "ERROR: %s: %s\n", __func__, curl_easy_strerror(res));
    return -1;
  }

  // anti-spam
  sleep(1);
  return 0;
}

static void init(Server_t *server, char *name) {
  // Set the Server URL
  server->url = malloc(sizeof(char) * strlen(URL));
  if (!server->url)
    exit_on_fail("ERROR: %s server initialization failed\n", __func__);

  memcpy(server->url, URL, strlen(URL));
  DEBUGPRINT("DEBUG: Server url: \"%s\"\n", server->url);

  // Set the Game name
  server->name = malloc(sizeof(char) * strlen(name));
  if (!server->name)
    exit_on_fail("ERROR: %s server initialization failed\n", __func__);

  sprintf(server->name, "%s", name);
  DEBUGPRINT("DEBUG: Game name: \"%s\"\n", server->name);

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
  if (!name) {
    fprintf(stderr, "ERROR: %s Something went wrong with your game's name\n", __func__);
    return -1;
  }

  init(server, name);
  set_request(server, ACTION_NEW, name);
  send_request(server);
  return 0;
}

int send_move(Server_t *server) {
  set_request(server, ACTION_TURN, "DOWN");
  send_request(server);
  return 0;
}