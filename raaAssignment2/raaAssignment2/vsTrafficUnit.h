#pragma once
#include <list>
#include <osg/MatrixTransform>

typedef std::list<class vsTrafficUnit*> vsTrafficUnits;

class vsTrafficUnit
{
public:
	
	osg::MatrixTransform* translation();
	osg::MatrixTransform* rotation();
	osg::MatrixTransform* scale();

protected:
	void innit();

	osg::MatrixTransform* m_pTranslation;
	osg::MatrixTransform* m_pRotation;
	osg::MatrixTransform* m_pScale;
	osg::Node* m_pRoot;
	static vsTrafficUnits sm_lFacardes;
};

