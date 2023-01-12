//INCLUDES


#include <acknex.h>
#include <default.c>
#include <mtlFX.c>
#include <mtlView.c>
#include "halo.c"


//DIFINES
#define PRAGMA_PATH "Models";
#define PRAGMA_PATH "CharModels";
#define SHealth skill1
#define SMana skill2
#define SLevel skill3
#define SAttack skill4
#define SCast skill5
#define SSeePlayer skill6
#define SFall skill7
#define SSwim skill8
#define SEnergy skill9
#define SRun skill10



//VARIABLES

var VPlayerSpellBook[20];
var VPlayerInventory[50];
var VCameraSensitivity;
var VEnergy;
int VCurrentSpell = 0;

//ENTITES
ENTITY* Eweapon;
ENTITY* ENPC;

//FONTS

FONT* arial_font = "Arial#40b"; 

//FUNCTIONS

function FStart_New_Game();

//STRINGS

STRING* GameTitle = "Satmond 0.0.0.1";


//PANELS

PANEL* PMainMenu =
{
  pos_x = 4; pos_y = 4;
 // digits(0,0,"Start new game",arial_font,1,0);
 // digits(60,60,"Load game",arial_font,1,0);
 // digits(120,120,"Exit",arial_font,1,0);
 digits(120,0,7,arial_font,5,VEnergy);
  
}

function FKillChar(ENTITY* AChar)
{
	var LAnim = 0;
	set(AChar,PASSABLE);
	while(LAnim < 99)
   {
      LAnim += 1*time_step;
      if (LAnim >=100){ LAnim = 99;
}
      ent_animate(AChar,"death",LAnim,ANM_CYCLE);
      wait(1);
	}
ent_animate(AChar,"death",100,ANM_CYCLE);
}

function FUseWeapon(ENTITY* AChar)
{
	var LAnim = 100;
	while (key_pressed(280))
	{
	while(LAnim > 10)
   {
      LAnim -= 10*time_step;
      ent_animate(AChar,"attack",LAnim,ANM_CYCLE);
      
      c_trace(my.x,vec_rotate(vector(1000,0,0),my.pan),IGNORE_ME|IGNORE_PASSABLE); 
		if (hit.entity != NULL)
		{
			if ( hit.entity.SHealth  >=1)
			{
				hit.entity.SHealth -= 1;
			}
		} 

     wait(1); 
   }	
   me.SAttack = 2;	
   wait(1);
   }
   me.SAttack = 0;
	
}

function FCastSpell(int ASpellId, ENTITY* AChar)
{
	
	
}


function main()
{	
	video_mode = 12;
   video_screen = 2;
   wait(1);
	video_window(vector(1,1,0), vector(1920,1080,0), 16, GameTitle);
	set(PMainMenu,SHOW);
	detail_size = 100;
	FStart_New_Game();
	VCameraSensitivity = 10;
	camera.arc = 60;
}

action AWeapon()
{
	Eweapon=me;
	set(me,PASSABLE);
}


