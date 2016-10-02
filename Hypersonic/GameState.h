#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <map>
#include <vector>

#include "GameEntities.h"
#include "GameLogic.h"
#include "ReaderWriter.h"

using namespace std;

class GameState
{
public:
   GameState(const GlobalInput&)
   {}
   ~GameState() = default;

   void updateOnNewTurn(const TurnInput& turnInput)
   {
      m_board.m_map = turnInput.m_map;
      m_board.m_bombs = turnInput.m_bombs;
      m_board.m_boxes = turnInput.m_boxes;
      m_board.m_objects = turnInput.m_objects;
      m_board.m_me = turnInput.m_me;
      m_board.m_players = turnInput.m_players;
   }

public:
   Board m_board;
   vector<vector<Node*>> m_graph;
};

#endif
