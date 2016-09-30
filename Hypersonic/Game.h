#ifndef __GAME_H__
#define __GAME_H__

#include "GameState.h"
#include "ReaderWriter.h"

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
