#include <litec.h>
#include <com.h>

typedef struct IPolynomAPItbl
{
//long __stdcall QueryInterface(void* This, REFIID iid);//, LPVOID* ppv);   
long __stdcall AddRef(void* This);   
long __stdcall Release(void* This);

 int _stdcall FileIndex() ;
  
   
} IPolynomAPItbl;

typedef interface IPolynomAPI { IPolynomAPItbl * lpVtbl; } IPolynomAPI;



IPolynomAPI *pIPolynomAPI;

int WinMain()
{
 //   ...
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

// use the class
   // pIFoo->Func1();
  //  pIFoo->Func2(12345);
  int i;

	printf(pIPolynomAPI->FileIndex); 
    pIPolynomAPI->Release();
    CoUninitialize();  
}
