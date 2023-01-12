/***************************************************
  snow.wdl, created with EasyParticle 3.1.
  This script can be used free and without credits.
  How to use:
    Implement the script via include <snow.wdl>; in your main WDL file.
    The action name is: effect_snow_action
   If you recognize any mistakes in the script, please contact the support ( Support@EasyParticle.de ).
   Have fun with the script
   TripleX --- Hawkgames
*****************************************************/
BMAP* effect_snowBmap = "glow1.tga";
var effect_snowAnzahlPartikel = 1; //number of particles used for this effect
#define skill_d skill1
#define skill_y skill2
#define skill_z skill3
#define skill_b skill7
#define skill_c skill8
#define lifespan skill9
#define vel_x skill4
#define vel_y skill5
#define vel_z skill6

function effect_snow_spec_fun()
{
    c_trace(vector(my.skill_d,my.skill_y,my.skill_z),my.x,IGNORE_ME|IGNORE_PASSABLE);
    if(trace_hit == 1)
    {
      my.vel_x = 0;
      my.vel_y = 0;
      my.vel_z = 0;
      my.lifespan = 0;
    }
    my.skill_d = my.x; my.skill_y = my.y; my.skill_z = my.z;
    my.alpha -= 0 *time_step;
    if(my.alpha < 0) { my.alpha = 0; my.lifespan = 0; }
    my.skill_b += 0.03;
    my.skill_c = 7.971  - my.skill_b;
    if(my.skill_c < 0) { my.lifespan = 0; }
}

function effect_snowspezial()
{
    my.blue = 128 ;
    my.green = 128 ;
    my.red = 128 ;
    my.skill_c = 0; my.skill_b = 0;
    my.bmap = effect_snowBmap;  //the effect bitmap
    my.vel_x = random( 60 ) - 30 ;
    my.vel_y = random( 60 ) - 30 ;
    my.vel_z = -40 ;
    my.size = 14 ;
    my.alpha = 100 ;
    my.x += random(1000)-500;
    my.y += random(1000)-500;
    my.z += random(100)-50;
    my.gravity = 0.010 ;
    my.streak = off;
    my.flare = on;
    my.bright = on;
    my.beam = off;
    my.move = on;
    my.transparent = on;
    my.function = effect_snow_spec_fun;
}


function effect_snow()
{
  while(1)
  {
     effect_snowAnzahlPartikel = max(1,random(1));
    var myPos[3];
    vec_set(myPOS,camera.x);
    temp.x = COS(camera.pan);
    temp.y = SIN(camera.pan);
    temp.Z = 600*COS(camera.tilt);
    MyPos.X = myPos.X + temp.Z*temp.X;
    MyPos.Y = myPos.Y + temp.Z*temp.Y;;
    MyPos.Z = myPos.Z + 600*SIN(camera.TILT);
    MyPos.Z = Camera.Z + 800;
    effect(effect_snowspezial,max(1,int(effect_snowAnzahlPartikel*time)),MyPOS,nullvector);
     wait(1);
  }
}


action effect_snow_action
{
	set(my,PASSABLE);
  effect_snow();
}


