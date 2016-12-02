//
// Created by nillyr on 02/12/16.
//

#include "game.h"

#include <stdlib.h>
#include <string.h>

#define FOR(i, n) for (int i = 0; i < (int)n; i++)
#define FORD(i, n) for (int i = (int)n; i > 0; i--)

void init_state(State_t *state) {
  state->positions[MAIN_PUYO].x_position = 3;
  state->positions[MAIN_PUYO].y_position = 10;

  state->location = lUP;

  FOR(i, NB_ROW)
    FOR(j, NB_COL)
        state->board[i][j] = NONE;
}

void print_state(State_t *state) {
  fprintf(stdout, "%d %d %d\n", state->positions[MAIN_PUYO].x_position,
                                state->positions[MAIN_PUYO].y_position,
                                state->location);

  fprintf(stdout, "%d %d\n\n", state->puyos[MAIN_PUYO], state->puyos[SECOND_PUYO]);
  FORD(i, NB_ROW) {
    fprintf(stdout, "%2d ", i);
    FOR(j, NB_COL) {
      switch (state->board[i][j]) {
        case R:
          fprintf(stdout, "R ");
          break;
        case G:
          fprintf(stdout, "G ");
          break;
        case Y:
          fprintf(stdout, "Y ");
          break;
        case B:
          fprintf(stdout, "B ");
          break;
        case X:
          fprintf(stdout, "X ");
          break;
        case NONE:
          // fall through
        default:
          fprintf(stdout, "_ ");
          break;
      }
    }
    fprintf(stdout, "\n");
  }
}

static void add_puyos(State_t *state) {
  switch (state->location) {
    case lUP:
      state->positions[SECOND_PUYO].x_position =
          state->positions[MAIN_PUYO].x_position;
      state->positions[SECOND_PUYO].y_position =
          state->positions[MAIN_PUYO].y_position + 1;
      break;
    case lDOWN:
      state->positions[SECOND_PUYO].x_position =
          state->positions[MAIN_PUYO].x_position;
      state->positions[SECOND_PUYO].y_position =
          state->positions[MAIN_PUYO].y_position - 1;
      break;
    case lLEFT:
      state->positions[SECOND_PUYO].x_position =
          state->positions[MAIN_PUYO].x_position - 1;
      state->positions[SECOND_PUYO].y_position =
          state->positions[MAIN_PUYO].y_position;
      break;
    case lRIGHT:
      state->positions[SECOND_PUYO].x_position =
          state->positions[MAIN_PUYO].x_position + 1;
      state->positions[SECOND_PUYO].y_position =
          state->positions[MAIN_PUYO].y_position;
      break;
  }
  state->board
  [state->positions[MAIN_PUYO].y_position][state->positions[MAIN_PUYO].x_position] =
      state->puyos[MAIN_PUYO];

  state->board
  [state->positions[SECOND_PUYO].y_position][state->positions[SECOND_PUYO].x_position] =
      state->puyos[SECOND_PUYO];
}

void parse_from_server(char *str, State_t *state) {
  char *token;
  const char delim[2] = "\n";
  token = strtok(str, delim);
  if (!token)
    return;

  // Init puyo's positions and rotation
  state->positions[MAIN_PUYO].x_position = token[0] - '0';
  state->positions[MAIN_PUYO].y_position =
      (((token[3] - '0') == 0) ? 10 : (token[2] - '0'));

  size_t remaining = (state->positions[MAIN_PUYO].y_position >= 10) ? 5 : 4;

  char rotation[5];
  memcpy(rotation, token + remaining, strlen(token) - remaining);

  if (!strcmp("UP", rotation))
    state->location = lUP;
  else if (!strcmp("DOWN", rotation))
    state->location = lDOWN;
  else if (!strcmp("LEFT", rotation))
    state->location = lLEFT;
  else
    state->location = lRIGHT;

  // Init puyo's type
  int puyo = MAIN_PUYO;
  token = strtok(NULL, delim);
  FOR(i, strlen(token)) {
    switch (token[i]) {
      case 'R':
        state->puyos[puyo++] = R;
        break;
      case 'G':
        state->puyos[puyo++] = G;
        break;
      case 'Y':
        state->puyos[puyo++] = Y;
        break;
      case 'B':
        state->puyos[puyo++] = B;
        break;
      default:
        break;
    }
  }

  // Init board
  int nb_row = 0;
  token = strtok(NULL, delim);
  while (token) {
    FOR(col, strlen(token)) {
      switch (token[col]) {
        case 'R':
          state->board[nb_row][col] = R;
          break;
        case 'G':
          state->board[nb_row][col] = G;
          break;
        case 'Y':
          state->board[nb_row][col] = Y;
          break;
        case 'B':
          state->board[nb_row][col] = B;
          break;
        case 'X':
          state->board[nb_row][col] = X;
          break;
        default:
          state->board[nb_row][col] = NONE;
          break;
      }
    }
    token = strtok(NULL, delim);
    nb_row++;
  }
  // Set puyos in the board
  add_puyos(state);
  print_state(state);
}