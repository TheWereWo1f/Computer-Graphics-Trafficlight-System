#pragma once
#include <list>
#include <osg/Callback>

class vsTrafficLightFacarde;
class raaCarFacarde;

typedef std::list<vsTrafficLightFacarde*>raaLights;
typedef std::list<raaCarFacarde*>raaVehicles;

namespace osg {
	class NodeVisitor;
	class Node;
}

class vsJunctionControl : public osg::NodeCallback
{
public:
	vsJunctionControl();
	virtual ~vsJunctionControl();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	int nextLight();

	static void addVehicle(raaCarFacarde* pVehicle);
	void addLightUnit(vsTrafficLightFacarde* pLight);


protected:
	static raaVehicles sm_lVehicles;
	raaLights m_lLights;
	int counter;
	int currentTrafficLight;
};

