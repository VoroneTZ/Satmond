#include "animation.c"
#include "vars.c"
#include "VTZEffects.c"
SOUND* man = "axlequest1.ogg";
SOUND* man2 = "axlequest2.ogg";
var GetAxle = 0;
var LSayQuest = 0;
var vExp=0;
var FDedQuest=0;
var FSheepHair=0;
var FButterfly=0;
var pp;
SOUND* ded1 = "ded1.ogg";
SOUND* ded2 = "ded2.ogg";
SOUND* ded3 = "ded3.ogg";
SOUND* ded4 = "ded4.ogg";
SOUND* ded5 = "ded5.ogg";
SOUND* ded6 = "ded6.ogg";
SOUND* locked = "locked.ogg";
SOUND* sndanekdot = "anekdot.ogg";
SOUND* tt="tap.wav";
SOUND* gop1="bandit1.wav";
SOUND* gop2="bandit2.wav";

SOUND* babushka1 = "babushka1.ogg";
SOUND* babushka2 = "babushka2.ogg";
SOUND* babushka3 = "babushka3.ogg";
SOUND* dragon= "dragon.ogg";
var hit_timer;
ENTITY* EDragon;
var gmover=0;

action ABabushka()
{
	var linitpan=my.pan;
	var lnewpan;
	var talk_handle;
	VECTOR temp;
	while (!player) {
		wait(1);
	}
	if (gmover==0)
	{
		while(1)
		{
			my.skill1 += 3 * time_step; // torso animation
			if (my.skill1 > 100) {
				my.skill1 = 0;
			}
			ent_animate(me, "stand", my.skill1, ANM_CYCLE);

			
			
			if (vec_dist(my.x, player.x) < 200)
			{
				vec_set(temp, player.x);
				vec_sub(temp, my.x);
				vec_to_angle(my.pan, temp);
				if (FDedQuest < 7)
				{
					if (my.skill2==0)
					{
						my.skill2=2;
						talk_handle = ent_playsound(my, babushka1, 700);
					}
					else
					{					
						
						if (key_pressed(280))
						{
							if (snd_playing(talk_handle)==0)
							talk_handle = ent_playsound(my, babushka1, 700);
						}
						
					}
				}
				else
				{
					if (my.skill2==0)
					{
						my.skill2=2;
						talk_handle = ent_playsound(my, babushka2, 700);
					}
					else
					{
						
						if (key_pressed(280))
						{
							if (snd_playing(talk_handle)==0)
							talk_handle = ent_playsound(my, babushka2, 700);
						}
						
					}
				}
				if (EDragon.skill16<0)
				{
					if (my.skill2<5)
					{
						my.skill2=5;
						talk_handle = ent_playsound(my, babushka3, 700);
						while(my.skill1<90)
						{	
							my.skill1 += 3 * time_step; // torso animation
							if (my.skill1 > 100) {
								my.skill1 = 0;
							}
							ent_animate(me, "attack", my.skill1, ANM_CYCLE);
							wait(1);
						}
					}
				}
				
			}
			else
			{
				my.pan=linitpan;
				
			}
			my.tilt=0;
			my.roll=0;
			wait(1);
		}
	}
	ent_remove(me);
}

action dragontrigger()
{
	while(1)
	{
		if (my.skill16<0)
		{
			EDragon.skill16=-10;
		}
		wait(1);
	}
}

action miner()
{
	while(1)
	{
		my.skill1 += 3 * time_step; // torso animation
		if (my.skill1 > 100) {
			my.skill1 = 0;
		}
		ent_animate(me, "cut_tree", my.skill1, ANM_CYCLE);
		wait(1);
	}
}

action ADragon()
{
	var talk_handle;
	EDragon=me;
	set(my,PASSABLE);
	if (gmover==0)
	{
		while(1)
		{
			if (my.skill16>0)
			{
				ent_animate(me, "death", 1, ANM_SKIP);
			}
			else
			{
				if (my.skill2<1)
				{
					snd_play(dragon, 100, 0);
				}
				my.skill2=1;
				gmover=1;
				my.skill1+=3*time_step;
				if (my.skill1>100){my.skill1=0;}
				ent_animate(me, "stand", my.skill1, ANM_CYCLE);
			}
			wait(1);
		}
	}
	ent_remove(me);
}

