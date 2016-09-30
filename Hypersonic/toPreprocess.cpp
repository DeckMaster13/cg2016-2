#ifndef __GAME_H__
#define __GAME_H__

#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif
#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
 
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

static bool isPositionEmpty(const Floor& object)
{
   return object.m_type != TYPE_BOX && object.m_type != TYPE_BOMB && object.m_type != TYPE_WALL;
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
   if (me.m_coord == destination) return vector<Pos>(1, destination);
  
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

static int willDieInXTurns()
{
   int turnsBeforeDying = 0;



   return turnsBeforeDying;
}

#endif

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
#ifndef __READER_WRITER_H__
#define __READER_WRITER_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
 
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

static bool isPositionEmpty(const Floor& object)
{
   return object.m_type != TYPE_BOX && object.m_type != TYPE_BOMB && object.m_type != TYPE_WALL;
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
   if (me.m_coord == destination) return vector<Pos>(1, destination);
  
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

static int willDieInXTurns()
{
   int turnsBeforeDying = 0;



   return turnsBeforeDying;
}

#endif

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
         m_map.push_back(vector<Floor>(settings.m_width));
         for (size_t j = 0; j < row.size(); ++j)
         {
            if (row[j] == '0')
            {
               //cerr << i << "|" << j << endl;
               m_map[i][j] = Floor(TYPE_BOX, Pos(i,j), 0);

               GameObject box(TYPE_BOX, -1, Pos(i,j), 0, 0);
               m_boxes.push_back(box);
            }
            else if (row[j] == '.')
            {
               m_map[i][j] = Floor(TYPE_NONE, Pos(i, j), 0);
            }
            else if (row[j] == '1')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, -1, Pos(i, j), 1, 0));
            }
            else if (row[j] == '2')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, -1, Pos(i, j), 2, 0));
            }
            else if (row[j] == 'X')
            {
               m_map[i][j] = Floor(TYPE_WALL, Pos(i, j), 0);
               m_walls.push_back(GameObject(TYPE_WALL, -1, Pos(i, j), 0, 0));
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

         GameObject object(enumEntityType, owner, Pos(y, x), param1, param2);
         if (enumEntityType == TYPE_BOMB)
         {
            m_bombs.push_back(object);
            m_map[y][x] = Floor(TYPE_BOMB, Pos(y, x), 0);
         }
         else if (enumEntityType == TYPE_OBJECT)
         {
            m_objects.push_back(object);
            m_map[y][x] = Floor(TYPE_OBJECT, Pos(y, x), 0);
         }
         else if (enumEntityType == TYPE_PLAYER)
         {
            //cerr << "TYPE PLAYER" << endl;
            m_players[object.m_ownerId] = object;
            if (object.m_ownerId == settings.m_myId)
            {
               m_me = object;
            }
         }
      }
   }

public:
	vector<vector<Floor>> m_map;
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
      for (const auto& floor : line)
      {
         switch (floor.m_type)
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
#ifndef __READER_WRITER_H__
#define __READER_WRITER_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#ifndef __POS_H__
#define __POS_H__


using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif

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
   GameObject(Type entityType, int ownerId, Pos coord, int param1, int param2)
      : m_entityType(entityType)
      , m_ownerId(ownerId)
      , m_coord(coord)
      , m_param1(param1)
      , m_param2(param2)
      , m_turnsBeforeDestruction(0)
   {}
   GameObject() = default;
   ~GameObject() = default;

public:
   Type m_entityType{TYPE_NONE};
   int m_ownerId{ -1 };
   Pos m_coord{};
   int m_param1{0};//used later
   int m_param2{ 0 };//used later
   int m_turnsBeforeDestruction{ 0 };
};

static ostream& operator<<(ostream& os, const GameObject& obj)
{
   os << "TYPE: " << obj.m_entityType << "|";
   os << "OWNER: " << obj.m_ownerId << "|";
   os << "COORD: " << obj.m_coord << "|";
   os << "PARAM1: " << obj.m_param1 << "|";
   os << "PARAM2: " << obj.m_param2 << "|";
   os << "TURNSBEFOREDESTR: " << obj.m_turnsBeforeDestruction<< "|";
   os << endl;
   return os;
}



#endif
 
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

