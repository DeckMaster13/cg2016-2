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
      //cerr << endl << turnInput << endl; 
      m_map = turnInput.m_map;
      m_bombs = turnInput.m_bombs;
      m_boxes = turnInput.m_boxes;
      m_objects = turnInput.m_objects;
      //cerr << "MEEEEE :" << turnInput.m_me.m_coord;
      //cerr << endl;
      m_me = turnInput.m_me;
      m_players = turnInput.m_players;

      m_bombTileScoresMap = vector<vector<int>>(HEIGHT, vector<int>(WIDTH));

      updateTurnsBeforeDestruction(m_bombs, m_players, m_map);
      m_timerBeforeNextBomb = updateTimerBeforeNextBomb(m_me, m_bombs);
      fillBombTilesScoreMap(m_boxes, m_me, m_bombTileScoresMap, m_map);
      //write(m_bombTileScoresMap);
      //write(m_map);
      //DEBUG
      /*for (const auto& row : m_map)
      {
         for (const auto& obj : row)
         {
            cerr << obj;
         }
      }*/

   }

public:
   vector<vector<Floor>> m_map;
   vector<vector<int>> m_bombTileScoresMap;
   vector<GameObject> m_bombs;
   vector<GameObject> m_boxes;
   vector<GameObject> m_objects;
   vector<GameObject> m_walls;
   GameObject m_me;
   int m_timerBeforeNextBomb;
   Pos m_objective{Pos(0,0)};
   Type m_objectiveType{TYPE_OBJECT};
   vector<Pos> m_objectiveShortestPath;
   map<int, GameObject> m_players;
};

#endif
