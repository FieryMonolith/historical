#include "StdAfx.h"
#include "SphereFactory.h"

CSphereFactory::CSphereFactory(void)
{
}

CSphereFactory::~CSphereFactory(void)
{
}

void CSphereFactory::Sphere(double p_radius, int partial, CGrTexture *tex)
{
	CGrPtr<CGrComposite> sphere = new CGrComposite;
	m_sphere = sphere;

	GLdouble a[] = {1, 0, 0};
	GLdouble b[] = {0, 0, -1};
	GLdouble c[] = {-1, 0, 0};
	GLdouble d[] = {0, 0, 1};
	GLdouble e[] = {0, 1, 0};
	GLdouble f[] = {0, -1, 0};

	int recurse = 4;

	SphereFace(sphere, recurse, p_radius, d, a, e, tex);
	SphereFace(sphere, recurse, p_radius, a, b, e, tex);

	if(partial < 2)
	{
		SphereFace(sphere, recurse, p_radius, b, c, e, tex);
		SphereFace(sphere, recurse, p_radius, c, d, e, tex);
	}
	if (partial < 1)
	{
		SphereFace(sphere, recurse, p_radius, a, d, f, tex);
		SphereFace(sphere, recurse, p_radius, b, a, f, tex);
		SphereFace(sphere, recurse, p_radius, c, b, f, tex);
		SphereFace(sphere, recurse, p_radius, d, c, f, tex);
	}
}
inline void Normalize3(GLdouble *v)
{
	GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void CSphereFactory::RingofSpheres(double s_radius, double r_radius, int partial, CGrTexture *tex)
{
	CGrPtr<CGrComposite> p_sphere = new CGrComposite;
	m_sphere = p_sphere;

	int num_spheres = (2*3.14159*r_radius)/(2*s_radius);
	if(num_spheres == 1)
		r_radius = 0;
	double angle_step = 2*3.14159/double(num_spheres);

	//int i=0;
	for (int i=0; i<num_spheres; i++)
	{
		CSphereFactory sphere;
		sphere.Sphere( s_radius, partial, tex);
		CGrPtr<CGrObject> berry = sphere.SceneGraph();
		p_sphere->Child(new CGrTranslate(r_radius*cos(i*angle_step), 0., -r_radius*sin(i*angle_step), berry));
	}
}

void CSphereFactory::SphereFace(CGrPtr<CGrComposite> &p_sphere, int p_recurse, double p_radius, GLdouble *a, 
								GLdouble *b, GLdouble *c, CGrTexture *tex)
{
	if(p_recurse > 1)
	{
		// Compute vectors halfway between the passed vectors 
		GLdouble d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
		GLdouble e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
		GLdouble f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};

		Normalize3(d);
		Normalize3(e);
		Normalize3(f);

		SphereFace(p_sphere, p_recurse-1, p_radius, a, d, f, tex);
		SphereFace(p_sphere, p_recurse-1, p_radius, d, b, e, tex);
		SphereFace(p_sphere, p_recurse-1, p_radius, f, e, c, tex);
		SphereFace(p_sphere, p_recurse-1, p_radius, f, d, e, tex);
	}

	CGrPtr<CGrPolygon> poly = new CGrPolygon;
	if(tex)
		poly->Texture(tex);

	double tx1 = atan2(a[0], a[2]) / (2. * 3.14159) + .5;
	double ty1 = asin(a[1]) / 3.14159 + .5;

	poly->AddNormal3dv(a);
	poly->AddTex2d(tx1, ty1);
	poly->AddVertex3d(a[0] * p_radius, a[1] * p_radius, a[2] * p_radius);

	double tx = atan2(b[0], b[2]) / (2. * 3.14159) + .5;
	double ty = asin(b[1]) / 3.14159 + .5;
	if (tx < .75 && tx1 > .75)
		tx += 1.;
	else if(tx > .75 && tx1 < .75)
		tx -= 1.;

	poly->AddNormal3dv(b);
	poly->AddTex2d(tx, ty);
	poly->AddVertex3d(b[0] * p_radius, b[1] * p_radius, b[2] * p_radius);

	tx = atan2(c[0], c[2]) / (2. * 3.14159) + .5;
	ty = asin(c[1]) / 3.14159 + .5;
	if (tx < .75 && tx1 > .75)
		tx += 1.;
	else if(tx > .75 && tx1 < .75)
		tx -= 1.;

	poly->AddNormal3dv(c);
	poly->AddTex2d(tx, ty);
	poly->AddVertex3d(c[0] * p_radius, c[1] * p_radius, c[2] * p_radius);

	p_sphere->Child(poly);	
}

