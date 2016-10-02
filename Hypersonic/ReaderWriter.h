#ifndef __READER_WRITER_H__
#define __READER_WRITER_H__

#include <cassert>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "GameEntities.h"
#include "Utilities.h"

using namespace std;

class GlobalInput
{
public:
	GlobalInput() = default;
	void fillFromInput()
	{
      cin >> m_width >> m_height >> m_myId; cin.ignore();
	}

public:
   int m_width{ 0 }; // always 13
   int m_height{ 0 }; // always 11
	int m_myId{ 0 }; // ID of your player (0, 1)
};

std::ostream& operator<<(std::ostream& os, const GlobalInput& obj)
{
	// write obj to stream
   os << "Width : " << obj.m_width << endl;
   os << "Height : " << obj.m_height << endl;
	os << "My player id      : " << obj.m_myId << endl;

	return os;
}

GlobalInput readGlobalInput()
{
	GlobalInput settings;
	settings.fillFromInput();
	return settings;
}

class TurnInput
{
public:
   TurnInput()
   {

   }
	void fillFromInput(const GlobalInput& settings)
	{
      for (int i = 0; i < settings.m_height; i++)
      {
         string row;
         getline(cin, row);
         m_map.push_back(vector<Tile>(settings.m_width));
         for (size_t j = 0; j < row.size(); ++j)
         {
            if (row[j] == '0')
            {
               GameObject box(TYPE_BOX, -1, Pos(i,j), 0, 0);
               m_map[i][j] = Tile(box);

               m_boxes.push_back(box);
            }
            else if (row[j] == '.')
            {
               m_map[i][j] = Tile(Pos(i, j), TYPE_NONE);
            }
            else if (row[j] == '1')
            {
               GameObject box(TYPE_BOX, -1, Pos(i, j), 1, 0);
               m_map[i][j] = Tile(box);

               m_boxes.push_back(box);
            }
            else if (row[j] == '2')
            {
               GameObject box(TYPE_BOX, -1, Pos(i, j), 2, 0);
               m_map[i][j] = Tile(box);

               m_boxes.push_back(box);
            }
            else if (row[j] == 'X')
            {
               GameObject wall(TYPE_BOX, -1, Pos(i, j), 0, 0);
               m_map[i][j] = Tile(wall);

               m_walls.push_back(wall);
            }
         }
      }
      int entities;
      cin >> entities; cin.ignore();
      for (int i = 0; i < entities; i++)
      {
         int entityType;
         int owner;
         int x;
         int y;
         int param1;
         int param2;
         cin >> entityType >> owner >> x >> y >> param1 >> param2; cin.ignore();

         Type enumEntityType;
         switch (entityType)
         {
         case 0:
            //cerr << "TYPE PLAYER0" << endl;
            enumEntityType = TYPE_PLAYER;
            break;
         case 1:
            //cerr << "TYPE BOMB0" << endl;
            enumEntityType = TYPE_BOMB;
            break;
         case 2:
            //cerr << "TYPE OBJECT0" << endl;
            enumEntityType = TYPE_OBJECT;
            break;
         default:
            assert(false);
            break;
         }

         GameObject obj(enumEntityType, owner, Pos(y,x), param1, param2);
         if (enumEntityType == TYPE_BOMB)
         {
            m_map[y][x] = Tile(obj);
            m_bombs.push_back(obj);
         }
         else if (enumEntityType == TYPE_OBJECT)
         {
            m_map[y][x] = Tile(obj);
            m_objects.push_back(obj);
         }
         else if (enumEntityType == TYPE_PLAYER)
         {
            //WE DO NOT ADD PLAYERS TO THE MAP
            m_players[obj.m_ownerId] = obj;
            if (obj.m_ownerId == settings.m_myId)
            {
               m_me = obj;
            }
         }
      }
   }

public:
	vector<vector<Tile>> m_map;
   vector<GameObject> m_bombs;
   vector<GameObject> m_boxes;
   vector<GameObject> m_objects;
   vector<GameObject> m_walls;
   GameObject m_me;
   int m_timerBeforeNextBomb;
   map<int, GameObject> m_players;
};

std::ostream& operator<<(std::ostream& os, const TurnInput& obj)
{
	// write obj to stream
	for (const auto& line: obj.m_map)
	{
      cerr << "|";
      for (const auto& tile : line)
      {
         switch (tile.getType())
         {
         case TYPE_NONE:
            cerr << "   ";
            break;
         case TYPE_BOX:
            cerr << " c ";
            break;
         case TYPE_BOMB:
            cerr << "kab";
            break;
         case TYPE_WALL:
            cerr << " x ";
            break;
         case TYPE_PLAYER:
            assert(false);
            break;
         default:
            assert(false);
            break;
         }
         cerr << "|";
      }
      cerr << endl;
	}
	return os;
}

TurnInput readTurnInput(const GlobalInput& settings)
{
	TurnInput turnInput;
	turnInput.fillFromInput(settings);
	return turnInput;
}

template <typename T>
void write(const T& obj)
{
	cerr << obj;
}

template <typename T>
void write(const vector<T>& vec)
{
   cerr << "|";
   for (const auto& elem : vec)
   {
      cerr << elem << "|";
   }
   cerr << endl;
}

template <typename T>
void write(const vector<vector<T>>& vecvec)
{
   for (const auto& row : vecvec)
   {
      cerr << "|";
      for (const auto& elem : row)
      {
         cerr << elem << "|";
      }
      cerr << endl;
   }
}

void write(const vector<vector<int>>& vecvec)
{
   for (const auto& row : vecvec)
   {
      cerr << "|";
      for (const auto& elem : row)
      {
         if (elem >= 0)
         {
            cerr << " ";
         }
         if (abs(elem) > 99)
         {}
         else if (abs(elem) > 9)
         {
            cerr << " ";
         }
         else if (abs(elem) <= 9)
         {
            cerr << "  ";
         }
         cerr << elem << "|";
      }
      cerr << endl;
   }
}

#endif
