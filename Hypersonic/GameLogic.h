#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "GameEntities.h"
#include "Pos.h"
#include "Utilities.h"
#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

static void computeBombTileScore(const Pos& boxCoord, int rangeDeltaX, int rangeDeltaY, vector<vector<int>>& bombTileScoreMap, const vector<vector<Floor>>& map)
{
   Pos delta(0, 0);
   int i = 0;
   while (i <= abs(rangeDeltaX) + abs(rangeDeltaY))
   {
      ++i;
      Pos newCoord = boxCoord + delta;
      if (isPositionValid(newCoord) && newCoord != boxCoord)
      {
         const Floor& newFloor = map[newCoord.m_x][newCoord.m_y];
         if (newFloor.m_type == TYPE_BOX || newFloor.m_type == TYPE_BOMB || newFloor.m_type == TYPE_WALL)
         {
            //EXPLOSION STOPPED
            break;
         }
         bombTileScoreMap[newCoord.m_x][newCoord.m_y] += 1;
      }
      
      int deltaDeltaX = 0;
      if (rangeDeltaX > 0)
         deltaDeltaX = 1;
      else if (rangeDeltaX < 0)
         deltaDeltaX = -1;

      int deltaDeltaY = 0;
      if (rangeDeltaY > 0)
         deltaDeltaY = 1;
      else if (rangeDeltaY < 0)
         deltaDeltaY = -1;

      delta += Pos(deltaDeltaX, deltaDeltaY);
   }

}

static void fillBombTilesScoreMap(const vector<GameObject>& boxes, const GameObject& me, vector<vector<int>>& bombTileScoreMap, const vector<vector<Floor>>& map)
{
   for (const auto& box : boxes)
   {
      computeBombTileScore(box.m_coord, me.m_param2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, -me.m_param2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, me.m_param2, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, -me.m_param2, bombTileScoreMap, map);
   }
}

static void updateTurnBeforeDestructionOnALine(const GameObject& bomb, int rangeDeltaX, int rangeDeltaY, vector<vector<Floor>>& map)
{
   Pos delta(-rangeDeltaX, -rangeDeltaY);
   int i = 0;
   while (i <= 2 * rangeDeltaX + 2 * rangeDeltaY)
   {
      Pos newCoord = bomb.m_coord + delta;
      if (isPositionValid(newCoord) && newCoord != bomb.m_coord)
      {
         assert(bomb.m_param1 >= 0);
         map[newCoord.m_x][newCoord.m_y].m_turnsBeforeDestruction = bomb.m_param1;
      }
      delta += Pos(rangeDeltaX != 0, rangeDeltaY != 0);
      ++i;
   }

}

static void updateTurnsBeforeDestruction(const vector<GameObject>& bombs, map<int, GameObject>& players, vector<vector<Floor>>& map)
{
   for (const auto& bomb : bombs)
   {
      GameObject player = players[bomb.m_ownerId];
      updateTurnBeforeDestructionOnALine(bomb, player.m_param2, 0, map);
      updateTurnBeforeDestructionOnALine(bomb, 0, player.m_param2, map);
   }
}

#endif
