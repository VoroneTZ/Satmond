//-----------------------------------------------------------------------------
// SSAO for GAMESTUDIO A7 & A8   v0.52
// Copyright (c) 2010-2011, Christian Behrenberg [1]
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
// CREDITS
// Lite-C code, user support, demos, shaders: Christian Behrenberg [1]
// Port of the Crytek fragment shader from ShaderX7: Nils Daumann [2]
//
// KUDOS
// Many thanks to Felix Pohl [8], Robert Jäger [9] and Christopher Bläsius [10]
// for sharing their time and knowledge to make it work on some of those
// nagging NVIDIA cards.
//
// USE OF RELATED WORKS
// SSAO fragment shader adopted from "ShaderX7: Advanced Rendering Techniques"
// by Wolfgang Engel [3], original SSAO technique by Crytek [4]. Modified
// thresholded blur shader adopted an modified from "GPU Accelerated Stylistic
// Augmented Reality" by Rifat Aras and Yuzhong Shen [5]. Bilateral upsampling
// shader adopted and modified from GDC'09 workshop-slides "Mixed Resolution
// Rendering" by Jeremy Shopf [6] and "Image-Based Proxy Accumulation for Real-
// Time Soft Global Illumination" by Peter-Pike Sloan, Naga K. Govindaraju,
// Derek Nowrouzezahrai and John Snyder [7].
//
// REFRENCES
//
//  [1] http://www.christian-behrenberg.com
//  [2] http://www.slindev.com
//  [3] http://www.shaderx7.com
//  [4] http://www.crytek.com
//  [5] http://www.vmasc.odu.edu/downloads/Capstone_Papers/Engineering/Aras.pdf
//  [6] http://www.jshopf.com/files/GDC09_Shopf_Mixed_Resolution_Rendering.ppt
//  [7] http://www.cs.duke.edu/~tomasi/papers/tomasi/tomasiIccv98.pdf
//  [8] http://www.superku.de
//  [9] http://www.firoball.de
// [10] http://www.swollen-eyeballs.org
//-----------------------------------------------------------------------------

#ifndef ppSsao_c
#define ppSsao_c

// include header, if .c was include first
#include "ppSsao.h"


// INTEGRATION INTO GAME


	// Adds SSAO chain after the camera view
	void ssao ()
	{
		integrateSsao(camera);
	}


	// Adds SSAO chain after an arbitrary base view
	void integrateSsao (VIEW* base)
	{
		if (!base) return; // no base view defined

		// Set base view
		{
			ssaoViewPre = base;
	
			#ifndef SSAO_NOPARTICLES
				set(base, NOPARTICLE); // deactivate particles
			#endif
		}

		// Assert that the deferred view is ready to go
		if (viewSsaoDeferred == NULL)
		{
			viewSsaoDeferred = view_create(0); // Create view

			// Synchronize with diffuse view and supress particle rendering
			set(viewSsaoDeferred, SSAO_VIEW_DEFERRED_FLAGS);

			// Overwrite all surface materials with the deferred rendering material
			viewSsaoDeferred->material = objMtlDeferredSwitch;

			// Unpack depth (3x8bit -> 32bit FP) after sampling (see remarks in .h!)
			viewSsaoDeferred->stage = viewSsaoUnpackDepth;
		}
		
		// Link base view to deferred view
		{
			base->stage = viewSsaoDeferred;
			base->bmap  = bmapSsaoDiffuseHires;
		}
		
		#ifdef A7
			ssaoFogNr = fog_color;
			fog_color = 0;
		#endif
	}
	

	
