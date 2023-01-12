#define STATE     skill1
#define ANIMATION skill2
#define CREATOR   skill3

var winner = 0;
ENTITY* player01;

void		PlayarTest01_Init();
void		PlayarTest01_Close();
action 	spell_fly();
function wizard_hit();
action 	wizard_walk();
action 	wizard_stand();

var		Map_TileSize = 32;

void		PlayerTest01_Init()
{
	if (player01)
		{
			shadow_mode |= IGNORE_FLAG2;
			ptr_remove(player01);
			player01 = NULL;
			return;
		}
	
	shadow_mode &= ~IGNORE_FLAG2 ;
	wait(3);
	
//	player01 = ent_create( "pguard3_w.mdl" , ActiveTileSquare.x /*vector(-1500,-1250,750)*/ , wizard_walk );
	player01 = ent_create( "marine.mdl" , ActiveTileSquare.x /*vector(-1500,-1250,750)*/ , wizard_walk );
	
}


void		PlayarTest01_Close()
{
	ptr_remove(player01);
	shadow_mode |= IGNORE_FLAG2 ;
}



action spell_fly()
{
	my.ambient = 50;  // medium bright
//	my.lightrange = 300; // activate dynamic light
//	vec_set(my.blue,vector(255,50,50)); // bluish light color
	
	my.lightrange = 50;
	vec_set(my.blue,vector(100,200,255)); // bluish light color
	vec_scale(my.scale_x,0.25);
	
	set(me,BRIGHT);   // additive blending

//	vec_scale(my.scale_x,0.15); // small size
	c_setminmax(me);   // set my bounding box to my real size
	my.pan = your.pan; // face same direction as player

// set FLAG2 to make the spell detectable by c_scan,
// and store the spell creator (you) in a skill
// so that the detecting entity knows its enemy	
	set(my,FLAG2);     
	my.CREATOR = you;  
	
//	var myspeed = 40;
	var myspeed = 200;
	
	my.STATE = 1;
	while(me)
	{
// state 1: flying ///////////////////////////////////////////  
		if (my.STATE == 1) 
		{
			my.roll += 20*time_step; 
			c_move(me,vector(40*time_step,0,0),NULL,IGNORE_YOU | IGNORE_PASSABLE);
			if (HIT_TARGET)  // collided? 
				my.STATE = 2;  // go to next state
		}

// state 2: exploding ////////////////////////////////////////  
		if (my.STATE == 2) 
		{
			set(me,ZNEAR);  // render in front of close objects
			my.pan = random(360);
			my.tilt = random(180)-90;
			my.roll = random(360);
			my.lightrange *= 1+0.5*time_step; // increase light range
			vec_scale(my.scale_x,1+0.5*time_step); // inflate size
			if (my.scale_x > 10) { // explosion finished? 
				ent_remove(me);
				return;
			}
		} 

		wait(1);	
	}
}


// hit event
function wizard_hit()
{
	my.ANIMATION = 0;
	my.STATE = 4; // 4 = dead
}


