#ifndef __GAME_H__
#define __GAME_H__

#include "GameState.h"
#include "GameState.h"
#include "ReaderWriter.h"

using namespace std;

static void fillScore(int& explosionScore, int& utilityScore, const Floor& targetFloor, int bombScore, int emptyFloorBonus, int distance)
{
   int distanceMalus = distance;
   int objectBonus = targetFloor.m_type == TYPE_OBJECT ? 50 : 0;
   int explosionMalus = targetFloor.m_turnsBeforeDestruction != 0 ? 500 * 1 / ((targetFloor.m_turnsBeforeDestruction - 0.5)*(targetFloor.m_turnsBeforeDestruction - 0.5)) : 0;
   int bombScoreBonus = bombScore > 0 ? (bombScore + 2)*(bombScore + 2) : 0;

   explosionScore = emptyFloorBonus + bombScoreBonus - explosionMalus - distanceMalus;//TODO: remove explosion malus
   utilityScore = emptyFloorBonus + objectBonus - explosionMalus - distanceMalus*distanceMalus;
   explosionScore = explosionScore;
   utilityScore = utilityScore;
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
      vector<vector<int>> explosionScores(HEIGHT, vector<int>(WIDTH));//todebug
      vector<vector<int>> utilityScores(HEIGHT, vector<int>(WIDTH));//todebug
      vector<vector<vector<Pos>>> shortestPaths(HEIGHT, vector<vector<Pos>>(WIDTH));
      int bestExplosionScoreSoFar = std::numeric_limits<int>::min();
      int bestUtilityScoreSoFar = std::numeric_limits<int>::min();
      Pos bestExplosionPosSoFar = Pos(0, 0);
      Pos bestUtilityPosSoFar = Pos(0, 0);

      //TODO: check, maybe useless
      //before posing the bomb to avoid being trapped by its own bomb explosion
      if (computeDistance(m_state.m_me, m_state.m_objective) == 1)
      {
         updateTurnBeforeDestructionOnALine(GameObject(TYPE_BOMB, OWNER_ME, m_state.m_objective, 9, 0), 2, 2, m_state.m_map);
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
            fillScore(explosionScores[i][j], utilityScores[i][j], targetFloor, m_state.m_bombTileScoresMap[i][j], emptyFloorBonus, distance);
            
            //best choices
            if (bestExplosionScoreSoFar < explosionScores[i][j])
            {
               bestExplosionScoreSoFar = explosionScores[i][j];
               bestExplosionPosSoFar = Pos(i, j);
            }
            if (bestUtilityScoreSoFar < utilityScores[i][j])
            {
               bestUtilityScoreSoFar = utilityScores[i][j];
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
         shortestPaths.pop_back();//remove the latest which is the tile where we stand
         m_state.m_objectiveShortestPath = shortestPaths[m_state.m_objective.m_x][m_state.m_objective.m_y];
         m_state.m_hasReachedObjective = false;
      }

      cerr << "numberOfPathsFound: " << numberOfPathsFound << endl;
      string actionToDo = "MOVE";
      //cerr << "Move" << endl;
      //cerr << "bestPosSoFar: " << bestPosSoFar << endl;
      //cerr << "objective: " << m_state.m_objective << endl;
      //cerr << "myPos: " << m_state.m_me.m_coord << endl;
      //cerr << "Move" << endl;
      if (m_state.m_objective == m_state.m_me.m_coord)
      {
         //cerr << "Place bomb" << endl;
         actionToDo = m_state.m_objectiveType == TYPE_BOMB ? "BOMB" : "MOVE";
         m_state.m_hasReachedObjective = true;
      }

      string placeToGo = "0 0";
      if (!m_state.m_objectiveShortestPath.empty())
      {
         ostringstream os;
         os << m_state.m_objectiveShortestPath.back();
         placeToGo = os.str();
      }
      else
      {
         ostringstream os;
         os << m_state.m_objective;
         placeToGo = os.str();
      }
      m_state.m_objectiveShortestPath.pop_back();

      //HACK for Boss
      //if (m_state.m_timerBeforeNextBomb == 0 && !(m_state.m_me.m_coord.m_x == 0 && m_state.m_me.m_coord.m_x == 0) && (m_state.m_me.m_coord.m_x % 2 == 0 || m_state.m_me.m_coord.m_y % 2 == 0))
      //{
      //   actionToDo = "BOMB";
      //}

      ostringstream os;
      os << actionToDo << " " << placeToGo << " " << actionToDo << " " << placeToGo;
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
		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
      string output = play();
      cout << output << endl;
	}

public:
	GameState m_state;
};

#endif
