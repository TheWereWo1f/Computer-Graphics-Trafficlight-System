#include <windows.h>
#include <osg/Geode>
#include <osg/Material>
#include <osgDB/ReadFile>
#include "vsTrafficLightFacarde.h"
#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
#include "raaBoundCalculator.h"
#include "raaFinder.h"

vsTrafficLight vsTrafficLightFacarde::sm_mTrafficLights;
vsTrafficLightFacardeStringMap vsTrafficLightFacarde::sm_mTrafficLight;

vsTrafficLightFacarde::vsTrafficLightFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) : raaNodeCallbackFacarde(pPart, vTrans, fRot, fScale)
{
	if (pPart) sm_mTrafficLight[pPart->getName()] = this;
}
/*
void vsTrafficLightFacarde::RedLightOn()
{
	trafficLightstatus = 0;
	m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOn, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void vsTrafficLightFacarde::AmberLightOn()
{
	trafficLightstatus = 1;
	m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOn, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void vsTrafficLightFacarde::RedAndAmberOn()
{
	trafficLightstatus = 2;
	m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOn, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOn, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void vsTrafficLightFacarde::GreenLightOn()
{
	trafficLightstatus = 3;
	m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOff, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOn, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}
*/

