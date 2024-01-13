//INCLUDES

#define PRAGMA_PATH "shade-c"
#define PRAGMA_PATH "stuff"
#define PRAGMA_PATH "Code"
#define PRAGMA_PATH "Models"
#define PRAGMA_PATH "Images"
#define PRAGMA_PATH "shaders"


#include <litec.h>
#include <d3d9.h>
#include <acknex.h>
#include <default.c>
#include <mtlFX.c>
#include <mtlView.c>
#include "sc_core.c"
//#include "sc_water.c"



#include "npc.c"
#include <shadows.c>
#include "VTZEffects.c"
#include "vars.c"

//DIFINES
#define PRAGMA_PATH "Models";
#define PRAGMA_PATH "CharModels";


#define SExp skill15
SOUND* sheepsnd = "sheep3.ogg";
SOUND* cutsheepsnd = "cut.ogg";

//VARIABLES
var compass_x,compass_y,m_x,m_y;
var vlevelload=0;
var vatt=0;
var vAir=0;

var VPlayerInventory[50];
var VCameraSensitivity;
var VEnergy=100;
var VHealth=30;
var VMana=30;
int VCurrentSpell = 0;

var VDay = 1;
var VSec = 0;
var rain_on = 0;
var vParInt =1;
var vParStrenth =1;
var vParStamina =1;
var vParAgility =1;

var vLevel=1;
STRING* vTextureName= "                      ";
STRING* vSpellName="                      ";

var rain_handle=0; // handle for the rain.wav sound
VECTOR vPlayer;

//ENTITES

BMAP* map_envMap = "sky04S+6.tga"; //an environment map for later use in a shader




ENTITY*  elf_1;
ENTITY* Eweapon;
ENTITY* ENPC;
ENTITY* skycube =
{
	//type = "sky04+6.png"; // the image for a backdrop scene
	type = "skycube+6.png";
	flags2 = SKY | CUBE | VISIBLE ;

}
//FONTS

FONT* arial_font = "Arial#30b";

//FUNCTIONS

function FStart_New_Game();
action APlayer();

//STRINGS

STRING* GameTitle = "Satmond 0.0.0.1";


//PANELS

function Spell1()
{
	if (FDedQuest>2)
	{
		VCurrentSpell=1;
		str_cpy(vSpellName,"Heal");
	}
}
function Spell2()
{
	if (FDedQuest>4)
	{
		VCurrentSpell=2;
		str_cpy(vSpellName,"Underwater breathe");
	}
}
function Spell3()
{
	if (FDedQuest>6)
	{
		VCurrentSpell=3;
		str_cpy(vSpellName,"Fireball");
	}
}
function Spell4()
{
	if (FDedQuest>10)
	{
		VCurrentSpell=4;
		str_cpy(vSpellName,"Summon");
	}
}

function AddStr()
{	
	if (vExp>0)
	if (vlevelload==0)
	{vLevel+=1;
		vlevelload=1;
		vExp-=1;
		player.SParStrenth+=1;
		vParStrenth=player.SParStrenth;
		wait(-1);
		vlevelload=0;
	}
}

function AddSta()
{	if (vExp>0)
	if (vlevelload==0)
	{vLevel+=1;
		vlevelload=1;
		vExp-=1;
		player.SParStamina+=1;
		vParStamina=player.SParStamina;
		wait(-1);
		vlevelload=0;
	}
}

function AddInt()
{	if (vExp>0)
	if (vlevelload==0)
	{vLevel+=1;
		vlevelload=1;
		vExp-=1;
		player.SParInt+=1;
		vParInt=player.SParInt;
		wait(-1);
		vlevelload=0;
	}
}

function AddAgi()
{	if (vExp>0)
	if (vlevelload==0)
	{
		vlevelload=1;
		vLevel+=1;
		vExp-=1;
		player.SParAgility+=1;
		vParAgility=player.SParAgility;
		wait(-1);
		vlevelload=0;
	}
}

PANEL* PSkills =
{
	pos_x = 4; pos_y = 150;
	digits(30, 0, "Level: %.0f", arial_font, 1, vLevel);
	digits(30, 30, "Skill points: %.0f", arial_font, 1, vExp);
	digits(30, 60, "[U] Add Strength: %.0f", arial_font, 1, vParStrenth);
	digits(30, 90, "[I] Add Stamina: %.0f", arial_font, 1, vParStamina);
	digits(30, 120, "[O] Add Agility: %.0f", arial_font, 1, vParAgility);
	digits(30, 150, "[P] Add Intelligence: %.0f", arial_font, 1, vParInt);
}

PANEL* PMainMenu =
{
	pos_x = 4; pos_y = 4;
	// digits(0,0,"Start new game",arial_font,1,0);
	// digits(60,60,"Load game",arial_font,1,0);
	// digits(120,120,"Exit",arial_font,1,0);
	
	//	digits(180, 10, "%.0f :", arial_font, 1, VEnergy);
	digits(80, 10, "%.0f :", arial_font, 1, VtimeH);
	digits(100, 10, 5, arial_font, 1, VtimeM);
	digits(160, 10, "%s" arial_font, 1, vSpellName);

	flags =  SHOW;
}
PANEL* PDoor =
{
	pos_x = 4; 
	pos_y = 4;

	digits(120, 80, "Press LMB to enter", arial_font, 1, Fstop_rain);
	
}

PANEL* PCross =
{
	pos_x = 4; 
	pos_y = 4;

	digits(0, 0, "+", arial_font, 1, Fstop_rain);
	flags =  SHOW;
}


PANEL* PSleep =
{
	pos_x = 4; 
	pos_y = 4;

	digits(120, 60, "Hold R to rest", arial_font, 1, Fstop_rain);
	
}

BMAP* compass_map = "compass.dds";
BMAP* hud_map = "hud.dds";
BMAP* magic_map = "magic.dds";

PANEL* air_pan=
{
	pos_x = 0; pos_y = 0;
	window(0,0,244,10,magic_map,vAir,m_y);
	flags = OVERLAY | SHOW;
}
PANEL* hud_pan =
{
	bmap = hud_map;	
	pos_x = 4; pos_y = 4;
	window(78,40,439,13,compass_map,compass_x,compass_y);
	window(71,56,244,10,magic_map,m_x,m_y);
	// digits(52,70, "$%.0f", "System#15b", 1, FPlayerPickupCount); 
	flags = OVERLAY | SHOW;
}

PANEL* panel_red =
{
	pos_x = 0; pos_y = 0;
	size_x = 1920; size_y = 1080;
	red = 255;
	green = 0;
	blue = 0;
	layer = 2;
	flags = LIGHT | TRANSLUCENT | SHOW;
	
}

