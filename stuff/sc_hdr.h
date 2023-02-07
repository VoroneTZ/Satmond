#define SC_HDR

#ifndef SC_SCENERT
	#define SC_SCENERT
#endif

/*******************************************************************************************************************
VARIABLES
*******************************************************************************************************************/

// rendertarget resize-factor. 4 looks good in most cases
var sc_hdrRT = 4;

// 16/32/64/128 hdr bitdepth
var sc_hdrBits = 32;

var sc_hdrParams[7] = {0.001, 4, 80, 0.09, 0.18, 0.8, 2};
//var sc_hdrParams[7] = {0.003, 4, 68, 0.06, 0.18, 0.8, 2};
// [0] = blurstrength 1 ("light scattering")
// [1] = blurstrength 2 (blurring)
// [2] = bloomstrength
// [3] = Highpass luminance
// [4] = Highpass middle grey
// [5] = Highpass white cutoff
// [6] = Dynamic Exposure Adaption Speed

//turn HDR on/off
var sc_bHDR = 1;

/*******************************************************************************************************************
MATERIALS
*******************************************************************************************************************/

//________________________________________________
//High Dyanmic Range Rendering
MATERIAL* sc_mtl_hdrDownsample =
{
	effect = "sc_hdrDownsample.fx";
}

MATERIAL* sc_mtl_hdrHighpass =
{
	effect = "sc_hdrHighpass.fx";
}

MATERIAL* sc_mtl_hdrBlur =
{
	effect = "sc_hdrBlur.fx";	
}

MATERIAL* sc_mtl_hdrHBlur =
{
	effect = "sc_hdrHBlur.fx";	
}

MATERIAL* sc_mtl_hdrVBlur =
{
	effect = "sc_hdrVBlur.fx";	
}

MATERIAL* sc_mtl_hdr =
{
	effect = "sc_hdr.fx";	
}

MATERIAL* sc_mtl_hdrGamma =
{
	effect = "sc_hdrGamma.fx";	
}

MATERIAL* sc_mtl_hdrGamma2 =
{
	effect = "sc_hdrGamma2.fx";	
}

MATERIAL* sc_mtl_hdrGamma3 =
{
	effect = "sc_hdrGamma3.fx";
}
//

/*******************************************************************************************************************
VIEWS
*******************************************************************************************************************/

//________________________________________________
//High Dyanmic Range Rendering
VIEW* sc_view_hdrDownsample = {material = sc_mtl_hdrDownsample; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrHighpass = {material = sc_mtl_hdrHighpass; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrBlur = {material = sc_mtl_hdrBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrHBlur = {material = sc_mtl_hdrHBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrVBlur = {material = sc_mtl_hdrVBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdr= {material = sc_mtl_hdr; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrGamma= {material = sc_mtl_hdrGamma; flags = PROCESS_TARGET; layer = -10; bmap="#64x64x32";}
VIEW* sc_view_hdrGamma2= {material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 16; size_y = 16; bmap="#16x16x32";}
VIEW* sc_view_hdrGamma3= {material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 4; size_y = 4; bmap="#4x4x32";}
VIEW* sc_view_hdrGamma4= {material = sc_mtl_hdrGamma3; flags = PROCESS_TARGET;size_x = 1; size_y = 1;}