action AVillager()
{
	while(!player){wait(1);}
	var anekdot;
	anekdot=ent_playsound(my, tt, 1);
	while (1)
	{
		
		if (snd_playing(anekdot)==0)
		{
			my.skill1 += 3 * time_step; // torso animation
			if (my.skill1 > 100) {
				my.skill1 = 0;
			}
			ent_animate(me, "sit", my.skill1, ANM_CYCLE);
			
			if (vec_dist(player.x,my.x)<100)
			{
				if (key_pressed(280))
				{
					anekdot=ent_playsound(my, sndanekdot, 700);
				}
			}
			
		}
		else
		{
			my.skill1 += 0.2 * time_step; // torso animation
			if (my.skill1 > 100) {
				my.skill1 = 0;
			}
			ent_animate(me, "talk-sit", my.skill1, ANM_CYCLE);
		}

		wait(1);
	}
}

function hit_player(var Apower)
{
	if (hit_timer<1)
	{
		snd_play(hitsnd,100,0);
		player.SHealth-=Apower;
		hit_timer=20;
		while (hit_timer>0)
		{
			hit_timer-=1*time_step;
			wait(1);
		}
		
	}
}

PANEL* PItem =
{
	pos_x = 4; 
	pos_y = 4;

	digits(120, 100, "Press LMB to take item", arial_font, 1, Fstop_rain);
	
}



action ADed ()
{
	
	var linitpan=my.pan;
	var lnewpan;
	var talk_handle;
	VECTOR temp;
	while (!player) {
		wait(1);
	}
	while(1)
	{
		my.skill1 += 3 * time_step; // torso animation
		if (my.skill1 > 100) {
			my.skill1 = 0;
		}
		ent_animate(me, "stand", my.skill1, ANM_CYCLE);

		
		
		if (vec_dist(my.x, player.x) < 200)
		{
			vec_set(temp, player.x);
			vec_sub(temp, my.x);
			vec_to_angle(my.pan, temp);
			if (FDedQuest == 0)
			{
				FDedQuest = 1;
				talk_handle = ent_playsound(my, ded1, 700);
			}
			if (FDedQuest == 1)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded1, 700);
				}
			}
			if (FDedQuest == 2)
			{
				vExp+=1;
				FDedQuest = 3;
				talk_handle = ent_playsound(my, ded2, 700);
			}
			if (FDedQuest == 3)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded2, 700);
				}
			}
			if (FDedQuest == 4)
			{
				FDedQuest = 5;
				vExp+=1;
				talk_handle = ent_playsound(my, ded3, 700);
				my.skill1 =0;
				while(my.skill1<90)
				{	
					my.skill1 += 3 * time_step; // torso animation
					if (my.skill1 > 100) {
						my.skill1 = 0;
					}
					ent_animate(me, "attack", my.skill1, ANM_CYCLE);
					wait(1);
				}
				
			}
			if (FDedQuest == 5)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded3, 700);
				}
			}
			if (FDedQuest == 6)
			{
				FDedQuest = 7;
				vExp+=1;
				talk_handle = ent_playsound(my, ded4, 700);
				FSheepHair=0;
			}
			if (FDedQuest == 7)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded4, 700);
				}
			}
			if (FDedQuest == 8)
			{
				FDedQuest = 9;
				vExp+=1;
				talk_handle = ent_playsound(my, ded5, 700);
			}
			if (FDedQuest == 9)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded5, 700);
				}
			}
			if (FDedQuest == 10)
			{
				FDedQuest = 11;
				vExp+=1;
				talk_handle = ent_playsound(my, ded6, 700);
			}
			if (FDedQuest == 11)
			{
				if (key_pressed(280))
				{
					if (snd_playing(talk_handle)==0)
					talk_handle = ent_playsound(my, ded6, 700);
				}
			}
		}
		else
		{
			my.pan=linitpan;
			
		}
		my.tilt=0;
		my.roll=0;
		wait(1);
	}
}


action AAxle()
{
	my.red   = 128;
	my.green = 128;
	my.blue  = 0;
	while (!player) {
		wait(1);
	}
	while (GetAxle == 0)
	{
		if (LSayQuest==1)
		{
			set(my,LIGHT);
			my.lightrange=50;
			if (vec_dist(my.x, player.x) < 100)
			{
				set(PItem,SHOW);
				if (mouse_left == 1) {
					GetAxle = 1;
					reset(PItem,SHOW);
				}
			}
			else
			if (vec_dist(my.x, player.x) < 200)
			reset(PItem,SHOW);
		}
		else
		{my.lightrange=0;reset(my,LIGHT);}
		wait(5);
	}
	reset(PItem,SHOW);
	ent_remove(me);
}







function npc_movement(var* pathDist) {
	VECTOR vDir;

	VECTOR newPosition;
	var npcDisplacement = walkSpeed * time_step;
	//Calculate new position

	{
		path_spline(me, newPosition, *pathDist);
		newPosition.z = my.z; // Adjust the height of the direction vector
		*pathDist += npcDisplacement;
	}

	//Get direction (remove tilt)
	vec_diff(vDir, newPosition, my.x);
	vec_to_angle(me.pan, vDir);
	my.tilt = 0;
	me.CurrentAction = ActionWalk;

	//Move NPC
	handle_movement(vector(npcDisplacement, 0, 0), NULL, 0);
}

