//
// Created by nillyr on 02/12/16.
//

#ifndef PUYOPLAYER_GAME_H
#define PUYOPLAYER_GAME_H

#include "server.h"

#define NB_ROW 12
#define NB_COL 6

#define MAIN_PUYO   0
#define SECOND_PUYO 1

typedef enum{NONE, R, G, Y, B, X} kType;
typedef enum{UP, DOWN, LEFT, RIGHT, ROTL, ROTR} kAction;
typedef enum{lUP, lDOWN, lLEFT, lRIGHT} kLocation;

/*
 *  y
 *  ^
 *  |
 *   __ > x
 */
typedef struct {
  int x_position;
  int y_position;
} Position_t;

typedef struct {
  Position_t positions[2];
  kLocation location;
  kType puyos[2];
  int board[NB_ROW][NB_COL];
} State_t;

void init_state(State_t *state);
void parse_from_server(Server_t *server, State_t *state);

#endif //PUYOPLAYER_GAME_H