void CSphereFactory::RingofCylinders(double b_radius, double t_radius, double r_radius, double angle, int num, CGrTexture *tex1, CGrTexture *tex2)
{
	CGrPtr<CGrComposite> p_cyl = new CGrComposite;
	m_sphere = p_cyl;

	if(num == 1)
		r_radius = 0;
	double angle_step = (2*3.14159)/(double(num));

	int i=0;
	for (int i=0; i<num; i++)
	{
		CSphereFactory cyl;
		cyl.CreateCylinder(b_radius, t_radius, r_radius, tex1, tex2);
		CGrPtr<CGrObject> branch = cyl.SceneGraph();
		p_cyl->Child( new CGrRotate(-90+angle, r_radius*cos(i*angle_step), 0., -r_radius*sin(i*angle_step), branch));
	}
}	

void CSphereFactory::CreateCylinder(double brad, double trad, double height, bool ends,
									CGrTexture *tex1, CGrTexture *tex2)
{

	CGrPtr<CGrComposite> cyl = new CGrComposite;
	m_sphere = cyl;

	CGrPtr<CGrPolygon> top;
	top = new CGrPolygon;
	cyl->Child(top);

	CGrPtr<CGrPolygon> bottom;
	bottom = new CGrPolygon;
	cyl->Child(bottom);

	for (int i=0; i<100; i++)
	{
		CGrPtr<CGrPolygon> poly;
		poly = new CGrPolygon;
		cyl->Child(poly);

		double theta1 = 6.32317*(double(i)/100.);
		double theta2 = 6.32317*(double(i+1)/100.);

		poly->Texture(tex1);
		poly->AddTexVertex3d(brad*cos(theta1), 0, -brad*sin(theta1), double(i)/99., 0.);
		poly->AddTexVertex3d(brad*cos(theta2), 0, -brad*sin(theta2), double(i+1)/99., 0.);
		poly->AddTexVertex3d(trad*cos(theta2), height, -trad*sin(theta2), double(i+1)/99., 1.);
		poly->AddTexVertex3d(trad*cos(theta1), height, -trad*sin(theta1), double(i)/99., 1.);
		poly->ComputeNormal();

		if(ends)
		{
			top->Texture(tex2);
			bottom->Texture(tex2);
			top->AddTexVertex3d(trad*cos(theta1), height, -trad*sin(theta1), .5*cos(theta1)+.5, -.5*sin(theta1)+.5 );
			bottom->AddTexVertex3d(brad*cos(6.32317-theta1), 0, -brad*sin(6.32317-theta1), .5*cos(6.32317-theta1)+.5, -.5*sin(6.32317-theta1)+.5 );
		}
	}
	if (ends)
	{
		top->ComputeNormal();
		bottom->ComputeNormal();
	}

}

void CSphereFactory::CreateHollowCylinder(double brad, double trad, 
										  double bradin, double tradin, double height, 
										  CGrTexture *tex1, CGrTexture *tex2)	
{
	CGrPtr<CGrComposite> cyl = new CGrComposite;
	m_sphere = cyl;

	for (int i=0; i<100; i++)
	{
		CGrPtr<CGrPolygon> poly;
		poly = new CGrPolygon;
		cyl->Child(poly);

		CGrPtr<CGrPolygon> poly2;
		poly2 = new CGrPolygon;
		cyl->Child(poly2);

		double theta1 = 6.32317*(double(i)/100.);
		double theta2 = 6.32317*(double(i+1)/100.);

		poly2->Texture(tex1);
		poly2->AddTexVertex3d(brad*cos(theta1), 0, -brad*sin(theta1), double(i)/99., 0.);
		poly2->AddTexVertex3d(brad*cos(theta2), 0, -brad*sin(theta2), double(i+1)/99., 0.);
		poly2->AddTexVertex3d(trad*cos(theta2), height, -trad*sin(theta2), double(i+1)/99., 1.);
		poly2->AddTexVertex3d(trad*cos(theta1), height, -trad*sin(theta1), double(i)/99., 1.);
		poly2->ComputeNormal();	

		poly->Texture(tex1);
		poly->AddTexVertex3d(tradin*cos(theta1), height, -tradin*sin(theta1), double(i)/99., 1.);
		poly->AddTexVertex3d(tradin*cos(theta2), height, -tradin*sin(theta2), double(i+1)/99., 1.);
		poly->AddTexVertex3d(bradin*cos(theta2), 0, -bradin*sin(theta2), double(i+1)/99., 0.);
		poly->AddTexVertex3d(bradin*cos(theta1), 0, -bradin*sin(theta1), double(i)/99., 0.);
		poly->ComputeNormal();
	}
	
	CSphereFactory top;
	top.CreateCylinder(trad, tradin, 0., false);
	CGrPtr<CGrObject> tops = top.SceneGraph();
	cyl->Child(new CGrTranslate(0., height, 0., tops ));

	CSphereFactory bottom;
	bottom.CreateCylinder(bradin, brad, 0., false);
	CGrPtr<CGrObject> bottoms = bottom.SceneGraph();
	cyl->Child(bottoms);

}
		


