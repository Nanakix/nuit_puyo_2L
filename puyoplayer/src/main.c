//
// Created by nillyr on 01/12/16.
//

#include "server.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

static noreturn void usage() {
  fprintf(stderr,
          "puyoplayer: Usage:\n\tpuyoplayer solo <player_name>\n"
          "\tpuyoplayer vs <game_name> <player_name>\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc < 3)
    usage();

  // Check arguments
  bool solo = !strcmp(argv[1], "solo");
  bool vs   = !strcmp(argv[1], "vs");

  if (!solo && !vs) {
    usage();
  }
  if (solo) {
    // Curl -H new game
  }
  if (vs) {
    // Curl -H new vs game
  }

  Server_t server;
  if (create_new_game(&server, "FAIL3") < 0)
    goto end;

  send_move(&server);
  printf("%s\n", server.mem_chunk.memory);

  end:
  // Free mem
  curl_easy_cleanup(curl);
  free(server.mem_chunk.memory);
  free(server.name);
  free(server.url);
  return 0;
}
