#define MONTE_CARLO

#pragma once
#include "grrenderer.h"
#include "GL/gl.h"
#include "GrTransform.h" 
#include "RayIntersection.h"
#include "jitter.h"
#include <list>

#ifdef MONTE_CARLO
#include <vector>
#include "GrPoint.h"
class RandomNumberGenerator;
#endif

using namespace std;

#ifdef MONTE_CARLO
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#endif

class CMyRaytraceRenderer :
	public CGrRenderer
{
public:
	CMyRaytraceRenderer(void);
	~CMyRaytraceRenderer(void);
	void SetDimensions(int height, int width, GLbyte  **rayimage);
	void SetWindow(CWnd *p_window);
	bool RendererStart();
	void RendererMaterial(CGrMaterial *p_material);
	void RendererEndPolygon();
	bool RendererEnd();
	void RendererPushMatrix();
  void RendererPopMatrix();
  void RendererRotate(double a, double x, double y, double z);
  void RendererTranslate(double x, double y, double z);
	GLbyte* ReturnColor(CRay ray);
	void SetIterations( int iter ) { m_iterations = iter; }
	void GetReflect(CRay Refray, CRayIntersection::Object *nearest, CGrPoint &newcolor);

#ifdef MONTE_CARLO
  virtual void AddLight(double x, double y, double z, double w, 
                const float *ambient, const float *diffuse, const float *specular);
  virtual void AddLight(const CGrPoint &p_loc, 
                const float *p_ambient, const float *p_diffuse, const float *p_specular);
  virtual void AddLight(double x, double y, double z, double w, 
                const float *p_ambient, const float *p_diffuse, const float *p_specular,
                const CGrPoint &p_orient, double p_radius=1.0, double p_sectionAngle=(M_PI*2.0),
                bool p_directional=false);
  virtual void AddLight(const CGrPoint &p_loc, 
                const float *p_ambient, const float *p_diffuse, const float *p_specular,
                const CGrPoint &p_orient, double p_radius=1.0, double p_sectionAngle=(M_PI*2.0),
                bool p_directional=false);

  void SetMonteCarloRays(unsigned int mcRays) { m_mcRays = mcRays; return; }
#endif

private:
	int m_rayimagewidth; 
	int m_rayimageheight; 
	GLbyte **m_rayimage; 
	CWnd    *m_window; 
	CRayIntersection m_intersection;
	std::list<CGrTransform>    m_mstack; 
	CGrMaterial *m_material;
	int m_iterations;

#ifdef MONTE_CARLO
  RandomNumberGenerator *d_rng;

  struct SphereLight : public Light {
    CGrPoint m_orientation;
    double   m_radius;
    double   m_sectionAngle;
    bool     m_directional;
  };

  // Assuming planeNormal is normalized
  void GetRandomCoordSystem(CGrPoint &right, CGrPoint &up, const CGrPoint &pointOnPlane,
                            const CGrPoint &planeNormal) const;
  // Assuming right and up are normalized
  void GetRandomPointOnDisc(CGrPoint &p, const CGrPoint &center, const CGrPoint &right,
                            const CGrPoint &up, double radius) const;
  // Assuming discFacing is normalized
  void ApproximateLightDirection(CGrPoint &lDir, const CGrPoint &lightCenter,
                                 const CGrPoint &discPoint, const CGrPoint &discFacing,
                                 double radius) const;
  // Assuming surface normal is normalized
  double GetSingleLightRayContribution(double lightMagnitude,
                                       const CGrPoint &lightDirection,
                                       const CGrPoint &lightPoint,
                                       const CRayIntersection::Object *nearest) const;
  void GetMonteCarloDiffuse(vector<double> &contributions,
                            vector<CGrPoint> &randomLightDirections,
                            const SphereLight &light,
                            const CGrPoint &surfacePoint,
                            const CRayIntersection::Object *,
                            unsigned int mcRays) const;

  std::vector<SphereLight> m_lights;
  unsigned int m_mcRays;
#else
  std::vector<Light> m_lights;
#endif
};