PANEL* panel_black=
{
	pos_x = 0; pos_y = 0;
	size_x = 1920; size_y = 1080;
	red = 0;
	green = 0;
	blue = 0;
	layer = 11;
	flags = LIGHT | SHOW | TRANSLUCENT ;
}



function fade_in()
{
	set(panel_black,SHOW);
	panel_black.alpha = 0;

	while (panel_black.alpha <80)
	{
		panel_black.alpha += 8*time_step; 
		wait(2);
	}
	panel_black.alpha = 100;
	wait(1);	
}

function fade_out()
{
	panel_black.alpha = 100;

	while (panel_black.alpha >20)
	{
		panel_black.alpha -= 8*time_step; 
		wait(2);
	}
	panel_black.alpha = 0;
	reset(panel_black,SHOW);
}

action AWeapon()
{
	set(my, FLAG2);
	Eweapon = me;
	my.layer = -1;
	set(me, PASSABLE);
	reset(me, SHADOW);
}

action ASun()
{
	set(my,LIGHT);
	VECTOR temp;
	 sc_skill(my, sc_myDepth, 2);
	var t, p;
	set(my, PASSABLE);
	my.scale_x=my.scale_x*2;
	my.scale_y=my.scale_y*2;
	while (fglobalstop==1)
	{
		
		if (VtimeH <= 11) {
			p = 0;
			t = ((((VtimeH * 60) + VtimeM+(VtimeS/60)) / 4) - 90) - 1;
		}
		else {
			p = 180;
			t = (((((12 - (VtimeH - 12)) * 60) - VtimeM-(VtimeS/60)) / 4) - 90) - 1;
		}
		if (!(my.eflags&CLIPPED))
		{
			vec_set(temp, camera.x);
			vec_sub(temp, my.x);
			vec_to_angle(my.pan, temp);
		}

		reset(my, SHADOW);
		if (p == 0) {
			my.x = camera.x + cosv(t) * 4000;
		}
		else
		{
			my.x = camera.x - cosv(t) * 4000;
		}
		my.y = camera.y;
		my.z = camera.z - sinv(t + 180) * 4000;
		wait(1);
	}
}
SOUND* doorcl="door_cl.ogg";
SOUND* doorop="door_op.ogg";

function Fload_level(STRING* AName)
{
	if (vlevelload==0)
	{
		fglobalstop=0;
		wait(10);
		snd_stopall(4); 
		wait(1);
		vlevelload=1;
		snd_play(doorop, 80, 0);
		wait(1);
		fade_in();
		wait(-2);
		wait(1);
			pssm_run(0);
		level_load(NULL);
		fglobalstop=1;
		wait(1);
		level_load(AName);
		reset(PSleep,SHOW);
		Eweapon = ent_create ("sw_arm.mdl", vector(300, 0, 200), AWeapon);
		player = ent_create ("guard.mdl", vector(300, 0, 200), APlayer);
		
		wait(3);
		sc_skill(skycube, sc_myDepth, 0.95); //sky is 5% affected by DoF. If you are using DoF, you should set this (not needed, but this way the sky remains sharp)
		
		sc_skill(skycube, sc_myVolPart, 0);
		bmap_to_cubemap(map_envMap);

		for (you = ent_next(NULL); you; you = ent_next(you))
		set(you, SHADOW);
		reset(Eweapon,SHADOW);
		if ((VtimeH >= 6) && (VtimeH <= 17)){
				pssm_run(4);
		}
		snd_play(doorcl, 80, 0);
		fade_out();
		vlevelload=0;
	}
}




action ABed()
{
	while (!player){wait(1);}
	while (fglobalstop==1)
	{
		wait(4);
		if (!(my.eflags&CLIPPED))
		{
			if (vec_dist(my.x,player.x)<200)
			{
				set(PSleep,SHOW);
				if (key_r)
				
				{
					VtimeH+=1;
					Fstop_rain-=300;
					if (player.SHealth<30+(player.SParStamina*4))
					{ 
						player.SHealth+=5;
						
						if (player.SHealth>30+(player.SParStamina*4))
						{
							player.SHealth=30+(player.SParStamina*4);
						}	
					}
					if (player.SMana<30+(player.SParInt*4))
					{ 
						player.SMana+=5;
						if (player.SMana>30+(player.SParInt*4))
						{
							player.SMana=30+(player.SParInt*4);
						}
						
					}
					if (VtimeH>23){VtimeH=0;}
					wait(-1);
				}
			}
			else
			reset(PSleep,SHOW);
		}
		else
		reset(PSleep,SHOW);
	}
}


action ADoor()
{
	while (fglobalstop==1)
	{	
		if (vec_dist(my.x, player.x) < 100)
		{
			set(PDoor,SHOW);
			if (mouse_left == 1)
			{
				vec_set(vPlayer, vector(my.skill2, my.skill3, my.skill4));
				if (my.skill1 == 1) {
					Fload_level("zone1.wmb");
					Vcave=0;
				}
				if (my.skill1 == 2) {
					Fload_level("house.wmb");
				}
				if (my.skill1 == 3) {
					if (LSayQuest>1)
					{
						Vcave=1;
						Fload_level("cave.wmb");
					}
					else
					{
						snd_play(locked, 100, 0);
						wait(-2);
					}
				}
			}
		}
		else
		{if (vec_dist(my.x, player.x) < 500)
		reset(PDoor,SHOW);}
		wait(1);
	}

}

function FKillChar(ENTITY* AChar)
{
	var LAnim = 0;
	set(AChar, PASSABLE);
	while (LAnim < 99)
	{
		LAnim += 5 * time_step;
		if (LAnim >= 100) {
			LAnim = 99;
		}
		ent_animate(AChar, "death", LAnim, 0);
		wait(1);
	}
	int i;
	i = 0;
	while (i < 100) {
		my.z -= 0.1 * time_step;
		i += 0.1 * time_step;
		wait(1);
	}
	ent_remove(me);
}
var hithandle=0;
function SwordAttEvent()
{ 	
	if (you){
		if (you.SHealth>0)
		{
			if (snd_playing(hithandle)==0)
			{
			hithandle=snd_play(hitsnd,100,0);}
			you.SHealth -=my.skill1;
		}
	}		
}

