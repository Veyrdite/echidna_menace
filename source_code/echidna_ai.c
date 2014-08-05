
/* The echidna AI has three modes:
 * 
 * 	MODE_HUNT
 *    The echidna tries to close the space between itself and the player
 * 
 *  MODE_FOLLOW_WALL
 *    The echidna has hit a wall and is now following it
 * 
 *  MODE_CORNER
 *    This mode makes the monster move around a corner after it has been following a wall
 * 
 * See the developer documentation for more information about how this large algorithm works.
 */
 
// Give each of the modes a numerical value
#define MODE_HUNT 1
#define MODE_FOLLOW_WALL 2

// The echidna structure is declared in 'level.c' so it can be loaded


// is_square solid returns 1 if a square is not passable ( ie a wall or closed door )
// and 0 if it is ( ie floor, button )
int is_square_solid( struct level_struct *currentlevel, int xpos, int ypos )
{		
	
	switch ( currentlevel->squares[ xpos ][ ypos ] )
	{
		case T_WALL:         return 1;
		case T_BLUE_DOOR_UP: return 1; 
		case T_YELL_DOOR_UP: return 1;
		case T_RED_DOOR_UP:  return 1;
	
		// The default case occurs when none of the above are met
		default: return 0; // ie the square is walkable on
	}
}

// run_all_echidnaAI_onestep runs one 'step' of echidna AI for every echidna in a level
void run_all_echidnaAI_onestep( struct level_struct *currentlevel )
{
	// Ieterate through each and every echinda
	int ech_no; // Echidna number	 
	for ( ech_no = 0; ech_no < 10; ech_no++ )
	{
		struct echidna *current_echidna = &currentlevel->echidnas[ech_no];
		
		
		if ( current_echidna->this_echidna_exists == 1 )
		{
			
			// This echidna is alive and we can move it
			
			// How the echidna is going to move, similiar to the player code;
			signed int x = 0;
			signed int y = 0;
			
			
			/////////////////////////////////////////////////////////////////////////////////////////////// HUNT MODE
			if ( current_echidna->mode == MODE_HUNT )
			{
				// Find the orthoganol ( x and y, not true or diagonal ) distances between the echidna and player
				// NB abs() returns the absolute value of something, which fixes negative numbers by changing
				// them to be position
				int distx = abs( current_echidna->xpos - currentlevel->px );
				int disty = abs( current_echidna->ypos - currentlevel->py );
	
				if ( distx > disty ) 
				{
					
					// Now check whether the player is to the left or right of the echidna
					if ( currentlevel->px > current_echidna->xpos )
					{
						// Echidna is left of player
						x = 1;
					}
					else
					{
						// Echidna is right of player
						x = -1;
					}
				}
				else
				{
					// Is the player abov or below the echidna
					if ( currentlevel->py > current_echidna->ypos )
					{
						// Echidna is below player
						y = 1;
					}
					else
					{
						// Echidna is above player
						y = -1;
					}
				}
				
				// Now check if this is going to make the echidna try to walk into a wall
				// If it will, change mode to MODE_FOLLOW_WALL
				if ( is_square_solid( currentlevel, current_echidna->xpos + x, current_echidna->ypos + y ) == 1 )
				{
					current_echidna->mode = MODE_FOLLOW_WALL;
					
					// Make a decision about which way to move
					// This scheme ensures the echidna moves clockwise
					current_echidna->move_x = 0;
					current_echidna->move_y = 0;
					if      ( x ==  1 ) current_echidna->move_y =  1;
					else if ( x == -1 ) current_echidna->move_y = -1;
					else if ( y ==  1 ) current_echidna->move_x = -1;
					else if ( y == -1 ) current_echidna->move_x =  1;		
					
					// Make sure the echidna checks the wall its following
					// to see if it still exists
					current_echidna->check_x = x;
					current_echidna->check_y = y;
				}
			}
			
			///////////////////////////////////////////////////////////////////////////////////////////// FOLLOW WALL MODE
			if ( current_echidna->mode == MODE_FOLLOW_WALL )
			{
				
				// Check the wall we are following
				if ( is_square_solid( currentlevel, current_echidna->xpos + current_echidna->check_x, current_echidna->ypos + current_echidna->check_y ) == 1 )
				{
					// The wall is still there, so keep following it
					x = current_echidna->move_x;
					y = current_echidna->move_y;
					
					// Check if a new wall is in the way
					while ( is_square_solid( currentlevel, current_echidna->xpos + x, current_echidna->ypos + y ) == 1 )
					{
						// We need to follow another wall now.  Same clockwise logic as before
						current_echidna->check_x = x;
						current_echidna->check_y = y;
						if      ( x ==  1 ) { x = 0; y = 1; }
						else if ( x == -1 ) { x = 0; y =-1; }
						else if ( y ==  1 ) { x =-1; y = 0; }
						else if ( y == -1 ) { x = 1; y = 0; }
						current_echidna->move_x = x;
						current_echidna->move_y = y;
					}
				}
				else
				{
					// The wall has gone, so immediately go around the corner
					x = current_echidna->check_x;
					y = current_echidna->check_y;
					
					
					// And resume HUNT mode next time
					current_echidna->mode = MODE_HUNT;
				}
			}
			
			////////////////////////////////////////////// MOVE THE ECHIDNA
			
			// Move
			current_echidna->xpos = current_echidna->xpos + x;
			current_echidna->ypos = current_echidna->ypos + y;
			
			// Check if we have moved ontop of an Echidna, and if so then move back to where we started
			// ( it will appear as if we have not moved at all )
			
			int target_num;
			for ( target_num = 0; target_num < ech_no; target_num++ ) // Check all of the echidnas BEFORE the current one, so we can have right of way
			{
				struct echidna target_echidna = currentlevel->echidnas[target_num];
				if ( target_echidna.this_echidna_exists == 1 )
				{
					if ( target_echidna.xpos == current_echidna->xpos && target_echidna.ypos == current_echidna->ypos )
					{
						// We have a clash, so move back to the original square
						current_echidna->xpos = current_echidna->xpos - x;
						current_echidna->ypos = current_echidna->ypos - y;
					}
				}
			}
		}
	}
}