action APlayer()
{
	player = me;
	my.SHealth=10;
	my.SEnergy=100;
	if ((my.eflags&FAT) && (my.eflags&NARROW)) // when FAT+NARROW are both set
 		my.min_z *= 0.5;
   var Lspeed_down = 0;   // downward speed by gravity
   var Lanim_percent = 0; // animation percentage
   var Lwalk_speed;
   var Ldist_down;
   var Ldist_ahead; 
   var Ldist_side; 
   VECTOR LvFeet;  
   vec_for_min(LvFeet,me); // vFeet.z = distance from player origin to lowest vertex
   
	
	while (my.SHealth>=1)
	{
			
      if (c_trace(my.x,vector(my.x,my.y,my.z-5000),IGNORE_ME | IGNORE_PASSABLE | USE_BOX) > 0)
         Ldist_down = my.z + LvFeet.z - target.z; 
      else
         Ldist_down = 0;

      if (Ldist_down > 0)  
         Ldist_down = clamp(Ldist_down,0,accelerate(Lspeed_down,5,0.1));
      else              
         Lspeed_down = 0;
         
      my.SRun = key_shift;  
      if (my.SEnergy < 1)
      	{my.SRun = 0;}
      	
      if (my.SRun == 1)
      	{my.SEnergy = my.SEnergy - 1 * time_step;}
      else
      	{if(my.SEnergy < 100)
      		{my.SEnergy += 0.1 * time_step;}}
 	
      
      VEnergy = my.SEnergy; 
		Lwalk_speed = 5 + 5 * my.SRun;
		
		
      Ldist_ahead = Lwalk_speed*(key_w-key_s)*time_step;
      Ldist_ahead = sign(Ldist_ahead)*(abs(Ldist_ahead) + 0.5*Ldist_down);
      c_move(me,vector(Ldist_ahead,0,0),vector(0,0,-Ldist_down),IGNORE_PASSABLE | GLIDE); 
		
		Ldist_side = 5*(key_a-key_d)*time_step;
      Ldist_side = sign(Ldist_side)*(abs(Ldist_side) + 0.5*Ldist_down);
      c_move(me,vector(0,Ldist_side,0),vector(0,0,-Ldist_down),IGNORE_PASSABLE | GLIDE);
      if (me.SAttack != 1)
      {
			if (Ldist_ahead != 0) 
	      {
	         Lanim_percent += 1.3*Ldist_ahead; 
	         ent_animate(me,"walk",Lanim_percent,ANM_CYCLE); 
	         ent_animate(Eweapon,"walk",Lanim_percent,ANM_CYCLE); 
	      }
	      else 
	      { 
	         Lanim_percent += 5*time_step; 
	         ent_animate(me,"stand",Lanim_percent,ANM_CYCLE);
	         ent_animate(Eweapon,"stand",Lanim_percent,ANM_CYCLE);
     		}
     	}
 	

		my.pan -= mouse_force.x * time_step * VCameraSensitivity;
 	  	camera.tilt += mouse_force.y * time_step * VCameraSensitivity;
 	  	
 	  	if (camera.tilt<=-56)
 	  	  	camera.tilt=-56;
 	  	 if (camera.tilt>=90)
 	  	 	camera.tilt=90;
 	  	 	
		Eweapon.x = my.x;
		Eweapon.y = my.y;
		Eweapon.z = my.z;
		Eweapon.pan=my.pan;
		
		camera.x = my.x;
		camera.y = my.y;
		camera.z = my.z+40;
		camera.pan = my.pan;
		
		
		if (key_pressed(280))
			{
				if (me.SAttack == 0) 
				{	
					me.SAttack = 1;
					FUseWeapon(Eweapon);	
					FUseWeapon(player);	
				}
			} 

		if (key_pressed(282))
			{
				FCastSpell(VCurrentSpell, me);	
			} 	
			
			
		wait(1);
	}
	FKillChar(me);
	FKillChar(Eweapon);
}

function FStart_New_Game()
{
	level_load("1lvl.wmb");
	fog_color = 1; 
	camera.fog_start = 1000; 
	camera.fog_end = 3000; 
	Eweapon = ent_create ("sword.mdl", vector(300,0,200), AWeapon);
	player = ent_create ("guard.mdl", vector(300,0,200), APlayer);	
	player.SHealth = 30;
	player.SMana = 10;
	player.SLevel = 1;
}


action ANPC()
{
	my.SHealth=10;
	my.SEnergy=100;
	if ((my.eflags&FAT);// when FAT+NARROW are both set
 		my.min_z *= 0.5;
   var Lspeed_down = 0;   // downward speed by gravity
   var Lanim_percent = 0; // animation percentage
   var Lwalk_speed;
   var Ldist_down;
   var Ldist_ahead; 
   var Ldist_side; 
   VECTOR LvFeet;  
   vec_for_min(LvFeet,me); // vFeet.z = distance from player origin to lowest vertex
   
	
	while (my.SHealth>=1)
	{
			
      if (c_trace(my.x,vector(my.x,my.y,my.z-5000),IGNORE_ME | IGNORE_PASSABLE | USE_BOX) > 0)
         Ldist_down = my.z + LvFeet.z - target.z; 
      else
         Ldist_down = 0;

      if (Ldist_down > 0)  
         Ldist_down = clamp(Ldist_down,0,accelerate(Lspeed_down,5,0.1));
      else              
         Lspeed_down = 0;
         
     
 	
      
      VEnergy = my.SEnergy; 
		Lwalk_speed = 5 + 5 * my.SRun;
		
	
      c_move(me,vector(0,0,0),vector(0,0,-Ldist_down),IGNORE_PASSABLE | GLIDE); 
	
      if (me.SAttack != 1)
      {
		//	if (Ldist_ahead != 0) 
	    //  {
	     //    Lanim_percent += 1.3*Ldist_ahead; 
	      //   ent_animate(me,"walk",Lanim_percent,ANM_CYCLE); 
	      //   ent_animate(Eweapon,"walk",Lanim_percent,ANM_CYCLE); 
	    //  }
	    //  else 
	      { 
	         Lanim_percent += 5*time_step; 
	         ent_animate(me,"stand",Lanim_percent,ANM_CYCLE);
	         ent_animate(Eweapon,"stand",Lanim_percent,ANM_CYCLE);
     		}
     	}
 	
 	

		
	//	if (key_pressed(280))
	//		{
	//			if (me.SAttack == 0) 
		//		{	
		//			me.SAttack = 1;
		//			FUseWeapon(Eweapon);	
		//			FUseWeapon(player);	
		//		}
		//	} 

	//	if (key_pressed(282))
		//	{
	//			FCastSpell(VCurrentSpell, me);	
		//	} 	
			
			
		wait(1);
	}	
	
	FKillChar(me);
}