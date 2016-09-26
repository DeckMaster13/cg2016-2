# 1 "toPreprocess.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "toPreprocess.cpp"
# 15 "toPreprocess.cpp"
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
   int m_param1{0};
   int m_param2{ 0 };
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
# 674 "toPreprocess.cpp"
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
   return pos.m_x >= 0 && pos.m_x < 11 && pos.m_y >= 0 && pos.m_y < 13;
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
      return 0;
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
   vector<vector<Pos*>> cameFrom(11, vector<Pos*>(13, nullptr));
   vector<vector<int>> costSoFar(11, vector<int>(13, -1));


   costSoFar[me.m_coord.m_x][me.m_coord.m_y] = 0;

   while (!frontier.empty())
   {
      Floor current = frontier.top();
      frontier.pop();

      if (current.m_coord == destination) break;

      for (auto next : getNeighbors(current, map))
      {
         int newCost = costSoFar[current.m_coord.m_x][current.m_coord.m_y] + 1;
         if (costSoFar[next.m_coord.m_x][next.m_coord.m_y] == -1 || newCost < costSoFar[next.m_coord.m_x][next.m_coord.m_y])
         {
            costSoFar[next.m_coord.m_x][next.m_coord.m_y] = newCost;

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

static void fillBombTilesScoreMap(const vector<GameObject>& boxes, vector<vector<int>>& bombTileScoreMap, const vector<vector<Floor>>& map)
{
   for (const auto& box : boxes)
   {
      computeBombTileScore(box.m_coord, 2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, -2, 0, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, 2, bombTileScoreMap, map);
      computeBombTileScore(box.m_coord, 0, -2, bombTileScoreMap, map);
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

static void updateTurnsBeforeDestruction(const vector<GameObject>& bombs, vector<vector<Floor>>& map)
{
   for (const auto& bomb : bombs)
   {
      updateTurnBeforeDestructionOnALine(bomb, 2, 0, map);
      updateTurnBeforeDestructionOnALine(bomb, 0, 2, map);
   }
}
# 1438 "toPreprocess.cpp"
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
   int m_width{ 0 };
   int m_height{ 0 };
 int m_myId{ 0 };
};

std::ostream& operator<<(std::ostream& os, const GlobalInput& obj)
{

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

               m_map[i][j] = Floor(TYPE_BOX, Pos(i,j), 0);

               GameObject box(TYPE_BOX, OWNER_NONE, Pos(i,j), 0, 0);
               m_boxes.push_back(box);
            }
            else if (row[j] == '.')
            {
               m_map[i][j] = Floor(TYPE_NONE, Pos(i, j), 0);
            }
            else if (row[j] == '1')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, OWNER_NONE, Pos(i, j), 1, 0));
            }
            else if (row[j] == '2')
            {
               m_map[i][j] = Floor(TYPE_BOX, Pos(i, j), 0);
               m_boxes.push_back(GameObject(TYPE_BOX, OWNER_NONE, Pos(i, j), 2, 0));
            }
            else if (row[j] == 'X')
            {
               m_map[i][j] = Floor(TYPE_WALL, Pos(i, j), 0);
               m_walls.push_back(GameObject(TYPE_WALL, OWNER_NONE, Pos(i, j), 0, 0));
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

            enumEntityType = TYPE_PLAYER;
            break;
         case 1:

            enumEntityType = TYPE_BOMB;
            break;
         case 2:

            enumEntityType = TYPE_OBJECT;
            break;
         default:
            assert(false);
            break;
         }

         GameObject object(enumEntityType, owner == settings.m_myId ? OWNER_ME : OWNER_HIM, Pos(y, x), param1, param2);
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

            if (object.m_owner == OWNER_ME)
            {
               cerr << "FOUND ME at " << object.m_coord << endl;
               m_me = object;
            }
            else if (object.m_owner == OWNER_HIM)
            {

               m_him = object;
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
   GameObject m_him;
   int m_timerBeforeNextBomb;
};

std::ostream& operator<<(std::ostream& os, const TurnInput& obj)
{

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



using namespace std;

class GameState
{
public:
   GameState(const GlobalInput&)
   {}
   ~GameState() = default;

   void updateOnNewTurn(const TurnInput& turnInput)
   {

      m_map = turnInput.m_map;
      m_bombs = turnInput.m_bombs;
      m_boxes = turnInput.m_boxes;
      m_objects = turnInput.m_objects;


      m_me = turnInput.m_me;
      m_him = turnInput.m_him;

      m_bombTileScoresMap = vector<vector<int>>(11, vector<int>(13));

      updateTurnsBeforeDestruction(m_bombs, m_map);
      m_timerBeforeNextBomb = updateTimerBeforeNextBomb(m_me, m_bombs);
      fillBombTilesScoreMap(m_boxes, m_bombTileScoresMap, m_map);
# 1728 "toPreprocess.cpp"
   }

public:
   vector<vector<Floor>> m_map;
   vector<vector<int>> m_bombTileScoresMap;
   vector<GameObject> m_bombs;
   vector<GameObject> m_boxes;
   vector<GameObject> m_objects;
   vector<GameObject> m_walls;
   GameObject m_me;
   GameObject m_him;
   int m_timerBeforeNextBomb;
   bool m_hasReachedObjective{ true };
   Pos m_objective;
   Type m_objectiveType;
};
# 4279 "toPreprocess.cpp"
using namespace std;

class Game
{
public:
 Game(const GameState& state)
  : m_state(state)
 {}

   string goToTheClosestOne()
   {
      int numberOfPathsFound = 0;
      vector<vector<int>> explosionScores(11, vector<int>(13));
      vector<vector<int>> utilityScores(11, vector<int>(13));
      int bestExplosionScoreSoFar = std::numeric_limits<int>::min();
      int bestUtilityScoreSoFar = std::numeric_limits<int>::min();
      Pos bestExplosionPosSoFar = Pos(0, 0);
      Pos bestUtilityPosSoFar = Pos(0, 0);



      if (computeDistance(m_state.m_me, m_state.m_objective) == 1)
      {
         updateTurnBeforeDestructionOnALine(GameObject(TYPE_BOMB, OWNER_ME, m_state.m_objective, 9, 0), 2, 2, m_state.m_map);
      }


      for (size_t i = 0; i < 11; ++i)
      {

         vector<int> row = m_state.m_bombTileScoresMap[i];
         for (size_t j = 0; j < 13; ++j)
         {

            const Floor& targetFloor = m_state.m_map[i][j];
            int bombScore = row[j];
            if (!isPositionValid(targetFloor.m_coord) || !isPositionEmpty(targetFloor)) continue;
            int reachableBonus = 10;
            int distance = 0;
            if (m_state.m_me.m_coord != targetFloor.m_coord)
            {
               vector<Pos> shortestPath = findShortestPath(m_state.m_me, targetFloor.m_coord, m_state.m_map);
               if (shortestPath.empty()) continue;
               numberOfPathsFound++;
               distance = shortestPath.size();
            }
            else
            {
               distance = 0;
            }

            int distanceMalus = distance;
            int objectBonus = targetFloor.m_type == TYPE_OBJECT ? 50 : 0;
            int explosionMalus = targetFloor.m_turnsBeforeDestruction != 0 ? 200 * 1 / ((targetFloor.m_turnsBeforeDestruction - 0.5)*(targetFloor.m_turnsBeforeDestruction - 0.5)) : 0;
            int bombScoreBonus = bombScore > 0 ? (bombScore + 2)*(bombScore + 2) : 0;

            int explosionScore = reachableBonus + bombScoreBonus - explosionMalus - distanceMalus;
            int utilityScore = reachableBonus + objectBonus - explosionMalus - distanceMalus*distanceMalus;
            explosionScores[i][j] = explosionScore;
            utilityScores[i][j] = utilityScore;
            if (bestExplosionScoreSoFar < explosionScore)
            {
               bestExplosionScoreSoFar = explosionScore;
               bestExplosionPosSoFar = Pos(i, j);
            }
            if (bestUtilityScoreSoFar < utilityScore)
            {
               bestUtilityScoreSoFar = utilityScore;
               bestUtilityPosSoFar = Pos(i, j);
            }
         }
      }


      write(explosionScores);
      cerr << endl;
      write(utilityScores);

      if (m_state.m_hasReachedObjective)
      {
         if (bestExplosionScoreSoFar > bestUtilityScoreSoFar)
         {
            m_state.m_objective = bestExplosionPosSoFar;
            m_state.m_objectiveType = TYPE_BOMB;
         }
         else
         {
            m_state.m_objective = bestUtilityPosSoFar;
            m_state.m_objectiveType = TYPE_OBJECT;
         }
         m_state.m_hasReachedObjective = false;
      }

      cerr << "numberOfPathsFound: " << numberOfPathsFound << endl;
      string actionToDo = "MOVE";





      if (m_state.m_objective == m_state.m_me.m_coord)
      {

         actionToDo = m_state.m_objectiveType == TYPE_BOMB ? "BOMB" : "MOVE";
         m_state.m_hasReachedObjective = true;
      }







      ostringstream os;
      os << actionToDo << " " << m_state.m_objective << " " << actionToDo << " " << m_state.m_objective;
      if (m_state.m_hasReachedObjective)
      {
         os << "*";
      }
      return os.str();
   }

 void updateState(const TurnInput& turnInput)
 {
  m_state.updateOnNewTurn(turnInput);
 }

 void playOneTurn()
 {


      string output = goToTheClosestOne();
      cout << output << endl;
 }

public:
 GameState m_state;
};



int main()
{
   GlobalInput globalInput = readGlobalInput();
   write(globalInput);

   GameState state(globalInput);
   Game game(state);

   while (1)
   {
      TurnInput turnInput = readTurnInput(globalInput);


      game.updateState(turnInput);
      game.playOneTurn();
   }
}
