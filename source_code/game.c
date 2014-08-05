#define PLAYER_IS_OK 1
#define PLAYER_IS_DEAD 2
#define PLAYER_HAS_WON 3



// player_action moves the player
// It returns one of the three PLAYER_ define values ( top of this file )
int player_action( signed int xdiff, signed int ydiff, struct level_struct *currentlevel )
{
	// Find the tile the player moved into/onto using the player's current position
	int newx = currentlevel->px + xdiff;
	int newy = currentlevel->py + ydiff;
	
	// Cycle through the player's faces
	currentlevel->player_face++;
	if ( currentlevel->player_face == 4 ) currentlevel->player_face = 0;
	
	// Check to make sure these directions are not solid ( ie walls )
	// and also to see if they are interactive ( ie buttons )
	char targetsquare = currentlevel->squares[newx][newy];
	
	if ( targetsquare == T_WALL || targetsquare == T_YELL_DOOR_UP || targetsquare == T_BLUE_DOOR_UP || targetsquare == T_RED_DOOR_UP ) // remember '||' means 'OR'
	{
		// Cannot move, so do nothing
		return PLAYER_IS_OK;
	}
	else
	{
		// Move onto the new square
		currentlevel->px = newx;
		currentlevel->py = newy;
		
		// Check to see if the new square is interactive ( ie a button )
		
		switch ( targetsquare )
		{
			case T_BLUE_BUTT_UP: // Blue button
				// Change the button to be depressed
				currentlevel->squares[newx][newy] = T_BLUE_BUTT_DOWN;
				// Change all blue doors in the level to be unlocked
				level_replace_tiles( currentlevel, T_BLUE_DOOR_UP, T_BLUE_DOOR_DOWN );
				return PLAYER_IS_OK;
			
			case T_YELL_BUTT_UP: // Yellow button
				// Change the button to be depressed
				currentlevel->squares[newx][newy] = T_YELL_BUTT_DOWN;
				// Change all blue doors in the level to be unlocked
				level_replace_tiles( currentlevel, T_YELL_DOOR_UP, T_YELL_DOOR_DOWN );
				return PLAYER_IS_OK;
				
			case T_EXIT:
				// User is on the exit
				return PLAYER_HAS_WON;
				
			case T_SPIKE:
				// The daft player walked onto spikes
				return PLAYER_IS_DEAD;
				
				
				
			default:
				// This has to be here to stop the compiler complaining
				// Basically the 'default' is run if no other case is run
				return PLAYER_IS_OK;
				
		}
	}
	
	// Code should never reach this point
	// The return line here is just to make the compiler happy
	
	return 0;
	
}


int play_level( struct level_struct *currentlevel, SDL_Surface * surf_screen )
{
	// Loop until the user completes the level
	int player_status = PLAYER_IS_OK;

	while ( player_status == PLAYER_IS_OK )
	{
		SDL_Event userinput;
		
		// WaitEvent is like PollEvent, but instead we pause the program's
		// execution until we get input
		SDL_WaitEvent( &userinput );
		render( currentlevel, surf_screen );
		
		// Player movement
		signed int y, x;
		
		if ( userinput.type == SDL_KEYDOWN )
		{
			switch ( userinput.key.keysym.sym )
			{
				case SDLK_UP:
					x = 0;
					y = -1;
					break;
					
				case SDLK_DOWN:
					x = 0;
					y = 1;
					break;
					
				case SDLK_RIGHT:
					x = 1;
					y = 0;
					break;
				
				case SDLK_LEFT:
					x = -1;
					y = 0;
					break;
				
				case SDLK_q:
					exit(0); // Quit the game
					break;
				
				case SDLK_h:
					// View the help screen
					help( surf_screen );
					x = 0;
					y = 0;
					
				default:
					// The user hits any other key
					// ( do nothing )
					x = 0;
					y = 0;
					break;
			}
			
			// Now move the player
			player_status = player_action( x, y, currentlevel );
			
			// Move all of the echidnas
			run_all_echidnaAI_onestep( currentlevel );
			// And make sure we open/close red doors if any of them are stepping on red levers
			check_echidna_on_switches( currentlevel );
			
			// Check to see if the player has run into an echidna
			if ( check_echidna_proximity( currentlevel ) == 1 ) player_status = PLAYER_IS_DEAD;
			
			// Show a losing screen if applicable
			if (player_status == PLAYER_IS_DEAD) render_a_losingscreen( currentlevel, surf_screen);
		}

	}
	
	
	
	// The code running play_level needs to know if too advance to the next level
	// or just repeat the current one ( if the player has died )
	return player_status;
}

void game_loop( SDL_Surface * surf_screen )
{

	struct level_struct currentlevel;
	level_load_resources( &currentlevel ); // load pixmaps
	
	// Initialise initial player face
	currentlevel.player_face = 2;
	
	int level_number; // Level's number in progression of game, also used as the filename to load the level from
	
	// Run through each of the levels
	
	for ( level_number = 1; level_number <= 10; level_number++ )
	{
		int level_state = PLAYER_IS_OK;
				
		while ( level_state != PLAYER_HAS_WON )
		{
			// Load the level
			level_load( level_number, &currentlevel );
			
			// Set the player's new position
			currentlevel.px = currentlevel.sx;
			currentlevel.py = currentlevel.sy;

			// Main game loop
			level_state = play_level( &currentlevel, surf_screen );

			
		}
		// When the game loop ends, then the user has successfully completed the level, and we can move on
	}
	
	// The user has now completed all of the game
	// Show them the winning screen
	SDL_BlitSurface( currentlevel.surf_winning_screen, NULL, surf_screen, NULL);
	SDL_Flip( surf_screen );
	
	SDL_Delay(20 * 1000); // 20 * 1000msecs = 20 seconds
	
	// Once this function ends, we return to the menu
}
