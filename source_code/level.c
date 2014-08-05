/* 
 * Each of the different types of tile in the levels are numbered 
 * These are also the values used in the level text files
 * 
 * All of the level tiles are stored in one image, hence the 'Rows'
 * below are literally the rows there are positioned in the image.
 * Have a look at tiles.png in the game's folder to see them all.
 */

// Row 1 - main squares
#define T_WALL  0
#define T_FLOOR 1
#define T_SPIKE 2
#define T_EXIT  3
// Row two - blue button related
#define T_BLUE_BUTT_UP 4   // unpressed button
#define T_BLUE_BUTT_DOWN 5 // depressed button
#define T_BLUE_DOOR_UP 6   // door unlocked by a button
#define T_BLUE_DOOR_DOWN 7 // door locked and impassible
// Row three - yellow button related
#define T_YELL_BUTT_UP 8
#define T_YELL_BUTT_DOWN 9
#define T_YELL_DOOR_UP 10
#define T_YELL_DOOR_DOWN 11
// Row four - red lever/switch related
#define T_RED_SWITCH_ON 12
#define T_RED_SWITCH_OFF 13
#define T_RED_DOOR_UP 14
#define T_RED_DOOR_DOWN 15

// An echidna is one of the monsters
// The structure is define here in level.c so it can be loaded from the level files
struct echidna 
{
	int xpos, ypos; // Position on map
	int face; // Which of the echidna pictures to use
	
	int mode; // As described at the top of the document
	
	int this_echidna_exists; // When this is equal to 1, the echidna is alive and on the level
	
	// These movement directions are used in the AI code
	signed int move_x;
	signed int move_y;
	// ditto here
	signed int check_x;
	signed int check_y;

};

/* The level structure is passed to the rendering module, so everything that needs to be seen
 * on-screen is also stored here as well
 * 
 * Eg:
 *    - player position
 *    - resource locations ( graphics )
 */
struct level_struct
{

	char squares[SCREEN_WIDTH_SQUARES][SCREEN_HEIGHT_SQUARES]; // Squares of the level

	// Player details
	int player_face;  // Picture number ( player has 4 different looks )
	int px, py;  // Player X and Y position, on the tiles
	int sx, sy;  // Player X and Y starting positions
	
	// Resources
	SDL_Surface * surf_entities; // Player and echidna pictures
	SDL_Surface * surf_tiles;    // Level tiles/squares
	
	// A various selection of losing screens
	SDL_Surface * surf_losingscreens[5];
	
	// The screen to display when the game ends
	SDL_Surface * surf_winning_screen;
	
	// Up to ten echidnas
	struct echidna echidnas[10];
	
};



/* eat_until_newline will 'eat' one character at a time from a file until it detects it has
 * finished a line, then stops.  This is used to make sure a line of text in the level files
 * has been finished before we move on to the next */
void eat_until_newline( FILE *currentfile )
{
	char letter='a';
	while ( letter != '\n' ) // The '\n' represents the invisible line ending characters (either CRLF or LF)
	{
		// Every time a character is read, the current position inside of the file is also moved forward
		// one character
		letter = fgetc( currentfile );
	}
}



// Load the graphics used in levels
void level_load_resources( struct level_struct *level )
{
	// Load the images for things into their own 'surfaces'
	level->surf_entities = IMG_Load( "entities.png" );
	level->surf_tiles = IMG_Load( "tiles.png" );
	
	// Do the same for the losing screens
	level->surf_losingscreens[0] = IMG_Load("failscreen_01.png");
	level->surf_winning_screen = IMG_Load("winningscreen.png");
	
}



// level_load will load a level from a text file into a level_struct
void level_load( int level_number, struct level_struct *level )
{
	
	// Create the level's actual filename from its number
	char levelname[20];
	sprintf( levelname, "level_%02d.level", level_number );
	
	// Attempt to open the file
	FILE *levelfile = fopen( levelname, "r"); // r = reading mode


	// Check if the file loaded, quit program if this has failed
	if ( levelfile == NULL )
	{
		// For some reason the file cannot be accessed
		// This could be because it does not exist or we don't have permission to open it

		fprintf(stderr, "Error: Could not open level file %d, perhaps it is somewhere else or does not exist?\n", level_number );
		fprintf(stderr, "Exiting... \n\n" );

		exit(1);
	}
	
	// First we need to skip the first line of the file, which is dedicated to title, author etc information
	eat_until_newline( levelfile );
	
	// The second line contains the starting position of the player
	fscanf( levelfile, "%d %d", &level->sx, &level->sy );
	eat_until_newline(levelfile);
	
	printf("Loading level %s, player at position: %d %d\n",  levelname,  level->sx, level->sy );

	/* Ieterate through the file and load the squares into the level's structure
	 * Each row/line in the file represents a row of squares in the level
	 * Each square is represented in the rows by a number from 0 to 255
     * Open up one of the level files with a text editor to study its structure further
	 */

	/* WARNING:  This code assumes the file is structured correctly in the first place!
	 * The game will behave in an unexpected manner if the level files are not perfect!
	 */

	int row, column; // Current position
	
	for ( row=0; row < SCREEN_HEIGHT_SQUARES; row++ )
	{
		for ( column=0; column < SCREEN_WIDTH_SQUARES; column++ )
		{
			/* we read each two letter number into a three letter character array ( string )
			 * the third character is used to represent the end or 'termination' of the string
			 */
			char tempstring[3];
			fgets( tempstring, 3, levelfile );

			// atoi() is a standard C function used to covert a string of numbers in character
			// form into an actual number ( integer )
			level->squares[column][row] = atoi( tempstring ); 
		}
		
		// We need to make sure that we finish the current line of numbers/characters in the
		// level file, and are up to the next, else bad things may happen
		eat_until_newline( levelfile );
	}
	
	/* Now start loading the echidna positions
	 * Every line after the main level tile body is an echidna
	 * The format for each line is:
	 *   xpos ypos face
	 * Where each of them is a number, face being a number from 0 to 3 choosing the particular face of the echidna
	 */
	 
	int ech_no; // Echidna number	 
	
	for ( ech_no = 0; ech_no < 10; ech_no++ )
	{
		level->echidnas[ech_no].this_echidna_exists = 0;
		level->echidnas[ech_no].mode = 1; // Start in hunt mode
		
		// Now load the echidna values in
		if ( fscanf( levelfile, "%d %d %d", &level->echidnas[ech_no].xpos, &level->echidnas[ech_no].ypos, &level->echidnas[ech_no].face  ) == 3 )
		{
			// When fscanf returns a number other than '3' ( which is how many numbers we are reading per line )
			// we know there are no more lines in the file and we should stop, hence the == 3
			level->echidnas[ech_no].this_echidna_exists = 1;
			
			printf("Echidna loaded at x:%d y:%d\n", level->echidnas[ech_no].xpos, level->echidnas[ech_no].ypos );
		}
	}
	 
	
}



// Replace every tile in the level of firsttype with secondtype
// Useful for eg opening doors
void level_replace_tiles( struct level_struct *currentlevel, char firsttype, char secondtype )
{
	int tilex, tiley; // Where we are up to
	
	for ( tiley=0; tiley < SCREEN_WIDTH_SQUARES; tiley++ )
	{
		for ( tilex=0; tilex < SCREEN_WIDTH_SQUARES; tilex++ )
		{
			if ( currentlevel->squares[tilex][tiley] == firsttype ) currentlevel->squares[tilex][tiley] = secondtype;
		}
	}
}












