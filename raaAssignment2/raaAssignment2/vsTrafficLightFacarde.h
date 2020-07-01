#pragma once
#include <osg/Material>
#include <osg/MatrixTransform>

#include "raaNodeCallbackFacarde.h"

typedef std::map<std::string, class vsTrafficLightFacarde*> vsTrafficLightFacardeStringMap;
typedef std::list<class vsTrafficLightFacarde*>vsTrafficLight;
using namespace std;

class vsTrafficLightFacarde : public raaNodeCallbackFacarde
{
public:
	
	vsTrafficLightFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale);
	static vsTrafficLightFacardeStringMap sm_mTrafficLight;
	static vsTrafficLight sm_mTrafficLights;
	osg::MatrixTransform* tileFinder(std::string sTileName);
	void RedLightOn();
	void AmberLightOn();
	void RedAndAmberOn();
	void GreenLightOn();
	int trafficLightstatus;

	
protected:
	osg::Geode* m_pRed;
	osg::Geode* m_pGreen;
	osg::Geode* m_pAmber;

	static osg::Material* sm_pRedOn;
	static osg::Material* sm_pRedOff;
	static osg::Material* sm_pAmberOn;
	static osg::Material* sm_pAmberOff;
	static osg::Material* sm_pGreenOn;
	static osg::Material* sm_pGreenOff;
};