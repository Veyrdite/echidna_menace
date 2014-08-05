// Standard libraries ( not written by me )
#include <stdio.h>
#include <string.h>

// SDL libraries ( not written by me )
#include "SDL.h"
#include "SDL_image.h"

// Window dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TILE_SIZE 32 // Width and height of 'squares' in the game

#define SCREEN_WIDTH_SQUARES SCREEN_WIDTH / TILE_SIZE
#define SCREEN_HEIGHT_SQUARES SCREEN_HEIGHT / TILE_SIZE

// Modules of the game I have written ( in directory with this source file )
#include "level.c"      // Level structures, loading, other levely stuff
#include "helpscreen.c" // Help-screen code
#include "echidna_ai.c" // Echidna movement A
#include "render.c"     // Normal in-game rendering
#include "game.c"       // Normal in-game code

/* error_sdl is called when SDL 'throws' an error
 * SDL can throw an error if the computer is for example incapable of displaying
 * the game, because it is too old ( ie  made before the year 1998 ) */
void error_sdl( char* reason )
{
	// Print the error message and then close the game
	fprintf( stderr, "Error (SDL): %s: %s\n", reason, SDL_GetError() );
	exit(1);
}




/* The main() module is the module run when you start the program.  Everything
 * starts from here */
int main()
{
	/// Initialisation

	/* First we need to tell SDL to create a window.  A 'surface' is created called surf_screen, 
	 * 'surfaces' are simply images of what we see on-screen. 
	 * 
	 * To make things appear on the screen, I copy ('blit') images onto this surface at different locations 
	 */

	// Initialise SDL
	printf("Initialising SDL... ");
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 ) error_sdl("Could not initialise SDL");
	atexit(SDL_Quit);

	// Set surf_screen
	SDL_Surface* surf_screen = NULL;
	surf_screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF );
	if ( surf_screen == NULL ) error_sdl("Could not set videomode");
	
	printf("done\n");
	
	
	/// Menu
	printf(" ==== Entered menu\n");

	// Events are variables used to store user input
	SDL_Event menuevent;
	// Load the menu graphic into memory
	SDL_Surface *surf_title = IMG_Load( "titlescreen.png" );
	
	
	/* We loop the menu continously for a number of reasons:
	 *  1) To make sure we capture and respond to user input
	 *  2) To continue redrawing the menu graphic.
	 * 
	 * If we did not do number 2 then the window would appear blank if minimised
	 * and then de-minimised again, amongst other situations
	 */
	while ( 1 )
	{
		SDL_BlitSurface( surf_title, NULL, surf_screen, NULL); // Draw menu
		SDL_Flip( surf_screen ); // Update screen

		// The while statement attempts to go through every key the user has pressed
		// it stops when the user has not pressed any more keys
		// this is useful incase the user presses keys fast or multiple at once
		while ( SDL_PollEvent( &menuevent ) )
		{
			if ( menuevent.type == SDL_KEYDOWN )
			{
				// Menu choice selection
				if      ( menuevent.key.keysym.sym == SDLK_p ) game_loop( surf_screen );
				else if ( menuevent.key.keysym.sym == SDLK_h ) help( surf_screen );
				else if ( menuevent.key.keysym.sym == SDLK_q || menuevent.key.keysym.sym == SDLK_ESCAPE ) exit(0);
			}
		}

		// If we do not 'wait' or sleep in every ieteration of the loop
		// the game will loop as fast as it can, utilising 100% of the CPU
		SDL_Delay( 200 ); 
	}
	
	
	
	
	/* Technically the computer will never reach this point, because the while statement
	 * above will run forever until the user quits the game.
	 * 
	 * The 'return' statement is still written here so the compiler does not complain
	 */
	
	return 0;
	
}	