static bool isPositionEmpty(const Floor& object)
{
   return object.m_type != TYPE_BOX && object.m_type != TYPE_BOMB && object.m_type != TYPE_WALL;
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
   if (me.m_coord == destination) return vector<Pos>(1, destination);
  
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

static int willDieInXTurns()
{
   int turnsBeforeDying = 0;



   return turnsBeforeDying;
}

#endif

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
         m_map.push_back(vector<Floor>(settings.m_width));
         for (size_t j = 0; j < row.size(); ++j)
         {
            if (row[j] == '0')
            {
               //cerr << i << "|" << j << endl;
               m_map[i][j] = Floor(TYPE_BOX, Pos(i,j), 0);

               GameObject box(TYPE_BOX, -1, Pos(i,j), 0, 0);
               m_boxes.push_back(box);
            }
            else if (row[j] == '.')
            {
               m_map[i][j] = Floor(TYPE_NONE, Pos(i, j), 0);
            }
            else if (row[j] == '1')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, -1, Pos(i, j), 1, 0));
            }
            else if (row[j] == '2')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, -1, Pos(i, j), 2, 0));
            }
            else if (row[j] == 'X')
            {
               m_map[i][j] = Floor(TYPE_WALL, Pos(i, j), 0);
               m_walls.push_back(GameObject(TYPE_WALL, -1, Pos(i, j), 0, 0));
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

         GameObject object(enumEntityType, owner, Pos(y, x), param1, param2);
         if (enumEntityType == TYPE_BOMB)
         {
            m_bombs.push_back(object);
            m_map[y][x] = Floor(TYPE_BOMB, Pos(y, x), 0);
         }
         else if (enumEntityType == TYPE_OBJECT)
         {
            m_objects.push_back(object);
            m_map[y][x] = Floor(TYPE_OBJECT, Pos(y, x), 0);
         }
         else if (enumEntityType == TYPE_PLAYER)
         {
            //cerr << "TYPE PLAYER" << endl;
            m_players[object.m_ownerId] = object;
            if (object.m_ownerId == settings.m_myId)
            {
               m_me = object;
            }
         }
      }
   }

