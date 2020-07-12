#include <gb/gb.h>
#include <stdio.h>
#include "Tileset.c"
#include "Background.c"
#include "Charz.c"
#include "GameCharacter.c"
#include "Splash_data.c"
#include "Splash_map.c"
GameCharacter Player;
GameCharacter Key;
const char blankmap[1] = {0x00};
UBYTE debug;
UBYTE haskey;

void Wait(UINT8 num)
{
	UINT8 i;
	for(i=0; i < num; i++)
	{
		wait_vbl_done();
	}
}

void FadeOUT()
{
  UINT8 i;
  for(i = 0;i<4;i++)
  {
    switch(i)
    {
      case 0: BGP_REG = 0xE4; break;
      case 1: BGP_REG = 0xF9; break;
      case 2: BGP_REG = 0xFE; break;
      case 3: BGP_REG = 0xFF; break;
    }
    Wait(50);
  }
}

void FadeIN()
{
  UINT8 i;
  for(i = 0;i<3;i++)
  {
    switch(i)
    {
      case 0: BGP_REG = 0xFE; break;
      case 1: BGP_REG = 0xF9; break;
      case 2: BGP_REG = 0xE4; break;
    }
    Wait(50);
  }
}

void BKG()
{
  set_bkg_data(0,219,Splash_data);
  set_bkg_tiles(0,0,20,18,Splash_map);
  SHOW_BKG;
  DISPLAY_ON;
  waitpad(J_START);
  FadeOUT();
}

void animatesprite(UINT8 spriteindex,INT8 movex,INT8 movey)
{
  while(movex!=0)
  {
    scroll_sprite(spriteindex,movex>0 ? -1 : 1,0);
    movex += (movex < 0 ? 1 : -1);
    wait_vbl_done();
  }
  while(movey!=0)
  {
    scroll_sprite(spriteindex,0,movey>0 ? -1 : 1);
    movey += (movey < 0 ? 1 : -1);
    wait_vbl_done();
  }
}

UBYTE CanyaMove(UINT8 newplayerx, UINT8 newplayery)
{
  UINT16 indexTLx = (newplayerx -8) / 8, indexTLy = (newplayery -16) / 8, tileindexTL = 20 * indexTLy + indexTLx;
  UBYTE result;
  result = Background[tileindexTL] == blankmap[0];

 if(debug && (indexTLx == 18 && indexTLy==16))
 {
    move_sprite(2,Key.x+16,Key.y);
    //set_bkg_tiles(18,16,1,1,blankmap[0]);
    haskey = 1;
 }
  return result;
}


void MoveGameCharacter(GameCharacter* character, UINT8 x,UINT8 y)
{
	move_sprite(character->spriteids,x,y);
}


void Player_Character()
{
  set_sprite_data(0,3,Charz);
  Player.x = 16;
	Player.y = 24;
	Player.width = 8;
	Player.height = 8;
  Player.spriteids = 1;
	set_sprite_tile(1,1);
  MoveGameCharacter(&Player,Player.x,Player.y);
  Key.x = 152;
	Key.y = 144;
	Key.width = 8;
	Key.height = 8;
  Key.spriteids = 2;
  set_sprite_tile(2,2);
  MoveGameCharacter(&Key,Key.x,144);
}

void main()
{
BKG();
set_bkg_data(0,2,Tileset);
set_bkg_tiles(0,0,20,18,Background);
FadeIN();
Player_Character();
SHOW_SPRITES;
SHOW_BKG;
DISPLAY_ON;
while(1)
{
  if(joypad() & J_LEFT)
  {
    if(CanyaMove(Player.x-8,Player.y))
    {
      Player.x = Player.x - 8;
      animatesprite(1,8,0);
    }
    //MoveGameCharacter(&Player,Player.x,Player.y);
  }

   if(joypad() & J_RIGHT)
   {
     if(CanyaMove(Player.x+8,Player.y))
     {
       Player.x = Player.x + 8;
       animatesprite(1,-8,0);
     }

     //MoveGameCharacter(&Player,Player.x,Player.y);
   }

   if(joypad() & J_UP)
   {
     if(CanyaMove(Player.x,Player.y-8))
     {
       Player.y = Player.y - 8;
       animatesprite(1,0,+8);
     }

   }

   if(joypad() & J_DOWN)
   {
     if(CanyaMove(Player.x,Player.y+8))
     {
       Player.y = Player.y + 8;
       animatesprite(1,0,-8);
     }

   }

   if(joypad() & J_A)
   {
     debug = 1;
     CanyaMove(Player.x,Player.y);
   }
   Wait(3);
 }
}