action ASwordAtt()
{
	my.skill2=5;
	my.skill1=you.SParStrenth/2;
	set(my,NARROW);
	set(my,INVISIBLE);
	my.pan=you.pan;
	my.z=my.z-10;
	set(my,FLAG2);
	my.tilt=you.tilt;
	my.emask |= (ENABLE_ENTITY); 
	my.event = SwordAttEvent;
	while(my.skill2>0)
	{
		my.skill2-=1*time_step;
		c_move(me,vector(20*time_step,0,0),nullvector,IGNORE_PASSABLE | IGNORE_YOU|IGNORE_SPRITES|IGNORE_FLAG2); // move ahead until obstacle is hit
		//	effect(flm_part_func2, 1, my.x, nullvector);		
		wait(1);
	}	
	wait(-2);
	ent_remove(me);
}

function FUseWeapon(ENTITY* AChar)
{

	me.SAttack = 1;
	var LAnim = 100;
	ent_create("bullet.mdl",camera.x,ASwordAtt);
	snd_play(misssnd,100,0);
	while (key_pressed(280))
	{
		while (LAnim > 10)
		{
			LAnim -= 10 * time_step;
			ent_animate(AChar, "attack", LAnim, ANM_CYCLE);
			wait(1);
		}
		me.SAttack = 2;
		wait(1);
	}
	me.SAttack = 0;
	

}




SOUND* fireball_wav = "explo.ogg";
function FireBallEvent()
{
	
	my.skill2=0;
	effect(flm_part_func3, 1, my.x, nullvector);
	wait (1);
	my.event = NULL; 
	if (snd_playing(hexplo)!=0)
	{
		snd_stop(hexplo);wait(1);
	}
	hexplo=ent_playsound(my, fireball_wav, 200);
	
	if (you){you.SHealth -=my.skill1;}
	
	
	set (my, INVISIBLE);
	set (my, PASSABLE);
	wait(-1);
	ent_remove (my);
	
}

action ATarget()
{
	my.SHealth=1;
	while ((my.SHealth>0)&&(fglobalstop==1))
	{
		wait(5);
	} 
	if (fglobalstop==1)
	{
		if (FDedQuest == 7){FSheepHair+=1;}
		if (FSheepHair==3){FDedQuest=8;}
	}
	ent_remove(me);
}

