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

#ifndef ppSsao_h
#define ppSsao_h

// ----------------------------------------------------------------------------
// COMPATIBILITY (for A7)
// ----------------------------------------------------------------------------

	#ifdef VERSION_810
		#define A8
	#endif

	#ifndef A8
		#define A7
	#endif

// ----------------------------------------------------------------------------
// CONFIGURATION   configurate this!
// ----------------------------------------------------------------------------

	// OCCLUDER RATIO
	// Determines the neglection of near vs. distant occluders. If you lower this
	// value between 0 and 1, surface dependent occlusion will be removed

		float ssaoRatioNearDistantOccluders = 0.5;

	// CONTRAST
	// To increase the brightness of highlighted edges (convex areas) or the darkness
	// of ambient occlusion (concave areas), you can use the following weights:

		float ssaoConvexHighlight = 1.1; // brightnes of edges
		float ssaoConcaveDarkness = 1.2; // darkness of ambient occlusion

	// SURFACE TYPES
	// =============

		#define SURFACE skill100 // Store surface data in skill 100

	/* #define SSAO_NOSOFTALPHA  // disables soft alpha support */

      // SURFACE TYPES
      // If none of these are set, entities are treated as being solid
      //
		// !!! DON'T CHANGE THESE VALUES !!!

		#define SOLID         (1<<0) // Solid surface
		#define BINARYALPHA   (1<<1) // Solid surface with sharp holes (if alpha < 50%)
		#define SOFTALPHA     (1<<2) // No AO-contribution; soft alpha masked

		void setSsaoSurfaceType (ENTITY* e, int type); // Sets the surface type of an entity
		BOOL hasSsaoSurfaceType (ENTITY* e, int type); // Returns if an entity has a specific surface type

	// PARTICLES
	// =========

      // Switch on if you don't want an additional particle pass or if you
		// don't use particles at all in your game

		/*  #define SSAO_NOPARTICLES  */

	// OWN INTEGRATION CONFIG

   	// Output render targets
		#define SSAO_DIFFUSE_SRC      bmap    // Diffuse output
		#define SSAO_DEPTH_SRC        target1 // Depth output
		#define SSAO_NORMAL_ALPHA_SRC target2 // Normal and alpha output

		#define SSAO_BASEVIEW     skill19 // Pointer to base view
		#define SSAO_SEPERATEVIEW skill20 // If additional depth view is used

// ----------------------------------------------------------------------------
// PUBLIC   use this!
// ----------------------------------------------------------------------------

	// INTEGRATION INTO GAME
	
		void ssao (); // Adds SSAO chain after the camera view
		void integrateSsao (VIEW* base); // Adds SSAO chain after an arbitrary base view
	
	// SWITCHING SSAO ON / OFF
	
		void hideSsao (); // Disables the previously integrated SSAO
		void showSsao (); // Enables the previously disabled integrated SSAO
		void toggleSsao (); // Switches from enabled to disabled SSAO and vice versa
		void setSsao (BOOL active); // Enables / disables SSAO basing upon a boolean

	// QUERIES
	
		BOOL isSsaoActive (); // Returns if SSAO ís currently active or not
	
	// REMOVING FROM SYSTEM

		void removeSsao (); // Decouples SSAO and frees memory
		
	// RENDER CONTEXT
	
		void ssaoAttachView (VIEW* predecessor, VIEW* newView); // Attaches a view, while taking care of the SSAO chain, if necessary
		void ssaoDetachView (VIEW* disattached); // Detaches a view, while taking care of the SSAO chain, if necessary
		void ssaoSetContext (VIEW* pre, VIEW* post); // Sets the first view BEFORE and AFTER the SSAO chain as dynamically adapted render context		

	// SURFACE TYPES

		void setSsaoSurfaceType (ENTITY* e, int type); // Sets the surface type of an entity
		BOOL hasSsaoSurfaceType (ENTITY* e, int type); // Returns if an entity has a specific surface type

	// Render context
	void ssaoAttachView (VIEW* predecessor, VIEW* newView);
	void ssaoDetachView (VIEW* disattached);
	void ssaoSetContext (VIEW* preSsao, VIEW* postSsao);

	// Fog
	void ssaoActivateFog (var colorNr);
	void ssaoDisableFog ();

