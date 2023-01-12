/*******************************************************************************************************************************************************
SIMPLE SHADOWMAPPING EXAMPLE (sType = 1)

void sc_smSunSetup(var SMMapRes, var SMRes, var SMdepth, float SMblur, var sType)
// SMMapRes = Shadowmap Resolution (must be smaller or equal to screen_size.x)
// SMRes = ShadowView Resolution | Area of Shadow (how many quants should be shadowed at once? keep this as low as possible for nice shadows)
// SMdepth = max depthmap depth (distance after which shadows are clipped. Keep this as low as possible for nice shadows)
// SMblur = shadow blur amount
// sType = shadow Type | 0 = Shader integrated Shadows (fast & nice), 1 = Shadows are rendered for the whole scene and multiplied with it (slow & ugly)

This example uses sType = 1, which will create ugly and slow shadows.
This is the easiest way to integrate dynamic softshadows in your project, but it's also ugly and the lighting won't be correct.
sType 1 shadows are also also slower than sType 0 shadows as they are renedered in anadditional pass which is then multiplied with the original scene.
ONLY use these shadows for rapid prototyping. Never use them in a final product, as they are (once again) slow and ugly. With sType = 1, all polygons
are rendered twice, keep that in mind!
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
		c_move(my,vector(key_force.y*20*time_step,-key_force.x*20*time_step,0),nullvector,GLIDE + IGNORE_PASSABLE);
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
	
	//we want dynamic softshadows, so activate them.
	// a shadowmap resolution of screen_size.x is used
	// a shadowviewsize of 2000 is used
	// a depthmapclip of 1500 is used
	// shadows are blurred by 0.002
	// shadows are of type 1
	sc_smSunSetup(screen_size.x, 2000, 1000, 0.002, 1);
	
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube,sc_myVolPart, 0);
	
	//create camera
	ent_create("xyz.mdl",vector(0,0,20),ent_camera);
	
	//fill level with objects
	ent_create("soldiera.mdl",vector(-70,12,10),NULL);
	
}