// Returns 1 if an echidna is in the same spot as the player
int check_echidna_proximity( struct level_struct *currentlevel )
{
	// Ieterate through eacha and every echinda
	int ech_no; // Echidna number	
	 

	for ( ech_no = 0; ech_no < 10; ech_no++ )
	{
		struct echidna currentechidna = currentlevel->echidnas[ech_no];
		
		// First make sure we are checking an echidna that ACTUALLY EXISTS
		// Until I wrote this code to check that, I was mysteriously dieing halfway
		// throught some levels.  Fun :)
		if ( currentechidna.this_echidna_exists == 1 )
		{
			if ( currentechidna.xpos == currentlevel->px && currentechidna.ypos == currentlevel->py ) // && means 'and'
			{
				// An echidna is atop a player
				return 1;
			}
		}
	}
	
	// No echidna is atop the player
	return 0;
}

// Echidnas are the onlt things that can toggler the red levers, and this loop checks to see if that need to happen
void check_echidna_on_switches( struct level_struct *currentlevel )
{
	// Ieterate through each and every echinda
	int ech_no; // Echidna number	
	 

	for ( ech_no = 0; ech_no < 10; ech_no++ )
	{
		struct echidna currentechidna = currentlevel->echidnas[ech_no];
		
		if ( currentechidna.this_echidna_exists == 1 )
		{
			// Get the square the echinda is standing on
			char square = currentlevel->squares[ currentechidna.xpos ][ currentechidna.ypos ] ;
			
			if ( square == T_RED_SWITCH_ON )
			{
				level_replace_tiles( currentlevel, T_RED_SWITCH_ON, T_RED_SWITCH_OFF ); // change ALL levers
				level_replace_tiles( currentlevel, T_RED_DOOR_UP, T_RED_DOOR_DOWN ); // open all red doors
			}
			else if ( square == T_RED_SWITCH_OFF )
			{
				level_replace_tiles( currentlevel, T_RED_SWITCH_OFF, T_RED_SWITCH_ON ); // change ALL levers
				level_replace_tiles( currentlevel, T_RED_DOOR_DOWN, T_RED_DOOR_UP ); // close all red doors
			}
		}
	}
}