function npc_talk() {
	var talking = false;
	var dist = 0;
	if (my.TalkingState == WaitingTalk || my.TalkingState == Talking) {
		dist = c_scan(me.x, me.pan, vector(180, 0, 100), IGNORE_ME | IGNORE_PASSABLE | SCAN_ENTS | SCAN_LIMIT);
		if (dist <= 0) {
			my.TalkingState = TalkingOutOfRange;
		}
		else talking = true;
	}
	if (my.TalkingState == WaitingTalk)
	my.CurrentAction = ActionIdle;
	if (my.TalkingState == Talking)
	my.CurrentAction = ActionTalk;
	return talking;
}

function npc_event() {
	switch (event_type) {
		case EVENT_TRIGGER:
		my.TalkingState = WaitingTalk;
		return;
		case EVENT_SCAN:
		beep();
		return;
	}
}

action npc_action() 
{
	wait(-1);
	while(!player){wait(1);}my.skill2 = 10;
	if (LSayQuest == 2){ my.skill2=0;}
	wait(1);
	my.hp = 1;
	my.eflags |= FAT;
	my.SizeOffset = 56;
	my.RadiusSize = 28;
	my.GravityAccel = 2;
	my.trigger_range = 200;
	VECTOR temp;
	var initpan = my.pan;
	var lnewpan;
	my.emask |= ENABLE_TRIGGER;
	my.event = npc_event;
	my.trigger_range = 200;
	var talk_handle;
	talk_handle = ent_playsound(my, tt, 700);
	//	vec_set(my.min_x, vector(-my.RadiusSize, -my.RadiusSize, -my.SizeOffset));
	//	vec_set(my.max_x, vector(my.RadiusSize, my.RadiusSize, my.SizeOffset));
	var pathDist = 0;   //Stores walked distance over the path
	var Lanim_percent;
	set(my, SHADOW | CAST);
	
	while (my.skill2>0) {
		
		pp=pathDist/100;
		if (pp>41)
		{
			my.skill2=0;
		}
		
		if (!npc_talk())
		my.CurrentAction = ActionIdle;
		if (LSayQuest != 2)
		{
			
			if (vec_dist(my.x, player.x) < 200)
			{
				vec_set(temp, player.x);
				vec_sub(temp, my.x);
				vec_to_angle(my.pan, temp);
				if (LSayQuest == 0)
				{
					LSayQuest = 1;
					talk_handle = ent_playsound(my, man, 700);
				}
				else
				{
					if (LSayQuest == 1)
					{
						if (key_pressed(280))
						{
							if (snd_playing(talk_handle)==0)
							talk_handle = ent_playsound(my, man, 700);
						}
					}
				}
				if (GetAxle == 1)
				{
					
					if (LSayQuest == 1)
					{
						LSayQuest = 2;
						vExp+=1;
						ent_playsound(my, man2, 700);
					}
				}
			}
			else
			{
				my.pan=initpan;
			}
			my.tilt = 0;
			my.roll = 0;
		} else
		npc_movement(pathDist);
		
		if (snd_playing(talk_handle)>0)
		{
			Lanim_percent += 1.5*time_step; 
			ent_animate(me,"talk_A",Lanim_percent,ANM_CYCLE);
		}
		else
		{ 
			handle_animation();
		}
		
		
		
		
		wait(1);
	}
	my.skill1 = 0;
	
	//snd_handle1=snd_play(snd_death_enemy,50,0);
	
	ent_remove(me);
}

