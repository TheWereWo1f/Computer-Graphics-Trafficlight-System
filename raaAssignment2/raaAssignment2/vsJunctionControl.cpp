#include "vsJunctionControl.h"
#include "vsTrafficLightFacarde.h"
#include "raaAnimatedFacarde.h"
#include "vsJunctionControl.h"
#include "raaCarFacarde.h"
#include <iostream>

raaVehicles vsJunctionControl::sm_lVehicles;

vsJunctionControl::vsJunctionControl()
{
	counter = 0;
	currentTrafficLight = 0;
}

vsJunctionControl::~vsJunctionControl()
{
}

void vsJunctionControl::addVehicle(raaCarFacarde* pVehicle)
{
	if (pVehicle && std::find(sm_lVehicles.begin(), sm_lVehicles.end(), pVehicle) == sm_lVehicles.end())
		sm_lVehicles.push_back(pVehicle);
}

void vsJunctionControl::addLightUnit(vsTrafficLightFacarde* pLight)
{
	if (pLight && std::find(m_lLights.begin(), m_lLights.end(), pLight) == m_lLights.end())
	{
		m_lLights.push_back(pLight);
	}
}

void vsJunctionControl::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (counter == 100)
	{
		int i = 0;
		for (raaLights::iterator light = m_lLights.begin(); light != m_lLights.end(), i < 4; light++, i++)
		{
			if (currentTrafficLight == i)
			{
				if ((*light)->trafficLightstatus != 0)
				{
					//red
					(*light)->RedAndAmberOn();
				}
				else if ((*light)->trafficLightstatus == 1)
				{
					//amber
					(*light)->RedLightOn();
					nextLight();
				}
				else if ((*light)->trafficLightstatus == 2)
				{
					//red&amber
					(*light)->GreenLightOn();
				}
				else if ((*light)->trafficLightstatus == 3)
				{
					//green
					(*light)->AmberLightOn();
				}
			}

		}
		counter = 0;
	}

	if (sm_lVehicles.size())
	{
		for (raaLights::iterator light = m_lLights.begin(); light != m_lLights.end(); light++)
		{
			osg::Matrixf mWorldToZone = osg::computeWorldToLocal((*light)->collisionDetection()->getParentalNodePaths()[0]);
			osg::BoundingSphere b = (*light)->collisionDetection()->getBound();

			unsigned int uiCount = 0;
			for (raaVehicles::iterator it = sm_lVehicles.begin(); it != sm_lVehicles.end(); it++)
			{
				osg::Vec3f vPos(0.0f, 0.0f, 0.0f);
				//multiply each of the 8 corners to turn to world coords
				//one element that takes a position 0 0 0 and for the ball get parentalnodepath
				//calculate the balltoworld matrix
				//whatever the local position of the ball in ints own hierarchy, calculate matrix between root and ball, including whatever cuurrent state of animation callback is, and generate into matrix
				//vpos worldcoords to local coords to test ( worldTozone)
				osg::Matrixf mBallToWorld = osg::computeLocalToWorld((*it)->root()->getParentalNodePaths()[0]);
				vPos = vPos * mBallToWorld;
				vPos = vPos * mWorldToZone;

				//if that bounding sphere contains local tile position of ball
				//if it does, set atribute modes
				if (b.contains(vPos))
				{
					//turn to green if contains ball
					//count number of balls

					//m_pBoxMT->getOrCreateStateSet()->setAttributeAndModes(m_pOnMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
					uiCount++;
					if ((*light)->trafficLightstatus == 3)
					{
						//green
						(*it)->setPause(false);
					}
					else
					{
						(*it)->setPause(true);
					}
				}
			}
			//if it has ball in collider box, turn green, if not go red
			//if count is still 0, turn back to red
			//if (!uiCount) m_pBoxMT->getOrCreateStateSet()->setAttributeAndModes(m_pOffMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

		}
	}
	counter++;
}

int vsJunctionControl::nextLight()
{
	currentTrafficLight++;
	if (currentTrafficLight > 3)
	{
		currentTrafficLight = 0;
	}
	return currentTrafficLight;
}
