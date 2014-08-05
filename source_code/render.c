

/* Different areas of the entities.png image contain different things.
 * For example the first row are player pictures whilst the second are
 * of echidnas.
 * 
 * 'SDL_Rect's are simple structures containing the location and size
 * of these individual pictures inside of the larger 'entities.png'
 * 
 * The same thing is done for the different level graphics
 */
 
// Sets the SDL_Rect * crop to the location of specified echinida
// ( there are four echidna pictures, so which_echidna can be 0-3)
void get_pixmap_echidna(int which_echidna, SDL_Rect * crop )
{
	crop->x = which_echidna * TILE_SIZE;
	crop->y = 1 * TILE_SIZE; // all echidnas are in the second row
}

// Same as above, but for the player (lemon)
void get_pixmap_lemon(int which_lemon, SDL_Rect * crop )
{
	crop->x = which_lemon * TILE_SIZE;
	crop->y = 0; // all lemons are in the first row
}

// Getting the rectangle for level tiles is a little harder, as they are spread
// across multiple rows.  Otherwise this does similarly to above modules
void get_pixmap_level( char number, SDL_Rect * crop )
{
	int row, column;
	
	row = 0;
	column = (int)number;
	
	// Compensate for the fact these pictures span multiple rows
	while ( column >= 4 )
	{
		column = column - 4;
		row++; // add one to row's value
	}
	
	crop->x = column * TILE_SIZE;
	crop->y = row * TILE_SIZE;
}




// render() paints everything onto the screen when called
void render( struct level_struct *currentlevel, SDL_Surface * surf_screen )
{
	/* The picture on-screen is made in three steps:
	 *   1) Draw the level onto the screen surface
	 *   2) Draw the player and echidnas ontop of this
	 *   3) 'flip' the screen, so the changes can be seen
	 */
	 
	SDL_Rect destination; // where the image is going to be painted to on-screen
	SDL_Rect pixmap_crop; // where the tile image is on the source image
	destination.w = TILE_SIZE;
	destination.h = TILE_SIZE;
	pixmap_crop.w = TILE_SIZE;
	pixmap_crop.h = TILE_SIZE;
	
	
	/// Level drawing

	// ieterate through each square of the level, drawing the appropriate
	// pictures for each onto the screen

	
	int tilex, tiley; // Where we are up to
	
	for ( tiley=0; tiley < SCREEN_WIDTH_SQUARES; tiley++ )
	{
		for ( tilex=0; tilex < SCREEN_WIDTH_SQUARES; tilex++ )
		{
			/// Determine which section of the entities image we are going to use
			get_pixmap_level( currentlevel->squares[tilex][tiley] , &pixmap_crop );
			// remember that the currentlevel structure contains the array of 
			// level squares/tiles
		
			/// Determine where we are going to draw this image on-screen
			destination.x = tilex * TILE_SIZE;
			destination.y = tiley * TILE_SIZE;

			/// Draw this tile onto the screen
			SDL_BlitSurface ( currentlevel->surf_tiles, &pixmap_crop, surf_screen, &destination );
		}
	}
	
	/// Entity drawing
	// First of all the character
	get_pixmap_lemon( currentlevel->player_face, &pixmap_crop );
	destination.x = currentlevel->px * TILE_SIZE;
	destination.y = currentlevel->py * TILE_SIZE;
	
	// Draw onto screen
	SDL_BlitSurface ( currentlevel->surf_entities, &pixmap_crop, surf_screen, &destination );
	
	// Now draw all of the echidnas
	int ech_no; // Echidna number	 
	
	for ( ech_no = 0; ech_no < 10; ech_no++ )
	{
		struct echidna current_echidna = currentlevel->echidnas[ech_no];
		
		if ( current_echidna.this_echidna_exists == 1 )
		{
			// This echidnas exists and is alive
			
			// Get its location and face
			get_pixmap_echidna( current_echidna.face, &pixmap_crop );
			destination.x = current_echidna.xpos * TILE_SIZE;
			destination.y = current_echidna.ypos * TILE_SIZE;
			
			// Draw it onscreen
			SDL_BlitSurface ( currentlevel->surf_entities, &pixmap_crop, surf_screen, &destination );
		}
			
	}
	
	/// Flip screen, so everything we have 'blitted' can now be seen
	SDL_Flip( surf_screen );
	
}

void render_a_losingscreen(  struct level_struct *currentlevel, SDL_Surface * surf_screen )
{
	// Render a losing screen
	SDL_BlitSurface ( currentlevel->surf_losingscreens[0], NULL, surf_screen, NULL );
	SDL_Flip( surf_screen );
	
	// Wait a second so the user can see the losing screen
	SDL_Delay(500);
}
