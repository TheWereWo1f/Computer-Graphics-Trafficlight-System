#include <iostream>
#include <windows.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/AnimationPath>
#include <osg/Matrix>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include "raaInputController.h"
#include "raaAssetLibrary.h"
#include "raaFacarde.h"
#include "raaRoadTileFacarde.h"
#include "raaAnimationPointFinder.h"
#include "raaCarFacarde.h"
#include "raaTrafficSystem.h"
#include "vsTrafficLightFacarde.h"

typedef std::vector<raaAnimationPointFinder>raaAnimationPointFinders;
osg::Group* g_pRoot = 0; // root of the sg
osg::MatrixTransform* m_pBoxMT;
osg::Material* m_pOffMaterial;
float g_fTileSize = 472.441f; // width/depth of the standard road tiles

std::string g_sDataPath = "../../data/";
enum raaRoadTileType
{
	Normal,
	LitTJunction,
	LitXJunction,
};

/*
 * Method that creates the roadtile from the facarde class and adds it to the parent group
 */
void addRoadTile(std::string sAssetName, std::string sPartName, int xUnit, int yUnit, float fRot, osg::Group* pParent)
{
	raaFacarde* pFacarde = new raaRoadTileFacarde(raaAssetLibrary::getNamedAsset(sAssetName, sPartName), osg::Vec3(g_fTileSize * xUnit, g_fTileSize * yUnit, 0.0f), fRot);
	pParent->addChild(pFacarde->root());
}

/*
 * Method that creates the trafficLights from the facarde class and adds it to the parent group
 */
void addTrafficLight(std::string sAssetName, std::string sPartName, int xUnit, int yUnit, float fRot, osg::Group* pParent)
{
	raaFacarde* pLight = new vsTrafficLightFacarde(raaAssetLibrary::getNamedAsset(sAssetName, sPartName), osg::Vec3f(xUnit, yUnit, 0.0f), fRot, 0.03f);
	pParent->addChild(pLight->root());
}

osg::Node* buildAnimatedVehicleAsset()
{
	osg::Group* pGroup = new osg::Group();
	osg::Geode* pGB = new osg::Geode();
	osg::ShapeDrawable* pGeomB = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 100.0f, 60.0f, 40.0f));
	osg::Material* pMat = new osg::Material();
	pMat->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3f, 0.3f, 0.1f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.8f, 0.8f, 0.3f, 1.0f));
	pMat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 0.6f, 1.0f));

	pGroup->addChild(pGB);
	pGB->addDrawable(pGeomB);

	pGB->getOrCreateStateSet()->setAttribute(pMat, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	pGB->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);

	return pGroup;
}


/*
 * Creates the collision target to see if the car fits it
 */
void addCollisionTarget()
{
	m_pBoxMT = new osg::MatrixTransform();
	osg::Geode* pBoxGeode = new osg::Geode();
	osg::ShapeDrawable* pBoxSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(-180.0f, -50.0f, 10.0f), 100.0f));

	m_pBoxMT->addChild(pBoxGeode);
	pBoxGeode->addDrawable(pBoxSD);
	pBoxGeode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pBoxMT->getOrCreateStateSet()->setAttributeAndModes(m_pOffMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	g_pRoot->addChild(m_pBoxMT);
}
/*
 * Loop through the animation points and insert them into the AnimationPath
 * Create the car into the scene
 * pass the animation path to the car and set the car speed
 */
void loopAnimPath(osg::AnimationPath* ap, raaAnimationPointFinders apfs, int num)
{
	float fVehicleSpeed = 40.0f;
	double dTime = 0.0f;
	for (int i = 0; i < apfs.size(); i++)
	{
		int b = i + 1;
		if (i < apfs.size() - 1)
		{

			double fDisplacement = (apfs[b].translation() - apfs[i].translation()).length();
			ap->insert(dTime, osg::AnimationPath::ControlPoint(apfs[i].translation(), apfs[i].rotation()));
			dTime += fDisplacement / fVehicleSpeed;
		}
	}
	if(num = 0 )
	{
		raaCarFacarde* pCar = new raaCarFacarde(g_pRoot, raaAssetLibrary::getNamedAsset("vehicle", "car0"), ap, fVehicleSpeed);
		g_pRoot->addChild(pCar->root());
	}
	if(num = 1)
	{
		raaCarFacarde* pCar2 = new raaCarFacarde(g_pRoot, raaAssetLibrary::getNamedAsset("vehicle", "car1"), ap, fVehicleSpeed);
		g_pRoot->addChild(pCar2->root());
	}
}

/*
 * Method that pushbacks the animation points and class other method
 */
