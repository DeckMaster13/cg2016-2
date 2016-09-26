#include "Game.h"

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