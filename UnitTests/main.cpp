#include <cassert>
#include <iostream>
#include "Utilities.h"
#include "GameLogic.h"
#include "ReaderWriter.h"

void testWrapperExecutor(void (*testFunction)())
{
   cout << "-----";
   cout << "Beginning test " << endl;
   testFunction();
   cout << "-----" << endl;
}

static void testComputeDistance()
{
   GameObject obj1;
   GameObject obj2;

   obj1.m_coord = Pos(0, 0);
   obj2.m_coord = Pos(5, 0);
   assert(computeDistance(obj1, obj2) == 5);

   obj1.m_coord = Pos(3, 0);
   obj2.m_coord = Pos(5, 8);
   assert(computeDistance(obj1, obj2) == 10);
}

static void testWillBombAtPosBlowBox()
{
   GameObject bomb;
   GameObject box;

   bomb.m_coord = Pos(0, 0);
   box.m_coord = Pos(5,0);
   assert(willBombAtPosBlowBox(bomb, box) == false);

   bomb.m_coord = Pos(0, 2);
   box.m_coord = Pos(2, 2);
   assert(willBombAtPosBlowBox(bomb, box) == true);
}

static void testIsPositionValid()
{
   assert(isPositionValid(Pos(-1, 0)) == false);
   assert(isPositionValid(Pos(0, -1)) == false);
   assert(isPositionValid(Pos(HEIGHT, 0)) == false);
   assert(isPositionValid(Pos(0, WIDTH)) == false);

   assert(isPositionValid(Pos(0, 0)) == true);
   assert(isPositionValid(Pos(HEIGHT - 1, 0)) == true);
   assert(isPositionValid(Pos(0, WIDTH - 1)) == true);
}