// ----------------------------------------------------------------------------
// PRIVATE   don't touch this!
// ----------------------------------------------------------------------------

	#define SSAO_HD 1 // full size
	#define SSAO_LD 2 // half size

	// RENDER TARGETS & -FORMATS
	//
	// Note: some formats are forcing some RT's to have more channels than they
	// need to (like SSAO_NORMAL_HD) because some nVidia cards are making trouble
	// when the total bit size of all render targets of a view are different!

		// HiRes

			BMAP* bmapSsaoDiffuseHires = NULL;
			#define SSAO_DIFF_HD 888 // 24bit

	      BMAP* bmapSsaoDepthHires = NULL;
			#define SSAO_DEPTH_HD 14 // 32bit

	      BMAP* bmapSsaoDepthEncodedHires = NULL;
			#define SSAO_DEPTH_ENCODED_HD 8888 // 32bit

	      BMAP* bmapSsaoNormalHires = NULL;
			#define SSAO_NORMAL_HD 8888 // 32bit

			#ifndef SSAO_NOSOFTALPHA
				BMAP* bmapSsaoAlphaHires = NULL;
				#define SSAO_ALPHA_HD 8888 // 32bit
			#endif

		// LoRes

			#define SSAO_NRMxMASK_LD     8888 // 32bit
			#define SSAO_BLUR_AOxDEPTH   122  // 32bit
			#define SSAO_UPSAMPLING      8888 // 32bit

	// SURFACE TYPES

		// Recognizes some surfaces types dependent on an entity's type
		void recognizeSsaoSurfaceType (ENTITY* e);

      // If true, surface has been recognized
		#define _RECOGNIZED (1<<8)

		// Soft alpha
		
			#ifndef SSAO_SOFTALPHA_QUALITY
				#define SSAO_SOFTALPHA_QUALITY SSAO_LD
			#endif		
		
			float ssaoSoftalphaQuality = SSAO_SOFTALPHA_QUALITY; // for shaders

			#ifndef SSAO_NOSOFTALPHA
				float ssaoUseSoftAlpha = 1.0; // Default
			#else
				float ssaoUseSoftAlpha = 0.0;
			#endif		

	// FOG (A7 only)
	#ifdef A7
		var ssaoFogNr;
	#endif

	// UTILITY FUNCTIONS

		// Asserts that a target bmap is 1.) allocated and 2.) is as big as the screen
		void ssaoAssertScreenSizedBmap (BMAP** bmap, int format, int lodRatio);
		
		// Frees a systems resources and nullifies the previous pointer (!!!)
		void ssaoFreePtr (void** ptrRef);

		// Gets the first view who assigned the <successor>-VIEW as .stage
		VIEW* ssaoGetViewPredecessor (VIEW* successor);

		// Syncs a destination view with a source view (pos, angle, arc)
		void ssaoSyncView (VIEW* src, VIEW* dst);
		
	// (PRE-)DEFERRED RENDER EVENT
	// Execute on deferred and pre-deferred view

		function mtlSsaoDeferred_ev ();
		MATERIAL* mtlSsaoDeferred = {
		  event = mtlSsaoDeferred_ev;
		  flags = ENABLE_VIEW;
		}

	// SHADERS

   	// Include <default.fx> for shader code
		#define PRAGMA_PATH "%EXE_DIR%\\code";
		#define PRAGMA_BIND "%EXE_DIR%\\code\\default.fx";
		#define PRAGMA_BIND "d3dcompiler_42.dll";

		// OBJECT SHADERS

			// Deferred View
			// -------------

				// SWITCHING SHADER

					void objMtlDeferredSwitch_ev();
					MATERIAL* objMtlDeferredSwitch = {
						effect = "ao_obj_solid.fx";
						flags = ENABLE_RENDER;
						event = objMtlDeferredSwitch_ev;
					}

					// SKY CLIP SHADER

						MATERIAL* objMtlDeferredSkyClip = {
							effect = "ao_obj_sky.fx";
						}

					// SOLID SHADER
					// Contributes depth from object, but no alpha

						MATERIAL* objMtlDeferredSolid = {
							effect = "ao_obj_solid.fx";
						}

					// BINARY ALPHA SHADER
					// Contributes depth from object via binary alpha

						MATERIAL* objMtlDeferredBinary = {
							effect = "ao_obj_binary.fx";
						}

		         // SOFT ALPHA SHADER
					// Contributes alpha from object, but no depth

						#ifndef SSAO_NOSOFTALPHA
						MATERIAL* objMtlDeferredAlpha = {
							effect = "ao_obj_soft.fx";
						}
						#endif


			// Particle View
			// -------------
			#ifdef A7
			#ifndef SSAO_NOPARTICLES
				function mtlSsaoParticle_ev ();
				MATERIAL* mtlSsaoParticle = // only for A7
				{
					// Dummy shader
					effect = "float4 PS () : COLOR {return(0);} technique t {pass p {pixelShader = compile ps_2_0 PS();}}";

					flags = ENABLE_TREE;
					event = mtlSsaoParticle_ev; // Clip event
				}
			#endif
			#endif


		// POSTPROCESSING SHADERS
		
			// DEPTH UNPACKING
			// Some nVidia cards have problems with alpha blending into single channel FP-
			// targets, so depth is blended on 8888 targets with the first 3 channels
			// storing the packed depth as 3x8bit tupel. Since the AO stage uses a bilinear
			// downsampled depth map, direct access and decoding to the packed depth map is
			// resulting in artifacts. To overcome this and avoid overhead of unpacking
			// calls, depth is unpacked seperately before any SSAO processing takes place.
			
				function mtlSsaoUnpackDepth_ev ();
				MATERIAL* mtlSsaoUnpackDepth = {
					effect = "ao_pp_888toFP.fx";
					flags = ENABLE_VIEW;
					event = mtlSsaoUnpackDepth_ev;
				}

			// SSAO APPROXIMATION
			// Microsphere SSAO approach (Crytek style). Downsamples and processes
			// the half resolution sized depth map. Takes soft alpha supression and
			// fog into account, if necessary.

				var mtlSsaoApproximation_ev();
				MATERIAL* mtlSsaoApproximation = {
					effect = "ao_pp_ssao.fx";
					flags = ENABLE_VIEW;
					event = mtlSsaoApproximation_ev;
				}

			// THRESHOLDED BLUR
			// Removes AO noise dependent on depth (dynamic parabolic depth thresholding)
			// and surface normals (weighted by normalized dot product)

         	// horizontal pass
				function mtlSsaoBlurPassH_ev();
				MATERIAL* mtlSsaoBlurPassH = {
					effect = "ao_pp_blur.fx";
					flags = ENABLE_VIEW;
					event = mtlSsaoBlurPassH_ev;
				}

				// vertical pass
				function mtlSsaoBlurPassV_ev();
				MATERIAL* mtlSsaoBlurPassV = {
					effect = "ao_pp_blur.fx";
					flags = ENABLE_VIEW;
					event = mtlSsaoBlurPassV_ev;
				}

			// BILATERAL UPSAMPLING
			// Upsamples to screen size again and tries to improve the regular bilinear
			// upsampling by taking surface normals and depth incontinuouties into
			// account. Upsamples 2x2 hires blocks from one coarse sample by weighting
			// respective bilinear weights with inversed depth differences and dot
			// products between a coarse texel and the surrounding source hires texels.

				function mtlSsaoUpsampling_ev();
				MATERIAL* mtlSsaoUpsampling = {
					effect = "ao_pp_upsample.fx";
					flags = ENABLE_VIEW;
					event = mtlSsaoUpsampling_ev;
				}

			// COMBINE
			// Simply multiplies the AO with the diffuse (RGB) view. If A7 is in use,
			// fog color blending is done here.

				function mtlSsaoCombine_ev();
				MATERIAL* mtlSsaoCombine = {
					effect = "ao_pp_combine.fx";
					event = mtlSsaoCombine_ev;
					flags = ENABLE_VIEW;
				}
				
	// POSTPROCESSING CHAIN

		// STAGES
		
			VIEW* ssaoViewPost; // First view after ssao chain (yet unused)

      	// Render particles into AO'ed scene
      	#ifndef SSAO_NOPARTICLES
				VIEW* viewSsaoParticle =
				{
					#ifdef A7
						flags = PROCESS_SCREEN | NOFOG | NOSKY | NOSHADOW | UNTOUCHABLE; // A7
						material = mtlSsaoParticle; // Dummy shader to prevent rendering
					#else
						flags = PROCESS_SCREEN | NOFOG | NOSKY | NOWORLD | NOENT | NOSHADOW | UNTOUCHABLE; // A8
					#endif
				}
			#endif

			// Combine = AO * RGB (+FOG if A7)
			VIEW* viewSsaoCombine =
			{
				flags = PROCESS_TARGET | CHILD;
				material = mtlSsaoCombine;
			}

			// Bilateral Upsampling -> crisp, hires AO
			VIEW* viewSsaoUpsampling =
			{
				flags = PROCESS_TARGET | CHILD;
				material = mtlSsaoUpsampling;
				stage = viewSsaoCombine;
			}

			// Thresholded Gaussian Blur -> smooth but lowres AO

				// vertical blur pass
				VIEW* viewSsaoBlurPassV = {
					flags = PROCESS_TARGET | CHILD;
					material = mtlSsaoBlurPassV;
					stage = viewSsaoUpsampling;
				}

				// horizontal blur pass
				VIEW* viewSsaoBlurPassH = {
					flags = PROCESS_TARGET | CHILD;
					material = mtlSsaoBlurPassH;
					stage = viewSsaoBlurPassV;
				}

			// SSAO -> raw, noisy and lowres AO
			VIEW* viewSsaoApproximation = {
				flags = PROCESS_TARGET | CHILD;
				material = mtlSsaoApproximation;
				stage = viewSsaoBlurPassH;
			}
			
			// unpacks the encoded 888 depth into a 32 FP target
			VIEW* viewSsaoUnpackDepth = {
				flags = PROCESS_TARGET | CHILD;
				material = mtlSsaoUnpackDepth;
				stage = viewSsaoApproximation;
			}

			// Deferred view -> normals, depth and soft alpha
			VIEW* viewSsaoDeferred = NULL;

				#ifndef SSAO_NOPARTICLES
					#define SSAO_VIEW_DEFERRED_FLAGS (CHILD | UNTOUCHABLE | NOFOG | NOSHADOW | NOPARTICLE)
				#else
					#define SSAO_VIEW_DEFERRED_FLAGS (CHILD | UNTOUCHABLE | NOFOG | NOSHADOW)
				#endif

			VIEW* ssaoViewPre = NULL;    // Last view before ssao chain
			VIEW* ssaoDiffuseSrc = NULL; // Diffuse source view (yet unused)

// Include code, if header was included first
#include "ppSsao.c"

#endif /* ppSsao_h */