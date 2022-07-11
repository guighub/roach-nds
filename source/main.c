// This code is based off the Nitro Engine examples
// Copyright (c) 2008-2011, 2019, Antonio Niño Díaz
#include <NEMain.h>
#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "roach_bin.h"
#include "texture_bin.h"

NE_Camera *Camera;
NE_Model *Model;
NE_Material *Material;

void Draw3DScene(void)
{
	NE_CameraUse(Camera);
	
	NE_PolyFormat(31, 0, NE_LIGHT_ALL, NE_CULL_BACK, 0);

	NE_ModelDraw(Model);
}

int main(void)
{
	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_VBLANK, NE_VBLFunc);
	irqSet(IRQ_HBLANK, NE_HBLFunc);

	NE_Init3D();
	// libnds uses VRAM_C for the text console, reserve A and B only
	NE_TextureSystemReset(0, 0, NE_VRAM_AB);
	// Init console in non-3D screen
	consoleDemoInit();
	printf("\x1B[2J");
	printf("Funky ahh roach!1!!!!");

	// Allocate objects
	Model = NE_ModelCreate(NE_Static);
	Camera = NE_CameraCreate();
	Material = NE_MaterialCreate();

	// Setup camera
	NE_CameraSet(Camera,
		     2, 0, 0,
		     0, 0, 0,
		     0, 1, 0);

	// Load model...
	NE_ModelLoadStaticMesh(Model,(u32*)roach_bin);
	NE_MaterialTexLoadBMPtoRGBA(Material, (void*) texture_bin, 0);
	NE_ModelSetMaterial(Model, Material);

	// Set some propierties to Material
	NE_MaterialSetPropierties(Material,
				 RGB15(31, 31, 31), // diffuse
				 RGB15(31, 31, 31), // ambient
				 RGB15(0, 0, 0),    // specular
				 RGB15(0, 0, 0),	// emission
				 false, false);     // vtxcolor, useshininess

	// Initiate soundbank
	mmInitDefaultMem((mm_addr)soundbank_bin);

	// Load sound
	mmLoad(MOD_AROUNDTHEWORLD);
	mmStart(MOD_AROUNDTHEWORLD, MM_PLAY_LOOP);

	bool b = true;
	float f = -0.15f;
	while (1)
	{
		// Rotate model
		NE_ModelRotate(Model, 0, -6, 0);
		if (b == true)
		{
			if (f <= 0.15f)
			{
				f += 0.02f;
			}
			else {
				b = false;
			}
		}
		else {
			if (f >= -0.15f)
			{
				f -= 0.02f;
			}
			else {
				b = true;
			}
		}
		NE_ModelTranslate(Model, 0, f, 0);
		
		NE_Process(Draw3DScene);
		NE_WaitForVBL(0);
	}
}