// SWITCHING SSAO ON / OFF


	// Disables the previously integrated SSAO
	void hideSsao ()
	{
		if (!ssaoViewPre) return; // no base defined
	
		// decouple ssao chain
		ssaoViewPre->stage =
		ssaoViewPre->bmap  = NULL; // render to screen

		#ifndef SSAO_NOPARTICLES
			// re-enable particles in base view
			reset(ssaoViewPre, NOPARTICLE);
		#endif
		
		#ifdef A7
		fog_color = ssaoFogNr;
		#endif
	}
	
	
	// Enables the previously disabled integrated SSAO
	void showSsao ()
	{
		integrateSsao(ssaoViewPre);
	}
	
	
	// Switches from enabled to disabled SSAO and vice versa
	void toggleSsao ()
	{
		if (!ssaoViewPre) return; // no base defined
		if (!viewSsaoDeferred) return; // not established

		if (isSsaoActive())
			hideSsao();
		else
			showSsao();
	}


	// Enables / disables SSAO basing upon a boolean
	void setSsao (BOOL active)
	{
		if (!ssaoViewPre) return; // no base defined
		if (!viewSsaoDeferred) return; // not established
	
		if (!active)
			hideSsao();
		else
			showSsao();
	}
	

	
// QUERIES


	// Returns if SSAO ís currently active or not
	BOOL isSsaoActive ()
	{
		if (!ssaoViewPre) return false; // no base defined
		if (!viewSsaoDeferred) return false; // not established

		return(ssaoViewPre->stage == viewSsaoDeferred);
	}



// REMOVING FROM SYSTEM


	// Decouples the SSAO chain and removes all allocated texture data and the
	// dynamically created deferred view
	void removeSsao ()
	{
	 	hideSsao(); // decouple
	
		// Remove render targets
		ssaoFreePtr(&bmapSsaoDiffuseHires);
		ssaoFreePtr(&bmapSsaoDepthHires);
		ssaoFreePtr(&bmapSsaoDepthEncodedHires);
		ssaoFreePtr(&bmapSsaoNormalHires);
		ssaoFreePtr(&(mtlSsaoBlurPassH->skin1));
		ssaoFreePtr(&(mtlSsaoBlurPassV->skin1));
		#ifndef SSAO_NOSOFTALPHA
			ssaoFreePtr(&bmapSsaoAlphaHires);
		#endif
		
		// Remove deferred view
		ssaoFreePtr(&viewSsaoDeferred);

		// Reset context
		ssaoViewPre = NULL;
		// TODO: ssaoViewPost
	}



// RENDER CONTEXT


	// Attaches a view, while taking care of the SSAO chain, if necessary
	void ssaoAttachView (VIEW* predecessor, VIEW* newView)
	{
		if ((predecessor != NULL) && (newView != NULL))
		{
			// New view is inserted between predecessor and deferred view
			if (predecessor->stage == viewSsaoDeferred)
			{
				predecessor->stage = newView;
				newView->stage = viewSsaoDeferred;
			}
		}
	}


	// Detaches a view, while taking care of the SSAO chain, if necessary
	void ssaoDetachView (VIEW* disattached)
	{
		VIEW* predecessor = ssaoGetViewPredecessor(disattached);

		if ((disattached != NULL) && (predecessor != NULL))
		{
			// Successor view is deferred view
			if (disattached->stage == viewSsaoDeferred)
			{
				predecessor->stage = viewSsaoDeferred;
				disattached->stage = NULL;
				ssaoViewPre = predecessor;
			}
			else
			{
				if (disattached == ssaoViewPost)
				{
					ssaoViewPost = disattached->stage;
					disattached->stage = NULL;
				}
			}
		}
	}


	// Sets the first view BEFORE and AFTER the SSAO chain as
	// dynamically adapted render context
	void ssaoSetContext (VIEW* preSsao, VIEW* postSsao)
	{
		if (preSsao != NULL)
			ssaoViewPre = preSsao;

		if (postSsao != NULL)
			ssaoViewPost = postSsao;
	}
	
	

