#include "AEImageLoader.h"
#include "AEUtility.h"
#include "AESystemParam.h"
#include "AEPhysics.h"

#include "AEKeyboard.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEParticleSystem.h"
#include "AEFont.h"

#include "AEObject.h"
#include "AEFileLoader.h"

#include "AEHud.h"
#include "AESprite.h"
#include "AECamera.h"

#include "AECollision.h"
#include "AEScene.h"
#include "AEAI.h"

AEResourceTable rTable;
AEObjectTable oTable;
AEBackgroundLibrary bgLib;
AEFontLibrary fontLib;
AEParticleSystem ptclSys;
AESceneManager sceneManager;
AECamera camera;