#ifndef __GAME_H__
#define __GAME_H__

#include "GameEntities.h"
#include "GameState.h"
#include "ReaderWriter.h"

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
