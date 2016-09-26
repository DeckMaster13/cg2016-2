#ifndef __GAME_H__
#define __GAME_H__

#include "GameState.h"
#include "GameState.h"
#include "ReaderWriter.h"

using namespace std;

class Game
{
public:
	Game(const GameState& state)
		: m_state(state)
	{}

   string goToTheClosestOne()
   {
      int numberOfPathsFound = 0;//toDebug
      vector<vector<int>> explosionScores(HEIGHT, vector<int>(WIDTH));//todebug
      vector<vector<int>> utilityScores(HEIGHT, vector<int>(WIDTH));//todebug
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
         //cerr << i << " ";
         vector<int> row = m_state.m_bombTileScoresMap[i];
         for (size_t j = 0; j < WIDTH; ++j)
         {
            //cerr << j << endl;
            const Floor& targetFloor = m_state.m_map[i][j];
            int bombScore = row[j];
            if (!isPositionValid(targetFloor.m_coord) || !isPositionEmpty(targetFloor)) continue;//|| targetObject.m_turnsBeforeDestruction != 0
            int reachableBonus = 10;
            int distance = 0;
            if (m_state.m_me.m_coord != targetFloor.m_coord)
            {
               vector<Pos> shortestPath = findShortestPath(m_state.m_me, targetFloor.m_coord, m_state.m_map);
               if (shortestPath.empty()) continue; // not reachable
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

            int explosionScore = reachableBonus + bombScoreBonus - explosionMalus - distanceMalus;//TODO: remove explosion malus
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

      //HACK for Boss
      //if (m_state.m_timerBeforeNextBomb == 0 && !(m_state.m_me.m_coord.m_x == 0 && m_state.m_me.m_coord.m_x == 0) && (m_state.m_me.m_coord.m_x % 2 == 0 || m_state.m_me.m_coord.m_y % 2 == 0))
      //{
      //   actionToDo = "BOMB";
      //}

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
		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
      string output = goToTheClosestOne();
      cout << output << endl;
	}

public:
	GameState m_state;
};

#endif