static void testFillBombTilesScoreMap()
{
   vector<vector<int>> bombTileScoreMap(HEIGHT, vector<int>(WIDTH));
   vector<vector<Floor>> map(HEIGHT, vector<Floor>(WIDTH));

   vector<GameObject> boxes;
   GameObject obj;
   obj.m_entityType = TYPE_BOX;
     
   obj.m_coord = Pos(0, 0);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);
   obj.m_coord = Pos(2, 2);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   fillBombTilesScoreMap(boxes, bombTileScoreMap, map);
   write(bombTileScoreMap);
   assert(bombTileScoreMap[0][0] == 0);
   assert(bombTileScoreMap[1][0] == 1);
   assert(bombTileScoreMap[2][0] == 2);
   assert(bombTileScoreMap[3][0] == 0);

   assert(bombTileScoreMap[0][1] == 1);
   assert(bombTileScoreMap[1][1] == 0);
   assert(bombTileScoreMap[2][1] == 1);
   assert(bombTileScoreMap[3][1] == 0);

   assert(bombTileScoreMap[0][2] == 2);
   assert(bombTileScoreMap[1][2] == 1);
   assert(bombTileScoreMap[2][2] == 0);
   assert(bombTileScoreMap[3][2] == 1);
   assert(bombTileScoreMap[4][2] == 1);
   assert(bombTileScoreMap[5][2] == 0);

   assert(bombTileScoreMap[0][3] == 0);
   assert(bombTileScoreMap[1][3] == 0);
   assert(bombTileScoreMap[2][3] == 1);
   assert(bombTileScoreMap[3][3] == 0);

   assert(bombTileScoreMap[0][4] == 0);
   assert(bombTileScoreMap[1][4] == 0);
   assert(bombTileScoreMap[2][4] == 1);
   assert(bombTileScoreMap[3][4] == 0);

   obj.m_entityType = TYPE_WALL;
   obj.m_coord = Pos(1, 2);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);


   cerr << endl;
   bombTileScoreMap.clear();
   bombTileScoreMap = vector<vector<int>>(HEIGHT, vector<int>(WIDTH));
   fillBombTilesScoreMap(boxes, bombTileScoreMap, map);
   write(bombTileScoreMap);


   assert(bombTileScoreMap[0][0] == 0);
   assert(bombTileScoreMap[1][0] == 1);
   assert(bombTileScoreMap[2][0] == 2);
   assert(bombTileScoreMap[3][0] == 0);

   assert(bombTileScoreMap[0][1] == 1);
   assert(bombTileScoreMap[1][1] == 0);
   assert(bombTileScoreMap[2][1] == 1);
   assert(bombTileScoreMap[3][1] == 0);

   assert(bombTileScoreMap[0][2] == 1);
   assert(bombTileScoreMap[1][2] == 0);
   assert(bombTileScoreMap[2][2] == 0);
   assert(bombTileScoreMap[3][2] == 1);
   assert(bombTileScoreMap[4][2] == 1);
   assert(bombTileScoreMap[5][2] == 0);

   assert(bombTileScoreMap[0][3] == 0);
   assert(bombTileScoreMap[1][3] == 0);
   assert(bombTileScoreMap[2][3] == 1);
   assert(bombTileScoreMap[3][3] == 0);

   assert(bombTileScoreMap[0][4] == 0);
   assert(bombTileScoreMap[1][4] == 0);
   assert(bombTileScoreMap[2][4] == 1);
   assert(bombTileScoreMap[3][4] == 0);

   cerr << endl;
   map = vector<vector<Floor>>(HEIGHT, vector<Floor>(WIDTH));
   bombTileScoreMap = vector<vector<int>>(HEIGHT, vector<int>(WIDTH));
   boxes = vector<GameObject>();
   vector<GameObject> walls;

   //BOXES
   obj.m_entityType = TYPE_BOX;
   obj.m_coord = Pos(0, 3);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(1, 2);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(2, 1);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(2, 2);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(3, 2);
   boxes.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   //WALL
   obj.m_entityType = TYPE_WALL;
   obj.m_coord = Pos(1, 1);
   walls.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(1, 3);
   walls.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(3, 1);
   walls.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   obj.m_coord = Pos(3, 3);
   walls.push_back(obj);
   map[obj.m_coord.m_x][obj.m_coord.m_y] = Floor(obj.m_entityType, obj.m_coord, obj.m_turnsBeforeDestruction);

   fillBombTilesScoreMap(boxes, bombTileScoreMap, map);
   write(bombTileScoreMap);

   assert(bombTileScoreMap[0][0] == 0);
   assert(bombTileScoreMap[0][1] == 1);
   assert(bombTileScoreMap[0][2] == 2);
   assert(bombTileScoreMap[0][3] == 0);

   assert(bombTileScoreMap[1][0] == 0);
   assert(bombTileScoreMap[1][1] == 0);
   assert(bombTileScoreMap[1][2] == 0);
   assert(bombTileScoreMap[1][3] == 0);

   assert(bombTileScoreMap[2][0] == 1);
   assert(bombTileScoreMap[2][1] == 0);
   assert(bombTileScoreMap[2][2] == 0);
   assert(bombTileScoreMap[2][3] == 1);

   assert(bombTileScoreMap[3][0] == 0);
   assert(bombTileScoreMap[3][1] == 0);
   assert(bombTileScoreMap[3][2] == 0);
   assert(bombTileScoreMap[3][3] == 0);

}

static void testFindShortestPath()
{
   GameObject me(TYPE_PLAYER, OWNER_ME, Pos(0, 0), 0, 0);
   vector<vector<Floor>> map(HEIGHT, vector<Floor>(WIDTH));
   for (size_t i = 0; i < HEIGHT; ++i)
   {
      for (size_t j = 0; j < WIDTH; ++j)
      {
         map[i][j] = Floor(TYPE_NONE, Pos(i,j), 0);
      }
   }
   map[0][1].m_type = TYPE_WALL;
   map[1][1].m_type = TYPE_WALL;
   map[2][1].m_type = TYPE_WALL;
   map[3][1].m_type = TYPE_WALL;
  
   Pos destination = Pos(0,2);
   vector<Pos> res = findShortestPath(me, destination, map);
   write(res);
   assert(res[0] == destination);

   map[3][0].m_type = TYPE_WALL;
   res = findShortestPath(me, destination, map);
   assert(res.empty());

   res = findShortestPath(me, me.m_coord, map);
   assert(res.size() == 1);
}

int main()
{
   testWrapperExecutor(testComputeDistance);
   testWrapperExecutor(testWillBombAtPosBlowBox);
   testWrapperExecutor(testIsPositionValid);
   testWrapperExecutor(testFillBombTilesScoreMap);
   testWrapperExecutor(testFindShortestPath);
   return 0;
}