
enum weaponName{ElectronGun, HadronGun, BaryonGun, PlasmaGun};

enum bonusType {BONUS_SHIELD, BONUS_QUARKB_A, BONUS_QUARKB_G, BONUS_QUARKT_A, BONUS_QUARKT_G,
	BONUS_QUARKU_A,	BONUS_QUARKU_G, BONUS_QUARKD_A, BONUS_QUARKD_G,  BONUS_QUARKC_A,
	BONUS_QUARKC_G, BONUS_QUARKS_A, BONUS_QUARKS_G, BONUS_LIFE_SMALL, BONUS_LIFE_MEDIUM, BONUS_LIFE_BIG, BONUS_COUNT};

enum LevelState {LEVEL_PLAYING, LEVEL_WON, LEVEL_LOST, LEVEL_FINISHING};

enum QuarkType {QuarkB, QuarkT, QuarkU, QuarkD, QuarkC, QuarkS};

enum heroState{STATIC, HIT, ENTER, DEAD, APPAR, DISPAR, EXITING, CURSOR};

//Definition of the values for the gameState variable
enum GameState {GAME_MENU, GAME_INGAME, GAME_GAMEOVER, GAME_WIN, GAME_PAUSE, GAME_LOADMENU, GAME_LOADLEVEL};

enum MenuState {MENU_MAIN, MENU_HIGHSCORE, MENU_NEWHIGHSCORE, MENU_CREDIT, MENU_GAMEOVER, MENU_SUCCESS, MENU_INTRO, MENU_LEVELSELECT};

enum EnemyType {CADMIUM, IRON, SILICON, COPPER, PYROXENE};

enum particleType {PARTICLE_POINT, PARTICLE_LINE, PARTICLE_DISC, PARTICLE_TEXTURED};

enum explosionType {STANDARD_EXPLOSION, SILICON_EXPLOSION};