// SURFACE TYPES


	// Sets the surface type of an entity
	void setSsaoSurfaceType (ENTITY* e, int type)
	{
		if (e)
		{
			e->SURFACE |= type;
		}
	}


	// Returns if an entity has a specific surface type
	BOOL hasSsaoSurfaceType (ENTITY* e, int type)
	{
		if (e)
			return(e->SURFACE & type);
		else
			return(false);
	}


	// !! PRIVATE !!
	// Recognizes some surfaces types dependent on an entity's type
	void recognizeSsaoSurfaceType (ENTITY* e)
	{
		int entityType = ent_type(e);

		if (entityType >= 2)
		{
			switch (entityType)
			{
				case 2: // ABT-maps have no alpha!
				        e->SURFACE |= SOLID;
				        break;
	
				case 3: // BSP-maps have no alpha!
				        e->SURFACE |= SOLID;
				        break;
	
				case 4: // Terrains have no alpha!
				        e->SURFACE |= SOLID;
				        break;
				        
				// case 5: // Models
				// UNKNOWN - CAN'T MAKE ANY ASSUMPTIONS!
	
				case 6: // Sprites
				        #ifndef SSAO_NOSOFTALPHA // If soft alpha is allowed
	
				           // Sprites use by default softalpha
				           e->SURFACE |= SOFTALPHA;
	
				        #else // Soft alpha is disabled by the user
	
				           // Give sprites then binary alpha
				           e->SURFACE |= BINARYALPHA;
	
				        #endif
	
				        break;
			}
	
			// Don't try to recognize this entity anymore
			e->SURFACE |= (_RECOGNIZED);
		}
	}



// UTILITY FUNCTIONS


	// Asserts that a target bmap is 1.) allocated and 2.) is as big as the screen
	void ssaoAssertScreenSizedBmap (BMAP** bmap, int format, int lodRatio)
	{
		VECTOR bitmapSize;
		bitmapSize.x = screen_size.x / lodRatio;
		bitmapSize.y = screen_size.y / lodRatio;

		// If bitmap is allocated, check dimensions
		if (*bmap != NULL) {
			if (((*bmap)->width != bitmapSize.x) || ((*bmap)->height != bitmapSize.y))
			{
				ptr_remove(*bmap); // Remove bitmap from memory
				*bmap = NULL; // Nullify bitmap
			}
		}

		// When no bitmap is allocated (or removed above) (re-)create it
		if (*bmap == NULL) {
			*bmap = bmap_createblack(bitmapSize.x, bitmapSize.y, format);
		}
	}


	// Removes an engine object and sets the pointer to NULL
	void ssaoFreePtr (void** ptrRef)
	{
		if (ptrRef =! NULL) {
			if (*ptrRef != NULL) {
				ptr_remove(*ptrRef);
				*ptrRef = NULL;
			}
		}
	}


	VIEW* ssaoGetViewPredecessor (VIEW* successor)
	{
		if (!successor)
			return(NULL);

		VIEW* result = NULL;
		VIEW* v = ptr_first(camera);

		while (v)
		{
			if (v->stage == successor)
			{
				result = v;
				break;
			}

			v = (VIEW*)(v->link.next);
		}

		return(result);
	}



	void ssaoSyncView (VIEW* src, VIEW* dst)
	{
		if ((!src) || (!dst))
			return;

		vec_set(dst->x,   src->x);
		vec_set(dst->pan, src->pan);
		
		dst->arc = src->arc;

		dst->clip_near = src->clip_near;
		dst->clip_far  = src->clip_far;
	}


