#include <litec.h>
#include <acknex.h>
#include <default.c>
#include <com.h>

typedef struct IPolynomAPItbl
{
	HRESULT  _stdcall QueryInterface(void* This,IID* iid, LPVOID* ppv);   
	DWORD    _stdcall AddRef(void* This);   
	DWORD    _stdcall Release(void* This);
	HRESULT _stdcall FileIndex(int* Value); 
	int _stdcall Check(); 
	 
} IPolynomAPItbl;

typedef interface IPolynomAPI { IPolynomAPItbl * lpVtbl; } IPolynomAPI;
int i;

IPolynomAPI *pIPolynomAPI;

PANEL* aircraft_pan =
{
  pos_x = 4; pos_y = 4;
  digits(0,0,9,"Arial#24bi",1,i);
  flags =  SHOW;
}

function cc()
{
 i = pIPolynomAPI->Check;
 printf("check");	
 
   HRESULT hr = pIPolynomAPI.FileIndex;
    if (hr!=S_OK)
    {
       printf("FileIndex Failed: %x\n\n", hr);
      // return 0;
    } else 
       printf("FileIndex succeeded\n");

}

function main()
{
	
  vec_set(screen_size,vector(800,400,0));
  vec_set(screen_color,vector(50,1,1)); // dark blue
  vec_set(sky_color,vector(50,1,1)); // dark blue
  video_window(NULL,NULL,0,"Test");
  d3d_antialias = 1;
  shadow_stencil = 3;
  
  level_load("zone1.wmb");
  vec_set(camera.x,vector(-250,0,50));
  vec_set(camera.pan,vector(0,-15,0));

 CoUninitialize();
// open the COM interface (only in Legacy Mode!)
    HRESULT hr = CoInitialize(NULL);
    if (hr!=S_OK)
    {
       printf("CoInitialize Failed: %x\n\n", hr);
      // return 0;
    } else 
       printf("CoInitialize succeeded\n");
    
// define the GUIDs (replace the question marks with the real GUIDs)
    GUID CLSID_PAPI;
    IID IID_IPAPI;
    IIDFromStr("{D4790516-7E0C-4E3F-9DA2-CC656AD6E218}",&CLSID_PAPI);
    IIDFromStr("{4D016CB3-CF33-4484-991B-0DB067382915}",&IID_IPAPI);

// get a pointer to the class function table    
    hr = CoCreateInstance(&CLSID_PAPI, NULL, CLSCTX_ALL,&IID_IPAPI, &pIPolynomAPI);

    if (hr!=S_OK) {
       printf("CoCreateInstance Failed: %x\n\n", hr);
       return 0;
     } else 
      printf("CoCreateInstance succeeded\n");
   pIPolynomAPI->AddRef();   
on_a = cc;
// pIPolynomAPI->Release();
 //   CoUninitialize();  

}

