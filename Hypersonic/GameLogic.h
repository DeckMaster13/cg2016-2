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

static void computeBombTileScore(const Pos& boxCoord, int rangeDeltaX, int rangeDeltaY, vector<vector<int>>& bombTileScoreMap, const vector<vector<Tile>>& map)
{
   Pos delta(0, 0);
   int i = 0;
   while (i <= abs(rangeDeltaX) + abs(rangeDeltaY))
   {
      ++i;
      Pos newCoord = boxCoord + delta;
      if (isPositionValid(newCoord) && newCoord != boxCoord)
      {
         const Tile& newTile = map[newCoord.m_x][newCoord.m_y];
         if (newTile.getType() == TYPE_BOX || newTile.getType() == TYPE_BOMB || newTile.getType() == TYPE_WALL)
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

static void fillBombTilesScoreMap(const vector<GameObject>& boxes, const GameObject& me, vector<vector<int>>& bombTileScoreMap, const vector<vector<Tile>>& map)
{
   for (const auto& box : boxes)
   {
      computeBombTileScore(box.m_coord, me.m_param2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, -me.m_param2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, me.m_param2, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, -me.m_param2, bombTileScoreMap, map);
   }
}

static void updateTurnBeforeDestructionOnALine(const GameObject& bomb, int rangeDeltaX, int rangeDeltaY, vector<vector<Tile>>& map)
{
   Pos delta(-rangeDeltaX, -rangeDeltaY);
   int i = 0;
   while (i <= 2 * rangeDeltaX + 2 * rangeDeltaY)
   {
      Pos newCoord = bomb.m_coord + delta;
      if (isPositionValid(newCoord) && newCoord != bomb.m_coord)
      {
         assert(bomb.m_param1 >= 0);
         map[newCoord.m_x][newCoord.m_y].m_turnsBeforeExplosion = bomb.m_param1;
      }
      delta += Pos(rangeDeltaX != 0, rangeDeltaY != 0);
      ++i;
   }

}

static void updateTurnsBeforeDestruction(const vector<GameObject>& bombs, map<int, GameObject>& players, vector<vector<Tile>>& map)
{
   for (const auto& bomb : bombs)
   {
      GameObject player = players[bomb.m_ownerId];
      updateTurnBeforeDestructionOnALine(bomb, player.m_param2, 0, map);
      updateTurnBeforeDestructionOnALine(bomb, 0, player.m_param2, map);
   }
}

static Node* applyChoice(Node* previousNode, Choice choice)
{
   assert(previousNode != nullptr);
   Board newBoard(previousNode->m_board);
   GameObject& me = newBoard.m_me;
   //PLACE BOMB
   if (choice.m_action == ACTION_BOMB)
   {
      GameObject bomb(TYPE_BOMB, me.m_ownerId, me.m_coord, 8, me.m_param2);
      newBoard.m_bombs.push_back(bomb);
      newBoard.m_map[choice.m_coord.m_x][choice.m_coord.m_y] = Tile(bomb);
      me.m_param1--;
   }

   //MOVE ME
   me.m_coord = choice.m_coord;

   //BOMB EXPLOSIONS
   for (auto& bomb : newBoard.m_bombs)
   {
      bomb.m_param1--;
   }
   updateTurnsBeforeDestruction(newBoard.m_bombs, newBoard.m_players, newBoard.m_map);

   for (auto& row : newBoard.m_map)
      for (auto& tile : row)
         if (tile.m_turnsBeforeExplosion == 0)
         {
            tile.m_turnsBeforeExplosion = -1;
            switch (tile.m_object.m_entityType)
            {
            case TYPE_BOMB:
               newBoard.m_bombs.erase(std::remove(newBoard.m_bombs.begin(), newBoard.m_bombs.end(), tile.m_object), newBoard.m_bombs.end());
               break;
            case TYPE_BOX:
               newBoard.m_boxes.erase(std::remove(newBoard.m_boxes.begin(), newBoard.m_boxes.end(), tile.m_object), newBoard.m_boxes.end());
               break;
            case TYPE_OBJECT:
               newBoard.m_objects.erase(std::remove(newBoard.m_objects.begin(), newBoard.m_objects.end(), tile.m_object), newBoard.m_objects.end());
               break;
            default:
               break;
            }
         }


   return new Node(previousNode, choice, newBoard);
}

#endif
