#ifndef __GAME_H__
#define __GAME_H__

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

using namespace std;

class GameState
{
public:
   GameState(const GlobalInput&)
   {}
   ~GameState() = default;

   void updateOnNewTurn(const TurnInput& turnInput)
   {
      m_board.m_map = turnInput.m_map;
      m_board.m_bombs = turnInput.m_bombs;
      m_board.m_boxes = turnInput.m_boxes;
      m_board.m_objects = turnInput.m_objects;
      m_board.m_me = turnInput.m_me;
      m_board.m_players = turnInput.m_players;
   }

public:
   Board m_board;
   vector<vector<Node*>> m_graph;
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

using namespace std;

//static void fillScore(int& score, int& explosionScore, const Floor& targetFloor, int bombScore, int emptyFloorBonus, int distance)
//{
//   int distanceMalus = distance*distance;
//   int objectBonus = targetFloor.m_type == TYPE_OBJECT ? 50 : 0;
//   int explosionMalus = targetFloor.m_turnsBeforeDestruction != 0 ? 500 * 1 / ((targetFloor.m_turnsBeforeDestruction - 0.5)*(targetFloor.m_turnsBeforeDestruction - 0.5)) : 0;
//   int bombScoreBonus = bombScore > 0 ? (bombScore + 2)*(bombScore + 2) : 0;
//
//   explosionScore = emptyFloorBonus + bombScoreBonus - distanceMalus;
//   score = emptyFloorBonus + bombScoreBonus + objectBonus - explosionMalus - distanceMalus;
//}

static void rankNode(Node* node)
{
   assert(node != nullptr);
   const Board& board = node->m_board;
   const GameObject& me = board.m_me;
   const vector<vector<Tile>>& map = board.m_map;
   const Tile& targetTile = map[me.m_coord.m_x][me.m_coord.m_y];

   node->m_score = node->m_previous ? node->m_previous->m_score : 0;
   
   if (targetTile.m_turnsBeforeExplosion >= 0)
   {
      if (targetTile.m_turnsBeforeExplosion == 0)
         node->m_score -= 2000;
      else if (targetTile.m_turnsBeforeExplosion > 0)
         node->m_score -= 400 * 1 / ((targetTile.m_turnsBeforeExplosion - 0.5)*(targetTile.m_turnsBeforeExplosion - 0.5));
   }

   if (me.m_param1 == 0)
   {
      node->m_score += 500;
   }
}

static vector<Node*> createTreeNextDepth(const vector<Node*>& currentDepth)
{
   vector<Node*> newDepth;
   for (Node* node : currentDepth)
   {
      const Board& board = node->m_board;
      const GameObject& me = board.m_me;
      const Pos& myPos = me.m_coord;
      Node* newNode = applyChoice(node, Choice(ACTION_MOVE, myPos));
      newDepth.push_back(newNode);
      if (me.m_param1 > 0)
      {
         newNode = applyChoice(node, Choice(ACTION_BOMB, myPos));
         newDepth.push_back(newNode);
      }

      vector<Tile> currentNeighbors = getNeighbors(board.m_map[myPos.m_x][myPos.m_y], board.m_map);
      for (const auto& neighbor : currentNeighbors)
      {
         //they are reachable
         newNode = applyChoice(node, Choice(ACTION_MOVE, neighbor.getCoord()));
         newDepth.push_back(newNode);
         if (me.m_param1 > 0)
         {
            newNode = applyChoice(node, Choice(ACTION_BOMB, neighbor.getCoord()));
            newDepth.push_back(newNode);
         }
      }
   }
   return newDepth;
}

static vector<vector<Node*>> createTree(const Board& board)
{
   const Pos& myPos = board.m_me.m_coord;

   vector<vector<Node*>> tree;
   vector<Node*> initialDepth;
   Node* initialNode = new Node(nullptr, Choice(ACTION_MOVE, myPos), board);
   initialDepth.push_back(initialNode);
   tree.push_back(initialDepth);

   vector<Node*> currentDepth = initialDepth;
   while (tree.size() < 4)
   {
      vector<Node*> newDepth = createTreeNextDepth(currentDepth);
      tree.push_back(newDepth);
      currentDepth = newDepth;
   }

   return tree;
}

static void rankTree(vector<vector<Node*>>& tree)
{
   for (auto& depth : tree)
      for (Node* node : depth)
         rankNode(node);
}

class Game
{
public:
	Game(const GameState& state)
		: m_state(state)
	{}

   string takeActionToDo(const vector<vector<Node*>>& tree)
   {
      const auto& lastDepth = tree.back();
      Node* bestNode = nullptr;
      int maxScore = 0;
      for (Node* node : lastDepth)
      {
         if (node->m_score >= maxScore)
         {
            maxScore = node->m_score;
            bestNode = node;
         }
      }
      
      cerr << "maxScore: " << maxScore << endl;
      Node* currentNode = bestNode;
      Node* previousNode = bestNode->m_previous;
      while (previousNode != nullptr && previousNode->m_previous != nullptr)
      {
         currentNode = previousNode;
         previousNode = currentNode->m_previous;
      }
      
      ostringstream os;
      os << currentNode->m_choice;
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
      vector<vector<Node*>> futureTree = createTree(m_state.m_board);
      rankTree(futureTree);
      string output = takeActionToDo(futureTree);
      cout << output << " " << output << endl;
      
      //cleaning
      for (auto& depth : futureTree)
      {
         for (auto* Node : depth)
         {
            delete Node;
         }
      }
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