action AFireBall()
{
	my.skill2=100;
	my.skill1=you.SParInt*8;
	if (snd_playing(hfire)!=0)
	{
		snd_stop(hfire);wait(1);
	}
	hfire = ent_playloop(my, fire, 100);
	set(my,NARROW);
	set(my,INVISIBLE);
	my.pan=you.pan;
	my.z=my.z-10;
	set(my,FLAG2);
	my.tilt=you.tilt;
	my.emask |= ( ENABLE_ENTITY); 
	my.event = FireBallEvent;
	while(my.skill2>0)
	{
		my.skill2-=1*time_step;
		c_move(me,vector(20*time_step,0,0),nullvector,IGNORE_PASSABLE | IGNORE_YOU|IGNORE_SPRITES|IGNORE_FLAG2); // move ahead until obstacle is hit
		my.lightrange = maxv(40, minv(150, (my.lightrange + random(10) - 5)));
		effect(flm_part_func2, 1, my.x, nullvector);
		//		effect(smoke_part_func, 1, my.x, nullvector);
		
		wait(1);
	}	
	snd_stop(hfire);
	wait(-2);`
	ent_remove(me);
}

SOUND* mgc1="gate_open.ogg";
SOUND* mgc2="gate_close.ogg";

function FCastSpell(int ASpellId, ENTITY* AChar)
{
	if (ASpellId==1)
	{
		if (AChar.SMana>10)
		{
			AChar.SMana -=10;
			AChar.SHealth +=(10*(AChar.SParInt))-(random(AChar.SParInt))/4;
			snd_play(mgc1, 100, 0);
		}
	}
	if (ASpellId==2)
	{
		if (AChar.SMana>20)
		{
			AChar.SMana -=20;
			AChar.SFall +=(200*(AChar.SParInt))+(random(AChar.SParInt*100));
			snd_play(mgc2, 100, 0);
		}
	}
	if (ASpellId==3)
	{
		if (AChar.SMana>3)
		{
			AChar.SMana -=3;
			
			ent_create("bullet.mdl",camera.x,AFireBall);
			
		}
	}


}

void ent_heatFlare()
{

	my.material = sc_mtl_obj_heatHaze;
	sc_skill(my, sc_myTexMov, 3);
	sc_skill(my, sc_myDepth, 2); //clip from depthmap, as it isn't really needed there and looks ugly with god rays
	vec_scale(my.scale_x, 0.25);

	VECTOR tempPos;

	while (fglobalstop==1)
	{
		//behave like a sprite -> face camera
		vec_set(tempPos, camera.x);
		vec_sub(tempPos, my.x);
		vec_to_angle(my.pan, tempPos);
		wait(1);
	}
}


void ent_volPart()
{
	sc_volPart(my); //model is a volumetric particle
	sc_skill(my, sc_myTexMov, 2.3); //animate/shift normalmap by factor 0.3
	vec_scale(my.scale_x, 0.15);
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


action ASheep()
{
	my.skill1 += random(100); 
	set(my, PASSABLE);
	my.skill2=0;
	my.red   = 128;
	my.green = 128;
	var sndhandle=0;
	my.blue  = 0;
	while (fglobalstop==1)
	{
		if (VtimeH>6 && VtimeH<19)
		{
			my.lightrange=0;reset(my,LIGHT);
			if (my.skill2==0)
			{
				my.skill3 -=  time_step;				
				if (my.skill3 < 1) 
				{
					my.skill2 = 1;
				}
				ent_animate(me, "death", my.skill3, ANM_CYCLE);
				
			}
			else
			{		
				if (!(my.eflags&CLIPPED))
				{
					my.skill1 += random(3) * time_step; // torso animation
					if (my.skill1 > 100) {
						my.skill1 = 0;
					}
					ent_animate(me, "Stand", my.skill1, ANM_CYCLE);
				}
			}
			if (!(my.eflags&CLIPPED))
			if (vec_dist(my.x, player.x) < 100)
			{
				if (mouse_left == 1) {
					if (snd_playing(sndhandle)==0)
					{sndhandle=ent_playsound(my, sheepsnd, 500);}
				}
			}
			
		}		
		else
		{
			if (my.skill3 < 66){my.skill3 += time_step;}
			ent_animate(me, "death", my.skill3, ANM_CYCLE);
			my.skill2=0;
			
			if ((FDedQuest == 3)&&(FSheepHair==0))
			{
				set(my,LIGHT);
				my.lightrange=50;
				if (vec_dist(my.x, player.x) < 100)
				{
					set(PItem,SHOW);
					if (mouse_left == 1) {
						FSheepHair=1;
						if ((FButterfly==1)&&(FSheepHair==1)){FDedQuest=4;}
						reset(PItem,SHOW);
						ent_playsound(my, cutsheepsnd, 500);
						
					}
				}
				else
				{reset(PItem,SHOW);}
			}
			else
			{
				my.lightrange=0;reset(my,LIGHT);
			}
		}

		wait(1);
	}
}

action AQuestGrass()
{
	set(my, PASSABLE);
	my.red   = 128;
	my.green = 128;
	my.blue  = 0;
	var snd_handle=0;
	var LGet=0;
	while ((LGet<5)&&(fglobalstop==1))
	{
		if (!(my.eflags&CLIPPED))
		{
			my.skill1 += 3 * time_step; // torso animation
			if (my.skill1 > 100) {
				my.skill1 = 0;
			}
			ent_animate(me, "Base", my.skill1, ANM_CYCLE);
		}
		if (FDedQuest==1)
		{
			set(my,LIGHT);
			my.lightrange=50;
			if (vec_dist(my.x, player.x) < 100)
			{
				set(PItem,SHOW);
				if (mouse_left == 1) {
					LGet = 11;
					FDedQuest=2;
					reset(PItem,SHOW);
					snd_handle = ent_playsound(my, cutsheepsnd, 500);
					wait(1);
				}
			}
			else
		{if (vec_dist(my.x, player.x) < 200){reset(PItem,SHOW);}}
		}
		else
		{my.lightrange=0;reset(my,LIGHT);}
		

		wait(2);
	}
	if (fglobalstop==1)
	{
		set(my,INVISIBLE);
		my.lightrange=0;
		while(snd_playing(snd_handle)!=0){wait(1);}
		ent_remove(me);
	}
}

action AGrass2()
{
	ent_remove(me);
	//		set(my, PASSABLE);
	//		reset(my,DYNAMIC);
	//		reset(my,SHADOW);
	//		wait(-5);
	//		reset(my,SHADOW);
}

action AGrass()
{
	ent_remove(me);
	//		set(my, PASSABLE);
	//		wait(-5);
	//		reset(my,SHADOW);
	//		while (1)
	//		{
		//				if (!(my.eflags&CLIPPED))
		//				{
			//						my.skill1 += 3 * time_step; // torso animation
			//						if (my.skill1 > 100) {
				//								my.skill1 = 0;
			//						}
			//						ent_animate(me, "Base", my.skill1, ANM_CYCLE);
			//						wait(2);
		//				}
		//				else
		//				{wait(-1);}
	//		}
}

function toggle_rain_timer()
{
	while(1)
	{
		
		if (VtimeH<6)
		{
			random_seed(0);
			if (random(100)<10)
			{
				if (Fstop_rain==0)
				{
					Fstop_rain=1000;
					toggle_rain2();
				}
			}
		}
		else
		{
			Fstop_rain=0;
		}
		wait(-1);
	}
}



function main()
{
	d3d_automaterial=4;
	d3d_antialias = 4;
	vec_set(d3d_lodfactor,vector(1,1.5,1.5));
	warn_level = 3; 
	shadow_lod = 1;
	video_mode = 12; // start in 640x480, 640x400, or 640x352 resolution
	video_screen = 1;
	//d3d_anisotropy = 7;
	//	 d3d_mipmapping = 4;
	// d3d_antialias = 9;
	panel_red.alpha=0;
	panel_black.alpha=100;
	camera.arc = 60;
	detail_size = 100;
	shadow_stencil = 8;
	FStart_New_Game();
	VCameraSensitivity = 10;
	
	wait(3);
	
	PCross.pos_x = ((screen_size.x)/2)-5; 
	PCross.pos_y = ((screen_size.y)/2)-5;
	air_pan.pos_x = ((screen_size.x)/2)-122; 
	air_pan.pos_y = ((screen_size.y)/2)-10;
	
	

	//setup shade-c. Activate everything as we want to use everything!
	sc_bHDR = 1;
	sc_bDOF = 1;
	sc_bRefract = 1;
	sc_bWater = 1;
	sc_bReflect = 1;
	sc_bVolParts = 1;
			sc_setup();
	camera.clip_near = 1; 
	camera.clip_far  = 1000000;
	//we want the Depth of Field Effect to autofocus objects, so activate the autofocus
		sc_dofDynFocus(100,900000,1);
	//we want dynamic softshadows, so activate them
	//sc_smSunSetup(screen_size.x, 2000, 150, 0.0002, 0);
	//setup sky
	sc_skill(skycube, sc_myDepth, 0.95); //sky is 5% affected by DoF. If you are using DoF, you should set this (not needed, but this way the sky remains sharp)
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube, sc_myVolPart, 0);

	//set parameters (can be changed at any time, as they will be updated in real time)
	sc_dofParams[0] = 1000; //set DoF focal length


	sc_hdrParams[2] = 82; //set bloom strength	
	sc_lightRayStr = 0.6; //set light ray strength
	sc_lightRayLength = 6.5; //set light ray length

		sc_lightRays();


	bmap_to_cubemap(map_envMap);
	


	for (you = ent_next(NULL); you; you = ent_next(you))
	set(you, SHADOW);
	reset(Eweapon,SHADOW);
	pssm_fbias = 0.0005;
	pssm_res = 2048;
	pssm_splitweight = 1;
	pssm_transparency=0.5;
		pssm_run(4);
	VDay = 1;
	toggle_rain_timer();
	//	camera.fog_start=100;
	//	camera.fog_end=1000;
	fog_color=0;

	fade_out();
	on_u = AddStr;
	on_i = AddSta;
	on_o = AddAgi;
	on_p = AddInt;
	on_1= Spell1;
	on_2= Spell2;
	on_3= Spell3;
	on_4= Spell4;
	camera.fog_start = 0; // fog starts at 80% of clip range
	camera.fog_end = 10000; 
	
	var piece_number;	
	var music_handle = 0;
	while (1)
	{
		if (media_playing (music_handle) == 0)
		{
			random_seed(0);
			piece_number = integer(random(3)+1 );
			if (piece_number == 1)
			{
				music_handle=	media_play("track1.mp3", NULL, 100);
			}
			if (piece_number == 2)
			{
				music_handle=	media_play("track2.mp3", NULL, 100);
			}
			if (piece_number == 3)
			{
				music_handle=	media_play("track3.mp3", NULL, 100);
			}
			if (piece_number == 4)
			{
				music_handle=	media_play("track4.mp3", NULL, 100);
			}
			
		}
		wait(-1);
	}
}



int GetTimeColor(int From, To)
{

	return (((To - From) / 60) * VtimeM);

}

function FSetSky(int ADayTime)
{
	var r, g, b;
	if (VtimeH <= 11) {
		sun_angle.pan = 0;
		sun_angle.tilt = ((((VtimeH * 60) + VtimeM + (VtimeS / 60)) / 4) - 90) - 1;
	}
	else {
		sun_angle.pan = 180;
		sun_angle.tilt = (((((12 - (VtimeH - 12)) * 60) - VtimeM - (VtimeS / 60)) / 4) - 90) - 1;
	}

	if ((ADayTime >= 6) && (ADayTime <= 17)) {
		vec_set(ambient_color, vector(50, 50, 50)) ;
		if (VDay == 0) {
			VDay = 1;
				pssm_run(4);
		}
	}
	else
	{
		vec_set(ambient_color, vector(20, 20, 20));
		if (VDay == 1) {
			VDay = 0;
				pssm_run(0);
		}
	}

	if (ADayTime == 6) {
		vec_set(ambient_color, vector(30 + VtimeM * 0.33, 30 + VtimeM * 0.33, 30 + VtimeM * 0.33));
	}
	if (ADayTime == 17) {
		vec_set(ambient_color, vector(50 - VtimeM * 0.33, 50 - VtimeM * 0.33, 50 - VtimeM * 0.33));
	}
	if (ADayTime >= 5)
	{
		if (ADayTime < 8)
		{
			vec_set(sun_color, vector((((VtimeH - 5) * 60) + VtimeM) * 0.83, (((VtimeH - 5) * 60) + VtimeM) * 0.83, (((VtimeH - 5) * 60) + VtimeM) * 0.83));
		}
		else
		{
			if (ADayTime < 18)
			{
				vec_set(sun_color, vector(150, 150, 150));
			}
			else if (ADayTime <= 20)
			{
				vec_set(sun_color, vector(((180 - ((VtimeH - 18) * 60) - VtimeM)) * 0.83, ((180 - ((VtimeH - 18) * 60) - VtimeM)) * 0.83, ((180 - ((VtimeH - 18) * 60) - VtimeM)) * 0.83));
			}
			else
			{
				vec_set(sun_color, vector(0, 0, 0));
			}
		}
	}
	else
	{
		vec_set(sun_color, vector(0, 0, 0));
	}

	if (ADayTime == 0 || ADayTime == 23) {
		if (ADayTime == 0) {
			r = GetTimeColor(43, 57);
			g = GetTimeColor(40, 5);
			b = GetTimeColor(67, 83);
		}
		if (ADayTime == 23) {
			r = 0;
			g = 0;
			b = 0;
		}
		skycube.red = 43 + r;
		skycube.green = 40 + g;
		skycube.blue = 67 + b;

	}
	if (ADayTime == 1 || ADayTime == 22) {
		if (ADayTime == 1) {
			r = GetTimeColor(57, 78);
			g = GetTimeColor(5, 70);
			b = GetTimeColor(83, 107);
		}
		if (ADayTime == 22) {
			r = GetTimeColor(57, 43);
			g = GetTimeColor(5, 40);
			b = GetTimeColor(83, 67);
		}
		skycube.red = 57 + r;
		skycube.green = 5 + g;
		skycube.blue = 83 + b;

	}
	if (ADayTime == 2 || ADayTime == 21) {
		if (ADayTime == 2) {
			r = GetTimeColor(78, 99);
			g = GetTimeColor(70, 80);
			b = GetTimeColor(107, 127);
		}
		if (ADayTime == 21) {
			r = GetTimeColor(78, 57);
			g = GetTimeColor(70, 5);
			b = GetTimeColor(107, 83);
		}
		skycube.red = 78 + r;
		skycube.green = 70 + g;
		skycube.blue = 107 + b;

	}
	if (ADayTime == 3 || ADayTime == 20) {
		if (ADayTime == 3) {
			r = GetTimeColor(99, 121);
			g = GetTimeColor(80, 90);
			b = GetTimeColor(127, 133);
		}
		if (ADayTime == 20) {
			r = GetTimeColor(99, 78);
			g = GetTimeColor(80, 70);
			b = GetTimeColor(127, 107);
		}
		skycube.red = 99 + r;
		skycube.green = 80 + g;
		skycube.blue = 127 + b;


	}
	if (ADayTime == 4 || ADayTime == 19) {
		if (ADayTime == 4) {
			r = GetTimeColor(121, 172);
			g = GetTimeColor(90, 91);
			b = GetTimeColor(133, 103);
		}
		if (ADayTime == 19) {
			r = GetTimeColor(121, 99);
			g = GetTimeColor(90, 80);
			b = GetTimeColor(133, 127);
		}
		skycube.red = 121 + r;
		skycube.green = 90 + g;
		skycube.blue = 133 + b;

	}
	if (ADayTime == 5 || ADayTime == 18) {
		if (ADayTime == 5) {
			r = GetTimeColor(172, 197);
			g = GetTimeColor(91, 116);
			b = GetTimeColor(103, 93);
		}
		if (ADayTime == 18) {
			r = GetTimeColor(172, 121);
			g = GetTimeColor(91, 90);
			b = GetTimeColor(103, 133);
		}
		skycube.red = 172 + r;
		skycube.green = 91 + g;
		skycube.blue = 103 + b;

	}
	if (ADayTime == 6 || ADayTime == 17) {
		if (ADayTime == 6) {
			r = GetTimeColor(197, 202);
			g = GetTimeColor(116, 138);
			b = GetTimeColor(93, 92);
		}
		if (ADayTime == 17) {
			r = GetTimeColor(197, 172);
			g = GetTimeColor(116, 91);
			b = GetTimeColor(93, 103);
		}
		skycube.red = 197 + r;
		skycube.green = 116 + g;
		skycube.blue = 93 + b;

	}
	if (ADayTime == 7 || ADayTime == 16) {
		if (ADayTime == 7) {
			r = GetTimeColor(202, 201);
			g = GetTimeColor(138, 158);
			b = GetTimeColor(92, 104);
		}
		if (ADayTime == 16) {
			r = GetTimeColor(202, 197);
			g = GetTimeColor(138, 116);
			b = GetTimeColor(92, 93);
		}
		skycube.red = 202 + r;
		skycube.green = 138 + g;
		skycube.blue = 92 + b;

	}
	if (ADayTime == 8 || ADayTime == 15) {
		if (ADayTime == 8) {
			r = GetTimeColor(201, 219);
			g = GetTimeColor(158, 189);
			b = GetTimeColor(104, 134);
		}
		if (ADayTime == 15) {
			r = GetTimeColor(201, 202);
			g = GetTimeColor(158, 138);
			b = GetTimeColor(104, 92);
		}
		skycube.red = 201 + r;
		skycube.green = 158 + g;
		skycube.blue = 104 + b;

	}
	if (ADayTime == 9 || ADayTime == 14) {
		if (ADayTime == 9) {
			r = GetTimeColor(219, 240);
			g = GetTimeColor(189, 224);
			b = GetTimeColor(134, 187);
		}
		if (ADayTime == 14) {
			r = GetTimeColor(219, 201);
			g = GetTimeColor(189, 158);
			b = GetTimeColor(134, 104);
		}
		skycube.red = 219 + r;
		skycube.green = 189 + g;
		skycube.blue = 134 + b;

	}
	if (ADayTime == 10 || ADayTime == 13) {
		if (ADayTime == 10) {
			r = GetTimeColor(240, 255);
			g = GetTimeColor(224, 255);
			b = GetTimeColor(187, 255);
		}
		if (ADayTime == 13) {
			r = GetTimeColor(240, 219);
			g = GetTimeColor(224, 189);
			b = GetTimeColor(187, 134);
		}
		skycube.red = 240 + r;
		skycube.green = 224 + g;
		skycube.blue = 187 + b;

	}
	if (ADayTime == 11 || ADayTime == 12) {
		if (ADayTime == 12) {
			r = GetTimeColor(255, 240);
			g = GetTimeColor(255, 224);
			b = GetTimeColor(255, 187);
		}
		if (ADayTime == 11) {
			r = 0;
			g = 0;
			b = 0;
		}
		skycube.red = 255 + r;
		skycube.green = 255 + g;
		skycube.blue = 255 + b;

	}
	d3d_fogcolor1.red = skycube.red;
	d3d_fogcolor1.green = skycube.green;
	d3d_fogcolor1.blue = skycube.blue; // pink fog


}

SOUND* water_snd = "lp_flus.ogg";
SOUND* unwater_snd = "lp_lava.ogg";

SOUND* tap1 = "tap.ogg";
SOUND* tap2 = "toprak.ogg";
SOUND* tap3 = "splash.ogg";

function HitPan()
{
	panel_red.alpha=20;
	while(panel_red.alpha>0)
	{
		panel_red.alpha=panel_red.alpha-1*time_step;
		wait(1);
	}
	
}


action APlayer()
{
//	set(my,SPOTLIGHT);
//	my.lightrange=800;
	my.SHealth = VHealth;
	my.SMana = VMana;
	my.SLevel = vLevel;
	my.SParAgility = vParAgility;
	my.SParStamina = vParStamina;
	my.SParStrenth = vParStrenth;
	my.SParInt = vParInt;
	my.SEnergy = VEnergy;
	my.SExp =vExp;
	var waterHandle=0;
	var htap=0;
	waterHandle = snd_loop(water_snd, 0, 0);
	var unwaterHandle = snd_loop(unwater_snd, 0, 0);
	set(my, FLAG2);
	my.x = vPlayer.x;
	my.y = vPlayer.y;
	my.z = vPlayer.z;
	player = me;
	if ((my.eflags & FAT) && (my.eflags & NARROW)) // when FAT+NARROW are both set
	my.min_z *= 0.5;
	var Lspeed_down = 0;   // downward speed by gravity
	var Lanim_percent = 0; // animation percentage
	var Lwalk_speed;
	var Ldist_down;
	var Ldist_ahead;
	var Ldist_side;
	VECTOR LvFeet;
	vec_for_min(LvFeet, me); // vFeet.z = distance from player origin to lowest vertex
	set(my, INVISIBLE);
	FSetSky(VtimeH);
	

	var temph, tempm;
	var lfootstep;me.SAttack = 0;

	while ((my.SHealth >= 1)&&(fglobalstop==1))
	{//////////                    13.52
		if (my.SFall>0){my.SFall-=time_step;}
		if (vAir>244){my.SHealth-=time_step;}
	if (vExp>0){set(PSkills,SHOW);}else{reset(PSkills,SHOW);}

		compass_x=((460/(30+(my.SParStamina*4)))*((30+(my.SParStamina*4))-my.SHealth));
		
		m_x=(245/(30+(my.SParInt*4)))*((30+(my.SParInt*4))-VMana);
		if (fog_color>0)
		fog_color = 1; // fog color 1 active 
		if (camera.z>120){snd_tune(waterHandle,1,0,0);}
		else
		if (camera.z>0)
		{
			snd_tune(waterHandle,(120-camera.z)/2,0,0);
			snd_tune(unwaterHandle,1,0,0);
			vAir=0;
			reset(air_pan,SHOW);
			if (Fstop_rain<1)
			{
			camera.fog_end = 10000;}
			else
			{camera.fog_end = 5000;}
		}
		else {
			snd_tune(waterHandle,1,0,0);
			fog_color=2;
			camera.fog_end = 1000; 
			snd_tune(unwaterHandle,20,0,0);
			if (my.SFall<1)
			{vAir+=(2-(vParStamina/10))*time_step;}
			set(air_pan,SHOW);
		}
		
		



		// if (tempm!=sys_seconds)
		{
			//   tempm=sys_seconds;
			//  VtimeM +=0.5*time_step ;
			VtimeS += 1 * time_step;
			if (VtimeS > 59) {
				VtimeS = 0;
				VtimeM += 1;
			}
			if (VtimeM > 59) {
				VtimeM = 0; VtimeH += 1; while (VtimeH > 23) {
					VtimeH = VtimeH - 24;
				}
			}
		}

		// if (VSec != VtimeM) {
			//   VSec = VtimeM;
			FSetSky(VtimeH);
			if (camera.z<0){skycube.red = 50;
				skycube.green = 100;
			skycube.blue = 255;}
		// }

		if (c_trace(my.x, vector(my.x, my.y, my.z - 5000), IGNORE_FLAG2 | IGNORE_ME | IGNORE_PASSABLE | USE_BOX|SCAN_TEXTURE) > 0)
		Ldist_down = my.z + LvFeet.z - target.z;
		else
		Ldist_down = 0;

		if (Ldist_down > -1)
		Ldist_down = clamp(Ldist_down, 0, accelerate(Lspeed_down, 5, 0.1));
		else
		Lspeed_down = 0;


		VMana = my.SMana;
		
		if (VHealth>my.SHealth)
		{
			HitPan();
		}
		
		
		VHealth = my.SHealth;
		Lwalk_speed = 8 + ((5+my.SParAgility) * my.SRun);
		my.tilt=camera.tilt;
	if (camera.z<10){Ldist_down=0; my.SSwim=1;}else{my.SSwim=0;}

		Ldist_ahead = Lwalk_speed * (key_w - key_s) * time_step;
		
		lfootstep+=abs(Ldist_ahead);
		if (lfootstep>70){
			lfootstep=0;
			if (hit.texname){
			str_cpy(vTextureName, hit.texname);}
			if (snd_playing(htap)!=0)
			{
				snd_stop(htap);wait(1);
			}
		if (my.z<35){ if (camera.z>0){htap=snd_play(tap3,20,0);}} else
			if (str_cmpi("ZONE1.HMP",vTextureName)){htap=snd_play(tap2,20,0);}
			else
			{htap=snd_play(tap1,20,0);}
		}
		
		
		
		
		Ldist_ahead = sign(Ldist_ahead) * (abs(Ldist_ahead) + 0.5 * Ldist_down);
		c_move(me, vector(Ldist_ahead, 0, 0), vector(0, 0, -Ldist_down), IGNORE_FLAG2 | IGNORE_PASSABLE | GLIDE|SCAN_TEXTURE);

		Ldist_side = 5 * (key_a - key_d) * time_step;
		Ldist_side = sign(Ldist_side) * (abs(Ldist_side) + 0.5 * Ldist_down);
		c_move(me, vector(0, Ldist_side, 0), vector(0, 0, -Ldist_down), IGNORE_FLAG2 | IGNORE_PASSABLE | GLIDE|SCAN_TEXTURE);
		if (me.SAttack != 1)
		{
			if (Ldist_ahead != 0)
			{
				Lanim_percent += 1.3 * Ldist_ahead;
				ent_animate(me, "walk", Lanim_percent, ANM_CYCLE);
				ent_animate(Eweapon, "walk", Lanim_percent, ANM_CYCLE);
			}
			else
			{
				Lanim_percent += 5 * time_step;
				ent_animate(me, "stand", Lanim_percent, ANM_CYCLE);
				ent_animate(Eweapon, "stand", Lanim_percent, ANM_CYCLE);
			}
		}
		
		my.SRun = key_shift;
		if (my.SEnergy < 1)
		{
			my.SRun = 0;
		}

		if (my.SRun == 1)
		{
			if (Ldist_ahead>0)
			my.SEnergy = my.SEnergy - (((1-(my.SParAgility/20))/2) * time_step);
		}
		else
		{ 
			if ((!key_shift)||(Ldist_ahead==0))if (my.SEnergy < 100+ (my.SParStamina*5))
			{
				my.SEnergy += (my.SParStrenth) * time_step;
			}
		}


		VEnergy = my.SEnergy;


		my.pan -= mouse_force.x * time_step * VCameraSensitivity;
		camera.tilt += mouse_force.y * time_step * VCameraSensitivity;

		if (camera.tilt <= -56)
		camera.tilt = -56;
		if (camera.tilt >= 80)
		camera.tilt = 80;

		Eweapon.x = my.x;
		Eweapon.y = my.y;
		Eweapon.z = my.z;
		Eweapon.pan = my.pan;

		camera.x = my.x;
		camera.y = my.y;
		if ((camera.z - 50) - my.z > 1) {
			camera.z = camera.z - 0.1 * abs((camera.z - 50) - my.z);
		}
		if ((camera.z - 50) - my.z < 1) {
			camera.z = camera.z + 0.1 * abs((camera.z - 50) - my.z);
		}

		//  camera.z = my.z+50;

		camera.pan = my.pan;

		if (key_pressed(280))
		{
			if (me.SAttack == 0)
			{
				FUseWeapon(Eweapon);
				FUseWeapon(player);
			}
		}

		if (key_pressed(281))
		{
			my.SCast=1;
		}
		else
		{  if (my.SCast==1)
			{
				FCastSpell(VCurrentSpell, me);
			}
		my.SCast=0;}

		wait(1);
	}
	if (fglobalstop==1)
	{
		compass_x=((460/(30+(my.SParStamina*4)))*((30+(my.SParStamina*4))-my.SHealth));		
		m_x=(245/(30+(my.SParInt*4)))*((30+(my.SParInt*4))-VMana);
		HitPan();
		fade_in();
		wait(-2);
		snd_stop(waterHandle);
		FKillChar(me);
		FKillChar(Eweapon);
	}
	snd_stop(waterHandle);
	snd_stop(unwaterHandle);
}

function FStart_New_Game()
{
	level_load("cave.wmb");

	//  fog_color = 1;
	//  camera.fog_start = 1000;
	//  camera.fog_end = 3000;
	vec_set(vPlayer, vector(0, 0, 500));
	Eweapon = ent_create ("sw_arm.mdl", vector(300, 0, 200), AWeapon);
	player = ent_create ("guard.mdl", vector(300, 0, 200), APlayer);


}



action ent_heatFlare()
{

	my.material = sc_mtl_obj_heatHaze;
	sc_skill(my, sc_myTexMov, 3);
	sc_skill(my, sc_myDepth, 2); //clip from depthmap, as it isn't really needed there and looks ugly with god rays
	vec_scale(my.scale_x, 0.25);

	VECTOR tempPos;

	while (fglobalstop==1)
	{
		//behave like a sprite -> face camera
		if (!(my.eflags&CLIPPED))
		{
			vec_set(tempPos, camera.x);
			vec_sub(tempPos, my.x);
			vec_to_angle(my.pan, tempPos);
		}
		wait(1);
	}
}





action my_mouse()
{
	my.red   = 128;
	my.green = 128;
	my.blue  = 0;
	set(my,PASSABLE);
	var run_frames;
	var distance = 0;
	var previous_pan;
	var following_pan;
	var min_speed = 1;
	var max_speed = 3;
	var mouse_speed;
	VECTOR last_pos[3];
	VECTOR direction[3];
	var LDestroy=0;

	while((LDestroy==0)&&(fglobalstop==1))

	{
		if ((VtimeH>6 && VtimeH<19)|| (is(my,FLAG3)))
		{
			reset(my,INVISIBLE);
			if (is(my,FLAG3)==false)
			if (((FDedQuest == 3)&&(FButterfly==0)))
			{
				set(my,LIGHT);
				my.lightrange=50;
				
				if (vec_dist(my.x, player.x) < 100)
				{
					set(PItem,SHOW);
					if (mouse_left == 1) {
						FButterfly=1;
						reset(PItem,SHOW);
						set(my,INVISIBLE);
						LDestroy=1;
						if ((FButterfly==1)&&(FSheepHair==1)){FDedQuest=4;}
					}
				}
				else
				{reset(PItem,SHOW);}
			}
			else
			{
				my.lightrange=0;reset(my,LIGHT);
			}
		}
		else
		{
			set(my,INVISIBLE);
		}
		previous_pan = my.pan;
		path_spline(my, my.x, distance);
		distance += mouse_speed * time_step;		// let the mouse look ahead

		vec_diff(direction, my.x, last_pos);
		vec_to_angle(my.pan, direction);
		vec_set(last_pos, my.x);
		wait(1);
		following_pan = my.pan;
		// sudden direction change during the last frame? Then lower the speed of the mouse!

		if (abs(following_pan - previous_pan) > 1)
		{
			mouse_speed -= 4 * time_step;
		}
		else
		{
			mouse_speed += 2 * time_step;
		}               

		mouse_speed = clamp(mouse_speed, min_speed, max_speed); // keep mouse_speed in the 1...3 interval
		ent_animate(my, "base", run_frames, ANM_CYCLE); // play the "run" animation - make sure that your mouse includes it
		run_frames += 30 * time_step; // "3" controls the run animation speed
		
		
		
		
	}
	ent_remove(me);
}



action AFish()
{
	my.red   = 128;
	my.green = 128;
	my.blue  = 0;
	set(my,PASSABLE);
	var run_frames;
	var distance = 0;
	var previous_pan;
	var following_pan;
	var min_speed = 1;
	var max_speed = 3;
	var mouse_speed;
	VECTOR last_pos[3];
	VECTOR direction[3];
	var LDestroy=0;

	while((LDestroy==0)&&(fglobalstop==1))

	{
		
		if (FDedQuest == 5)
		{
			set(my,LIGHT);
			my.lightrange=50;
			if (vec_dist(my.x, player.x) < 100)
			{
				set(PItem,SHOW);
				wait(1);
				if (mouse_left == 1) {
					reset(PItem,SHOW);
					set(my,INVISIBLE);
					LDestroy=1;
					FDedQuest=6;
				}
			}
			else
			{if (vec_dist(my.x, player.x) < 200)
			reset(PItem,SHOW);}
		}
		else
		{
			my.lightrange=0;reset(my,LIGHT);
		}
		
		previous_pan = my.pan;
		path_spline(my, my.x, distance);
		distance += mouse_speed * time_step;		// let the mouse look ahead

		vec_diff(direction, my.x, last_pos);
		vec_to_angle(my.pan, direction);
		vec_set(last_pos, my.x);
		wait(1);
		following_pan = my.pan;
		// sudden direction change during the last frame? Then lower the speed of the mouse!

		if (abs(following_pan - previous_pan) > 1)
		{
			mouse_speed -= 4 * time_step;
		}
		else
		{
			mouse_speed += 2 * time_step;
		}               

		mouse_speed = clamp(mouse_speed, min_speed, max_speed); // keep mouse_speed in the 1...3 interval
		ent_animate(my, "ogon", run_frames, ANM_CYCLE); // play the "run" animation - make sure that your mouse includes it
		run_frames += 10 * time_step; // "3" controls the run animation speed
		
		
		
		
	}
	ent_remove(me);
}




//
//
//function my_Elf()
//{
	//  my.target = elf_1;     // target to follow
	//  elf_1 = me; // set fish target to me (for next fish to follow)
	//  ent_create("gelb.pcx", my.x, gelb_fligt);
	//
	////  MY.ORIENTED = ON;
	//  my.event = Elf_TargetEvent;
	//  set(my,PASSABLE);
	//  set(my,TRANSLUCENT);
	//  my.alpha=20;
	//
	//  set(my,BRIGHT);
	//  my.ambient = 100;
	//  wait(1);  // don't overwrite rw_fishTargetSyn for next fish!!!
	//
	//  // while player is in the room...
	//  while(1)
	//  {
		//
		//    my.FRAME += time_step * 2.1;
		//
		//    if(MY.FRAME >= 4)
		//    {
			//      MY.FRAME = 1;
		//    }
		//
		//    // set target (fish or external)
		//    elf_1 = MY.ENTITY1;
		//
		//    vec_set(MY._SPEED, elf_1.X);
		//    vec_sub(MY._SPEED, MY.X);
		//      MY._SPEED_Z = 0;   // MY.SPEED is now the distance to target in XY plane
		//
		//    // use distance to set speed
		//    if((vec_length(MY._SPEED)) < 50)
		//    {
			//      // slow
			//      vec_normalize(MY._SPEED, (0.75*TIME));
		//    }
		//    else
		//    {
			//      // fast
			//      vec_normalize(MY._SPEED, (3*TIME));
		//    }
		//
		//    move(ME,nullskill,MY._SPEED);   // move fish towards target
		//    vec_to_angle(MY.PAN,MY._SPEED); // face target
		//
		//
		//    effect(roomLS_elf_fire, 1, my.x, nullvector);  // produce elf-fire particle
		//
		//
		//    wait(-1);
	//  }
	//  ent_remove(me);
	//
//}
//
//action Elf_Target {
	//  reset(my,VISIBLE);
	//  my.event = Elf_TargetEvent;
	//  my.emask |= (ENABLE_BLOCK | ENABLE_ENTITY);
	//
	//
	//  //  if(qRoomLightShadow == 1)     // player entered room
	//    {
		//      // create fish followers (6)
		//      elf_1 = me; // set this as the target for first roomW_myFish
		//
		//      vec_set(temp,my.x);
		//      temp.z = -285;  // adjusted this value because target has moved up
		//      temp.x -= 20;
		//      ent_create("elf.mdl", temp, my_Elf);
		//      temp.z += 30; //30
		//      temp.x -= 20;
		//      ent_create("elf.mdl", temp, my_Elf);
		//      temp.z -= 20; //10
		//      temp.x -= 30;
		//      temp.y -= 20;
		//      ent_create("elf.mdl", temp, my_Elf);
		//      temp.z += 40; //50
		//      temp.x -= 30;
		//      temp.y += 50;
		//      ent_create("elf.mdl", temp, my_Elf);
		//      temp.z -= 10; //40
		//      temp.x -= 40;
		//      ent_create("elf.mdl", temp, my_Elf);
		//      temp.z -= 20; // 20
		//      temp.x += 100;
		//      ent_create("elf.mdl", temp, my_Elf);
		//
		//
		//      // while the player is in the room...
		//      while(1)
		//      {
			//        // move target
			//        temp.X = 20 * time_step;
			//        temp.Y = 0;
			//        temp.Z = 0;
			//
			//        c_move(me, temp, NULL,GLIDE);
			//
			//        wait(1);
		//      }
		//      // player has exited room
	//    }
	//    wait(-0.25);  // 1/4 second wait
//}
//
//

