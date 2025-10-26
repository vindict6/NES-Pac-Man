	//update_list[17]=0x10+dots_remaining/100;
	//update_list[18]=0x10+dots_remaining/10%10;
	//update_list[19]=0x10+dots_remaining%10;
	//update_list[20]=0x00;
	//update_list[21]=0x10+quadbitnum%10;
	//update_list[22]=0x00;
	//update_list[23]=0x10+not_eaten;
	//update_list[23]=0x10+((quadval >> quadbitnum)  & 0x01);

#include ".\lib\neslib.h"
#define MAX_LIVES 5
#define GOD_MODE 0
#define NUM_OF_MAPS 4
#define DEBUG 0

//#pragma bssseg (push,"ZEROPAGE")
//#pragma dataseg (push,"ZEROPAGE")

//#pragma bssseg (push,"BSS")
//#pragma dataseg (push,"BSS")
//Prototypes
void wait(unsigned char);
void waitframes(unsigned char);
void reset_ghosts(void);
void update_screen(char);
void move_out_house(char);
void move_flip(char);
void move_vertical_flip(char);
void move_randomly(char);
void move_towards_target(char);
void do_ghost(void);
void draw_map(char);
void draw_title(void);
void remove_dot(unsigned char);
void setup_map(char);
void do_pac(void);
void setMasterPhase(void);
void do_pac_and_ghosts(void);
void do_main_counters(void);
void doMenu(void);
void main(void);

//Constants
const unsigned char FOUR_CHAR_ZEROS[4] = {0,0,0,0};

const unsigned char title_anim_x[68] = { //row 1
									   16, 24, 32, 40, 48, 56, 64,
									   72, 80, 88, 96,104,112,120,
									  128,136,144,152,160,168,176,
									  184,192,200,208,216,224,232,
									
									  //row 2-8
									  232,232,232,232,232,232,232,
									  
									  //row 8
									  224,216,208,200,192,184,176,
									  168,160,152,144,136,128,120,
									  112,104, 96, 88, 80, 72, 64,
									   56, 48, 40, 32, 24, 16,
									   
									  //row 7-1 back
									  16,16,16,16,16,16
};
const unsigned char title_anim_y[68] = {//row 1
									  47,47,47,47,47,47,47,
									  47,47,47,47,47,47,47,
									  47,47,47,47,47,47,47,
									  47,47,47,47,47,47,47,
									  //rows 2-7
									  55, 63, 71, 79, 87, 95,//-
									  //row 8 back
									 103,103,103,103,103,103,103,
									 103,103,103,103,103,103,103,
									 103,103,103,103,103,103,103,
									 103,103,103,103,103,103,103,
									  //rows 7-2 back	
									  95, 87, 79, 71, 63, 55
									 
	
};

//music/sounds
const unsigned char mus_title[250] = {
	 0x80,0xb5,0x81,0x80,0x82,0x02,0x83,0xb5,0x84,0x3a,0x85,0x02,0x0a,0x80,0x74,0x81,
	 0xfb,0x82,0x01,0x04,0x83,0xb4,0x06,0x81,0xab,0x0a,0x80,0xb5,0x81,0x80,0x82,0x02,
	 0x07,0x83,0xb3,0x02,0x80,0x74,0x81,0xfb,0x82,0x01,0x0a,0x81,0xab,0x0a,0x80,0xb5,
	 0x81,0x80,0x82,0x02,0x83,0xb2,0x09,0x80,0x74,0x81,0xfb,0x82,0x01,0x06,0x83,0xb1,
	 0x04,0x80,0xb5,0x81,0x3a,0x82,0x02,0x83,0xb5,0x84,0x80,0x0a,0x80,0x74,0x81,0xc4,
	 0x82,0x01,0x04,0x83,0xb4,0x05,0x81,0x7c,0x0a,0x80,0xb5,0x81,0x3a,0x82,0x02,0x08,
	 0x83,0xb3,0x02,0x80,0x74,0x81,0xc4,0x82,0x01,0x09,0x81,0x7c,0x0a,0x80,0xb5,0x81,
	 0x3a,0x82,0x02,0x01,0x83,0xb2,0x09,0x80,0x74,0x81,0xc4,0x82,0x01,0x06,0x83,0xb1,
	 0x03,0x80,0xb5,0x81,0xfb,0x83,0xb5,0x84,0xa6,0x0a,0x80,0x74,0x81,0xab,0x04,0x83,
	 0xb4,0x06,0x81,0x52,0x09,0x80,0xb5,0x81,0xfb,0x08,0x83,0xb3,0x02,0x80,0x74,0x81,
	 0xab,0x0a,0x81,0x52,0x09,0x80,0xb5,0x81,0xfb,0x01,0x83,0xb2,0x09,0x80,0x74,0x81,
	 0xab,0x06,0x83,0xb1,0x04,0x80,0xb5,0x81,0xfb,0x83,0xb5,0x84,0xf8,0x85,0x03,0x09,
	 0x80,0x74,0x81,0xab,0x05,0x83,0xb4,0x05,0x81,0x52,0x83,0xb5,0x84,0x89,0x0a,0x80,
	 0xb5,0x81,0xfb,0x04,0x83,0xb4,0x05,0x80,0x74,0x81,0xab,0x83,0xb5,0x84,0x56,0x0a,
	 0x81,0x52,0x04,0x83,0xb4,0x06,0x80,0xb5,0x81,0xfb,0x83,0xb5,0x84,0x74,0x85,0x04,
	 0x09,0x80,0x74,0x81,0xab,0x05,0x83,0xb4,0x03,0x00
};
const unsigned char * music[1]={mus_title};
//Title Screen
static unsigned char title_counter=0;
static unsigned char title_phase=0;
static unsigned char selection=0;
static unsigned char selected=0;
static unsigned char last_button=0;
static unsigned int button_time=0;
static unsigned char title_done=0;
static unsigned char title_anim=0;
static unsigned char title_dot_anim[6]={67,0,1, 33,34,35};

//General Variables
static unsigned char controller;
static unsigned char j;
static unsigned char i;
static unsigned int k;
static unsigned char pacman_can_animate;
static unsigned char pacman_animation_divider;
static unsigned char pacactive;
static unsigned char master_phase_counter;
static unsigned char pacdead=0;
static unsigned char hideghosts=0;
static unsigned char lives = MAX_LIVES;
static unsigned char taken_first_life = 0;
static unsigned char gameover=0;
static unsigned char render_phase=0;
static unsigned char inventory_count=0;


//Fruits
static unsigned char fruit_activated = 0;
static unsigned char fruit_hidden = 1;
static unsigned char fruit_shown_at_score = 0;
static unsigned char current_fruit = 0;
static unsigned int c_fruit_timeout = 0;
const unsigned int fruit_score[8]={10,30,50,70,100,200,300,500};
const unsigned char fruit_palette_top[8]=   {3,3,3,3,3,1,0,0};
const unsigned char fruit_palette_bottom[8]={3,3,3,3,3,0,0,0};
static unsigned char inventory[4]={0,0,0,0};//inventory
static unsigned char do_inventory_update=0;

//score sprites
static unsigned int c_fruit_score_timeout = 0;
const unsigned char fruit_score_card[8]={0x80,0x82,0x83,0x84,0x80,0x81,0x82,0x83};
const unsigned char fruit_score_mult[8]={0x86,0x86,0x86,0x86,0x85,0x85,0x85,0x85};
const unsigned char enemy_score_card[4]={0x53,0x55,0x59,0x5A};
const unsigned char enemy_score_mult[4]={0x5C,0x5C,0x5C,0x5B};
const unsigned int ENEMY_SCORE[4]={20,40,80,160};
static unsigned char enemies_ate = 0;
static unsigned char escorevalue[4]={0,0,0,0};

//Player Position/Orientation Variables
const unsigned char pacstartx = 124;
const unsigned char pacstarty = 168;
static unsigned char pacx;
static unsigned char pacy;
static unsigned char pdir=2;
static unsigned int c_seconds=1;//sets master_phase
static unsigned char c_speed=0;

//Scorekeeping Variables
static unsigned char score[7]={0,0,0,0,0,0,0};
static unsigned char queued_score=0;
static unsigned int map_score=0;
static unsigned char dots_remaining;

//Palettes
const unsigned char PAL_BG[4]={0x0f,0x30,0x12,0x28};
const unsigned char PAL_MAP[NUM_OF_MAPS]={0x12,0x16,0x19,0x14 };
const unsigned char PAL_SPR[16]={ 0x0f,0x28,0x38,0x11, 0x0f,0x05,0x12,0x20, 0x0f,0x16,0x24,0x20, 0xff,0x06,0x19,0x37 };
static unsigned char c_map_palette;

//Menus
static unsigned char current_menu=0;

static unsigned char drawn_menu=20;

static unsigned char menu1[36]={//main menu
	0x00,0x33,0x34,0x21,0x32,0x34,0x00,0x27,0x21,0x2d,0x25,0x00,
	0x2c,0x25,0x36,0x25,0x2c,0x00,0x33,0x25,0x2c,0x25,0x23,0x34,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00//0x00,0x00,0x33,0x25,0x34,0x34,0x29,0x2e,0x27,0x33,0x00,0x00
};

