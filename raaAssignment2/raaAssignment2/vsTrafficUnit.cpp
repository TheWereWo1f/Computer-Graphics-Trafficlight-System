#include "vsTrafficUnit.h"
#include "raaFinder.h"

void vsTrafficUnit::innit()
{
	if(m_pRoot)
	{
		m_pRoot->ref();
		raaFinder<osg::MatrixTransform> t("Translation", m_pRoot);
		m_pTranslation = t.node();
		raaFinder<osg::MatrixTransform> r("Rotation", m_pRoot);
		m_pRotation = r.node();
		raaFinder<osg::MatrixTransform> s("Scale", m_pRoot);
		m_pScale = s.node();
	}
}
