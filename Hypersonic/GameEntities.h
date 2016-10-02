#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#include "Pos.h"
#include <map>
#include <vector>

#define WIDTH 13
#define HEIGHT 11

using namespace std;

enum Type
{
   TYPE_NONE,
   TYPE_BOX,
   TYPE_BOMB,
   TYPE_OBJECT,
   TYPE_WALL,
   TYPE_PLAYER
};

enum Action
{
   ACTION_MOVE,
   ACTION_BOMB
};

struct Choice
{
   Choice(Action action, const Pos& coord)
      : m_action(action)
      , m_coord(coord)
   {}

   Action m_action{ACTION_MOVE};
   Pos m_coord{};
};

ostream& operator<<(ostream& os, const Choice& choice)
{
   if (choice.m_action == ACTION_BOMB)
      os << "BOMB ";
   else if (choice.m_action == ACTION_MOVE)
      os << "MOVE ";

   os << choice.m_coord;

   return os;
}

struct GameObject
{
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
   {}
   GameObject() = default;
   ~GameObject() = default;

   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
};

static bool operator<(const GameObject& obj1, const GameObject& obj2)
{
   if (obj1.m_coord.m_x != obj2.m_coord.m_x)
      return obj1.m_coord.m_x < obj2.m_coord.m_x;

   if (obj1.m_coord.m_y != obj2.m_coord.m_y)
      return obj1.m_coord.m_y < obj2.m_coord.m_y;

   if (obj1.m_entityType != obj2.m_entityType)
      return obj1.m_entityType < obj2.m_entityType;

   if (obj1.m_ownerId != obj2.m_ownerId)
      return obj1.m_ownerId < obj2.m_ownerId;

   return false;
}

static bool operator==(const GameObject& obj1, const GameObject& obj2)
{
   return !(obj1 < obj2) && !(obj2 < obj1);
}

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << endl;
   return os;
}

struct Tile
{
   Tile() = default;
   Tile(const Pos& coord, Type type)
      : m_object(type, -1, coord, 0, 0)
   {}

   explicit Tile(const GameObject& obj)
     : m_object(obj)
   {}

   Type getType() const
   {
      return m_object.m_entityType;
   }

   const Pos& getCoord() const
   {
      return m_object.m_coord;
   }

   int getX() const
   {
      return m_object.m_coord.m_x;
   }

   int getY() const
   {
      return m_object.m_coord.m_y;
   }

   int m_turnsBeforeExplosion{ -1 };
   GameObject m_object{};
};

static bool operator<(const Tile& tile1, const Tile& tile2)
{
   return tile1.m_object < tile2.m_object;
}

static ostream& operator<<(ostream& os, const Tile& tile)
{
   os << "OBJECT: " << endl;
   os << tile.m_object.m_entityType << endl;
   os << endl;
   return os;
}

struct Board
{
   Board()
      : m_map(HEIGHT, vector<Tile>(WIDTH))
   {}

   vector<vector<Tile>> m_map;
   vector<GameObject> m_bombs;
   vector<GameObject> m_boxes;
   vector<GameObject> m_objects;
   vector<GameObject> m_walls;
   map<int, GameObject> m_players;
   GameObject m_me;
};

struct Node
{
   Node() = default;

   Node(Node* previous, const Choice& choice, const Board& newBoard)
      : m_previous(previous)
      , m_choice(choice)
      , m_board(newBoard)
   {}

   Node* m_previous{ nullptr };
   Choice m_choice{ACTION_MOVE, Pos()};
   Board m_board;
   int m_score{ 0 };
};

#endif
