BMAP* sc_bmap_shadowAlpha = "sc_shadowAlpha.bmp";

VIEW* sc_view_shadowDepth;
VIEW* sc_view_shadowDepth1;
MATERIAL* sc_mtl_shadow;

VECTOR lightPos;// = vector(0,0,0);

void blubb()
{
	if(render_view == sc_view_shadowDepth1)
	{
		//mat_set(sc_mtl_shadow.matrix,matWorld);
		//mat_multiply(sc_mtl_shadow.matrix,matView);
		//mat_multiply(sc_mtl_shadow.matrix,matProj);
		mat_set(sc_mtl_shadow.matrix,matProj);
		mat_effect1 = matViewInv;
	}
}

MATERIAL* sc_mtl_shadowDepth =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadowDepth.fx";
	//effect = "sc_shadowDepthOmni.fx";
	effect = "sc_vsmDepth.fx";
}

MATERIAL* sc_mtl_shadow =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadow.fx";
	//effect = "sc_shadowOmni.fx";
	effect = "sc_vsm.fx";

}


//BMAP* sc_bmap_shadowDepth = "#1024x768x32";
BMAP* sc_bmap_shadow = "#1024x768x32";
//BMAP* sc_bmap_shadowDepthCube = "#256x1536x32";
//BMAP* sc_bmap_shadowDepthCube = "#32x192x32";


var sc_shadowDepthSize = 512;

VIEW* sc_view_shadowDepth = {material = sc_mtl_shadowDepth; layer = -10;}

VIEW* sc_view_shadowDepth1 = {material = sc_mtl_shadowDepth; layer = -11; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}
VIEW* sc_view_shadowDepth2 = {material = sc_mtl_shadowDepth; layer = -12; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}
VIEW* sc_view_shadowDepth3 = {material = sc_mtl_shadowDepth; layer = -13; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}
VIEW* sc_view_shadowDepth4 = {material = sc_mtl_shadowDepth; layer = -14; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}
VIEW* sc_view_shadowDepth5 = {material = sc_mtl_shadowDepth; layer = -15; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}
VIEW* sc_view_shadowDepth6 = {material = sc_mtl_shadowDepth; layer = -16; size_x = sc_shadowDepthSize; size_y = sc_shadowDepthSize;}

VIEW* sc_view_shadow = {material = sc_mtl_shadow; layer = 2;}