void carPathOne(osg::Group* group)
{
	raaAnimationPointFinders apfs;
	apfs.push_back((raaAnimationPointFinder("tile7", 2, group)));
	apfs.push_back((raaAnimationPointFinder("tile7", 0, group)));
	apfs.push_back((raaAnimationPointFinder("tile11", 0, group)));
	apfs.push_back((raaAnimationPointFinder("tile11", 10, group)));
	apfs.push_back((raaAnimationPointFinder("tile12", 2, group)));
	apfs.push_back((raaAnimationPointFinder("tile12", 0, group)));
	apfs.push_back((raaAnimationPointFinder("tile13", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile13", 4, group)));
	apfs.push_back((raaAnimationPointFinder("tile13", 5, group)));
	apfs.push_back((raaAnimationPointFinder("tile14", 2, group)));
	apfs.push_back((raaAnimationPointFinder("tile14", 0, group)));
	apfs.push_back((raaAnimationPointFinder("tile15", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile15", 4, group)));
	apfs.push_back((raaAnimationPointFinder("tile15", 5, group)));
	apfs.push_back((raaAnimationPointFinder("tile16", 1, group)));
	apfs.push_back((raaAnimationPointFinder("tile16", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile17", 5, group)));
	apfs.push_back((raaAnimationPointFinder("tile17", 4, group)));
	apfs.push_back((raaAnimationPointFinder("tile5", 1, group)));
	apfs.push_back((raaAnimationPointFinder("tile5", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile1", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile1", 4, group)));
	apfs.push_back((raaAnimationPointFinder("tile1", 5, group)));
	apfs.push_back((raaAnimationPointFinder("tile0", 1, group)));
	apfs.push_back((raaAnimationPointFinder("tile0", 3, group)));
	apfs.push_back((raaAnimationPointFinder("tile3", 5, group)));
	apfs.push_back((raaAnimationPointFinder("tile3", 6, group)));
	apfs.push_back((raaAnimationPointFinder("tile3", 7, group)));
	apfs.push_back((raaAnimationPointFinder("tile7", 2, group)));

	osg::AnimationPath* ap = new osg::AnimationPath();
	int num = 0;
	loopAnimPath(ap, apfs, num);

}
/*
 * Method that pushbacks the animation points and class other method
 */
void carPathTwo(osg::Group* group)
{
	raaAnimationPointFinders apfs2;
	apfs2.push_back((raaAnimationPointFinder("tile7", 1, group)));
	apfs2.push_back((raaAnimationPointFinder("tile7", 3, group)));
	apfs2.push_back((raaAnimationPointFinder("tile3", 0, group)));
	apfs2.push_back((raaAnimationPointFinder("tile3", 1, group)));
	apfs2.push_back((raaAnimationPointFinder("tile3", 2, group)));
	apfs2.push_back((raaAnimationPointFinder("tile0", 2, group)));
	apfs2.push_back((raaAnimationPointFinder("tile0", 0, group)));
	apfs2.push_back((raaAnimationPointFinder("tile1", 0, group)));
	apfs2.push_back((raaAnimationPointFinder("tile1", 1, group)));
	apfs2.push_back((raaAnimationPointFinder("tile1", 2, group)));
	apfs2.push_back((raaAnimationPointFinder("tile5", 2, group)));
	apfs2.push_back((raaAnimationPointFinder("tile5", 0, group)));
	apfs2.push_back((raaAnimationPointFinder("tile17", 8, group)));
	apfs2.push_back((raaAnimationPointFinder("tile17", 9, group)));
	apfs2.push_back((raaAnimationPointFinder("tile17", 7, group)));
	apfs2.push_back((raaAnimationPointFinder("tile9", 1, group)));
	apfs2.push_back((raaAnimationPointFinder("tile9", 3, group)));
	apfs2.push_back((raaAnimationPointFinder("tile11", 8, group)));
	apfs2.push_back((raaAnimationPointFinder("tile11", 9, group)));
	apfs2.push_back((raaAnimationPointFinder("tile11", 7, group)));
	apfs2.push_back((raaAnimationPointFinder("tile7", 1, group)));
	
	osg::AnimationPath* ap2 = new osg::AnimationPath();
	int num = 1;
	loopAnimPath(ap2, apfs2, num);
	
}


int main(int argc, char** argv)
{
	raaAssetLibrary::start();
	raaTrafficSystem::start();
	
	osgViewer::Viewer viewer;

	for(int i=0;i<argc;i++)
	{
		if (std::string(argv[i]) == "-d") g_sDataPath = argv[++i];
	}
	
	// the root of the scene - use for rendering
	g_pRoot = new osg::Group();
	g_pRoot->ref();

	// build asset library - instances or clones of parts can be created from this
	raaAssetLibrary::loadAsset("roadStraight", g_sDataPath+"roadStraight.osgb");
	raaAssetLibrary::loadAsset("roadCurve", g_sDataPath + "roadCurve.osgb");
	raaAssetLibrary::loadAsset("roadTJunction", g_sDataPath + "roadTJunction.osgb");
	raaAssetLibrary::loadAsset("roadXJunction", g_sDataPath + "roadXJunction.osgb");
	raaAssetLibrary::loadAsset("trafficLight", g_sDataPath + "raaTrafficLight.OSGB");
	raaAssetLibrary::loadAsset("fountain", g_sDataPath + "fountain.osgt");
	raaAssetLibrary::insertAsset("vehicle", buildAnimatedVehicleAsset());

	// add a group node to the scene to hold the road sub-tree
	osg::Group* pRoadGroup = new osg::Group();
	g_pRoot->addChild(pRoadGroup);
	
	//Add the road tiles to the scene, pass coordinate variables as the well the root group
	addRoadTile("roadStraight", "tile0", 0, 0, 0.0f, pRoadGroup);
	addRoadTile("roadCurve", "tile1", 1, 0, 90.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile2", -2, 0, 0, pRoadGroup);
	addRoadTile("roadTJunction", "tile3", -1, 0, 270.0f, pRoadGroup);
	addRoadTile("roadCurve", "tile4", -3, 0, 0, pRoadGroup);	
	addRoadTile("roadStraight", "tile5", 1, 1, 90.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile6", -3, 1, 90.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile7", -1, 1, 90.0f, pRoadGroup);
	addRoadTile("roadCurve", "tile8", -3, 2, 270, pRoadGroup);
	addRoadTile("roadStraight", "tile9", 0, 2, 0.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile10", -2, 2, 0, pRoadGroup);
	addRoadTile("roadStraight", "tile12", -1, 3, 90.0f, pRoadGroup);
	addRoadTile("roadCurve", "tile13", -1, 4, 270.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile14", 0, 4, 0.0f, pRoadGroup);
	addRoadTile("roadCurve", "tile15", 1, 4, 180.0f, pRoadGroup);
	addRoadTile("roadStraight", "tile16", 1, 3, 90.0f, pRoadGroup);
	addRoadTile("roadTJunction", "tile17", 1, 2, 0.0f, pRoadGroup);
	addRoadTile("roadXJunction", "tile11", -1, 2, 90.0f, pRoadGroup);

	//Create some scene enhancement, fountain was one of them
	osg::Node* fountain = osgDB::readNodeFile("../../Data/fountain.osgt");
	osg::MatrixTransform* matrix = new osg::MatrixTransform(osg::Matrix::translate(0.0f, 700.0f, 2.0f) * osg::Matrix::scale(0.7f, 0.7f, 0.7f));
	matrix->addChild(fountain);
	g_pRoot->addChild(matrix);

	//Add some traffic lights to the scene
	osg::Group* pTrafficLight = new osg::Group();
	
	addTrafficLight("trafficLight", "trafficLight1", 300.0f, 700.0f, 0.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight2", 200.0f, 1100.0f, 270.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight3", 650.0f, 1200.0f, 180.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight4", -200.0f, -180.0f, 90.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight5", -320.0f, 300.0f, 180.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight6", -750.0f, 180.0f, 270.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight7", -200.0f, 800.0f, 90.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight8", -650.0f, 680.0f, 0.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight9", -750.0f, 1100.0f, 270.0f, pTrafficLight);
	addTrafficLight("trafficLight", "trafficLight10", -300.0f, 1250.0f, 180.0f, pTrafficLight);

	//Pushback all the animation points as well as insert them in to the AnimationPoint
	carPathOne(pRoadGroup);
	carPathTwo(pRoadGroup);
	
	g_pRoot->addChild(pTrafficLight);

	addCollisionTarget();

	// osg setup stuff
	osg::GraphicsContext::Traits* pTraits = new osg::GraphicsContext::Traits();
	pTraits->x = 20;
	pTraits->y = 20;
	pTraits->width = 600;
	pTraits->height = 480;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = 0;

	osg::GraphicsContext* pGC = osg::GraphicsContext::createGraphicsContext(pTraits);
	osgGA::KeySwitchMatrixManipulator* pKeyswitchManipulator = new osgGA::KeySwitchMatrixManipulator();
	pKeyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
	pKeyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
	pKeyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
	viewer.setCameraManipulator(pKeyswitchManipulator);
	osg::Camera* pCamera = viewer.getCamera();
	pCamera->setGraphicsContext(pGC);
	pCamera->setViewport(new osg::Viewport(0, 0, pTraits->width, pTraits->height));

	// add own event handler - thish currently switches on an off the animation points
	viewer.addEventHandler(new raaInputController(g_pRoot));

	// add the state manipulator
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	viewer.addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	viewer.addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer.addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

	// set the scene to render
	viewer.setSceneData(g_pRoot);

	viewer.realize();

	return viewer.run();
}