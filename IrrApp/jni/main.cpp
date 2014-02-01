/** Example 003 Custom SceneNode

This Tutorial is more advanced than the previous ones.
If you are currently just playing around with the Irrlicht
engine, you may want to look at other examples first.
This tutorials shows how to create a custom scene node and
how to use it in the engine. A custom scene node is needed
if you want to implement a render technique the Irrlicht
Engine currently does not support. For example, you can write
an indoor portal based renderer or an advanced terrain scene
node with it. By creating custom scene nodes, you can
easily extend the Irrlicht Engine and adapt it to your own
needs.
 
I will keep the tutorial simple: Keep everything very
short, everything in one .cpp file, and I'll use the engine
here as in all other tutorials.

To start, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#include <irrlicht.h>
#include "driverChoice.h"
#include <vector>
#include <windows.h>
#include "ObjLoader.h"
#include <string>

using namespace irr;
using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif



class CSampleSceneNode2 : public scene::ISceneNode
{

	/*
	First, we declare some member variables:
	The bounding box, 4 vertices, and the material of the tetraeder.
	*/
	core::aabbox3d<f32> Box;
	


public:
	std::vector<video::S3DVertex> vertexbuffer;
	std::vector<u16> indexbuffer;
	video::SMaterial Material;


	CSampleSceneNode2(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
		: scene::ISceneNode(parent, mgr, id)
	{
	
	}
	
	
	/*
	Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
	method of every scene node in the scene is called by the scene manager.
	If the scene node wishes to draw itself, it may register itself in the
	scene manager to be drawn. This is necessary to tell the scene manager
	when it should call irr::scene::ISceneNode::render(). For
	example, normal scene nodes render their content one after another,
	while stencil buffer shadows would like to be drawn after all other
	scene nodes. And camera or light scene nodes need to be rendered before
	all other scene nodes (if at all). So here we simply register the
	scene node to render normally. If we would like to let it be rendered
	like cameras or light, we would have to call
	SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
	After this, we call the actual
	irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
	which simply lets also all the child scene nodes of this node register
	themselves.
	*/
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	/*
	In the render() method most of the interesting stuff happens: The
	Scene node renders itself. We override this method and draw the
	tetraeder.
	*/
	virtual void render()
	{
		Material.Wireframe = false;
		Material.Lighting = false;

		Box.reset(vertexbuffer[0].Pos);
		for (s32 i=1; i<vertexbuffer.size(); ++i)
			Box.addInternalPoint(vertexbuffer[i].Pos);

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&vertexbuffer[0], vertexbuffer.size()/3	, &indexbuffer[0], indexbuffer.size()/3, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	}

	/*
	And finally we create three small additional methods.
	irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
	this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
	amount of materials in this scene node (our tetraeder only has one
	material), and irr::scene::ISceneNode::getMaterial() returns the
	material at an index. Because we have only one material here, we can
	return the only one material, assuming that no one ever calls
	getMaterial() with an index greater than 0.
	*/
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}	
};


