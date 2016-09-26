#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#include "Pos.h"

#define WIDTH 13
#define HEIGHT 11

using namespace std;

enum Type
{
   TYPE_NONE,
   TYPE_BOX,
   TYPE_BOMB,
   TYPE_PLAYER,
   TYPE_OBJECT,
   TYPE_WALL
};

enum Owner
{
   OWNER_NONE,
   OWNER_ME,
   OWNER_HIM
};

struct Floor
{
   Floor() = default;
   Floor(Type type, const Pos& coord, int turnsBeforeDestruction)
      : m_type(type)
      , m_coord(coord)
      , m_turnsBeforeDestruction(turnsBeforeDestruction)
   {}

   bool operator<(const Floor& obj) const
   {
      //int sumCoord = m_coord.m_x + m_coord.m_y;
      //int sumCoordObj = obj.m_coord.m_x + obj.m_coord.m_y;
      //if (sumCoord < sumCoordObj)
      //   return true;

      if (m_coord.m_x < obj.m_coord.m_x)
         return true;
      else if (m_coord.m_x == obj.m_coord.m_x)
         return m_coord.m_y < obj.m_coord.m_y;

      return false;
   }

   Type m_type{ TYPE_NONE };
   Pos m_coord{};
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const Floor& obj)
{
   os << "TYPE: " << obj.m_type << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction << "|";
   os << endl;
   return os;
}

class GameObject
{
public:
   GameObject(Type entityType, Owner owner, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_owner(owner)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   Owner m_owner{ OWNER_NONE };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_owner << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
