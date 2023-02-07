#define PRAGMA_PATH "shade-c/fx"
#define PRAGMA_PATH "shade-c/bmap"

/*******************************************************************************************************************
VARIABLES
*******************************************************************************************************************/

// DON'T CHANGE ANYTHING BELOW THIS LINE ---------------------------------------------------------------------------

var sc_myDepth = 1;
var sc_myTeam = 2;
var sc_myLuminance = 3;
var sc_myVolPart = 4;
var sc_myTexMov = 5;
var sc_myRefrCol = 6;

var sc_scSetup = 0;

void sc_reset();

/*******************************************************************************************************************
STRUCTS
*******************************************************************************************************************/

typedef struct {
	
   float myDepth;
   VECTOR* myLuminance;
   float myRefrR, myRefrG, myRefrB;
   float myVolPart;
   float myTexMov;
   float myTeamR, myTeamG, myTeamB;
} SC_OBJECT;

/*******************************************************************************************************************
BITMAPS
*******************************************************************************************************************/

BMAP* sc_map_scene = 0;
BMAP* sc_map_depth = 0;
BMAP* sc_bmap_dof = 0;
BMAP* sc_bmap_gamma = "#1x1x32";


/*******************************************************************************************************************
VIEWS
*******************************************************************************************************************/
//________________________________________________
VIEW* sc_view_last;

MATERIAL* sc_mtl_showScene =
{
	effect =
	"
	texture mtlSkin1;
	technique tech_01
	{
		pass pass_00
		{
			Texture[0] = <mtlSkin1>;
		}
	}
	";
}

VIEW* sc_view_showScene = {flags= PROCESS_TARGET; material = sc_mtl_showScene;}

/*******************************************************************************************************************
GLOBAL FUNCTIONS
*******************************************************************************************************************/

void sc_skill(ENTITY* ent,var objMode, var objVar)
{	
	VECTOR* objVec;
	if(objMode == 0){return;}
	
	SC_OBJECT *myData = malloc(sizeof(SC_OBJECT));

	if(my)
	{
		if(my.skill99) //check if my.skill99 has already been set
		{	
			//restore data that is already there
			SC_OBJECT* ObjData = (SC_OBJECT*)(my->skill99);
			myData = ObjData;
			
			/*
			myData->myDepth = ObjData->myDepth;
			myData->myTeam = ObjData->myTeam;
			myData->myLuminance = ObjData->myLuminance;
			myData->myRefractColor = ObjData->myRefractColor;
			myData->myVolPart = ObjData->myVolPart;
			*/
		}
		else //if there is no data, write default null data
		{
			myData->myDepth = 0;
			myData->myLuminance = 0;
			myData->myVolPart = 0;
			myData->myTexMov = 0;
			
			myData->myTeamR = 1;
			myData->myTeamG = 1;
			myData->myTeamB = 1;
			
			myData->myRefrR = 1;
			myData->myRefrG = 1;
			myData->myRefrB = 1;
		}
	}
	
	if(objMode == sc_myDepth){
		myData->myDepth = objVar;
	}
		
	if(objMode == sc_myTeam){
		objVec = objVar;
		myData->myTeamR = objVec.x/255;
		myData->myTeamG = objVec.y/255;
		myData->myTeamB = objVec.z/255;
	}
	
	if(objMode == sc_myLuminance){
		myData->myLuminance = objVar;
		myData->myLuminance->x /= 255;
		myData->myLuminance->y /= 255;
		myData->myLuminance->z /= 255;
	}
	
	if(objMode == sc_myVolPart){
		myData->myVolPart = objVar;
		myData->myDepth = 2; //clip from depthmap
	}
	
	if(objMode == sc_myTexMov){
		myData->myTexMov = objVar;
	}
	
	if(objMode == sc_myRefrCol){
		objVec = objVar;
		myData->myRefrR = objVec.x/255;
		myData->myRefrG = objVec.y/255;
		myData->myRefrB = objVec.z/255;
	}
	
	ent->skill99 = myData;
}
