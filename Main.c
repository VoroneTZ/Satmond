//INCLUDES

#define PRAGMA_PATH "shade-c"
#define PRAGMA_PATH "stuff"

#include <litec.h>
#include <d3d9.h> // <--- THIS IS IMPORTANT!
#include <acknex.h>
#include <default.c>
#include "sc_core.c"
#include "sc_water.c"

#include <mtlFX.c>
#include <mtlView.c>
#include "halo.c"
#include <ackphysx.h>

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
var VHealth;
var VMana;
int VCurrentSpell = 0;

//ENTITES

BMAP* map_envMap = "sky04S+6.tga"; //an environment map for later use in a shader
ENTITY* Eweapon;
ENTITY* ENPC;
ENTITY* skycube =
{
  type = "sky04+6.tga"; // the image for a backdrop scene
  flags2 = SKY | CUBE | VISIBLE ; // sky cube, visible
//  green = 140;
//  blue = 140;
}
//FONTS

FONT* arial_font = "Arial#30b"; 

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
 digits(120,0,5,arial_font,1,VHealth);
 digits(120,20,5,arial_font,1,VMana);
 digits(120,40,5,arial_font,1,VEnergy);
 digits(10,10,5.1,*,5,time_step);
  
}

function FKillChar(ENTITY* AChar)
{
	var LAnim = 0;
	set(AChar,PASSABLE);
	while(LAnim < 99)
   {
      LAnim += 5*time_step;
      if (LAnim >=100){ LAnim = 99;
}
      ent_animate(AChar,"death",LAnim,0);
      wait(1);
	}
int i;
	i = 0;
	while (i < 100){
		my.z -=0.1 * time_step;
		i +=0.1 * time_step;
		wait(1);
	}
	ent_remove(me);
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
      c_scan(AChar.x,AChar.pan,vector(30,0,200),IGNORE_FLAG2|IGNORE_PASSABLE|IGNORE_WORLD);
      if (you != NULL){
      if ( you.SHealth  >=1)
			{
				you.SHealth -= 1*time_step;
		}}
     // c_trace(AChar.x,vec_rotate(vector(1000,0,0),AChar.pan),IGNORE_ME|IGNORE_PASSABLE); 
	//	if (hit.entity != NULL)
	///	{
	//		if ( hit.entity.SHealth  >=1)
	//		{
	//			hit.entity.SHealth -= 1*time_step;
	//		}
	//	} 

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

void ent_heatFlare()
{
	
   my.material = sc_mtl_obj_heatHaze;
   sc_skill(my, sc_myTexMov, 3);
      sc_skill(my, sc_myDepth, 2); //clip from depthmap, as it isn't really needed there and looks ugly with god rays
	vec_scale(my.scale_x,0.25);
	
   VECTOR tempPos;

   while(1)
   {
      //behave like a sprite -> face camera
      vec_set(tempPos,camera.x); 
      vec_sub(tempPos,my.x);
      vec_to_angle(my.pan,tempPos); 
      wait(1);
   }
}


void ent_volPart()
{
   sc_volPart(my); //model is a volumetric particle
   sc_skill(my, sc_myTexMov, 2.3); //animate/shift normalmap by factor 0.3
   vec_scale(my.scale_x,0.15);
} 

//A custom shader, that was built out of sc_obj_aio.fx. See Shade-C Manual for more information (Customizing Shade-C -> "Click together" Shaders)
MATERIAL* mtl_advObject =
{
	// set standardt material settings. These will be used by the shader
	diffuse_red = 200;
	diffuse_green = 200;
	diffuse_blue = 200;
	specular_blue = 180;
	specular_green = 180;
	specular_red = 180;
	power = 10;
	
	skin1 = map_envMap; //Set the environment map, as we are using Environment Mapping
   effect = "myShader.fx"; //This is a custom shader. You can easily create your own out of sc_obj_aio.fx in the shade-c/fx folder
   event = sc_mtlEvent_aio; //As we created a custom shader out of sc_obj_aio.fx, it is a good idea to set sc_mtlEvent_aio as our event, to set up the shader
	
}

MATERIAL* mtl_DoShadow =
{
	
	 effect = "sc_Terrain_doShadow.fx"; //This is a custom shader. You can easily create your own out of sc_obj_aio.fx in the shade-c/fx folder
}


function main()
{	
 video_mode = 12; // start in 640x480, 640x400, or 640x352 resolution
  video_screen = 2; 
	detail_size=60;
	camera.arc = 60;
	detail_size = 100;
	FStart_New_Game();
	VCameraSensitivity = 10;
	wait(3);
	
	//setup shade-c. Activate everything as we want to use everything!
	sc_bHDR = 1;
	sc_bDOF = 1;
	sc_bRefract = 1;
	sc_bWater = 1;
	sc_bReflect = 1; 
	sc_bVolParts = 1;
	sc_setup();
	camera.clip_far  = 100000; 
	//we want the Depth of Field Effect to autofocus objects, so activate the autofocus
	sc_dofDynFocus(400,50000,1);
	//we want dynamic softshadows, so activate them
	sc_smSunSetup(screen_size.x, 2000, 1500, 0.002, 0);
	//setup sky
	sc_skill(skycube,sc_myDepth, 0.95); //sky is 5% affected by DoF. If you are using DoF, you should set this (not needed, but this way the sky remains sharp)
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube,sc_myVolPart, 0);
	
	//set parameters (can be changed at any time, as they will be updated in real time)
	sc_dofParams[0] = 3500; //set DoF focal length
	sc_dofParams[1] = 200; //set DoF focal pos (this will be overwritten by sc_dofDynFocus() each frame)
	   sc_dofDynFocus(400,50000,1);
	sc_hdrParams[2] = 6; //set bloom strength	
	sc_lightRayStr = 0.6; //set light ray strength
	sc_lightRayLength = 6.5; //set light ray length
	
	//activate light rays
	sc_lightRays();
	
	//set mat_model to shadow material. Now all models without any material will receive shadows
	mat_model.effect = "sc_obj_doShadow.fx";
	//set mat_flat to shadow material. Now all level blocks with flat shading and without any material will receive shadows
	mat_flat.effect = "sc_level_doShadow.fx";
	mat_shaded.effect = "sc_level_doShadow.fx";
	
	
	
	
	
	
		
	//make the sun panel dummy visible, just so you know where the sun is ;)
//	set(sc_pan_sunDummy,SHOW);
	
	bmap_to_cubemap(map_envMap);
	sc_setup();
}

