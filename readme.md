Fun 2D SDL maze-game I created as a high-school project.  Notable features include:
* No header files.  I didn't understand them :D
* Plaintext level files, easily editable
* Curious state-machine AI.  Fun to mess with
* Completely event-based gameplay.  The same input sequence will always yield the same game, and nothing but the menu is real-time
* Very simple and portable source: compiles well on ARM platforms
* Overly commented (requirement of the task)

The game is possible to win.  Once you discover the wonderful logic hole brought about by the grid-based gameplay you can dance with the AI harmlessly.  I'm not sure if the game is winnable without exploiting this glitch: that's a challenge for you!

I wanted to split up my project into multiple C files but I did not understand how to use header files.  To get around this I #include the various C files, effectively just catencating all of the source files into one.  



## Licensing
All content: GPLv3 
I hereby also publish the contents under GPLv2
If you want a more permissive licence or similiar, just ask.  It's unlikely I'll say no :)
