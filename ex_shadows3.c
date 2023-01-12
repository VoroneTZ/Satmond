/*******************************************************************************************************************************************************
SHADOWMAPPING EXAMPLE (sType = 0)

void sc_smSunSetup(var SMMapRes, var SMRes, var SMdepth, float SMblur, var sType)
// SMMapRes = Shadowmap Resolution (must be smaller or equal to screen_size.x)
// SMRes = ShadowView Resolution | Area of Shadow (how many quants should be shadowed at once? keep this as low as possible for nice shadows)
// SMdepth = max depthmap depth (distance after which shadows are clipped. Keep this as low as possible for nice shadows)
// SMblur = shadow blur amount
// sType = shadow Type | 0 = Shader integrated Shadows (fast & nice), 1 = Shadows are rendered for the whole scene and multiplied with it (slow & ugly)

This example is similar to ex_shadows2.c, except that we are using standardt A7 materials and shaders now, instead of just applying one shader to all objects
*********************************************************************************************************************************************************/

#define PRAGMA_PATH "shade-c"
#define PRAGMA_PATH "stuff"

#include <litec.h>
#include <d3d9.h> // <--- THIS IS IMPORTANT!
#include <acknex.h>
#include <default.c>
#include "sc_core.c"

BMAP* map_envMap = "sky04S+6.tga"; //an environment map for later use in a shader

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

//A standardt A7 Material with dynamic shadowmapping
MATERIAL* mtl_object =
{
	diffuse_red = 200;
	diffuse_green = 200;
	diffuse_blue = 200;
	specular_blue = 255;
	specular_green = 255;
	specular_red = 255;
	power = 10;
	
	effect = "sc_obj_doShadow.fx"; //set sc_obj_doShadow.fx as the effect for all your standardt materials, if you want an object to receive shadows
}

//A standardt A7 Material with dynamic shadowmapping. Has a more greenish color
MATERIAL* mtl_greenObject =
{
	ambient_blue = 100;
	ambient_green = 255; 
	ambient_red = 100;
	diffuse_blue = 0;
	diffuse_green = 100;
	diffuse_red = 0;
	specular_blue = 255;
	specular_green = 255;
	specular_red = 255;
	power = 10;
	
	effect = "sc_obj_doShadow.fx"; //set sc_obj_doShadow.fx as the effect for all your standardt materials, if you want an object to receive shadows
}

//A custom shader, that was built out of sc_obj_aio.fx. See Shade-C Manual for more information (Customizing Shade-C -> "Click together" Shaders)
MATERIAL* mtl_advObject =
{
	// set standardt material settings. These will be used by the shader
	diffuse_red = 200;
	diffuse_green = 200;
	diffuse_blue = 200;
	specular_blue = 255;
	specular_green = 255;
	specular_red = 255;
	power = 10;
	
	skin1 = map_envMap; //Set the environment map, as we are using Environment Mapping
   effect = "myShader.fx"; //This is a custom shader. You can easily create your own out of sc_obj_aio.fx in the shade-c/fx folder
   event = sc_mtlEvent_aio; //As we created a custom shader out of sc_obj_aio.fx, it is a good idea to set sc_mtlEvent_aio as our event, to set up the shader
	
}

//A custom shader, that was built out of sc_obj_aio.fx. Has a more greenish color. See Shade-C Manual for more information (Customizing Shade-C -> "Click together" Shaders)
MATERIAL* mtl_advGreenObject =
{
	ambient_blue = 100;
	ambient_green = 255; 
	ambient_red = 100;
	diffuse_blue = 0;
	diffuse_green = 100;
	diffuse_red = 0;
	specular_blue = 255;
	specular_green = 255;
	specular_red = 255;
	power = 10;
	
	skin1 = map_envMap; //Set the environment map, as we are using Environment Mapping
   effect = "myShader.fx"; //This is a custom shader. You can easily create your own out of sc_obj_aio.fx in the shade-c/fx folder
   event = sc_mtlEvent_aio; //As we created a custom shader out of sc_obj_aio.fx, it is a good idea to set sc_mtlEvent_aio as our event, to set up the shader
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
	// a depthmapclip of 1000 is used
	// shadows are blurred by 0.002
	// shadows are of type 0
	sc_smSunSetup(screen_size.x, 2000, 1000, 0.002, 0);
	
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube,sc_myVolPart, 0);
	
	//create camera
	ent_create("xyz.mdl",vector(0,0,20),ent_camera);
	
	//fill level with objects
	you = ent_create("soldiera.mdl",vector(-70,10,10),NULL);
	you.material = mtl_object;
	you = ent_create("soldiera.mdl",vector(-70,-30,10),NULL);
	you.material = mtl_greenObject;
	you = ent_create("soldiera.mdl",vector(-70,-70,10),NULL);
	you.material = mtl_advObject;
	you = ent_create("soldiera.mdl",vector(-70,-110,10),NULL);
	you.material = mtl_advGreenObject;
	
	//apply a shader to mat_model to enable shadows for all models, that don't have any other material/shader applied to
	mat_model.effect = "sc_obj_doShadow.fx";
	
	//apply a shader to mat_flat/mat_shaded to enable shadows for all level blocks, that don't have any other material/shader applied to
	mat_flat.effect = "sc_level_doShadow.fx";
	mat_shaded.effect = "sc_level_doShadow.fx";
	
	//the custom shaders are using a cubemap for environment mapping, so create the environment map
	bmap_to_cubemap(map_envMap);
}