// MATERIAL EVENTS


	// DEFFERED OBJECT SHADER SWITCH
	void objMtlDeferredSwitch_ev ()
	{
		// Sky
		if (my->flags2 & (SKY))
		{
			mtl = objMtlDeferredSkyClip;
		}
		else // Mesh surface
		{
			// Automatically recognize surface type
			if (!(my->SURFACE & _RECOGNIZED)) {
				recognizeSsaoSurfaceType(my);
			}

			// Priority: 1) solid 2) binary 3) soft alpha

			// Override if SOLID is explicitely set
			if (!(my->SURFACE & SOLID))
			{
				// Prefer binary alpha over softalpha
				if (my->SURFACE & BINARYALPHA)
				{
					mtl = objMtlDeferredBinary;
				}
				#ifndef SSAO_NOSOFTALPHA
				else
				{
					// Soft alpha
					if (my->SURFACE & SOFTALPHA)
					{
						mtl = objMtlDeferredAlpha;
					}

					// Nothing else...
					// Use solid surface shader as default case
				}
				#endif
			}
		}
		
		// set depth normalization interval
		mtl->skill1 = floatv(render_view->clip_near);
		mtl->skill2 = floatv(render_view->clip_far);		

		return(0);
	}


	function mtlSsaoDeferred_ev ()
	{
		if (!viewSsaoDeferred) return;

		// DEFERRED VIEW
		if (render_view == viewSsaoDeferred)
		{
			// assert render targets
			
				ssaoAssertScreenSizedBmap(&bmapSsaoDepthHires, SSAO_DEPTH_HD,  SSAO_HD);
				ssaoAssertScreenSizedBmap(&bmapSsaoDepthEncodedHires, SSAO_DEPTH_ENCODED_HD, SSAO_HD);
				ssaoAssertScreenSizedBmap(&bmapSsaoNormalHires, SSAO_NORMAL_HD, SSAO_HD);

				#ifndef SSAO_NOSOFTALPHA
					ssaoAssertScreenSizedBmap(&bmapSsaoAlphaHires, SSAO_ALPHA_HD, SSAO_HD);
				#endif

			// update render targets

				viewSsaoDeferred->bmap = bmapSsaoDepthEncodedHires; // encoded depth
				viewSsaoDeferred->target1 = bmapSsaoNormalHires; // normals (xyz)

				#ifndef SSAO_NOSOFTALPHA
					viewSsaoDeferred->target2 = bmapSsaoAlphaHires; // alpha (r)
				#endif
				
			// set depth normalization interval
			mtl->skill1 = floatv(render_view->clip_near);
			mtl->skill2 = floatv(render_view->clip_far);
		}
		else
		{
			// DIFFUSE VIEW
			if (render_view->stage == viewSsaoDeferred)
			{
				// sync clipping parameters
				viewSsaoDeferred->clip_near = render_view->clip_near;
				viewSsaoDeferred->clip_far  = render_view->clip_far;

				// make sure that diffuse target is in correct size
				ssaoAssertScreenSizedBmap(&bmapSsaoDiffuseHires, SSAO_DIFF_HD, SSAO_HD);

				// redirect render target
				render_view->bmap = bmapSsaoDiffuseHires;

				#ifdef A7
					fog_color = 0;
				#endif
			}
		}
	}
	

	// Depth unpacking
	function mtlSsaoUnpackDepth_ev ()
	{
		mtl->skin1 = bmapSsaoDepthEncodedHires; // source
		render_view->bmap = bmapSsaoDepthHires; // target

		// set FP projection interval
		mtl->skill1 = floatv(viewSsaoDeferred->clip_near);
		mtl->skill2 = floatv(viewSsaoDeferred->clip_far);
		mtl->skill3 = floatv(viewSsaoDeferred->clip_far - viewSsaoDeferred->clip_near);
	}


	// SSAO
	void mtlSsaoApproximation_ev ()
	{
		// update shader parameters

			// Fog usage
			var fogColor;

			#ifdef A7
				fogColor = ssaoFogNr;
			#else
				fogColor = fog_color;
			#endif

			mtl->skill3 = floatv(fogColor != 0); // Is fog in use?
			mtl->skill4 = floatv(1.0 - (fogColor != 0)); // Is fog NOT in use?
			mtl->skill1 = floatv(viewSsaoDeferred->clip_near);
			mtl->skill2 = floatv(viewSsaoDeferred->clip_far);

		// assert target
		ssaoAssertScreenSizedBmap(&(mtl->skin1), SSAO_NRMxMASK_LD, SSAO_LD);

		// update targets

      	// Write (ao, depth) in BlurPassH->skin1
			render_view->bmap = mtlSsaoBlurPassH->skin1;

			// Store (normal.xyz, isProcessed) in my mtl->skin1
			render_view->target1 = mtl->skin1;
	}
	

	function mtlSsaoBlurPassH_ev ()
	{
		// update shader parameters
		mtl->skill1 = floatv(1.0); // horizontally
		mtl->skill2 = floatv(0.0);

		// fetch shader sources
		mtl->skin2 = mtlSsaoApproximation->skin1;

		// assert and set render target
		ssaoAssertScreenSizedBmap(&(mtl->skin1), SSAO_BLUR_AOxDEPTH, SSAO_LD);
		render_view->bmap = mtlSsaoBlurPassV->skin1;
	}


	function mtlSsaoBlurPassV_ev ()
	{
		// update shader parameters
		mtl->skill1 = floatv(0.0);
		mtl->skill2 = floatv(1.0); // vertically

		// fetch shader sources
		mtl->skin2 = mtlSsaoApproximation->skin1;

		// assert and set render target
		ssaoAssertScreenSizedBmap(&(mtl->skin1), SSAO_BLUR_AOxDEPTH, SSAO_LD);
		render_view->bmap =  mtlSsaoBlurPassH->skin1;
	}


	function mtlSsaoUpsampling_ev ()
	{
		// fetch shader sources
		mtl->skin1 = mtlSsaoBlurPassH->skin1;
		mtl->skin2 = mtlSsaoApproximation->skin1;

		// assert and set render target
		ssaoAssertScreenSizedBmap(&(mtl->skin3), SSAO_UPSAMPLING, SSAO_LD);
		render_view->bmap = mtl->skin3;
	}
	

   function mtlSsaoCombine_ev ()
   {
		// fetch shader sources
		mtl->skin1 = mtlSsaoUpsampling->skin3;
		
		#ifdef A7
		if (ssaoFogNr != 0)
		{
			mtl->skill1 = floatv(ssaoViewPre->clip_far);
			
			COLOR* color = NULL;
			
			switch ((int)ssaoFogNr)
			{
				case 1: color = &d3d_fogcolor1; break;
				case 2: color = &d3d_fogcolor2; break;
				case 3: color = &d3d_fogcolor3; break;
				case 4: color = &d3d_fogcolor4; break;
			}
			
			if (color)
			{
				mtl->skill2 = floatv(color->red   / 255.0);
				mtl->skill3 = floatv(color->green / 255.0);
				mtl->skill4 = floatv(color->blue  / 255.0);
			}
		}
		#endif

		#ifndef SSAO_NOPARTICLES

      	// Use particle pass if and only if particles are possibly visible
			if (num_particles > 0)
			{
				render_view->stage = viewSsaoParticle;
				ssaoSyncView(render_view, render_view->stage);

				viewSsaoParticle->stage = ssaoViewPost;
			}
			else {
				render_view->stage = ssaoViewPost;
			}

		#else
			render_view->stage = ssaoViewPost;
		#endif
	}


	#ifdef A7
	#ifndef SSAO_NOPARTICLES
		function mtlSsaoParticle_ev ()
		{
			return(1);
		}
	#endif
	#endif
	
	
// FOG (only A7)
//
// Remarks: the term "safely" in the descriptions is used due to the fact that
// for A7 the fog_color is always supressed to 0 to avoid fog plane artifacts
// in the deferred view -and- that the (original) fog_color is saved in another
// variable. So, when you use these function, everything is allright!

	// activates -safely- a fog color
	void ssaoActivateFog (var colorNr)
	{
		#ifdef A7
			if (isSsaoActive())
			{
				ssaoFogNr = colorNr;
				fog_color = 0;
			}
			else
		#endif
				fog_color = colorNr; // A8
	}


	// disables -safely- the fog
	void ssaoDisableFog ()
	{
		#ifdef A7
			if (isSsaoActive())
				ssaoFogNr = 0;
			else
		#endif
				fog_color = 0; // A8
	}
	

	// returns -safely- the fog color
	var ssaoFogColor ()
	{
		#ifdef A7
			if (isSsaoActive())
				return ssaoFogNr;
			else
		#endif
				return fog_color; // A8
	}

#endif /* ssao_c */