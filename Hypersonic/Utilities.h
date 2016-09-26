#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <cmath>
#include <vector>
#include <queue>

#include "GameEntities.h"
 
static int computeDistance(const GameObject& obj1, const Pos& pos)
{
   return abs(obj1.m_coord.m_x - pos.m_x) + abs(obj1.m_coord.m_y - pos.m_y);
}

static int computeDistance(const GameObject& obj1, const GameObject& obj2)
{
   return abs(obj1.m_coord.m_x - obj2.m_coord.m_x) + abs(obj1.m_coord.m_y - obj2.m_coord.m_y);
}

static bool willBombAtPosBlowBox(const GameObject& bomb, const GameObject& box)
{
   return (computeDistance(bomb, box) < 3);
}

static bool isPositionValid(const Pos& pos)
{
   return pos.m_x >= 0 && pos.m_x < HEIGHT && pos.m_y >= 0 && pos.m_y < WIDTH;
}

static bool isPositionEmpty(const Floor& object)
{
   return object.m_type != TYPE_BOX && object.m_type != TYPE_BOMB && object.m_type != TYPE_WALL;
}

static int howManyBombsRemaining(const GameObject& me, const std::vector<GameObject>& bombs)
{
   int myBombsOnTheFieldCount = 0;
   for (const auto& bomb : bombs)
   {
      if (bomb.m_owner == OWNER_ME)
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
         if (bomb.m_owner == OWNER_ME)
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

static vector<Floor> getNeighbors(const Floor& current, const vector<vector<Floor>>& map)
{
   vector<Floor> res;

   Pos top = current.m_coord + Pos(1, 0);
   Pos bottom = current.m_coord + Pos(-1, 0);
   Pos left = current.m_coord + Pos(0, -1);
   Pos right = current.m_coord + Pos(0, 1);

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

static vector<Pos> findShortestPath(const GameObject& me, const Pos& destination, const vector<vector<Floor>>& map)
{
   priority_queue<Floor> frontier;
   frontier.push(Floor(TYPE_NONE, me.m_coord, 0));
   vector<vector<Pos*>> cameFrom(HEIGHT, vector<Pos*>(WIDTH, nullptr));
   vector<vector<int>> costSoFar(HEIGHT, vector<int>(WIDTH, -1));

   //cerr << "ME findShortest:" << me.m_coord << endl;
   costSoFar[me.m_coord.m_x][me.m_coord.m_y] = 0;

   while (!frontier.empty())
   {
      Floor current = frontier.top();
      frontier.pop();

      if (current.m_coord == destination) break;

      for (auto next : getNeighbors(current, map))
      {
         int newCost = costSoFar[current.m_coord.m_x][current.m_coord.m_y] + 1;//cost to next == 1
         if (costSoFar[next.m_coord.m_x][next.m_coord.m_y] == -1 || newCost < costSoFar[next.m_coord.m_x][next.m_coord.m_y])
         {
            costSoFar[next.m_coord.m_x][next.m_coord.m_y] = newCost;
            //int priority = newCost + 0;//heuristic cost between next and goal
            frontier.push(next);
            cameFrom[next.m_coord.m_x][next.m_coord.m_y] = new Pos(current.m_coord);
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
