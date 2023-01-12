/*******************************************************************************************************************************************************
SUNRAY EXAMPLE

This example shows how to add sunrays/godrays/volumetric lightbeams to your level
*********************************************************************************************************************************************************/

#define PRAGMA_PATH "shade-c"
#define PRAGMA_PATH "stuff"

#include <litec.h>
#include <d3d9.h> // <--- THIS IS IMPORTANT!
#include <acknex.h>
#include <default.c>
#include "sc_core.c"

ENTITY* skycube =
{
  type = "sky04+6.tga"; // the image for a backdrop scene
  flags2 = SKY | CUBE | VISIBLE; // sky cube, visible
  red = 140;
  green = 140;
  blue = 140;
}

void ent_camera()
{
	set(my,INVISIBLE);
	set(my,PASSABLE);
	while(1)
	{
		c_move(my,vector(key_force.y*10*time_step,-key_force.x*10*time_step,0),nullvector,GLIDE + IGNORE_PASSABLE);
		my.pan += -mickey.x;
		my.tilt += -mickey.y;
		
		vec_set(camera.x,my.x);
		vec_set(camera.pan,my.pan);
		wait(1);
	}
}

void main ()
{
	video_switch(9,32,0);
	camera.arc = 90;
	level_load("level.wmb");
	
	wait(3);
	
	//setup shade-c so we can use light rays. We don't need any advanced effects, so disable them
	sc_bHDR = 1;
	sc_bDOF = 0;
	sc_bRefract = 0;
	sc_bReflect = 0;
	sc_bWater = 0;
	sc_bVolParts = 0;
	sc_setup();
	
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube,sc_myVolPart, 0);
	
	//set light ray strength and length. These can be set during runtime to alter the effect in realtime
	sc_lightRayStr = 0.4;
	sc_lightRayLength = 3.5;
	
	//activate light rays
	sc_lightRays();
	
	//create camera
	ent_create("xyz.mdl",vector(0,0,20),ent_camera);
	
	//fill level with objects
	ent_create("soldiera.mdl",vector(-70,12,10),NULL);
	
	
	
	
	//make the sun panel dummy visible, just so you know where the sun is ;)
	set(sc_pan_sunDummy,SHOW);
	
	//tilt the sun to 20°, so we don't have to look up in the sky to find the sun, this is just to better show the light ray effect
	sun_angle.tilt = 20;
	
}