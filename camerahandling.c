///////////////////////////////////////////////////////////////////////
// camera and mouse initialization script:
// - defines camera movement by watching an object
// - shows selected tile by a square-shaped sprite -> cube model
///////////////////////////////////////////////////////////////////////

var Map_TileSize = 32;

// camera_control - should be adjustable from main game menü options
var ZoomSpeed = 15;
var RotateSpeed = 7.5;
var MoveSpeed = 100;
var ZoomDownLimit = 64;
var ZoomUpLimit = 6400;
var CamObjMinHeight = 64;
var CamObjMaxHeight = 4092;
var XMax =  0;							// set later based on actual map size
var XMin =  0;							// set later based on actual map size
var YMax =  0;							// set later based on actual map size
var YMin =  0;							// set later based on actual map size

// camera data
#define CAMZ 500.0
#define CAMH 100.0
STRING* CameraModel = "placement_cube_blue.mdl";  // any simple modell
ENTITY* CameraObject;							// that is followed by the camera
var CamZoom = 500;								// camera height 
var CamAngle = 270-45; 							// rotation of camera around camera object: 270 = watch from South i.e. watch to North
var CamHeight = 100;								// height of camera object - should be constant but over surface, or constantly over surface

// active tile data
ENTITY* ActiveTileSquare;												// the square entity
STRING* square1 = "square1.tga";      				// build square
STRING* square2 = "square2.tga";						// endpoint square 
STRING* square3 = "square3.tga";						// wait square 
STRING* square0 = "placement_cube_red.mdl";		
	
// path- and ActiveTileSquare-sprite height offset values
var RoadLevel = 0.2;
var PathLevel = 0.5;
var SquareLevel = 1;

var square_outofborder = 0;

////////////////////////////////////////////////////////////////////////////////////
// function declarations

// initialization
void		Interface_Init();

// camera
void		Camera_Init();
action 	SetCamera();   
void		TestLimits();

// ActiveTilePos functions
void		Square_Init();
action 	Square();
void		Mouse3D();
void		SnapIt();

/////////////////////////////////////////////////////////////////////////////
// code

void 		Interface_Init()
{				
	// set camera position and zoom
	CamZoom = (var)CAMZ;							
	CamAngle = 270-45; 					
	CamHeight = (var)CAMH;							
//	ZoomDownLimit = 225;
//	ZoomUpLimit = 1000;
	
	// set camera move limits - development parameters
	XMax =  abs(vScale.x);
	XMin = -abs(vScale.x);
	YMax =  abs(vScale.y);
	YMin = -abs(vScale.y);
	
	enable_polycollision = 0;			
	Camera_Init(); 		// CameraObject entity creation , to be followed by camera		
	wait_for(Camera_Init);	
	wait(3);		
	Square_Init();   		// ActiveTileSquare creation and settings		
	wait_for(Square_Init);	
	wait(3);
	enable_polycollision = 1;
}



/////////////////////////////////////////////////
// camera handling functions

// set min and max coordinates of camera object
void TestLimits()
{
	clamp( my.x , XMin , XMax );
	clamp( my.y , YMin , YMax );	
//	my.x = maxv(minv(my.x, XMax), XMin);
//	my.y = maxv(minv(my.y, YMax), YMin);
}

