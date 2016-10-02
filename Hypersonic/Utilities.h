#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <cmath>
#include <vector>
#include <queue>

#include "GameEntities.h"
 
static int computeDistance(const Pos& pos1, const Pos& pos2)
{
   return abs(pos1.m_x - pos2.m_x) + abs(pos1.m_y - pos2.m_y);
}

static int computeDistance(const GameObject& obj1, const Pos& pos)
{
   return computeDistance(obj1.m_coord, pos);
}

static int computeDistance(const GameObject& obj1, const GameObject& obj2)
{
   return computeDistance(obj1.m_coord, obj2.m_coord);
}

static bool willBombAtPosBlowBox(const GameObject& bomb, const GameObject& box)
{
   return (computeDistance(bomb, box) < 3);
}

static bool isPositionValid(const Pos& pos)
{
   return pos.m_x >= 0 && pos.m_x < HEIGHT && pos.m_y >= 0 && pos.m_y < WIDTH;
}

static bool isPositionEmpty(const Tile& tile)
{
   return tile.getType() != TYPE_BOX && tile.getType() != TYPE_BOMB && tile.getType() != TYPE_WALL;
}

static int howManyBombsRemaining(const GameObject& me, const std::vector<GameObject>& bombs)
{
   int myBombsOnTheFieldCount = 0;
   for (const auto& bomb : bombs)
   {
      if (bomb.m_ownerId == me.m_ownerId)
      {
         myBombsOnTheFieldCount++;
      }
   }
   return me.m_param1 - myBombsOnTheFieldCount;
}

static int updateTimerBeforeNextBomb(const GameObject& me, const std::vector<GameObject>& bombs)
{
   if (howManyBombsRemaining(me, bombs) != 0)
   {
      return 0;//no waiting, have bomb
   }
   else
   {
      int minTurnsBeforeExplosion = 8;
      for (const auto& bomb : bombs)
      {
         if (bomb.m_ownerId == me.m_ownerId)
         {
            int turnsBeforeExplosion = bomb.m_param1;
            if (turnsBeforeExplosion < minTurnsBeforeExplosion)
            {
               minTurnsBeforeExplosion = turnsBeforeExplosion;
            }
         }
      }
      return minTurnsBeforeExplosion;
   }
}

static vector<Tile> getNeighbors(const Tile& current, const vector<vector<Tile>>& map)
{
   vector<Tile> res;

   Pos top = current.getCoord() + Pos(1, 0);
   Pos bottom = current.getCoord() + Pos(-1, 0);
   Pos left = current.getCoord() + Pos(0, -1);
   Pos right = current.getCoord() + Pos(0, 1);

   if (isPositionValid(top) && isPositionEmpty(map[top.m_x][top.m_y]))
   {
      res.push_back(map[top.m_x][top.m_y]);
   }
   if (isPositionValid(bottom) && isPositionEmpty(map[bottom.m_x][bottom.m_y]))
   {
      res.push_back(map[bottom.m_x][bottom.m_y]);
   }
   if (isPositionValid(left) && isPositionEmpty(map[left.m_x][left.m_y]))
   {
      res.push_back(map[left.m_x][left.m_y]);
   }
   if (isPositionValid(right) && isPositionEmpty(map[right.m_x][right.m_y]))
   {
      res.push_back(map[right.m_x][right.m_y]);
   }

   return res;
}

static vector<Pos> findShortestPath(const GameObject& me, const Pos& destination, const vector<vector<Tile>>& map)
{
   if (me.m_coord == destination) return vector<Pos>(1, destination);
  
   priority_queue<Tile> frontier;
   frontier.push(Tile(me.m_coord, TYPE_NONE));
   vector<vector<Pos*>> cameFrom(HEIGHT, vector<Pos*>(WIDTH, nullptr));
   vector<vector<int>> costSoFar(HEIGHT, vector<int>(WIDTH, -1));

   //cerr << "ME findShortest:" << me.m_coord << endl;
   costSoFar[me.m_coord.m_x][me.m_coord.m_y] = 0;

   while (!frontier.empty())
   {
      Tile current = frontier.top();
      frontier.pop();

      if (current.getCoord() == destination) break;

      for (auto next : getNeighbors(current, map))
      {
         int newCost = costSoFar[current.getX()][current.getY()] + 1;//cost to next == 1
         if (costSoFar[next.getX()][next.getY()] == -1 || newCost < costSoFar[next.getX()][next.getY()])
         {
            costSoFar[next.getX()][next.getY()] = newCost;
            //int priority = newCost + 0;//heuristic cost between next and goal
            frontier.push(next);
            cameFrom[next.getX()][next.getY()] = new Pos(current.getCoord());
         }
      }

   }

   vector<Pos> res;
   Pos pos = destination;
   Pos* previousPos = cameFrom[destination.m_x][destination.m_y];
   
   if (previousPos == NULL) return vector<Pos>();

   res.push_back(pos);
   while (previousPos != NULL)
   {
      pos = *previousPos;
      previousPos = cameFrom[pos.m_x][pos.m_y];
      res.push_back(pos);
   }

   for (const auto& row : cameFrom)
   {
      for (Pos* ptr : row)
      {
         delete ptr;
      }

   }
   return res;
}

#endif
