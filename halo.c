action HaloLight()
{
	var dist;
	set(my,TRANSLUCENT);
	my.alpha = 0;	
	while(1)
	{
		if (vec_dist(player.x,my.x)<1000)
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
			my.alpha = 100 - (vec_dist(player.x,my.x) - 1000)/3;
			wait(5);
		}
	}	
}