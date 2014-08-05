
void help( SDL_Surface * surf_screen )
{
	// Load help image
	SDL_Surface *surf_help = IMG_Load( "helpscreen.png" );

	// Draw this image onto screen
	SDL_BlitSurface( surf_help, NULL, surf_screen, NULL);
	SDL_Flip( surf_screen );
	
	// now free the memory we used to draw this image
	free( surf_help );
	
	// Now wait until the user presses a key
	SDL_Event event;
	int waiting_for_user = 1;
	
	while ( waiting_for_user )
	{
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_KEYDOWN )
			{
				waiting_for_user = 0;
				break;
			}
		}
	}
}