function npcenemy_movement(var* pathDist){
	VECTOR vDir;
	VECTOR newPosition;
	var npcDisplacement = walkSpeed * time_step;
	//Calculate new position
	if (is(my,FLAG1))
	{

		if (vec_dist(my.x,player.x)>my.skill1)
		{
			if (is(my,FLAG3))
			{
				if (vec_dist(my.x,vector(my.skill18,my.skill19,my.skill10))>300)
				{
					newPosition.x=my.skill18;
					newPosition.y=my.skill19;
					newPosition.z=my.skill20;
					me.CurrentAction = ActionWalk;
					
					//Move NPC
					handle_movement(vector(npcDisplacement,0,0), NULL, 0);
					//Get direction (remove tilt)
					vec_diff(vDir,newPosition,my.x);
					vec_to_angle(me.pan,vDir);
					my.tilt = 0;
				}
				else
				{
					me.CurrentAction = ActionIdle;
					my.pan=my.skill21;
				}
			}
			else
			{
				path_spline(me,newPosition,*pathDist);
				newPosition.z = my.z;	// Adjust the height of the direction vector
				*pathDist += npcDisplacement;	
				
				me.CurrentAction = ActionWalk;
				
				//Move NPC
				handle_movement(vector(npcDisplacement,0,0), NULL, 0);
				//Get direction (remove tilt)
				vec_diff(vDir,newPosition,my.x);
				vec_to_angle(me.pan,vDir);
				my.tilt = 0;			
			}
		}
		else
		{
			newPosition.x=player.x;
			newPosition.y=player.y;
			newPosition.z=player.z;
			if (is(my,FLAG5))
			{
				if (my.skill19!=100)
				{
					my.skill19=100;
					ent_playsound(me,gop1,800);
				}
			}
			me.CurrentAction = ActionWalk;
			
			//Move NPC
			handle_movement(vector(npcDisplacement,0,0), NULL, 0);
			//Get direction (remove tilt)
			vec_diff(vDir,newPosition,my.x);
			vec_to_angle(me.pan,vDir);
			my.tilt = 0;
		}
	}
	else
	if ((is(my,FLAG1))==false)
	{
		
		path_spline(me,newPosition,*pathDist);
		newPosition.z = my.z;	// Adjust the height of the direction vector
		*pathDist += npcDisplacement;
		
		me.CurrentAction = ActionWalk;
		
		//Move NPC
		handle_movement(vector(npcDisplacement,0,0), NULL, 0);
		//Get direction (remove tilt)
		vec_diff(vDir,newPosition,my.x);
		vec_to_angle(me.pan,vDir);
		my.tilt = 0;
	}
	
	
}

action npc_enemy() 
{
	//	Adjust collision parameters
	my.skill18=my.x;
	my.skill19=my.y;
	my.skill20=my.z;
	my.skill21=my.pan;
	wait(1);	
	my.hp=1;
	my.eflags |= FAT;
	my.SizeOffset = 56;
	my.RadiusSize = 28;
	my.GravityAccel = 2;
	my.trigger_range = 200;
	
	my.emask |= ENABLE_TRIGGER;
	//	my.event = npc_event;
	my.trigger_range = 200;
	
	//	vec_set(my.min_x,vector(-my.RadiusSize,-my.RadiusSize,-my.SizeOffset));
	//	vec_set(my.max_x,vector(my.RadiusSize,my.RadiusSize,my.SizeOffset));
	var pathDist = 0;		//Stores walked distance over the path
	my.skill20=0;
	set(my,SHADOW|CAST);
	while(my.SHealth>0) 
	{
		
		if ((is(my,FLAG4)==false)||(VtimeH<5))
		{
			if (is(my,FLAG4)==false)
			{
				if (my.skill20>0)
				{
					effect(flm_part_func3, 1, my.x, nullvector);
					my.skill20-=1*time_step;
				}
			}
			reset(my,PASSABLE);
			reset(my,INVISIBLE);
			npcenemy_movement(pathDist);
			handle_animation();
			
			if (vec_dist(my.x,player.x)< 90)
			{
				my.skill10 =0;
				ent_playsound(me,misssnd,300);
				while(my.skill10<50)
				{	
					my.skill10 += 6 * time_step; // torso animation
					ent_animate(me, "attack", my.skill10, ANM_CYCLE);
					wait(1);
				}
				if (vec_dist(my.x,player.x)< 110)	
				hit_player(6+random(6));
				while(my.skill10<90)
				{	
					my.skill10 += 6 * time_step; // torso animation
					ent_animate(me, "attack", my.skill10, ANM_CYCLE);
					wait(1);
				}
			}
			
		}
		else
		{ 
			if (my.skill20<20)
			{
				effect(flm_part_func3, 1, my.x, nullvector);
				my.skill20+=1*time_step;
			}
			set(my,PASSABLE);
			set(my,INVISIBLE);
		}
		wait(1);
	}
	
	if (is(my,FLAG5))
	{
		
		my.skill20=1;
		ent_playsound(me,gop2,600);
		
	}
	my.emask = NULL;
	my.skill1=0;	set(my,PASSABLE);
	if (is(my,FLAG4))
	{
		if (FDedQuest==9)
		{FDedQuest = 10;}
	}
	while(my.skill1<80) 
	{
		my.skill1 += 3*time_step;
		ent_animate(me,"death",my.skill1,ANM_CYCLE);
		wait(1);
	}

	my.skill1=0;
	set(my,TRANSLUCENT);
	while(my.skill1<100) 
	{
		my.skill1+=0.5*time_step;	
		my.alpha=100-my.skill1;
		wait(1);	
	}

	ent_remove(me); 
}