static unsigned char menu2[36]={//level select
	0x00,0x00,0x00,0x2c,0x25,0x36,0x25,0x2c,0x1a,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



const unsigned char * MENU[2]={menu1,menu2};
static unsigned char menu_size[2]={2,1};

static unsigned char menu_type[2]={0,1};

static unsigned char level_selection = 1;

//Drawing Variables
static unsigned char title_list[46]={ //10 11 12
	MSB(NTADR_A(10,19))|NT_UPD_HORZ , LSB(NTADR_A(10,19)) , 12 , 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	MSB(NTADR_A(10,21))|NT_UPD_HORZ , LSB(NTADR_A(10,21)) , 12 , 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	MSB(NTADR_A(10,23))|NT_UPD_HORZ , LSB(NTADR_A(10,23)) , 12 , 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	NT_UPD_EOF
};

static unsigned char update_list[90]={
	MSB(NTADR_A(0,0))|NT_UPD_HORZ , LSB(NTADR_A(0,0)) , 1 , 0x00 ,
	MSB(NTADR_A(13,2))|NT_UPD_HORZ , LSB(NTADR_A(13,2)) , 7 ,//score
	0x10,0x10,0x10,0x10,0x10,0x10,0x10, //7-13
	
	MSB(NTADR_A(14,4))|NT_UPD_HORZ,LSB(NTADR_A(14,4)),4,//debug
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,//17-20 -- 21-23
	
	MSB(NTADR_A(11,17))|NT_UPD_HORZ,LSB(NTADR_A(11,17)),10,//ready text
	0x00,0x00,0x72,0x73,0x74,0x75,0x76,0x77,0x00,0x00, //27,28,29,30,31,32,33,34,35,36
	
	MSB(NTADR_A(0,0))|NT_UPD_HORZ,LSB(NTADR_A(0,0)),1,0xFE,//BIG dots //37,38,39,40
	MSB(NTADR_A(0,0))|NT_UPD_HORZ,LSB(NTADR_A(0,0)),1,0xFE,//41,42,43,44
	MSB(NTADR_A(0,0))|NT_UPD_HORZ,LSB(NTADR_A(0,0)),1,0xFE,//45,46,47,48
	MSB(NTADR_A(0,0))|NT_UPD_HORZ,LSB(NTADR_A(0,0)),1,0xFE,//49,50,51,52	
	
	MSB(NTADR_A(1,1))|NT_UPD_HORZ,LSB(NTADR_A(1,1)),10,//lives 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //56-65
	MSB(NTADR_A(1,2))|NT_UPD_HORZ,LSB(NTADR_A(1,2)),10,//lives 2 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //69-78
	
	MSB(NTADR_A(21,1))|NT_UPD_HORZ,LSB(NTADR_A(21,1)),7,//trophies
	0x00,0x00,0x00,0x00,0x00,0x00,0x00, //82-88

	NT_UPD_EOF
};
static unsigned char showtargets=0;
static unsigned char pac_death_counter=0;
static unsigned char pac_death_done=0;

//Map Variables
static unsigned char big_dot_exists[4];
static unsigned char big_dot_x[4];
static unsigned char big_dot_y[4];
static unsigned char c_big_dot_placed;
static unsigned char dot_bit_row[25][4]; //24 rows with 4 bytes split into bits for the dots
static unsigned char current_map_index;//used to traverse within 0 and NUM_OF_MAPS for collisions.
static unsigned int c_map_number; //used to count how many maps we have done total.

const unsigned char map0[928]={//32*29. After generating map, remove the last row.
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x23,0x2f,0x32,0x25,0x1a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x5f,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x60,0x7c,0x7d,0x7e,0x7f,0x5f,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x60,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,0x00,0x00,0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x3d,0x3f,0x3e,0xff,0x48,0x5b,0x5b,0x5b,0x5b,0x64,0xff,0x3d,0x3f,0x3e,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x5e,0x00,
	0x00,0x5d,0xfe,0x4b,0x3c,0x3c,0x3c,0x40,0xff,0x4b,0x3c,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4b,0x3c,0x40,0xff,0x4b,0x3c,0x3c,0x3c,0x40,0xfe,0x5e,0x00,
	0x00,0x5d,0xff,0x48,0x4a,0x4a,0x4a,0x49,0xff,0x48,0x4a,0x49,0xff,0x52,0x54,0x46,0x47,0x54,0x53,0xff,0x48,0x4a,0x49,0xff,0x48,0x4a,0x4a,0x4a,0x49,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4b,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x4b,0x40,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x4b,0x3c,0x3c,0x3c,0x40,0xff,0x4b,0x41,0x4a,0x4a,0x49,0xff,0x48,0x49,0xff,0x48,0x4a,0x4a,0x42,0x40,0xff,0x4b,0x3c,0x3c,0x3c,0x40,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x48,0x4a,0x4a,0x4a,0x49,0xff,0x4b,0x40,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0x4b,0x40,0xff,0x48,0x4a,0x4a,0x4a,0x49,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4b,0x40,0xfd,0x63,0x43,0x43,0x45,0x45,0x43,0x43,0x61,0xfd,0x4b,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x57,0x54,0x54,0x54,0x54,0x54,0x53,0xff,0x48,0x49,0xfd,0x4f,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0x44,0xfd,0x48,0x49,0xff,0x52,0x54,0x54,0x54,0x54,0x54,0x56,0x00,
	0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xfd,0x4f,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0x44,0xfd,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,
	0x00,0x58,0x54,0x54,0x54,0x54,0x54,0x53,0xff,0x52,0x53,0xfd,0x4f,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0x44,0xfd,0x52,0x53,0xff,0x52,0x54,0x54,0x54,0x54,0x54,0x55,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfd,0x62,0x4e,0x4e,0x4e,0x4e,0x4e,0x4e,0x49,0xfd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x3d,0x3e,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0x3d,0x3e,0xff,0x3d,0x3f,0x3f,0x3f,0x3e,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x4b,0x3c,0x3c,0x3c,0x40,0xff,0x4b,0x40,0xff,0x3d,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3e,0xff,0x4b,0x40,0xff,0x4b,0x3c,0x3c,0x3c,0x40,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x48,0x4a,0x4a,0x42,0x40,0xff,0x4b,0x40,0xff,0x4b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x40,0xff,0x4b,0x40,0xff,0x4b,0x41,0x4a,0x4a,0x49,0xff,0x5e,0x00,
	0x00,0x5d,0xfe,0xff,0xff,0xff,0x4b,0x40,0xff,0x48,0x49,0xff,0x48,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x49,0xff,0x48,0x49,0xff,0x4b,0x40,0xff,0xff,0xff,0xfe,0x5e,0x00,
	0x00,0x50,0x5c,0x5c,0x5a,0xff,0x4b,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfd,0xfd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4b,0x40,0xff,0x3d,0x5c,0x5c,0x59,0x00,
	0x00,0x00,0x00,0x00,0x5d,0xff,0x4b,0x40,0xff,0x3d,0x3e,0xff,0x3d,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3e,0xff,0x3d,0x3e,0xff,0x4b,0x40,0xff,0x5e,0x00,0x00,0x00,0x00,
	0x00,0x5f,0x5b,0x5b,0x51,0xff,0x48,0x49,0xff,0x4b,0x40,0xff,0x48,0x4a,0x4a,0x42,0x41,0x4a,0x4a,0x49,0xff,0x4b,0x40,0xff,0x48,0x49,0xff,0x48,0x4e,0x4e,0x65,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4b,0x40,0xff,0xff,0xff,0xff,0x4b,0x40,0xff,0xff,0xff,0xff,0x4b,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x3d,0x3f,0x3f,0x3f,0x3f,0x3f,0x4d,0x4c,0x3f,0x3f,0x3e,0xff,0x4b,0x40,0xff,0x3d,0x3f,0x3f,0x4d,0x4c,0x3f,0x3f,0x3f,0x3f,0x3f,0x3e,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0x48,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x49,0xff,0x48,0x49,0xff,0x48,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x49,0xff,0x5e,0x00,
	0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5e,0x00,
	0x00,0x50,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x59,0x00
};

const unsigned char title[736]={
	0x00,0x5f,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x5b,0x60,0x00,
	0x00,0x5d,0xee,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xee,0x5e,0x00,
	0x00,0x5d,0xef,0x3d,0x3f,0x91,0x00,0x3d,0x3f,0x3f,0x3e,0xaa,0x3f,0x3f,0xab,0x3d,0x97,0x00,0x00,0xa0,0x3e,0x3d,0x3f,0x3f,0x3e,0x3d,0x97,0x3d,0x3e,0xef,0x5e,0x00,
	0x00,0x5d,0xef,0x4b,0x41,0xa1,0xa2,0x4b,0x41,0x42,0x40,0xac,0x42,0x41,0xad,0x4b,0x98,0x9b,0x9e,0x9f,0x40,0x4b,0x41,0x42,0x40,0x4b,0x98,0x95,0x40,0xef,0x5e,0x00,
	0x00,0x5d,0xef,0x4b,0x40,0xa8,0xa6,0x4b,0x40,0x4b,0x40,0x00,0x4b,0x40,0x00,0x4b,0x96,0x9c,0x9d,0xa3,0x40,0x4b,0x4c,0x4d,0x40,0x4b,0x96,0x99,0x40,0xef,0x5e,0x00,
	0x00,0x5d,0xef,0x4b,0x40,0xa9,0xa7,0x4b,0x40,0x4b,0x40,0x00,0x4b,0x40,0x00,0x4b,0x40,0xa5,0xa4,0x4b,0x40,0x4b,0x41,0x42,0x40,0x4b,0x40,0x9a,0x40,0xef,0x5e,0x00,
	0x00,0x5d,0xef,0x4b,0x4c,0xae,0xaf,0x4b,0x4c,0x4d,0x40,0x00,0x4b,0x40,0x00,0x4b,0x40,0x00,0x00,0x4b,0x40,0x4b,0x40,0x4b,0x40,0x4b,0x40,0x4b,0x40,0xef,0x5e,0x00,
	0x00,0x5d,0xef,0x48,0x4a,0x90,0x00,0x48,0x4a,0x4a,0x49,0x00,0x48,0x49,0x00,0x48,0x49,0x00,0x00,0x48,0x49,0x48,0x49,0x48,0x49,0x48,0x49,0x48,0x49,0xef,0x5e,0x00,
	0x00,0x5d,0xee,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xee,0x5e,0x00,
	0x00,0x50,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x59,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb1,0x00,0x33,0x34,0x21,0x32,0x34,0x00,0x27,0x21,0x2d,0x25,0x00,0xb1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2c,0x25,0x36,0x25,0x2c,0x00,0x33,0x25,0x2c,0x25,0x23,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x25,0x34,0x34,0x29,0x2e,0x27,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3b,0x12,0x10,0x12,0x12,0x00,0x36,0x29,0x2e,0x33,0x29,0x38,0x00,0x27,0x21,0x2d,0x25,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};




const unsigned char * MAP[NUM_OF_MAPS]={map0,map0,map0,map0};

//Enemy Position/Orientation Variables
static unsigned char enx[4];
static unsigned char eny[4];
static unsigned char etx[4];
static unsigned char ety[4];
const unsigned char ENEMY_START_X[4]={124, 140, 108, 124};
const unsigned char ENEMY_START_Y[4]={88, 112, 112, 112};
const unsigned char OPPOSITE[4] = {2,3,0,1};
const unsigned char ENEMY_START_DIR[4]={2,1,1,1};
static unsigned char endir[4];
static unsigned char enanim;
static unsigned char coinanim;
//Enemy Target Variables
//Enemy Movement Phase Variables
const unsigned char ENEMY_START_PHASE[4] = {2,0,0,0};
static unsigned char enemy_movement_phase[4]={2,0,0,0};
static unsigned char enemy_last_phase[4]={2,0,0,0};
static unsigned char master_phase = 2;
static unsigned char exiting_house_eaten[4];
static unsigned char c_exit_house_delay[4]={1,1,1,1};
static unsigned char final_phase_flip;
//Enemy Scared State Variables
const unsigned char enrandpath0[4]={1,0,2,3};
const unsigned char enrandpath1[4]={0,2,3,1};
const unsigned char enrandpath2[4]={2,3,1,0};
const unsigned char enrandpath3[4]={3,1,0,2};
const unsigned char * RANDOM_DIRECTION[5]={enrandpath0,enrandpath1,enrandpath2,enrandpath3,enrandpath2}; //fifth one to fix 4 square roundabouts
static unsigned char c_randomize[4];
static unsigned char enemy_scared[4];
static unsigned char enemy_scared_counter[4];
static unsigned char c_enemy_slowdown=0;
static unsigned char c_enemy_score_timeout[4];
static unsigned char c_scared_ticks;
static unsigned char c_scared_seconds;
static unsigned char flip_enemies_buffer = 0;


//scared mode
const unsigned char ENEMY_FRIGHT_DURATION[18] = {6,5,4,3,2,6,2,2,1,5,2,1,1,3,1,1,0,1};
const unsigned char ENEMY_FRIGHT_FLASHES[18] =  {5,5,5,5,5,5,5,5,3,5,5,3,3,5,3,3,0,3};
static unsigned char enemy_flashed;
static unsigned char c_enemy_scared_flashed[4];
//dot counter
static unsigned char c_dots_eaten[4] = {0,0,0,0};//ghosts' dot counters
static unsigned char dots_eaten=0;
static unsigned char c_dots_eaten_enabled = 1; //if any ghost dot counter is active.
static unsigned char sel_dot_counter_ghost = 4;//which ghost's dot counter is active.
static unsigned char global_dot_counter=0;
//level start, 


//variables to keep update_list from updating every frame
static unsigned char lastLivesUpdate;
static unsigned int lastMapUpdate=0;
static unsigned char big_dot_removed[4];

//const unsigned char test[10000];

void wait(unsigned char s)
{
	unsigned char seconds = s;
	for(k=0;k<seconds*60;k++){delay(1);controller=pad_poll(0);}
}
void waitframes(unsigned char f)
{
	unsigned char frames = f;
	for(k=0;k<frames;k++){delay(1);controller=pad_poll(0);}
}

void addScore(int amount)
{
	unsigned int s = amount;
	
	queued_score=s;//score is/10 before passing to function
	map_score+=s;
	
	
	if(queued_score>9)//add hundreds to score
	{
		for(k=0;k<queued_score/10;k++)
		{
			score[4]++;
			for(j=5;j>1;j--)
			{
				if(score[j-1]==0x0a)
				{
					score[j-1]=0;
					score[j-2]++;
				}
				else
				{
					break; //save time by stopping when a carry over is performed
				}
			}
		}
	}
	else//add tens to score
	{
		for(k=0;k<queued_score;k++)
		{
			score[5]++;
			for(j=6;j>1;j--)
			{
				if(score[j-1]==0x0a)
				{
					score[j-1]=0;
					score[j-2]++;
				}
				else
				{
					break; //save time by stopping when a carry over is performed
				}
			}
		}
	}
	update_list[7]=0x10+score[0];
	update_list[8]=0x10+score[1];
	update_list[9]=0x10+score[2];
	update_list[10]=0x10+score[3];
	update_list[11]=0x10+score[4];
	update_list[12]=0x10+score[5];
}

char checkFruitRow()
{
	unsigned char counter = 0;
	//20,21,22 160-176
	if(pacy>124 && pacy<148)
	{
		counter++;
	}
	for(i=0;i<4;i++)
	{
		if(eny[i]>128 && eny[i]<144)
		{
			counter++;
		}
	}
	return counter;
}
void update_screen(char all_phases)
{	
	unsigned char animation_offset = enanim>8;
	unsigned char allphases = all_phases;
	unsigned int displayed_map = c_map_number+1;
	unsigned char color_alternator = 0;
	unsigned char sprite_alternator = 0;
	unsigned char index_offset = 0;
	
	if(enanim<18) enanim++;
	else enanim=0;
	if(coinanim<31) coinanim++;
	else coinanim=0;
	
	
	for(i=0;i<4;i++)
	{
		if(big_dot_removed[i]==0)
		{
			if(big_dot_exists[i])
			{
				update_list[40+(4*i)]=0x66+(coinanim/4);
			}
			else
			{
				big_dot_removed[i]=1;
				update_list[40+(4*i)]=0x00;
			}
		}
	}
	
	//update_list updates
	if(lastLivesUpdate!=lives)
	{
		for(i=0;i<5;i++)
		{
			if(i<lives)//for each life
			{
				update_list[65-(2*i)]=0x6f;//top right
				update_list[64-(2*i)]=0x6e;//top left
				update_list[78-(2*i)]=0x71;//bottom right
				update_list[77-(2*i)]=0x70;//bottom left
			}
			else//draw blank
			{
				update_list[65-(2*i)]=0x00;//top right
				update_list[64-(2*i)]=0x00;//top left
				update_list[78-(2*i)]=0x00;//bottom right
				update_list[77-(2*i)]=0x00;//bottom left
			}
		}
		lastLivesUpdate = lives;
	}
	if(lastMapUpdate!=displayed_map && DEBUG==0)
	{
		lastMapUpdate=displayed_map;
		
		if(displayed_map<10)
		{
			update_list[17]=0x00;
			update_list[18]=0x03;
			update_list[19]=0x10+displayed_map%10;
			update_list[20]=0x00;
		}
		else if(displayed_map<100)
		{
			update_list[17]=0x00;
			update_list[18]=0x10+displayed_map/10%10;
			update_list[19]=0x10+displayed_map%10;	
			update_list[20]=0x00;
		}
		else if(displayed_map<1000)
		{
			update_list[17]=0x00;
			update_list[18]=0x10+displayed_map/100;
			update_list[19]=0x10+displayed_map/10%10;
			update_list[20]=0x10+displayed_map%10;
		}
		else
		{
			update_list[17]=0x10+displayed_map/1000%10;
			update_list[18]=0x10+displayed_map/100%10;
			update_list[19]=0x10+displayed_map/10%10;
			update_list[20]=0x10+displayed_map%10;
		}
	}
	if(DEBUG==1)
	{
		update_list[17]=0x10+pdir/100%10;
		update_list[18]=0x10+pdir/10%10;
		update_list[19]=0x10+pdir%10;
	}

	//if(showtargets)
	//{
		//ghost 0 target - Red
		//oam_spr(etx[0],ety[0]-1,0x62+(enanim<10),1,4);
		
		//ghost 1 target - Orange
		//oam_spr(etx[1],ety[1]-1,0x63-(enanim<10),2,8);
		
		//ghost 2 target - Blue
		//oam_spr(etx[2],ety[2]-1,0x60+(enanim<10),1,12);
		
		//ghost 3 target - Pink
		//oam_spr(etx[3],ety[3]-1,0x60+(enanim<10),2,16);
	//}

	if(pacdead==0 && hideghosts==0)
	{
		for(i=0;i<4;i++)
		{
			if(i%2==render_phase || all_phases==1)//proud of this.
			{
				index_offset=(16*i);
				if(enemy_scared[i]==1)
				{
						//scared
						oam_spr((enx[i]-4),(eny[i]-4),0x44+enemy_flashed,1,index_offset);
						oam_spr((enx[i]+4),(eny[i]-4),0x44+enemy_flashed,65,4+index_offset);
						oam_spr((enx[i]-4),(eny[i]+4),0x46+enemy_flashed+(2*animation_offset),1,8+index_offset);
						oam_spr((enx[i]+4),(eny[i]+4),0x46+enemy_flashed+(2*animation_offset),65,12+index_offset);
					
				}
				else
				{
					if(enemy_movement_phase[i]==5)//eyes
					{
						oam_spr((enx[i]-4),(eny[i]-4),0x4a + endir[i],1,index_offset);
						oam_spr((enx[i]+4),(eny[i]-4),0x4e + endir[i],1,4+index_offset);
						oam_spr(0,0,0x6a,1,8+index_offset);
						oam_spr(0,0,0x6a,65,12+index_offset);
					}
					else if(enemy_movement_phase[i]==4)//score
					{
						oam_spr((enx[i]-4),(eny[i]-4),enemy_score_card[escorevalue[i]],2,index_offset);
						oam_spr((enx[i]+4),(eny[i]-4),enemy_score_mult[escorevalue[i]],2,4+index_offset);
						
						oam_spr((enx[i]-4),(eny[i]-4),enemy_score_card[escorevalue[i]],2,8+index_offset);
						oam_spr((enx[i]+4),(eny[i]-4),enemy_score_mult[escorevalue[i]],2,12+index_offset);
						//oam_spr(0,0,0x6b,2,8+index_offset);
						//oam_spr(0,0,0x6b,2,12+index_offset);
					}
					else
					{
						//normal ghost
						oam_spr((enx[i]-4),(eny[i]-4),0x30+sprite_alternator+(2*endir[i]),1+color_alternator,index_offset);
						oam_spr((enx[i]+4),(eny[i]-4),0x38+sprite_alternator+(2*endir[i]),1+color_alternator,4+index_offset);
						oam_spr((enx[i]-4),(eny[i]+4),0x40+sprite_alternator+(2*animation_offset),1+color_alternator,8+index_offset);
						oam_spr((enx[i]+4),(eny[i]+4),0x40+sprite_alternator+(2*animation_offset),65+color_alternator,12+index_offset);
					}
				}
			}
			color_alternator=1-color_alternator;
			if(color_alternator==0){sprite_alternator=1;}//only flips on second color_alternator 0 (at i=3).
		}
	}
	else
	{
		fruit_activated=0;
		oam_clear();//THIS WILL POSE A PROBLEM WITH ONCE-UPDATING FRUIT CHART AT TOP
		do_inventory_update=1;
	}
	
	//fruit
	
		if(fruit_activated==1)//make sprite drawing efficient
		{
			if(checkFruitRow()<4 || render_phase==0)
			{
				oam_spr(120,129,0x78+current_fruit,fruit_palette_top[current_fruit],64);
				oam_spr(128,129,0x80+current_fruit,fruit_palette_top[current_fruit],68);
				oam_spr(120,137,0x88+current_fruit,fruit_palette_bottom[current_fruit],72);
				oam_spr(128,137,0x90+current_fruit,fruit_palette_bottom[current_fruit],76);
			}
			else
			{
				oam_spr(0,232,0x77,0,64);
				oam_spr(0,232,0x77,0,68);
				oam_spr(0,232,0x77,0,72);
				oam_spr(0,232,0x77,0,76);
			}
		}
		else if(fruit_hidden==0)
		{
			oam_spr(0,232,0x77,0,64);
			oam_spr(0,232,0x77,0,68);
			oam_spr(0,232,0x77,0,72);
			oam_spr(0,232,0x77,0,76);
			fruit_hidden=1;
		}
		if(fruit_activated==2)
		{
			update_list[31]=fruit_score_card[current_fruit];
			update_list[32]=fruit_score_mult[current_fruit];
		}
		else if(update_list[31]!=0 && update_list[31]!=0x74)//not empty OR "Ready!" isn't shown
		{
			update_list[31]=0x00;
			update_list[32]=0x00;
		}
		
	//Inventory
	if(do_inventory_update==1)
	{
		do_inventory_update=0;
		
		if(inventory_count!=0)
		{
			for(i=0;i<inventory_count;i++)
			{
				index_offset=(i*16);
				oam_spr(176+index_offset,5,0x78+inventory[i],fruit_palette_top[inventory[i]],80+index_offset);
				oam_spr(176+index_offset,13,0x88+inventory[i],fruit_palette_bottom[inventory[i]],84+index_offset);
				oam_spr(184+index_offset,5,0x80+inventory[i],fruit_palette_top[inventory[i]],88+index_offset);
				oam_spr(184+index_offset,13,0x90+inventory[i],fruit_palette_bottom[inventory[i]],92+index_offset);
			}
		}
		
		
	}
	
	//pacman
	if(pac_death_done==0 && pacdead==0)
	{
		//pacman_animation_divider
			oam_spr(pacx-4,pacy-4,0x00+(6*pdir)+(2*pacman_can_animate),0,152); 
			oam_spr(pacx+4,pacy-4,0x01+(6*pdir)+(2*pacman_can_animate),0,156);
			oam_spr(pacx-4,pacy+4,0x18+(6*pdir)+(2*pacman_can_animate),0,160);
			oam_spr(pacx+4,pacy+4,0x19+(6*pdir)+(2*pacman_can_animate),0,164);

	}
	else
	{
		
			oam_spr(pacx-4,pacy-4,0x5d+(pac_death_counter/4),0,152); 
			oam_spr(pacx+4,pacy-4,0x5d+(pac_death_counter/4),64,156);
			oam_spr(pacx-4,pacy+4,0x6b+(pac_death_counter/4),0,160);
			oam_spr(pacx+4,pacy+4,0x6b+(pac_death_counter/4),64,164);
		
	}
	//update_list[17]=0x10+NTADR_A(0,0)/100;
	//update_list[18]=0x10+NTADR_A(0,0)/10%10;
	//update_list[19]=0x10+NTADR_A(0,0)%10;
	
	//update_list[17]=0x10+master_phase;
}

void move_out_house(char ghostID)
{			
	unsigned char g = ghostID;
	if(enx[g]<124)
	{
		endir[g]=2;
		enx[g]+=c_exit_house_delay[g];
	}
	else if(enx[g]>124)
	{
		endir[g]=0;
		enx[g]-=c_exit_house_delay[g];
	}
	else
	{
		if(eny[g]>88)
		{
			endir[g]=1;
			eny[g]-=c_exit_house_delay[g];
		}
	}
	c_exit_house_delay[g]=1-c_exit_house_delay[g];
}

void move_into_house(char ghostID)
{			
	unsigned char g = ghostID;
	if(g==0)
	{
		if(eny[g]<ENEMY_START_Y[g]+24)
		{
			endir[g]=3;
			eny[g]++;
		}
		else
		{
			if(enx[g]<ENEMY_START_X[g])
			{
				endir[g]=2;
				enx[g]++;
			}
			else if(enx[g]>ENEMY_START_X[g])
			{
				endir[g]=0;
				enx[g]--;
			}
		}
	}
	else
	{
		if(eny[g]<ENEMY_START_Y[g])
		{
			endir[g]=3;
			eny[g]++;
		}
		else
		{
			if(enx[g]<ENEMY_START_X[g])
			{
				endir[g]=2;
				enx[g]++;
			}
			else if(enx[g]>ENEMY_START_X[g])
			{
				endir[g]=0;
				enx[g]--;
			}
		}
	}
}

void move_flip(char ghostID)
{
	unsigned char g = ghostID;
	unsigned char available_direction[4];
	if(enemy_movement_phase[g]!=0 && enemy_movement_phase[g]<4)
	{
		available_direction[0] = MAP[current_map_index][(enx[g]-1)/8+((eny[g]/8)*32)]>0xFB;
		available_direction[1] = MAP[current_map_index][(enx[g])/8+(((eny[g]-1)/8)*32)]>0xFB;
		available_direction[2] = MAP[current_map_index][(enx[g]+8)/8+((eny[g]/8)*32)]>0xFB;
		available_direction[3] = MAP[current_map_index][(enx[g])/8+(((eny[g]+8)/8)*32)]>0xFB;
		if(available_direction[OPPOSITE[endir[g]]])
		{
			endir[g]=OPPOSITE[endir[g]];
		}
		else if(available_direction[3] && endir[g]!=3)
		{
			endir[g]=3;
		}
		else if(available_direction[1] && endir[g]!=1)
		{
			endir[g]=1;
		}
		else if((available_direction[2] || eny[g]==248)  && endir[g]!=2)
		{
			endir[g]=2;
		}
		else if((available_direction[0] || eny[g]==0) && endir[g]!=0)
		{
			endir[g]=0;
		}
	}
}
void move_vertical_flip(char ghostID)
{
	unsigned char g = ghostID;
	unsigned char available_direction[4];

	available_direction[1] = MAP[current_map_index][(enx[g])/8+(((eny[g]-1)/8)*32)]>0xFB;
	available_direction[3] = MAP[current_map_index][(enx[g])/8+(((eny[g]+8)/8)*32)]>0xFB;
		
	if(!available_direction[endir[g]])
	{
		endir[g]=OPPOSITE[endir[g]];
	}
}

void move_randomly(char ghostID)
{
	unsigned char g = ghostID;
	unsigned char available_direction[4];
	
	if(c_enemy_slowdown==1)
	{
		if((enx[g]%8==0 && eny[g]%8==0) && enx[g]!=248 && eny[g]!=0)
		{
			
				available_direction[0] = (MAP[current_map_index][(enx[g]-1)/8+((eny[g]/8)*32)]>0xFB && endir[g]!=2);
				available_direction[1] = MAP[current_map_index][(enx[g])/8+(((eny[g]-1)/8)*32)]>0xFB && endir[g]!=3;
				available_direction[2] = (MAP[current_map_index][(enx[g]+8)/8+((eny[g]/8)*32)]>0xFB && endir[g]!=0);
				available_direction[3] = MAP[current_map_index][(enx[g])/8+(((eny[g]+8)/8)*32)]>0xFB && endir[g]!=1;

				if(available_direction[RANDOM_DIRECTION[c_randomize[g]][0]])
				{
					endir[g]=RANDOM_DIRECTION[c_randomize[g]][0];
					if(c_randomize[g]<4){ c_randomize[g]++; }
					else{ c_randomize[g]=0; }
				}
				else if(available_direction[RANDOM_DIRECTION[c_randomize[g]][1]])
				{
					endir[g]=RANDOM_DIRECTION[c_randomize[g]][1];
					if(c_randomize[g]<4){ c_randomize[g]++; }
					else{ c_randomize[g]=0; }
				}
				else if(available_direction[RANDOM_DIRECTION[c_randomize[g]][2]])
				{
					endir[g]=RANDOM_DIRECTION[c_randomize[g]][2];
					if(c_randomize[g]<4){ c_randomize[g]++; }
					else{ c_randomize[g]=0; }
				}
				else if(available_direction[RANDOM_DIRECTION[c_randomize[g]][3]])
				{
					endir[g]=RANDOM_DIRECTION[c_randomize[g]][3];
					if(c_randomize[g]<4){ c_randomize[g]++; }
					else{ c_randomize[g]=0; }
				}
		}
		
		if(endir[g]==0)
		{
			enx[g]--;		
		}
		else if(endir[g]==1)
		{
			eny[g]--;
		}
		else if(endir[g]==2)
		{
			enx[g]++;
		}
		else if(endir[g]==3)
		{
			eny[g]++;
		}
	}
}

void move_towards_target(char ghostID)//g is the ghost index number
{
	unsigned char g = ghostID;
	unsigned char horizontal_sign = 0;
	unsigned char horizontal_distance = 0;
	unsigned char vertical_sign = 0;
	unsigned char vertical_distance = 0;
	unsigned char priority_direction = 4;
	unsigned char priority_axis = 0;
	unsigned char already_flipped_axis = 0;
	unsigned char available_direction[4];
	
	available_direction[0] = MAP[current_map_index][(enx[g]-1)/8+((eny[g]/8)*32)]>0xFB && endir[g]!=2;
	available_direction[1] = MAP[current_map_index][(enx[g])/8+(((eny[g]-1)/8)*32)]>0xFB && endir[g]!=3;
	available_direction[2] = MAP[current_map_index][(enx[g]+8)/8+((eny[g]/8)*32)]>0xFB && endir[g]!=0;
	available_direction[3] = MAP[current_map_index][(enx[g])/8+(((eny[g]+8)/8)*32)]>0xFB && endir[g]!=1;

	if(enx[g]%8==0 && eny[g]%8==0)
	{
		
		if(enx[g]<etx[g])
		{
			horizontal_sign=1;//positive
			horizontal_distance=etx[g]-enx[g];
		}
		else
		{
			horizontal_distance=enx[g]-etx[g];
		}
		
		if(eny[g]<ety[g])
		{
			vertical_sign=1;//positive
			vertical_distance=ety[g]-eny[g];
		}
		else
		{
			vertical_distance=eny[g]-ety[g];
		}
		
		if(vertical_distance>horizontal_distance)
		{
			priority_axis=1;
		}
		
dirfromaxis:		
		if( priority_axis==0 && horizontal_sign==0 && available_direction[0])//want to and can move left
		{	priority_direction=0;	}
		else if( priority_axis==0 && horizontal_sign==1 && available_direction[2])//want to and can move left
		{	priority_direction=2;	}
		else if( priority_axis==1 && vertical_sign==0 && available_direction[1])//want to and can move up
		{	priority_direction=1;	}
		else if( priority_axis==1 && vertical_sign==1 && available_direction[3])//want to and can move dn
		{	priority_direction=3;	}
		if(priority_direction==4)
		{
			if(already_flipped_axis==0) //if haven't flipped preferred axis and tried again, do so.
			{
				priority_axis = 1 - priority_axis;
				already_flipped_axis=1;
				goto dirfromaxis;
			}
			else
			{
				if(available_direction[3])
				{
					priority_direction=3;
				}
				else if(available_direction[2] || enx[g]==248)
				{
					priority_direction=2;
				}
				else if(available_direction[1])
				{
					priority_direction=1;
				}
				else if(available_direction[0] || enx[g]==0)
				{
					priority_direction=0;
				}
			}
		}
		endir[g]=priority_direction;
	}
	if(MAP[current_map_index][(enx[g])/8+((eny[g]/8)*32)]==0xFC)
	{
		if(endir[g]==0)
		{
			enx[g]-=c_enemy_slowdown;		
		}
		else if(endir[g]==1)
		{
			eny[g]-=c_enemy_slowdown;
		}
		else if(endir[g]==2)
		{
			enx[g]+=c_enemy_slowdown;
		}
		else if(endir[g]==3)
		{
			eny[g]+=c_enemy_slowdown;
		}
	}
	else
	{
		if(endir[g]==0)
		{
			enx[g]--;		
		}
		else if(endir[g]==1)
		{
			eny[g]--;
		}
		else if(endir[g]==2)
		{
			enx[g]++;
		}
		else if(endir[g]==3)
		{
			eny[g]++;
		}
	}
	
	
}

void doDeath(ghostID)
{
	unsigned char g = ghostID;
	
	if(enemy_movement_phase[g]==4)
	{
		if(c_enemy_score_timeout[g]==50)
		{
			enemy_movement_phase[g]=5;
			c_enemy_score_timeout[g]=0;
		}
	}
	else if(enemy_movement_phase[g]==5)//running home
	{
		if(enx[g]/8==ENEMY_START_X[0]/8 && eny[g]/8==ENEMY_START_Y[0]/8)
		{
			enx[g]=124;
			enemy_movement_phase[g]=6;
		}
		else
		{
			etx[g]=124;
			ety[g]=88;
		}
	}
}

void doRed()
{
	if(enemy_movement_phase[0]==0)//bounce in house
	{
		enemy_movement_phase[0]=1;
	}
	else if(enemy_movement_phase[0]==1 && enx[0]==124 && eny[0]==88)//exit house and complete
	{
		endir[0]=2;
		enemy_movement_phase[0]=master_phase;
	}
	else if(enemy_movement_phase[0]==2)//scatter
	{
		enemy_movement_phase[0]=master_phase;
		etx[0]=232;
		ety[0]=24;
	}
	else if(enemy_movement_phase[0]==3)//chase
	{
		//ghost 0 chase - red
		enemy_movement_phase[0]=master_phase;
		etx[0]=pacx;
		ety[0]=pacy;
		//if (x1−x2)^2+(y1−y2)^2 <=(r1+r2)^2
	}	
	else
	{
		doDeath(0);
	}
}

void doOrange()//clyde
{
	if(enemy_movement_phase[1]==0)//bounce in house
	{
		move_vertical_flip(1);
		if(c_dots_eaten_enabled==1)//individual counter is active
		{
			if(c_map_number==0)
			{
				if(c_dots_eaten[1]>59)
				{
					enemy_movement_phase[1]=1;
				}
			}
			else if(c_map_number==1)
			{
				if(c_dots_eaten[1]>49)
				{
					enemy_movement_phase[1]=1;
				}
			}
		}
	}
	else if(enemy_movement_phase[1]==1 && enx[1]==124 && eny[1]==88)//exit house and complete
	{
		endir[1]=2;
		enemy_movement_phase[1]=master_phase;
	}
	else if(enemy_movement_phase[1]==2)//scatter
	{
		etx[1]=16;
		ety[1]=216;
	}
	else if(enemy_movement_phase[1]==3)//chase
	{
		enemy_movement_phase[1]=master_phase;
		//ghost 1 chase - orange
		if( ((((enx[1]/8)-pacx/8)*((enx[1]/8)-pacx/8) ) + ( ((eny[1]/8) - pacy/8) * ((eny[1]/8) - pacy/8))) <= 81 )
		{
			etx[1]=16;
			ety[1]=216;
		}
		else
		{
			etx[1]=pacx;
			ety[1]=pacy;
		}
	}
	else
	{
		doDeath(1);
	}
}

void doBlue()//inky #2(0,1,2,3) //done
{
	unsigned char fpx=pacx; //flip point for blue[2]'s vector flip
	unsigned char fpy=pacy;
	unsigned char horizontal_distance = 0, vertical_distance = 0, horizontal_sign = 0, vertical_sign = 0;
	if(enemy_movement_phase[2]==0)//bounce in house
	{
		move_vertical_flip(2);
		if(c_dots_eaten_enabled==1)//individual counter is active
		{
			if(c_map_number==0)
			{
				if(c_dots_eaten[2]>29)
				{
						enemy_movement_phase[2]=1;
				}
			}
			else
			{
					enemy_movement_phase[2]=1;
				//endir[2]=2;
			}
		}
		else//global counter is active
		{
			if(global_dot_counter>16)
			{
					enemy_movement_phase[2]=1;
				//endir[2]=2;
			}
		}
	}
	else if(enemy_movement_phase[2]==1 && enx[2]==124 && eny[2]==88)//exit house and complete
	{
		endir[2]=2;
		enemy_movement_phase[2]=master_phase;
	}
	else if(enemy_movement_phase[2]==2)//scatter
	{
		enemy_movement_phase[2]=master_phase;
		etx[2]=232;
		ety[2]=216;
	}
	else if(enemy_movement_phase[2]==3)//chase
	{
		//ghost 2 chase - blue
		enemy_movement_phase[2]=master_phase;
		if(pdir==0){fpx=pacx-16;}
		else if(pdir==1){fpy=pacy-16; fpx=pacx-16;}//add the overflow bug for red's flip point.
		else if(pdir==2){fpx=pacx+16;}
		else if(pdir==3){fpy=pacy+16;}
		if(enx[0]<fpx)
		{
			horizontal_sign=1;//positive
			horizontal_distance=fpx-enx[0];
		}
		else
		{
			horizontal_distance=enx[0]-fpx;
		}
			
		if(eny[0]<fpy)
		{
			vertical_sign=1;//positive
			vertical_distance=fpy-eny[0];
		}
		else
		{
			vertical_distance=eny[0]-fpy;
		}
		if(horizontal_sign == 1){ etx[2] = fpx + (horizontal_distance); }
		else{ etx[2] = fpx - (horizontal_distance); }
		if(vertical_sign == 1){ ety[2] = fpy + (vertical_distance); }
		else{ ety[2] = fpy - (vertical_distance); }
	}
	else
	{
		doDeath(2);
	}
}

void doPink()//pinky
{
	
	if(enemy_movement_phase[3]==0)//bounce in house
	{
		move_vertical_flip(3);
		if(c_dots_eaten_enabled==0)
		{
			if(global_dot_counter>6)
			{
				if(enemy_scared[3]==0)
				{
					enemy_movement_phase[3]=1;
				}
			}
		}
		else
		{
			if(enemy_scared[3]==0)
			{
				enemy_movement_phase[3]=1;
			}
		}
		//enemy_movement_phase[3]=1;//immediately exit house
	}
	else if(enemy_movement_phase[3]==1 && enx[3]==124 && eny[3]==88)//exit house and complete
	{
		enemy_movement_phase[3]=master_phase;
		endir[3]=2;
	}
	else if(enemy_movement_phase[3]==2)//scatter
	{
		enemy_movement_phase[3]=master_phase;
		etx[3]=16;
		ety[3]=24;
	}
	else if(enemy_movement_phase[3]==3)//chase
	{
		//ghost 3 chase - pink
		enemy_movement_phase[3]=master_phase;
		if(pdir==0){etx[3]=pacx-32; ety[3]=pacy;}
		else if(pdir==1){ety[3]=pacy-32; etx[3]=pacx-32;}//add the overflow bug for pathfinding when pacman is facing upwards.
		else if(pdir==2){etx[3]=pacx+32; ety[3]=pacy;}
		else if(pdir==3){ety[3]=pacy+32; etx[3]=pacx;}
	}
	else
	{
		doDeath(3);
	}
}

void do_ghost()
{
	c_enemy_slowdown=1-c_enemy_slowdown;
	if(pacactive)
	{
		for(i=0;i<4;i++)//deal with pacman collision.
		{
			if(enx[i]/8==pacx/8 && eny[i]/8==pacy/8)//touching pacman
			{
				if(enemy_movement_phase[i]==2 || enemy_movement_phase[i]==3)
				{
					if(enemy_scared[i]==0)
					{
						pacactive=0;
						pacdead=1;
						wait(1);
						return;
					}
					else
					{
						enemy_scared[i]=0;//keep score or homerun ghosts from hurting pacman.
						enemy_movement_phase[i]=4;//score ghost
						if(enemies_ate<3)
						{
							addScore(ENEMY_SCORE[enemies_ate]);
							escorevalue[i]=enemies_ate;
							enemies_ate++;
						}
					}
				}
			}
				
			if(enemy_movement_phase[i]!=enemy_last_phase[i])
			{
				if(enemy_scared[i]==0 && enemy_movement_phase[i]==3 && enemy_movement_phase[i]==2)
				{
					move_flip(i);
				}
				enemy_last_phase[i]=enemy_movement_phase[i];
			}
			if(i==0){doRed();}
			else if(i==1){doOrange();}
			else if(i==2){doBlue();}
			else if(i==3){doPink();}
			
			
setMove:			
			if(enemy_scared[i]==1)
			{ 
				if(c_scared_seconds==ENEMY_FRIGHT_DURATION[c_map_number])
				{
					enemy_scared[i]=0;
					enemy_flashed=0;
					goto setMove;
				}//break back here
				if(enemy_movement_phase[i]==1){ move_out_house(i); }
				else{move_randomly(i); }
				
			}
			else if(enemy_movement_phase[i]==1){ move_out_house(i); }
			else if(enemy_movement_phase[i]==4){c_enemy_score_timeout[i]++;}
			else if(enemy_movement_phase[i]==5){move_towards_target(i);move_towards_target(i);}
			else if(enemy_movement_phase[i]==6)
			{
				if(i==0)
				{
					if(enx[i]==ENEMY_START_X[i] && eny[i]==ENEMY_START_Y[i]+24)
					{
						enemy_movement_phase[i]=1;
					}
					else
					{
						move_into_house(i);
					}
				}
				else
				{
					if(enx[i]==ENEMY_START_X[i] && eny[i]==ENEMY_START_Y[i])
					{
						enemy_movement_phase[i]=0;
					}
					else
					{
						move_into_house(i);
					}
				}
			}
			else{ move_towards_target(i); }	
		}
	}
}

		//0=bounce in house
		//1=exit house
		//2=scatter //a master phase
		//3=chase //a master phase
		//4=score
		//5=running home
		//6=enter house

void draw_map(char nodots)
{
	for(i=0;i<29;i++)
	{
		vram_adr(NTADR_A(0,i));
		for(j=0;j<32;j++)
		{
			if(nodots==1)
			{
				if(MAP[current_map_index][j+(i*32)]<0xFE)
				{
					vram_put(MAP[current_map_index][j+(i*32)]);
				}
				else
				{
					vram_put(0x00);
				}
			}
			else
			{
				if(MAP[current_map_index][j+(i*32)] == 0xFF)
				{
					dots_remaining++;
				}
				else if(MAP[current_map_index][j+(i*32)] == 0xFE)
				{
					big_dot_x[c_big_dot_placed]=j;
					big_dot_y[c_big_dot_placed]=i;
					update_list[37+(4*c_big_dot_placed)]=MSB(NTADR_A(j,i))|NT_UPD_HORZ;
					update_list[38+(4*c_big_dot_placed)]=LSB(NTADR_A(j,i));
					big_dot_exists[c_big_dot_placed]=1;
					c_big_dot_placed++;
					dots_remaining++;
				}
				vram_put(MAP[current_map_index][j+(i*32)]);
			}

		}
		if(i<25)
		{
			dot_bit_row[i][0]=0xff;
			dot_bit_row[i][1]=0xff;
			dot_bit_row[i][2]=0xff;
			dot_bit_row[i][3]=0xff;
		}
	}
}

void draw_title()
{
	for(i=0;i<23;i++)
	{
		vram_adr(NTADR_A(0,i+5));
		for(j=0;j<32;j++)
		{
			vram_put(title[j+(i*32)]);
		}
	}
}

void getScared()
{
	if(c_map_number<18)
	{
		for(i=0;i<4;i++)
		{
			if(enemy_movement_phase[i]<4)//if the ghost can get scared
			{
				enemy_scared[i]=1;
				if(ENEMY_FRIGHT_DURATION[c_map_number]>0)
				{
					c_enemy_scared_flashed[i]=0;
					c_scared_ticks=0;
					c_scared_seconds=0;
				}
			}
		}
	}
}

void doHouseCounters()
{
	if(c_dots_eaten_enabled==1)
	{
		if(enemy_movement_phase[3]==0)
		{
			c_dots_eaten[3]++;
		}
		else if(enemy_movement_phase[2]==0)
		{
			c_dots_eaten[2]++;
		}
		else if(enemy_movement_phase[1]==0)
		{
			c_dots_eaten[1]++;
		}
	}
	else
	{
		if(enemy_movement_phase[1]==0 && global_dot_counter==32)
		{
			c_dots_eaten_enabled=1;
			global_dot_counter=0;
		}
		global_dot_counter++;
	}
}


void remove_dot(unsigned char type)//0=pel 10pts, 1=powerpel 50pts
{
	//unsigned char quad = col/8;// quad 
	unsigned char row = (pacy/8)-3;
	unsigned char col = (pacx/8)-2;
	unsigned char quadbitnum = 7-col%8;
	unsigned char quadval = dot_bit_row[row][col/8];
		
	if(((quadval >> quadbitnum) & 0x01))//if not eaten
	{
		if(type==0xFE)
		{ 
			for(i=0;i<4;i++)
			{
				if(big_dot_x[i]==pacx/8 && big_dot_y[i]==pacy/8)
				{
					big_dot_exists[i]=0;
				}
			}
			addScore(5);
			enemy_flashed=0;
			dots_remaining--;
			dots_eaten++;
			doHouseCounters();
			enemies_ate = 0;
			
			if(c_map_number < 20)
			{
				getScared();
			}
			flip_enemies_buffer=1;
		}
		else if(type==0xFF){ dots_eaten++; addScore(1); dots_remaining--; doHouseCounters();}
		else return;
			
		dot_bit_row[row][col/8] = quadval & ~(1<<quadbitnum);
	}
	update_list[0]=MSB(NTADR_A(pacx/8,pacy/8))|NT_UPD_HORZ;
	update_list[1]=LSB(NTADR_A(pacx/8,pacy/8));	
}

void reset_ghosts()
{
	memcpy(endir,ENEMY_START_DIR,4);
	memcpy(enx,ENEMY_START_X,4);
	memcpy(eny,ENEMY_START_Y,4);
	memcpy(enemy_movement_phase, ENEMY_START_PHASE, 4);
	memcpy(enemy_last_phase, ENEMY_START_PHASE, 4);
	memcpy(c_randomize,FOUR_CHAR_ZEROS,4);
	memcpy(exiting_house_eaten,FOUR_CHAR_ZEROS,4);
	memcpy(enemy_scared_counter,FOUR_CHAR_ZEROS,4);
	memcpy(enemy_scared,FOUR_CHAR_ZEROS,4);
}

void reset_pac()
{
	pacactive=0;
	pacx=pacstartx;
	pacy=pacstarty;
	pdir=2;
	pacman_can_animate=0;
}

void setup_map(char died)//1=died, 2=gameover
{
	ppu_off();
	update_list[0]=MSB(NTADR_A(0,0))|NT_UPD_HORZ;
	update_list[1]=LSB(NTADR_A(0,0));	
	global_dot_counter=0;//reset global dot counter
	//fruits
	fruit_activated = 0;
	fruit_hidden = 1;
	c_fruit_timeout = 0;
	
	if(died==2)//game over
	{
		pal_col(3,0x05);
		update_list[27]=0x78;
		update_list[28]=0x74;
		update_list[29]=0x79;
		update_list[30]=0x73;
		update_list[31]=0x00;
		update_list[32]=0x7a;
		update_list[33]=0x7b;
		update_list[34]=0x73;
		update_list[35]=0x72;
		update_list[36]=0x77;
		
		update_list[40]=0;
		update_list[44]=0;
		update_list[48]=0;
		update_list[52]=0;
		pacx=pacstartx;
		pacy=pacstarty;
		draw_map(1);
		ppu_on_all();
		return;
		//0x00,0x00,0x60,0x61,0x62,0x63,0x64,0x65,0x00,0x00, //27,28,29,30,31,32,33,34,35,36
	}
	else//not game over
	{
		reset_pac();
		reset_ghosts();
		c_big_dot_placed=0;
		pacdead=0;
		pac_death_done=0;
		pac_death_counter=0;
		if(died==0)            //next level or first level
		{
			
			fruit_shown_at_score = 0;
			for(i=0;i<25;i++)
			{
				for(j=0;j<4;j++)
				{
					controller=pad_poll(0);
					dot_bit_row[i][j]=0;
				}
			}
			big_dot_removed[0]=0;
			big_dot_removed[1]=0;
			big_dot_removed[2]=0;
			big_dot_removed[3]=0;
			c_dots_eaten[0] = 0;
			c_dots_eaten[1] = 0;
			c_dots_eaten[2] = 0;
			c_dots_eaten[3] = 0;
			dots_eaten=0;
			sel_dot_counter_ghost=3;//pinky first
			c_dots_eaten_enabled=1;//ghost dot counter is set
			dots_remaining=0;
			pal_col(2,PAL_MAP[c_map_palette]);
			//set_vram_update(NULL);
			draw_map(0);
			set_vram_update(update_list);
			map_score=0;
			
			if(c_map_number==1)//Strawberry 30. 1
			{
				current_fruit=1;
			}
			else if(c_map_number==2)//Peach 50. 2 and 3
			{
				current_fruit=2;
			}
			else if(c_map_number==4)//Apple 70. 4 and 5
			{
				current_fruit=3;
			}
			else if(c_map_number==6)//Grapes 100. 6 and 7
			{
				current_fruit=4;
			}
			else if(c_map_number==8)//Galaxian 200. 8 and 9
			{
				current_fruit=5;
			}
			else if(c_map_number==10)//Bell 300. 10 and 11
			{
				current_fruit=6;
			}
			else if(c_map_number==12)//key 500. 12+
			{
				current_fruit=7;
			}
		}
		else
		{
			sel_dot_counter_ghost = 3;
			c_dots_eaten_enabled=0;//global dot counter is set
			update_list[29]=0x72;
			update_list[30]=0x73;
			update_list[31]=0x74;
			update_list[32]=0x75;
			update_list[33]=0x76;
			update_list[34]=0x77;
			//0x00,0x00,0x60,0x61,0x62,0x63,0x64,0x65,0x00,0x00, //27,28,29,30,31,32,33,34,35,36
		}
		update_screen(1);
		final_phase_flip=0;
		master_phase=2;
		c_seconds=1;
		pacactive=1;
		ppu_on_all();
		master_phase_counter=0;
		wait(2);
		if(taken_first_life==0){lives--; taken_first_life=1;}
		if(died==1)
		{
				lives--;
		}
		update_list[27]=0x00;//clear ready text
		update_list[28]=0x00;
		update_list[29]=0x00;
		update_list[30]=0x00;
		update_list[31]=0x00;
		update_list[32]=0x00;
		update_list[33]=0x00;
		update_list[34]=0x00;
		update_list[35]=0x00;
		update_list[36]=0x00;
	}
	
}

void do_pac()
{
	unsigned char animate = 1,queue_direction=pdir;
	unsigned char available_direction[4];
	unsigned char current_tile = MAP[current_map_index][pacx/8+((pacy/8)*32)];
	available_direction[0] = MAP[current_map_index][(pacx-1)/8+((pacy/8)*32)]>0xFB || pacx/8<1;
	available_direction[1] = MAP[current_map_index][(pacx)/8+(((pacy-1)/8)*32)]>0xFB;
	available_direction[2] = MAP[current_map_index][(pacx+8)/8+((pacy/8)*32)]>0xFB || pacx/8>30;
	available_direction[3] = MAP[current_map_index][(pacx)/8+(((pacy+8)/8)*32)]>0xFB;

	if(pacactive)
	{
		if(pacx%8==0 && pacy%8==0 && current_tile>0xFD)
		{
			remove_dot(current_tile);
		}
		
		
		if(controller&PAD_LEFT) queue_direction=0;
		else if(controller&PAD_UP) queue_direction=1;
		else if(controller&PAD_RIGHT) queue_direction=2;
		else if(controller&PAD_DOWN) queue_direction=3;
		else if(controller&PAD_A){}


		if(available_direction[queue_direction] && pacx%8==0 && pacy%8==0)
		{
			pdir=queue_direction;
			//break;
		}
		
		if(pdir==0 && available_direction[0])
		{
				pacx--;
		}
		else if(pdir==1 && available_direction[1])
		{
				pacy--;
		}
		else if(pdir==2 && available_direction[2])
		{
				pacx++;
		}
		else if(pdir==3 && available_direction[3])
		{
				pacy++;
		}
		else {pacman_can_animate=0; return;}
		
		if(pacman_animation_divider<2) pacman_animation_divider++;
		else 
		{
			if(pacman_can_animate<2) pacman_can_animate++;
			else pacman_can_animate=0;
			pacman_animation_divider=0;
		}	
	}
	else if(pacdead)
	{
		if(pac_death_done==0)
		{
			if(pac_death_counter<50)
			{
				pac_death_counter++;
			}
			else
			{
				pac_death_done=1;
				wait(2);
			}
		}
	}
}

void setMasterPhase()
{
	if(enemy_scared[0]!=1 && enemy_scared[1]!=1 && enemy_scared[2]!=1 && enemy_scared[3]!=1)
	{
		master_phase_counter++;
		if(master_phase_counter==60)
		{
			c_seconds++;
			master_phase_counter=1;
		}
		if(c_map_number>3)//map3+ 2for5,3for20,2for5,3for20,2for5,3for1037,2for1/60,3forever
		{
			if(c_seconds<5){master_phase=2;}
			else if(c_seconds<25){master_phase=3;}
			
			else if(c_seconds<30){master_phase=2;}
			else if(c_seconds<50){master_phase=3;}
			
			else if(c_seconds<55){master_phase=2;}
			else if(c_seconds<1092){master_phase=3;}
			
			else if(c_seconds==1093 && final_phase_flip==0)
			{
				//only flip once this second. fix this
				final_phase_flip=1;
				flip_enemies_buffer=1;
			}

		}
		else if(c_map_number>0)//map1-map3 2for7,3for20,2for7,3for20,2for5,3for1033,2for1/60,3forever
		{
			if(c_seconds<7){master_phase=2;}
			else if(c_seconds<27){master_phase=3;}
			
			else if(c_seconds<34){master_phase=2;}
			else if(c_seconds<54){master_phase=3;}
			
			else if(c_seconds<59){master_phase=2;}
			else if(c_seconds<1092){master_phase=3;}
			
			else if(c_seconds==1093 && final_phase_flip==0)
			{
				final_phase_flip=1;
				flip_enemies_buffer=1;
			}
		}
		else //map0 2for7,3for20,2for7,3for20,2for5,3for20,2for5,3forever
		{
			if(c_seconds<7){master_phase=2;}
			else if(c_seconds<27){master_phase=3;}
			
			else if(c_seconds<34){master_phase=2;}
			else if(c_seconds<54){master_phase=3;}
			
			else if(c_seconds<59){master_phase=2;}
			else if(c_seconds<79){master_phase=3;}
			
			else if(c_seconds<84){master_phase=2;}
			else {master_phase=3;}
		}
	}
}

void do_pac_and_ghosts()
{
	controller=pad_poll(0);
	if(c_map_number>19)
	{
		if(c_speed%20!=0)
		{
			do_ghost();
		}
		if(c_speed%10!=0)
		{
			do_pac();
		}
	}
	else if(c_map_number>3)
	{
		if(c_speed%20!=0)
		{
			do_ghost();
		}
		do_pac();
	}
	else if(c_map_number>0)
	{
		if(c_speed%7!=0)
		{
			do_ghost();
		}
		if(c_speed%10!=0)
		{
			do_pac();
		}
	}
	else
	{
		if(c_speed%4!=0)
		{
			do_ghost();
		}
		if(c_speed%5!=0)
		{
			do_pac();
		}
	}
	
}

void do_main_counters()
{
	unsigned char scared_happening=0;
	render_phase=1-render_phase;

	if(fruit_activated==1)
	{
		c_fruit_timeout++;
	}
	else if(fruit_activated==2)
	{
		c_fruit_score_timeout++;
	}
	
	if(c_speed<99)
	{
		c_speed++;
	}
	else
	{
		c_speed=0;
	}
	if(c_seconds<2000)
	{
		setMasterPhase();
	}
	if(c_map_number<18)
	{
		for(i=0;i<4;i++)
		{
			if(enemy_scared[i]==1)
			{
				scared_happening=1;
			}
		}
		if(scared_happening==1)
		{
			if(c_scared_ticks<60)
			{
				c_scared_ticks++;//flashing on last second
				if(c_scared_seconds>=ENEMY_FRIGHT_DURATION[c_map_number]-1)
				{//if in the last two seconds
					if(c_scared_ticks%6==0)//flash every half second
					{
						enemy_flashed=1-enemy_flashed;//flip the flashed
					}
				}
			}
			else
			{
				c_scared_ticks=0;
				c_scared_seconds++;
			}
		}
	}
}
void doBuffers()
{
	if(flip_enemies_buffer==1)
	{
		move_flip(0);
		move_flip(1);
		move_flip(2);
		move_flip(3);
		flip_enemies_buffer=0;
	}
}

void addInventory()
{
	for(i=3;i>0;i--)
	{
		inventory[i]=inventory[i-1];
	}
	inventory[0]=current_fruit;
	if(inventory_count<4){inventory_count++;}
	do_inventory_update=1;
}

void doFruit()
{
	if((dots_eaten==70 || dots_eaten==170) && fruit_shown_at_score!=dots_eaten)
	{
		fruit_shown_at_score=dots_eaten;
		fruit_activated=1;
		c_fruit_timeout=0;
		fruit_hidden=0;
	}
	
	if(fruit_activated==1 && c_fruit_timeout>1000)//
	{
		fruit_activated=0;
		c_fruit_timeout=0;
	}
	else if(fruit_activated==2 && c_fruit_score_timeout>50)//
	{
		fruit_activated=0;
		c_fruit_timeout=0;
		c_fruit_score_timeout=0;
	}
	
	if(pacy/8==17 && (pacx/8==15 || pacx/8==16))//touching fruit
	{
		if(fruit_activated==1)
		{
			addScore(fruit_score[current_fruit]);
			fruit_activated=2;
			addInventory();
			c_fruit_timeout=0;
			c_fruit_score_timeout=0;
		}
	}
}

void setMenuText()
{
	for(i=0;i<12;i++)
	{
		title_list[3+i]=MENU[current_menu][i];
		title_list[18+i]=MENU[current_menu][12+i];
		title_list[33+i]=MENU[current_menu][24+i];
	}
}

void changeMenu(char menu_number)
{
	current_menu=menu_number;
	selection=0;
}

void doMenu()
{
	if(title_counter%40==0)
	{
		for(i=0;i<6;i++)
		{
			if(title_dot_anim[i]<67)
			{
				title_dot_anim[i]++;
			}
			else
			{
				title_dot_anim[i]=0;
			}	
			if(title_dot_anim[i]==0 || title_dot_anim[i]==27 || title_dot_anim[i]==34 || title_dot_anim[i]==61)
			{
				oam_spr(title_anim_x[title_dot_anim[i]],title_anim_y[title_dot_anim[i]],0xBE,4,16+(4*i));
			}
			else
			{
				oam_spr(title_anim_x[title_dot_anim[i]],title_anim_y[title_dot_anim[i]],0xBF,4,16+(4*i));
			}
		}
		
	}
	if(drawn_menu!=current_menu)
	{
		setMenuText();
		ppu_wait_nmi();
		drawn_menu=current_menu;
	}
	controller=pad_poll(0);
	if(menu_type[current_menu]==0)
	{
		//pacman selectors
		oam_spr(64,(19+(2*selection))*8,0xa4+title_phase,64,240);
		oam_spr(184,(19+(2*selection))*8,0xa0+title_phase,64,248);
		
		//number (blank)
		oam_spr(64,19*8,0xFF,64,216);
		oam_spr(64,19*8,0xFF,64,224);
		oam_spr(184,19*8,0xFF,64,232);
	}
	else
	{
		//arrows
		oam_spr(64,19*8,0xB1,64,240);
		oam_spr(184,19*8,0xB0,64,248);
		
		//number
		oam_spr(120,168,0xB4+(level_selection/100),4,216);
		oam_spr(128,168,0xB4+(level_selection/10%10),4,224);
		oam_spr(136,168,0xB4+(level_selection%10),4,232);
	}
	if(title_counter<254){title_counter++;}
	else
	{
		title_counter=0;
		if(title_phase<3){title_phase++;}
		else{title_phase=0;}
		
	}
	if(menu_type[current_menu]==0)
	{
		if(selected==0)
		{
			if(controller&PAD_DOWN)
			{
				if(last_button!=1 || button_time>700)
				{
					if(selection<menu_size[current_menu]-1)
					{
						selection++;
					}
					else
					{
						selection=0;
					}
					last_button=1;
					button_time=0;
				}
				else
				{
					button_time++;
				}
			}
			else if(controller&PAD_UP)
			{
				if(last_button!=2 || button_time>700)
				{
					if(selection>0)
					{
						selection--;
					}
					else
					{
						selection=menu_size[current_menu]-1;
					}
					last_button=2;
					button_time=0;
				}
				else
				{
					button_time++;
				}
			}
			else if(controller&PAD_A)
			{
				if(last_button!=3)
				{
					if(selection>0)
					{
						selected=1;
						last_button=3;
					}
					else
					{
						c_map_number=level_selection-1;
						c_map_palette=c_map_number%4;
						current_map_index=c_map_number%4;
						title_done=1;
					}
				}
			}
			else
			{
				
					last_button=0;
					button_time=0;
				
			}
		}
		else
		{
			selected=0;
			changeMenu(selection);
		}
	}
	else if(menu_type[current_menu]==1)//is the level selection menu
	{
		if(selected==0)
		{
			if(controller&PAD_RIGHT)
			{
				if((last_button!=1 || button_time>500) || (last_button==1 && button_time>500))
				{
					if(level_selection<255)
					{
						level_selection++;
					}
					else
					{
						level_selection=1;
					}
					
					if(last_button!=1)
					{
						button_time=0;
					}
					else
					{
						button_time=460;
					}
					last_button=1;
				}
				else
				{
					button_time++;
				}
			}
			else if(controller&PAD_LEFT)
			{
				if((last_button!=2 || button_time>500) || (last_button==2 && button_time>500))
				{
					if(level_selection>1)
					{
						level_selection--;
					}
					else
					{
						level_selection=255;
					}
					
					if(last_button!=2)
					{
						button_time=0;
					}
					else
					{
						button_time=460;
					}
					last_button=1;
					last_button=2;
				}
				else
				{
					button_time++;
				}
			}
			else if(controller&PAD_A)
			{
				if(last_button!=3)
				{
					selected=1;
					last_button=3;
				}
			}
			else
			{
				
					last_button=0;
					button_time=0;
				
			}
		}
		else
		{
			selected=0;
			changeMenu(0);
		}
	}
}

void main(void)
{
	bank_spr(1);
	bank_bg(0);
	pal_bg(PAL_BG);
	pal_spr(PAL_SPR);
	
	//title
titlescreen:
	ppu_off();
	set_vram_update(title_list);
	draw_title();
	ppu_on_all();
	music_play(0);
	while(title_done==0)
	{
		doMenu();
	}
	music_stop();
	oam_clear();
	//end title
	
next_map:
	//current_map_index=0;
	//c_map_number=0; 
	setup_map(0);
deadboi:
	while(dots_remaining!=0 && gameover==0)//per frame
	{
		do_main_counters();	
		doBuffers();
		do_pac_and_ghosts();
		doFruit();
		update_screen(0);
		ppu_wait_nmi();
		
		
		if(pac_death_done==1 && pacdead==1)
		{
			if(lives>0)
			{
				setup_map(1);
				goto deadboi;
			}
			else
			{
				gameover=1;
				break;
			}
		}
	}
	if(gameover==0)
	{
		
		wait(2);
		hideghosts=1;
		update_screen(1);
		ppu_off();
		ppu_wait_nmi();
		pal_col(2,0x30);
		ppu_on_all();
		waitframes(8);
		ppu_off();
		waitframes(8);
		ppu_on_all();
		waitframes(8);
		ppu_off();
		waitframes(8);
		ppu_on_all();
		waitframes(8);
		ppu_off();
		waitframes(8);
		ppu_on_all();
		waitframes(8);
		ppu_off();
		ppu_wait_nmi();
		hideghosts=0;
		wait(2);
		
		if(current_map_index<NUM_OF_MAPS-1)
		{
			current_map_index++;
		}
		else
		{ 
			current_map_index=0;
		}
		c_map_number++;
		if(c_map_palette<3)
		{
			c_map_palette++;
		}
		else
		{
			c_map_palette=0;
		}
		goto next_map;
	}
	else
	{
		setup_map(2);//game over
		wait(10);
		asm("jmp $8000");//don't worry about resetting crap.
	}
	//on gameover, freeze (for now)
}