// make it undetectable by c_trace calls
action SetCamera()
{
	set( me , PASSABLE | FLAG2 | INVISIBLE );
	
	var temp = 0;
	VECTOR vTemp;
	STRING* temp_str = str_create("");
	while(me)
	{				
				
		//--------------------------------------------------------   
		// camera objext x y and movements by mouse and keyboard
		// forward
		if ((mouse_pos.y <= 2) || (key_cuu))
			{
				my.y += sin(camera.pan) * MoveSpeed * time_step + (key_shift * sin(camera.pan) * MoveSpeed * time_step);
				my.x += cos(camera.pan) * MoveSpeed * time_step + (key_shift * cos(camera.pan) * MoveSpeed * time_step);
				TestLimits();
			}
		// back
		if ((mouse_pos.y >= screen_size.y -2) || (key_cud))
			{
				my.y -= sin(camera.pan) * MoveSpeed * time_step + (key_shift * sin(camera.pan) * MoveSpeed * time_step);
				my.x -= cos(camera.pan) * MoveSpeed * time_step + (key_shift * cos(camera.pan) * MoveSpeed * time_step);
				TestLimits();
			}
		// left
		if ( ((mouse_pos.x <= 2) && (mouse_pos.y > screen_size.y/2)) || (key_cul))
			{
				my.y += cos(camera.pan) * MoveSpeed * time_step + (key_shift * cos(camera.pan) * MoveSpeed * time_step);
				my.x -= sin(camera.pan) * MoveSpeed * time_step + (key_shift * sin(camera.pan) * MoveSpeed * time_step);
				TestLimits();
			}
		// right
		if ( ((mouse_pos.x >= screen_size.x -2) && (mouse_pos.y > screen_size.y/2)) || (key_cur))
			{
				my.y -= cos(camera.pan) * MoveSpeed * time_step + (key_shift * cos(camera.pan) * MoveSpeed * time_step);
				my.x += sin(camera.pan) * MoveSpeed * time_step + (key_shift * sin(camera.pan) * MoveSpeed * time_step);
				TestLimits();
			}

		// rotate left-------
		if ( ((mouse_pos.x <= 2) && (mouse_pos.y <= screen_size.y/2)) || (key_pgup))
			{
				CamAngle += RotateSpeed * time_step;                        
			}
		// rotate right
		if ( ((mouse_pos.x >= screen_size.x -2) && (mouse_pos.y <= screen_size.y/2)) || (key_pgdn))
			{
				CamAngle -= RotateSpeed * time_step;                      
			}
			
		// zoom	------------
		if (((!(mickey.z==0)) || (key_home) || (key_end)) && (!key_shift)) 		
			{
				if (!(mickey.z==0)) CamZoom += ZoomSpeed * 0.05 * mickey.z ;
				if (key_home) CamZoom += ZoomSpeed * time_step *2;
				if (key_end) CamZoom -= ZoomSpeed * time_step *2;
				
				if (CamZoom < ZoomDownLimit)
					{
						CamZoom = ZoomDownLimit;
					}	
				if (CamZoom > ZoomUpLimit)
					{
						CamZoom = ZoomUpLimit;
					}
			}
		// camera object level
		if (((!(mickey.z==0)) || (key_home) || (key_end)) && (key_shift)) 
			{
				if (!(mickey.z==0)) CamHeight += ZoomSpeed * 0.05 * mickey.z ;
				if (key_home) CamHeight += ZoomSpeed * time_step *2;
				if (key_end) CamHeight -= ZoomSpeed * time_step *2;
				
				if (CamHeight < CamObjMinHeight)
					{
						CamHeight = CamObjMinHeight;
					}	
				if (CamHeight > CamObjMaxHeight)
					{
						CamHeight = CamObjMaxHeight;
					}
			}	
		// camera object z ---------------------------------------
		PosInfo3D(my.x, my.y);		
		if (HIT_TARGET) 
			{
				temp = my.z;						// previous position
				my.z = hit.z + CamHeight;		// new pos
				temp = my.z - temp;				// difference between previous and current pos
				CamZoom += temp;					// terrain following camera.z
				ZoomDownLimit += temp;
				ZoomUpLimit += temp;			
			}				
//		draw_text("camera-object z:",100,175,vector(255,100,100));
//		str_for_num(temp_str,my.z);
//		draw_text(temp_str,100,200,vector(255,100,100));
		
		// camera x y z ----------------------------------------------
		camera.z = CamZoom;
		// prevent camera from going under surface - and under water
		PosInfo3D(camera.x, camera.y);		
		if (HIT_TARGET) 
			{
				 camera.z = maxv( camera.z , hit.z + Map_TileSize*1 );			// *1 to make conversion to var				 
				 camera.z = maxv( camera.z , 0 + Map_TileSize*1 );				 
				 CamZoom  = camera.z;
			}			
		camera.x = my.x + CamZoom * cos(CamAngle); 
		camera.y = my.y + CamZoom * sin(CamAngle); 				
		vec_set(vTemp,my.x);
		vec_sub(vTemp,camera.x);
		vec_to_angle(camera.pan,vTemp);              
		CamAngle %= 360;    
		
//		draw_text("camera z:",100,275,vector(100,255,100));
//		str_for_num(temp_str,camera.z);
//		draw_text(temp_str,100,300,vector(100,255,100));
		
		//----------------------------------------------------
		wait(1);
	}
}