public:
	vector<vector<Floor>> m_map;
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
      for (const auto& floor : line)
      {
         switch (floor.m_type)
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

using namespace std;

static void fillScore(int& score, int& explosionScore, const Floor& targetFloor, int bombScore, int emptyFloorBonus, int distance)
{
   int distanceMalus = distance*distance;
   int objectBonus = targetFloor.m_type == TYPE_OBJECT ? 50 : 0;
   int explosionMalus = targetFloor.m_turnsBeforeDestruction != 0 ? 500 * 1 / ((targetFloor.m_turnsBeforeDestruction - 0.5)*(targetFloor.m_turnsBeforeDestruction - 0.5)) : 0;
   int bombScoreBonus = bombScore > 0 ? (bombScore + 2)*(bombScore + 2) : 0;

   explosionScore = emptyFloorBonus + bombScoreBonus - distanceMalus;
   score = emptyFloorBonus + bombScoreBonus + objectBonus - explosionMalus - distanceMalus;
}

static string decideActionToDo(const GameState& state, int distanceToObjective, bool& shouldChangeObjective)
{
   string actionToDo = "MOVE";
   if ((distanceToObjective == 0 || distanceToObjective == 1) && state.m_objectiveType == TYPE_OBJECT)
   {
      actionToDo = "MOVE";
      shouldChangeObjective = true;
   }
   else if (distanceToObjective == 0 && state.m_objectiveType == TYPE_BOMB)
   {
      actionToDo = "BOMB";
      shouldChangeObjective = true;
   }
   return actionToDo;
}

static string decidePlaceToGo(GameState& state)
{
   string placeToGo = "0 0";
   if (!state.m_objectiveShortestPath.empty())
   {
      ostringstream os;
      os << state.m_objectiveShortestPath.back();
      state.m_objectiveShortestPath.pop_back();
      placeToGo = os.str();
   }
   else
   {
      ostringstream os;
      os << state.m_me.m_coord;
      placeToGo = os.str();
   }
   return placeToGo;
}

static void changeObjective(GameState& state, vector<vector<vector<Pos>>>& shortestPaths, const Pos& bestObjectiveSoFar, const Pos& bestExplosionSoFar)
{
   state.m_objective = bestObjectiveSoFar;
   shortestPaths.pop_back();//remove the latest which is the tile where we stand
   state.m_objectiveShortestPath = shortestPaths[state.m_objective.m_x][state.m_objective.m_y];
   state.m_objectiveType = computeDistance(bestObjectiveSoFar, bestExplosionSoFar) == 0 ? TYPE_BOMB : TYPE_OBJECT;
}

class Game
{
public:
	Game(const GameState& state)
		: m_state(state)
	{}

   string play()
   {
      int numberOfPathsFound = 0;//toDebug
      vector<vector<int>> bestScores(HEIGHT, vector<int>(WIDTH));//todebug
      vector<vector<int>> bestExplosionScores(HEIGHT, vector<int>(WIDTH));//todebug
      vector<vector<vector<Pos>>> shortestPaths(HEIGHT, vector<vector<Pos>>(WIDTH));
      int bestScoreSoFar = std::numeric_limits<int>::min();
      int bestExplosionScoreSoFar = std::numeric_limits<int>::min();
      Pos bestObjectiveSoFar = Pos(0, 0);
      Pos bestExplosionSoFar = Pos(0, 0);

      //TODO: check, maybe useless
      //before posing the bomb to avoid being trapped by its own bomb explosion
      if (computeDistance(m_state.m_me, m_state.m_objective) == 1)
      {
         updateTurnBeforeDestructionOnALine(GameObject(TYPE_BOMB, m_state.m_me.m_ownerId, m_state.m_objective, 9, 0), m_state.m_me.m_param2, m_state.m_me.m_param2, m_state.m_map);
      }

      //vector<vector<int>> bombAndDistanceTileScoreMap;
      for (size_t i = 0; i < HEIGHT; ++i)
      {
         for (size_t j = 0; j < WIDTH; ++j)
         {
            const Floor& targetFloor = m_state.m_map[i][j];
            if (!isPositionValid(targetFloor.m_coord) || !isPositionEmpty(targetFloor)) continue;
            int emptyFloorBonus = 10;

            //path, distance
            shortestPaths[i][j] = findShortestPath(m_state.m_me, targetFloor.m_coord, m_state.m_map);
            if (shortestPaths[i][j].empty()) continue; // not reachable
            numberOfPathsFound++;
            int distance = shortestPaths[i][j].size() - 1;

            //score
            fillScore(bestScores[i][j], bestExplosionScores[i][j], targetFloor, m_state.m_bombTileScoresMap[i][j], emptyFloorBonus, distance);
            if (bestExplosionScoreSoFar < bestExplosionScores[i][j])
            {
               bestExplosionScoreSoFar = bestExplosionScores[i][j];
               bestExplosionSoFar = Pos(i, j);
            }

            //best choices
            if (bestScoreSoFar < bestScores[i][j])
            {
               bestScoreSoFar = bestScores[i][j];
               bestObjectiveSoFar = Pos(i, j);
            }
         }
      }

      cerr << endl;
      write(bestScores);
      cerr << endl;
      cerr << "numberOfPathsFound: " << numberOfPathsFound << endl;
      //cerr << "Move" << endl;
      //cerr << "bestPosSoFar: " << bestPosSoFar << endl;
      //cerr << "objective: " << m_state.m_objective << endl;
      //cerr << "myPos: " << m_state.m_me.m_coord << endl;
      //cerr << "Move" << endl;

      int distanceToObjective = computeDistance(m_state.m_objective, m_state.m_me.m_coord);
      if (shortestPaths.empty())
      {
         changeObjective(m_state, shortestPaths, bestObjectiveSoFar, bestExplosionSoFar);
      }
      bool shouldChangeObjective = false;
      string actionToDo = decideActionToDo(m_state, distanceToObjective, shouldChangeObjective);
      string placeToGo = decidePlaceToGo(m_state);


      //HACK for Boss
      //if (m_state.m_timerBeforeNextBomb == 0 && !(m_state.m_me.m_coord.m_x == 0 && m_state.m_me.m_coord.m_x == 0) && (m_state.m_me.m_coord.m_x % 2 == 0 || m_state.m_me.m_coord.m_y % 2 == 0))
      //{
      //   actionToDo = "BOMB";
      //}

      ostringstream os;
      os << actionToDo << " " << placeToGo << " " << actionToDo << " " << placeToGo;
      if (shouldChangeObjective)
      {
         changeObjective(m_state, shortestPaths, bestObjectiveSoFar, bestExplosionSoFar);
         os << "*" << " >> " << bestObjectiveSoFar;
      }
      return os.str();
   }

	void updateState(const TurnInput& turnInput)
	{
		m_state.updateOnNewTurn(turnInput);
	}

	void playOneTurn()
	{
		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
      string output = play();
      cout << output << endl;
	}

public:
	GameState m_state;
};

#endif

int main()
{
   GlobalInput globalInput = readGlobalInput();
   write(globalInput);

   GameState state(globalInput);
   Game game(state);
   // game loop
   while (1)
   {
      TurnInput turnInput = readTurnInput(globalInput);
      //write(turnInput);

      game.updateState(turnInput);
      game.playOneTurn();
   }
}