action wizard_walk()
{ 
//	camera_follow(me);
	
	my.event = wizard_hit;
	my.emask |= ENABLE_IMPACT;   
 	
	// set z always by c_tracing
	VECTOR vFeet;
	vec_for_min(vFeet,me);
	vec_mul(vFeet,my.scale_x);
	my.z = PosInfo3D2(my.x, my.y) - vFeet.z ;	// no minimum detection needed , it should be placed with its origin at the surface
	  
 	wait(1);
	my.eflags |= FAT | NARROW; // set both flags to prevent automatic recalculation on scale changes
 	vec_set(my.min_x,vector(-10,-10,-10)); // set bounding box to individual values
 	vec_set(my.max_x,vector(10,10,10));
	wait(1);	
	
// set FLAG2 to make the wizard detectable by c_scan, 
// and store the wizard pointer in its own CREATOR skill 
// so that the detecting entity knows its enemy	
	set(my, FLAG2 | SHADOW);   
	
	if (shadow_stencil<8)
		my.shadow = ent_unitshadow_rot;	// decal shadow - does not work with stencil_blur(1); and works also with shadow_stencil = 8; without pssm run
	
	//-------------------------------------------
//	fx_bump();											
//	my.material = mtl_bump;				// mtl_bump - darker ,  mtl_specBump - crash
//	set(my,CAST);							// bad if normal mapping used, so self shadows should be surpressed
	//-------------------------------------------
	
   my.CREATOR = me;
   
	my.STATE = 1;
	while (me)
	{
// state 1: walking ////////////////////////////////////////////
		if (my.STATE == 1)
		{
// rotate the entity with the arrow keys     
			my.pan += (key_a-key_d)*5*time_step;   

// move the entity forward/backward with the arrow keys
			var distance = (key_w-key_s)*5*time_step ;
			if (key_shift) distance *=2;
			c_move(me, vector(distance,0,0), NULL, GLIDE | IGNORE_PASSABLE );

// animate the entity    
			my.ANIMATION += 2*distance;
//			ent_animate(me,"walk",my.ANIMATION,ANM_CYCLE);
			
			ent_animate(me,NULL,0,0); // reset all bones
			ent_animate(me,"walk",my.ANIMATION,ANM_CYCLE);
			ent_animate(me,"wpose",my.ANIMATION,ANM_ADD);

// adjust entity to the ground height, using a downwards trace
			c_trace(my.x,vector(my.x,my.y,my.z-1000),IGNORE_ME | IGNORE_PASSABLE);
			my.z = hit.z - vFeet.z; // always place player's feet on the ground

			if (key_e) 
			{ // key pressed -> go to state 2
				my.ANIMATION = 0;
				my.STATE = 2;
			}
		}
		
// state 2: casting a spell /////////////////////////////////// 
		if (my.STATE == 2) 
		{
//			my.ANIMATION += 8*time_step;
//			ent_animate(me,"attack",my.ANIMATION,0);
//			
//			if (my.ANIMATION > 100) 
			{ // create the spell and go to state 3
//				ent_create("spell.mdl",vector(my.x,my.y,my.z+20),spell_fly);
				
				VECTOR vGunEnd;
				vec_for_vertex(vGunEnd,my,5035);
				ent_create("spell.mdl",vGunEnd,spell_fly);
				
				my.ANIMATION = 0;
				my.STATE = 3;
			}
		}  
		
// state 3: waiting for key release ///////////////////////////
		if (my.STATE == 3) 
		{
			if (!key_space) 
			{
				my.ANIMATION += 8*time_step;
				ent_animate(me,"reload",my.ANIMATION,0);
				
				if (my.ANIMATION > 100) 
					my.STATE = 1; // when key was released, back to state 1
			}
//				my.STATE = 1; // when key was released, back to state 1
		}
	
// state 4: death animation //////////////////////////////////// 
		if (my.STATE == 4) 
		{
			winner = 2; // other wizard won
			my.ANIMATION += 3*time_step; 
			ent_animate(me,"death",my.ANIMATION,0);
			if (my.ANIMATION > 70)
				return;
		}		
		
		wait(1);
	}
}


action wizard_stand()
{
	my.event = wizard_hit;
	my.emask |= ENABLE_IMPACT;   

  ENTITY* enemy = NULL; // spell target
	
	set(my,FLAG2 | SHADOW | CAST);  
	my.shadow = ent_unitshadow_rot; 
	
	my.STATE = 1;
	while (me)
	{  
// state 1: wait until wizard or spell comes close enough ////// 
		if(my.STATE == 1) 
		{
// detect all FLAG2 entities within 750 quants 			
			c_scan(my.x,my.pan,vector(360,0,750),SCAN_ENTS | SCAN_FLAG2 | IGNORE_ME);
			if (you) {  // red wizard or flying spell detected?
				my.STATE = 2; 
				enemy = your.CREATOR;
			}
		}
	
// state 2: turn towards enemy, cast spells /////////////////// 
		if (my.STATE == 2) 
		{
// get the angle to the enemy			
			VECTOR vDirection;
			ANGLE vTargetAngle;
			vec_diff(vDirection,enemy.x,my.x);
			vec_to_angle(vTargetAngle,vDirection);
// vAngle is now the angle to the enemy.		
// Turn right or left depending on the difference
// between the current and the target pan angle			
			my.pan += time_step * sign(ang(vTargetAngle.pan - my.pan));
				
			my.ANIMATION += 3*time_step;
			ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);

			if (my.ANIMATION > 100) { // create the spell
				ent_create("spell.dds",vector(my.x,my.y,my.z+10),spell_fly);
				my.ANIMATION -= 100;    // continue the cycle from the beginning
			}

			if (enemy.STATE == 4) // enemy dead?
				my.STATE = 3;
		}  
		
// state 3: killed the enemy => dance of joy //////////////////// 
		if(my.STATE == 3) 
		{
			my.ANIMATION += 8*time_step; 
			ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);
		}
		
// state 4: death animation //////////////////////////////////// 
		if(my.STATE == 4) 
		{
			winner = 1; // other wizard (red) won
			my.ANIMATION += 3*time_step; 
			ent_animate(me,"death",my.ANIMATION,0); // animate the entity
			if (my.ANIMATION > 70) 
				return;
		}

		wait(1);
	}
}