// place camera object entity to be followed by camera
void Camera_Init()
{ 		
	CameraObject = ent_create( CameraModel, vector(-1500,-1250,CamHeight), SetCamera);
}



/////////////////////////////////////////////////////////////////////////////////////////
// ActiveTileSquare related functions

// get mouse position on terrain surface
void Mouse3D()
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,50000) ); // should be a big value       		
	vec_for_screen (pos2, camera);

	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_MODELS | IGNORE_MAPS ); 
	// ignoring tile sprites, models, map-entities and other passable entities including passable terrains (water)      
}


// snapping to tile midpoint, within grid limits - limit if not free placement mode is active - i.e. in free placement mode no limiting
void SnapIt()
{  
	// x-axis snapping
	if (my.x < 0)
	{
		// snap
		my.x = Map_TileSize * (integer(my.x / Map_TileSize)) - (Map_TileSize / 2);		
	}
	else if (my.x >= 0)
	{
		// snap
		my.x = Map_TileSize * (integer(my.x / Map_TileSize)) + (Map_TileSize / 2);
	}       
	
	// y-axis snapping
	if (my.y < 0)
	{
		// snap
		my.y = Map_TileSize * (integer(my.y / Map_TileSize)) - (Map_TileSize / 2);
	}
	else if (my.y >= 0)
	{
		// snap
		my.y = Map_TileSize * (integer(my.y / Map_TileSize)) + (Map_TileSize / 2);
	}  
}


// set ActiveTileSquare place and properties, and refresh in each frame
action Square()
{          
	var temp;

	set( me , PASSABLE);			// passable to avoid c_tracing, | ZNEAR render over other entities  | INVISIBLE
//	my.scale_x = 0.5;
//	my.scale_y = 0.5;
//	// horizintal alignment
//	vec_set( my.pan , vector(270,90,0) ); 
	
	wait(3);
	
	VECTOR pos1, pos2;
	STRING* temp_str = str_create("");
	while(me)
	{         	                
//				Mouse3D();	// call c_tracing to get 3D position on terrain		

				vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
				vec_for_screen (pos1, camera);
				
				vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,50000) ); // should be a big value       		
				vec_for_screen (pos2, camera);
			
				c_trace (pos1, pos2, IGNORE_ME | IGNORE_CONTENT | IGNORE_SPRITES | IGNORE_PASSENTS | IGNORE_MODELS | IGNORE_MAPS);  // | SCAN_TEXTURE
				// ignoring tile sprites, models, map-entities and other passable entities including passable terrains (water)   
								
				// move square only if mouse points within terrain - else it remains in previous position		
				if (HIT_TARGET)  					// without this check it would point to Node (0,0) if mouse points out of terrain
				{
					my.x=hit.x;
					my.y=hit.y;					
					SnapIt();  						// snap to middle of tile

					my.z=hit.z;// + Map_TileSize/4;
	
				}		
		
//		draw_text("tile-object z:",100,375,vector(100,100,255));
//		str_for_num(temp_str,my.z);
//		draw_text(temp_str,100,400,vector(100,100,255));
				
		wait(1);
	}
	
}


// create ActiveTileSquare anywhere
void Square_Init()
{	
	ActiveTileSquare = ent_create( square0, nullvector, Square);	
}



