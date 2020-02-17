#ifndef _SPHEREFACTORY_H
#define _SPHEREFACTORY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrObject.h"
#include "GrTexture.h"	   // Added by ClassView

class CShapeFactory
{
public:
	CShapeFactory(void);
	~CShapeFactory(void);

	void Sphere(double p_radius, int partial, CGrTexture *tex=NULL );
	void RingofSpheres(double s_radius, double r_radius, int partial, CGrTexture *tex=NULL);
	void SphereFace(CGrPtr<CGrComposite> &p_sphere, int p_recurse, double p_radius, GLdouble *a, 
							   GLdouble *b, GLdouble *c, CGrTexture *tex);
	void RingofCylinders(double b_radius, double t_radius, double r_radius, double angle, int num,
		CGrTexture *tex1=NULL, CGrTexture *tex2=NULL);
	void CreateCylinder(double brad, double trad, double height, bool ends=true,
								   CGrTexture *tex1=NULL, CGrTexture *tex2=NULL);
	void CreateHollowCylinder(double brad, double trad, double bradin, double tradin, 
								double height, CGrTexture *tex1=NULL, CGrTexture *tex2=NULL);	
	CGrPtr<CGrObject> m_sphere;
	 CGrObject *SceneGraph() {return m_sphere;}
};



#endif