action AWeapon()
{
	Eweapon=me;
	my.layer=-1;
	set(me,PASSABLE);	
}

function FSetSky(int ADayTime)
{
	sun_angle.tilt=(((sys_hours*60)+sys_minutes)/4)-90;
		sun_angle.tilt=(((sys_hours*60)+sys_minutes)/4)-90;	
	if (ADayTime==0 || ADayTime==23){
		skycube.red = 43;
		skycube.green = 40;
		skycube.blue = 67;
	}
	if (ADayTime==1 || ADayTime==22){
		skycube.red = 57;
		skycube.green = 50;
		skycube.blue = 83;
	}
	if (ADayTime==2 || ADayTime==21){
		skycube.red = 78;
		skycube.green = 70;
		skycube.blue = 107;
	}
	if (ADayTime==3 || ADayTime==20){
		skycube.red = 99;
		skycube.green = 80;
		skycube.blue = 127;
	}
	if (ADayTime==4 || ADayTime==19){
		skycube.red = 121;
		skycube.green = 90;
		skycube.blue = 133;
	}
	if (ADayTime==5 || ADayTime==18){
		skycube.red = 172;
		skycube.green = 91;
		skycube.blue = 103;
	}
	if (ADayTime==6 || ADayTime==17){
		skycube.red = 197;
		skycube.green = 116;
		skycube.blue = 93;
	}
	if (ADayTime==7 || ADayTime==16){
		skycube.red = 202;
		skycube.green = 138;
		skycube.blue = 92;
	}
	if (ADayTime==8 || ADayTime==15){
		skycube.red = 201;
		skycube.green = 158;
		skycube.blue = 104;
	}
	if (ADayTime==9 || ADayTime==14){
		skycube.red = 219;
		skycube.green = 189;
		skycube.blue = 134;
	}
	if (ADayTime==10 || ADayTime==13){
		skycube.red = 240;
		skycube.green = 224;
		skycube.blue = 187;
	}
	if (ADayTime==11 || ADayTime==12){
		skycube.red = 255;
		skycube.green = 255;
		skycube.blue = 255;
	}
}


action APlayer()
{
	
	player = me;
	player.SHealth=10;
	player.SEnergy=100;
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
   set(my,INVISIBLE);
	
	while (my.SHealth>=1)
	{
		
	//	FSetSky(sys_hours);
	FSetSky(sys_hours);
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
      VMana = my.SMana;
      VHealth = my.SHealth;
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
		camera.z = my.z+50;
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
	level_load("zone1.wmb");
//	fog_color = 1; 
//	camera.fog_start = 1000; 
//	camera.fog_end = 3000; 
	Eweapon = ent_create ("sw_arm.mdl", vector(300,0,200), AWeapon);
	player = ent_create ("guard.mdl", vector(300,0,200), APlayer);	
	player.SHealth = 30;
	player.SMana = 10;
	player.SLevel = 1;

}

action AWaterEntity()
{
   set(my,PASSABLE);
   sc_water(my);
} 

action AInvis()
{
set(my,TRANSLUCENT);

 
 while (1)
 {
 	if (my.alpha<100)
 	{
 		my.alpha = my.alpha + 1*time_step;
   }
	else
	{
		my.alpha =0;
	}
 	wait (1);
 }	

reset(my,TRANSLUCENT);
}

action ANPC()
{
	set(my,POLYGON);
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
         
      if (my.SEnergy < 1)
      	{my.SRun = 0;}
      	
      if (my.SRun == 1)
      	{my.SEnergy = my.SEnergy - 1 * time_step;}
      else
      	{if(my.SEnergy < 100)
      		{my.SEnergy += 0.1 * time_step;}}

	
     // Ldist_ahead = Lwalk_speed*(key_w-key_s)*time_step;
      Ldist_ahead = sign(Ldist_ahead)*(abs(Ldist_ahead) + 0.5*Ldist_down);
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
			
		wait(1);
	}
	
	FKillChar(me);
}