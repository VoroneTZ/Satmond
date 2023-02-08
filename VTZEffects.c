#include "vars.c"

BMAP* effect_RainBmap = "rain.tga";
var Fstop_rain=0;
SOUND* fire = "fireplace.ogg";
BMAP*  bmp_flm_part = "fpart.pcx";
BMAP* bmp_smoke_part = "burns.pcx";
function rain_fader(PARTICLE *p)
{
	if (p.z < p.skill1) {
		p.lifespan = 0;
	}
	if (p.z<0){p.lifespan = 0;}
}
function rain_sprites2(PARTICLE *p)
{
	set(p, PASSABLE);
	set(p, TRANSLUCENT);
	p.bmap = effect_RainBmap;
	p.alpha = 20 + random(75);
	set(p, UNLIT);

	p.skill1 = p.z - 250;


	p.x = p.x + 25 - random(50);
	p.y = p.y + 25 - random(50);

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = -(25  + random(30));

	p.alpha = 100;
	set(p, BRIGHT);
	set(p, MOVE);
	p.event = rain_fader;
}
SOUND* whamm_snd = "rain1.wav";

function toggle_rain2()
{
	var rainHandle;
	var lvolume;
	VECTOR rain_origin;
	var i;
	while (!player) {
		wait (1);
	}
	lvolume=1;
	rainHandle = snd_loop(whamm_snd, 1, 0);
	
	while (Fstop_rain>0)
	{
		if (snd_playing(rainHandle)==0)
		{
			rainHandle = snd_loop(whamm_snd, lvolume, 0);
		}
		if (lvolume<50)
		{
			lvolume=lvolume+0.1;			
		}
	if (camera.z<0){snd_tune(rainHandle,1,0,0);}else{snd_tune(rainHandle,lvolume,0,0);}
		Fstop_rain-=0.1;
		rain_origin.x = player.x + 250 - random(500);
		rain_origin.y = player.y + 250 - random(500);
		rain_origin.z = player.z + 300;
		rain_origin.z = 300 + rain_origin.z - (c_trace(rain_origin.x, vector(rain_origin.x, rain_origin.y, rain_origin.z - 1000), IGNORE_FLAG2));
		effect(rain_sprites2, 5, rain_origin, nullvector);
		if (Vcave==1)
		{
			lvolume=0;
			Fstop_rain=0;
		}
		wait (1);
	}
	while (lvolume>0)
	{
		if (snd_playing(rainHandle)==0){rainHandle = snd_loop(whamm_snd, lvolume, 0);}
		if (camera.z<0){snd_tune(rainHandle,1,0,0);} else 
		{
			snd_tune(rainHandle,lvolume,0,0);
		}
		lvolume-=0.1;
		wait(1);
		rain_origin.x = player.x + 250 - random(500);
		rain_origin.y = player.y + 250 - random(500);
		rain_origin.z = player.z + 300;
		rain_origin.z = 300 + rain_origin.z - (c_trace(rain_origin.x, vector(rain_origin.x, rain_origin.y, rain_origin.z - 1000), IGNORE_FLAG2));
		effect(rain_sprites2, 1, rain_origin, nullvector);
	}
	snd_stop(rainHandle);
}


action HaloLight()
{
	var dist;
	my.lightrange=100;
	set(my,OVERLAY);
	set(my,TRANSLUCENT);
	my.alpha = 0;	
	while(fglobalstop==1)
	{
		if (vec_dist(player.x,my.x)<3000)
		{	
			my.scale_x=(vec_dist(player.x,my.x)/1000);
			my.scale_y=(vec_dist(player.x,my.x)/1000);
			my.scale_z=(vec_dist(player.x,my.x)/1000);
			my.roll=(10000/(vec_dist(player.x,my.x)))	;
			my.alpha=50;
			//	c_trace (camera.x,my.x,USE_POLYGON|IGNORE_PASSABLE);
			//	if (you == me)
			//	{
				//		if (my.alpha<100)
				//	{
					//		my.alpha+=1;
				//	}
			//	}
			//	else
			//	{
				//	if (my.alpha>50)
				//	{
					//		my.alpha-=1;
				//	}
			//	}								
			wait(2);
		}
		else
		{
			my.alpha = 0;//100 - (vec_dist(player.x,my.x) - 1000)/3;
			wait(5);
		}
	}	
}




function flm_part_fader(PARTICLE *p)
{
	p.alpha -= (2.5 * time_step);
	if (p.alpha <= 0) {
		p.lifespan = 0; // remove
	}
}
function smoke_part_fader(PARTICLE *p)
{
	p.alpha -= (0.75 * time_step);
	if (p.alpha <= 0) {
		p.lifespan = 0; // remove
	}
}

function flm_part_func3(PARTICLE *p)
{
	p.bmap = bmp_flm_part;
	p.size = 50;

	p.x += random(6) - 3;
	p.y += random(6) - 3;
	p.z += 2.0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = random(0.85);

	p.alpha = 100;
	set(p, BRIGHT);
	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}

function flm_part_func2(PARTICLE *p)
{
	p.bmap = bmp_flm_part;
	p.size = 5;

	p.x += random(6) - 3;
	p.y += random(6) - 3;
	p.z += 2.0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = random(0.85);

	p.alpha = 100;
	set(p, BRIGHT);
	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}
function flm_part_func(PARTICLE *p)
{
	p.bmap = bmp_flm_part;
	p.size = 16;

	p.x += random(6) - 3;
	p.y += random(6) - 3;
	p.z += 2.0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = random(0.85);

	p.alpha = 100;
	set(p, BRIGHT);
	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}

function smoke_part_func(PARTICLE *p)
{
	p.bmap = bmp_smoke_part;
	p.size = 8;

	p.x += random(3) - 1.5;
	p.y += random(3) - 1.5;
	p.z += 25.0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = random(0.45) + 0.40;

	p.alpha = 25;
	set(p, BRIGHT);
	set(p,OVERLAY);
	set(p, TRANSLUCENT);
	set(p, MOVE);
	p.event = smoke_part_fader;
}


action init_flame()
{
	var whosh_handle;

	reset(my, VISIBLE);
	my.red   = 128;
	my.green = 50;
	my.blue  = 0;
	var whosh_handle;
	whosh_handle = ent_playloop(my, fire, 100);

	while (fglobalstop==1)
	{
		if (vec_dist(camera.x,my.x)<500)
		{
			if (snd_playing(whosh_handle)==0)
			{
				whosh_handle = ent_playloop(my, fire, 100);
			}
		}
		else
		{
			if (!snd_playing(whosh_handle))
			{
				snd_stop(whosh_handle);
			}
		}
		my.lightrange = maxv(40, minv(150, (my.lightrange + random(10) - 5)));
		effect(flm_part_func, 1, my.x, nullvector);
		effect(smoke_part_func, 1, my.x, nullvector);
		wait(1);
	}
	snd_stop(whosh_handle);
}

action ACrystal()
{
	set(my,TRANSLUCENT);
	my.alpha=90;
	set(my,LIGHT);
	my.red   = 128;
	my.green = 50;
	my.blue  = 255;
	var lr=50;
	var dir=0;

	while (fglobalstop==1)
	{
	if (dir==0){lr+=1*time_step;}else{lr-=1*time_step;}
		if (lr>100){dir=1;}
		if (lr<50){dir=0;}
		my.lightrange = lr;
		wait(1);
	}
}