std::vector<CSampleSceneNode2*> load(irr::IrrlichtDevice* vd, wchar_t* strFilename, scene::ISceneNode* parent, scene::ISceneManager* mgr)
{
		std::vector<CSampleSceneNode2*> renderlist;

        //const wchar_t* fileType = ::wcschr(strFilename, L'.');
        AFileLoader* loader = new ObjLoader(vd);
      

        if (!loader || !loader->Read(strFilename))
            return renderlist;
	
		s32 id = 0;

        irr::core::vector3d<f32>  min, max;
        min.X = min.Y = min.Z = FLT_MAX;
        max.X = max.Y = max.Z = -FLT_MAX;
     
		irr::core::vector3d<f32> defaultNormal( 0, 0, 1 );
        irr::core::vector2d<f32> defaultUV(0,0);
		video::SMaterial* defaultmaterial = new video::SMaterial();
		
        while(loader->Glist.size() > 0)
        {
            AFileLoader::Group *grp = loader->Glist.front();

            if (grp->flist.size() > 0 && grp->geomType == AFileLoader::Mesh)
            {
                if (grp->has_norm == false)
                {
                    grp->computeNormals();
                    grp->has_norm = true;
                }


                if (min.X > grp->min.X) min.X = grp->min.X;
                if (min.Y > grp->min.Y) min.Y = grp->min.Y;
                if (min.Z > grp->min.Z) min.Z = grp->min.Z;
                if (max.X < grp->max.X) max.X = grp->max.X;
                if (max.Y < grp->max.Y) max.Y = grp->max.Y;
                if (max.Z < grp->max.Z) max.Z = grp->max.Z;

                // Get the required vertex format from the effect for the group's effect instance.
				video::SMaterial* pInstance  = grp->mat == NULL ?
                    defaultmaterial : grp->mat->pEffectInstance;


				std::vector<video::S3DVertex> vertexbufffer;
				std::vector<u16> indexbuffer;
              
                float* inData = (float*) &grp->vplist[0];                                        

                for( int i = 0; i < grp->vsize(); ++i )
                {
					video::S3DVertex singlevertex;
                    // add a new position to the builder
                    irr::core::vector3d<f32>* pPos = (irr::core::vector3d<f32>*)inData;
					singlevertex.Pos = *pPos;
                  
                    inData += 3;
					irr::core::vector3d<f32>* norm = grp->has_norm ? (irr::core::vector3d<f32>*)inData : &defaultNormal;
                    singlevertex.Normal = *norm;

                       
                    if (grp->has_norm) 
                        inData += 3;

                    // add a new texture coordinate to the builder
					singlevertex.TCoords = *(grp->has_tex ? (irr::core::vector2d<f32>*)inData : &defaultUV);                            
                      
                  
                    if (grp->has_tex) 
                        inData += 2;

					vertexbufffer.push_back(singlevertex);

					
                }
				for(int i=0; i<(int) grp->flist.size(); i++)
				{
					indexbuffer.push_back(grp->flist[i]);
				}


				CSampleSceneNode2 * pNode = new  CSampleSceneNode2(parent, mgr, id++);
				pNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
				pNode->vertexbuffer = vertexbufffer;
				pNode->indexbuffer = indexbuffer;
				pNode->Material = *pInstance;
              
                renderlist.push_back(pNode);
				

            }// if (grp->flist.size() > 0 && grp->geomType == AFileLoader::Mesh)
            else if (grp->geomType == AFileLoader::Lines && grp->vsize())
            {
				std::vector<video::S3DVertex> vertexbufffer;
				std::vector<u16> indexbuffer;
   
                float *inData = (float *) &grp->vplist[0];
				 
				video::SMaterial* pInstance  = grp->mat == NULL ?
                NULL : grp->mat->pEffectInstance;


                for (int ii=0; ii < (int) grp->lineLengths.size(); ii++)
                {
                    if (min.X > grp->min.X) min.X = grp->min.X;
                    if (min.Y > grp->min.Y) min.Y = grp->min.Y;
                    if (min.Z > grp->min.Z) min.Z = grp->min.Z;
                    if (max.X < grp->max.X) max.X = grp->max.X;
                    if (max.Y < grp->max.Y) max.Y = grp->max.Y;
                    if (max.Z < grp->max.Z) max.Z = grp->max.Z;				

               


					video::S3DVertex singlevertex;
                    // add a new position to the builder
                    irr::core::vector3d<f32>* pPos = (irr::core::vector3d<f32>*)inData;
					singlevertex.Pos = *pPos;
                  
                    inData += 3;
                    
					irr::core::vector3df* norm = grp->has_norm ? (irr::core::vector3df*)inData : &defaultNormal;
					
					singlevertex.Normal = *norm;
                       
                    if (grp->has_norm) 
                        inData += 3;

                    // add a new texture coordinate to the builder
					irr::core::vector2d<f32>* texcoor = grp->has_tex ? (irr::core::vector2d<f32>*)inData : &defaultUV;                            

					singlevertex.TCoords = *texcoor;
                      
                  
                    if (grp->has_tex) 
                        inData += 2;

					vertexbufffer.push_back(singlevertex);

					
                }
				for(int i=0; i<(int) grp->flist.size(); i++)
				{
					indexbuffer.push_back(grp->flist[i]);
				}
                   
				CSampleSceneNode2 * pNode = new  CSampleSceneNode2(parent, mgr, id++);
				pNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
				pNode->vertexbuffer = vertexbufffer;
				pNode->indexbuffer = indexbuffer;
				pNode->Material = *pInstance;
              
				renderlist.push_back(pNode);
            }//else

            loader->Glist.pop_front();

            if(grp)
            {
                delete grp;
                grp = NULL;
            }

        }//while
	
    return renderlist;
}
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device

	IrrlichtDevice *device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false);
		
	if (device == 0)
		return 1; // could not create selected driver.

	// create engine and camera

	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	smgr->addCameraSceneNode(0, core::vector3df(0,-4,0), core::vector3df(0,0,0));

	  wchar_t Dir[256] = {0};
    GetCurrentDirectoryW( 256, Dir );

	std::wstring path(Dir);
	path += L"\\objfiles\\chevalier.obj";

	std::vector<CSampleSceneNode2*> obworld = load(device, (wchar_t*)path.c_str(), smgr->getRootSceneNode(), smgr);

	//for(
	/*
	To animate something in this boring scene consisting only of one
	tetraeder, and to show that you now can use your scene node like any
	other scene node in the engine, we add an animator to the scene node,
	which rotates the node a little bit.
	irr::scene::ISceneManager::createRotationAnimator() could return 0, so
	should be checked.
	*/
	scene::ISceneNodeAnimator* anim =
		smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

	if(anim)
	{
		vector<CSampleSceneNode2*>::iterator iter;  
    for (iter=obworld.begin();iter!=obworld.end();iter++)  
    {  
        (*iter)->addAnimator(anim); 
    }  


		//myNode->addAnimator(anim);
		
		/*
		I'm done referring to anim, so must
		irr::IReferenceCounted::drop() this reference now because it
		was produced by a createFoo() function. As I shouldn't refer to
		it again, ensure that I can't by setting to 0.
		*/
		anim->drop();
		anim = 0;
	}

	/*
	I'm done with my CSampleSceneNode object, and so must drop my reference.
	This won't delete the object, yet, because it is still attached to the
	scene graph, which prevents the deletion until the graph is deleted or the
	custom scene node is removed from it.
	*/

	vector<CSampleSceneNode2*>::iterator iter;  
    for (iter=obworld.begin();iter!=obworld.end();iter++)  
    {  
        (*iter)->drop();
		(*iter) = 0;
    }  

	//myNode->drop();
	//myNode = 0; // As I shouldn't refer to it again, ensure that I can't

	/*
	Now draw everything and finish.
	*/
	u32 frames=0;
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		smgr->drawAll();

		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}

	device->drop();
	
	return 0;
}

/*
That's it. Compile and play around with the program.
**/
