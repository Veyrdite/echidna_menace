Fun 2D SDL maze-game I created as a high-school project.  Notable features include:
* No header files.  I didn't understand them :D
* Plaintext level files, easily editable
* Curious state-machine AI
* Completely event-based gameplay.  The same input sequence will always yield the same game, and nothing but the menu is real-time
* Very simple and portable source: compiles well on ARM platforms
* Overly commented code and a folder full of PDF docs (requirements of the task)

The game is very much possible to win even though it quickly becomes a challenge.  Once you discover the wonderful logic hole brought about by the grid-based gameplay,  you will be able to dance with the AI harmlessly.  I'm not sure if the game is winnable without exploiting this glitch: there is a challenge for you!

The source code is split over multiple C files which are #included into main.c .  When I started writing this I took one look at #IFDEF shenanigans and ran away.

The AI is what this game is all about.  If you can survive my sense of humour (it was an early project -- I was bareley squeezing it out) then you will find the AI implementation quite interesting.  A* and related are not hard to copy and use, but I didn't understand them, so I went for my own hybrid approach using a simple 'hunt' via the shortest path and 'wall-follow' behaviors.  The interaction these have with the puzzles in the levels reveal challenge more complex than you would expect.

## Using the menac
Requires:
* SDL and sdl_image

A makefile is included.  To compile and run the game:
* cd source_code
* make
* ./echidna

## Licensing
All files state the code is under GPLv3, but I also give permission for it to be used under GPLv2.  If you want a more permissive licence then just ask :)
