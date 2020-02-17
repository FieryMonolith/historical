#include "StdAfx.h"
#include "myraytracerenderer.h"

#ifdef MONTE_CARLO
#include "random.h"
#endif

CMyRaytraceRenderer::CMyRaytraceRenderer(void)
{
	 m_window = NULL;
#ifdef MONTE_CARLO
   d_rng = new RandomNumberGenerator;
   assert(d_rng);

   m_mcRays = 1;

   return;
#endif
}

CMyRaytraceRenderer::~CMyRaytraceRenderer(void)
{
#ifdef MONTE_CARLO
  if(d_rng != NULL) {
    delete d_rng;
    d_rng = NULL;
  }
#endif
}

void CMyRaytraceRenderer::SetDimensions(int height, int width, GLbyte  **rayimage)
{
	m_rayimageheight = height;
	m_rayimagewidth = width;
	m_rayimage = rayimage;
}

void CMyRaytraceRenderer::SetWindow(CWnd *p_window)
{
    m_window = p_window;
}

bool CMyRaytraceRenderer::RendererStart()
{
	m_material = NULL;
	m_intersection.Initialize();
	m_mstack.clear();

	// We have to do all of the matrix work ourselves.
	// Set up the matrix stack.
	CGrTransform t;
	t.LookAt(Eye().X(), Eye().Y(), Eye().Z(), 
		Center().X(), Center().Y(), Center().Z(), 
		Up().X(), Up().Y(), Up().Z());

	m_mstack.push_back(t);

	CGrPoint p(0, 0, 0);
	CGrPoint check = m_mstack.back() * p;

  // Now we overload the add light function
#ifndef MONTE_CARLO
	for (int i=0; i<LightCnt(); i++)
	{
		Light light = GetLight(i);

		light.m_pos = m_mstack.back()*light.m_pos;

		// debug
		double x = light.m_pos.X();
		double y = light.m_pos.Y();
		double z = light.m_pos.Z();

		m_lights.push_back(light);
	}
#else
	for (int i=0; i<m_lights.size(); i++)
	{
    double x5 = m_lights[i].m_orientation.X();
    double y5 = m_lights[i].m_orientation.Y();
    double z5 = m_lights[i].m_orientation.Z();
    CGrPoint t(m_lights[i].m_pos + Normalize3(m_lights[i].m_orientation));
    double x = t.X();
    double y = t.Y();
    double z = t.Z();
    t = m_mstack.back()*t;
    double x1 = t.X();
    double y1 = t.Y();
    double z1 = t.Z();

    double x2 = m_lights[i].m_pos.X();
    double y2 = m_lights[i].m_pos.Y();
    double z2 = m_lights[i].m_pos.Z();
    m_lights[i].m_pos = m_mstack.back()*m_lights[i].m_pos;
    double x3 = m_lights[i].m_pos.X();
    double y3 = m_lights[i].m_pos.Y();
    double z3 = m_lights[i].m_pos.Z();

    m_lights[i].m_orientation = (t - m_lights[i].m_pos);
    //m_lights[i].m_orientation = m_mstack.back()*m_lights[i].m_orientation;
    double x4 = m_lights[i].m_orientation.X();
    double y4 = m_lights[i].m_orientation.Y();
    double z4 = m_lights[i].m_orientation.Z();
    m_lights[i].m_orientation.Normalize3();
    double x6 = m_lights[i].m_orientation.X();
    double y6 = m_lights[i].m_orientation.Y();
    double z6 = m_lights[i].m_orientation.Z();
  }
#endif

	return true;
}

#ifdef MONTE_CARLO
//
// Name :         CMyRaytraceRenderer::AddLight()
// Description :  Add a light to the list of lights.
//
void CMyRaytraceRenderer::AddLight(const CGrPoint &p_loc, const float *p_ambient, 
                                   const float *p_diffuse, const float *p_specular)
{
  AddLight(p_loc, p_ambient, p_diffuse, p_specular, CGrPoint(0.0, -1.0, 0.0, 0.0));
}

//
// Name :         CMyRaytraceRenderer::AddLight()
// Description :  Alternative versions
//
void CMyRaytraceRenderer::AddLight(double x, double y, double z, double w, const float *p_ambient, 
                                   const float *p_diffuse, const float *p_specular)
{
   AddLight(CGrPoint(x, y, z, w), p_ambient, p_diffuse, p_specular);
}

void CMyRaytraceRenderer::AddLight(double x, double y, double z, double w,
                                   const float *p_ambient, const float *p_diffuse,
                                   const float *p_specular, const CGrPoint &p_orient,
                                   double p_radius, double p_sectionAngle, bool p_directional)
{
   AddLight(CGrPoint(x, y, z, w), p_ambient, p_diffuse, p_specular, p_orient, p_radius, p_sectionAngle,
            p_directional);
}

void CMyRaytraceRenderer::AddLight(const CGrPoint &p_loc,
                                   const float *p_ambient, const float *p_diffuse,
                                   const float *p_specular, const CGrPoint &p_orient,
                                   double p_radius, double p_sectionAngle, bool p_directional)
{
   SphereLight light;
   light.m_pos = p_loc;

   for(int i=0;  i<4;  i++)
   {
      if(p_ambient)
         light.m_ambient[i] = p_ambient[i];
      if(p_diffuse)
         light.m_diffuse[i] = p_diffuse[i];
      if(p_specular)
         light.m_specular[i] = p_specular[i];
   }

   light.m_radius = p_radius;
   light.m_directional = p_directional;
   light.m_sectionAngle = p_sectionAngle;
   light.m_orientation = p_orient;

   m_lights.push_back(light);
}
#endif

void CMyRaytraceRenderer::RendererMaterial(CGrMaterial *p_material)
{
   m_material = p_material;
}

//
// Name : CMyRaytraceRenderer::RendererEndPolygon()
// Description : End definition of a polygon. The superclass has
// already collected the polygon information
//

void CMyRaytraceRenderer::RendererEndPolygon()
{
   const list<CGrPoint> &vertices = PolyVertices();
   const list<CGrPoint> &normals = PolyNormals();
   const list<CGrPoint> &tvertices = PolyTexVertices();

   // Allocate a new polygon in the ray intersection system
   m_intersection.PolygonBegin();
   m_intersection.Material(m_material);

   if(PolyTexture())
   { 
      m_intersection.Texture(PolyTexture());
   }

   list<CGrPoint>::const_iterator normal = normals.begin();
   list<CGrPoint>::const_iterator tvertex = tvertices.begin();

   for(list<CGrPoint>::const_iterator i=vertices.begin(); i!=vertices.end(); i++)
   {
      if(normal != normals.end())
      {
         m_intersection.Normal(m_mstack.back() * *normal);
         normal++;
	  }

	  if(tvertex != tvertices.end())
	  {
		  m_intersection.TexVertex(*tvertex);
		  tvertex++;
	  }

	  m_intersection.Vertex(m_mstack.back() * *i);

	  //debug
	  CGrPoint l = *i;
	  CGrPoint p = m_mstack.back() * l;


	  double x1 = l.X();
	  double y1 = l.Y();
	  double z1 = l.Z();

	  double x = p.X();
	  double y = p.Y();
	  double z = p.Z();
	  double n = 2;
   }

   m_intersection.PolygonEnd();
}

bool CMyRaytraceRenderer::RendererEnd()
{
	m_intersection.LoadingComplete();

	double yhit = 2*tan(ProjectionAngle()/360. * 3.14159);
	double ymin = -yhit/2.;
	double xmin = ymin*ProjectionAspect();
	double xwid = -2*xmin;

	for (int r=0; r<m_rayimageheight; r++)
  {
		for (int c=0; c<m_rayimagewidth; c++)
		{
			double avghallcolor[3] = {0., 0., 0.};
			double hall[3];

			for (int alias = 0; alias < m_iterations; alias++)
			{
				if(m_iterations > 16) m_iterations = 16;
				while(JITTER[m_iterations] == NULL && m_iterations < 16) m_iterations ++;

				double x = ((c + JITTER[m_iterations][alias].X()) / m_rayimagewidth * xwid + xmin);
				double y = ((r + JITTER[m_iterations][alias].Y()) / m_rayimageheight * yhit + ymin);

				// Construct a Ray
				CRay ray(CGrPoint(0, 0, 0), Normalize3(CGrPoint(x, y, -1, 0)));

				CRayIntersection::Object *nearest;   // Pointer to intersecting object

				double t;                           // Will be distance to intersection
				CGrPoint intersect;                 // Will by x,y,z location of intersection

				if(m_intersection.Intersect(ray, NULL, nearest, t, intersect))
				{
					// We hit something...
					m_rayimage[r][c * 3] = GLbyte(255);
					m_rayimage[r][c * 3 + 1] = GLbyte(255);
					m_rayimage[r][c * 3 + 2] = GLbyte(255);
					// Determine information about the intersection
					CGrPoint N;
					CGrMaterial *material;
					CGrTexture *texture;
					CGrPoint texcoord;

					m_intersection.IntersectInfo(ray, nearest, t, N, material, texture, texcoord);

					if(material != NULL)
					{
						float texcolor[3];

						if (texture)
						{
							double  s = (texture->Height()-1) * fmod(texcoord.Y(), 1.);
							double  t = (texture->Width()-1) * fmod(texcoord.X(), 1.);

							double fs = fmod(s, 1.);
							double ft = fmod(t, 1.);

							for (int p=0; p<3; p++)
							{
								double ct =texture->Row(int(s)+1)[int(t)*3+p] * (1.-ft) + texture->Row(int(s)+1)[(int(t)+1)*3+p]*ft; 
								double cb =texture->Row(int(s))[int(t)*3+p] * (1.-ft) + texture->Row(int(s))[(int(t)+1)*3+p]*ft;

								texcolor[p] = ct * fs + cb * (1.-fs);

							}

						} else {
							texcolor[0] = texcolor[1] = texcolor[2] = 1.f;
            } // end if/else (texture)

						hall[0] = hall[1] = hall[2] = 0.0;

						//diffuse
						//
#ifdef MONTE_CARLO
            vector<double> contributions;
            vector<CGrPoint> randomLightDirections;
            contributions.reserve(m_mcRays);
            randomLightDirections.reserve(m_mcRays);
#endif

            for(int l=0;  l<m_lights.size();  l++) 
						{    
#ifdef MONTE_CARLO
              contributions.clear();
              randomLightDirections.clear();

              SphereLight light = m_lights.at(l);
#else
							Light light = m_lights.at(l);
#endif

							//ambient
							for(int p=0;  p<3;  p++)
							{
								hall[p] += light.m_ambient[p] * material->Ambient(p) * texcolor[p];
							}

							//diffuse
#ifndef MONTE_CARLO
							CGrPoint point1 = light.m_pos - intersect;
							CRay lightray(intersect, Normalize3(point1));

							double dist;
							CGrPoint intersectLight;
							CRayIntersection::Object *lightnearest;   // Pointer to intersecting object

							if (m_intersection.Intersect( lightray, nearest, lightnearest, dist, intersectLight ))
							{
								double lightdist = sqrt( point1.X()*point1.X() + point1.Y()*point1.Y() + point1.Z()*point1.Z() );
                if(lightdist > dist) {
									continue;
                }
							}

							CGrPoint lightdirection;

              //????????  Shouldn't this be point1?
              if(light.m_pos.Z() == 0) {
								lightdirection = light.m_pos;	
              } else {
								lightdirection = (light.m_pos - intersect);
              }
							lightdirection.Normalize3();

              double lAngle = Dot3(N, lightdirection);
              if(lAngle<0) { continue; }
#else
              GetMonteCarloDiffuse(contributions, randomLightDirections,
                                   light, intersect, nearest, m_mcRays);

              double lAngle = 0.0;
              for(unsigned int randomLights=0; randomLights < m_mcRays; randomLights++) {
                double contribution = contributions[randomLights];

                double cAngle = Dot3(N, randomLightDirections[randomLights]);
                if(cAngle < 0) {
                  contributions[randomLights] = 0.0;
                  cAngle = 0.0;
                }

                lAngle += contribution * cAngle;
              }

              if(m_mcRays > 0) {
                lAngle /= m_mcRays;
              }
#endif
              // Compute diffuse contribution
							for(int p=0;  p<3;  p++)
							{
								double  newlight = light.m_diffuse[p] * material->Diffuse(p) * lAngle * texcolor[p];
								hall[p] += newlight;
							}

							//specular
							//
#ifndef MONTE_CARLO
							CGrPoint half = lightdirection + intersect*-1;
							half.Normalize3();
							
							double sif;
              if( material->Shininess() > 1) {
								 sif = pow(Dot3(N, half), (double)material->Shininess()); 
              } else {
								 sif = pow(Dot3(N, half), 1.0);
              }
#else
              double sif = 0.0;
              for(unsigned int randomLights=0; randomLights < m_mcRays; randomLights++) {
                double contribution = contributions[randomLights];
                if(contribution <= 0) { continue; }

                CGrPoint half = randomLightDirections[randomLights] + intersect*-1;
                half.Normalize3();

                if(material->Shininess() > 1) {
  								 sif += pow(Dot3(N, half), (double)material->Shininess()); 
                } else {
		  						 sif += pow(Dot3(N, half), 1.0);
                }
              }

              if(m_mcRays > 0) {
                sif /= m_mcRays;
              }
#endif

							bool specular = false;
							for(int p=0;  p<3;  p++)
							{
								if (material->Specular(p) >= 0 && material->Specular(p) <= 1.0)
								{
									specular = true;
									hall[p] += light.m_specular[p] * material->Specular(p) * sif * texcolor[p]; 
								}
							}

							//specular from other objects
							//
							if (specular)
							{
								CGrPoint R = N*2*Dot3(N, intersect*-1) - intersect*-1;

								CRay refray(intersect, Normalize3(R));

								CGrPoint newcolor;

								GetReflect(refray, nearest, newcolor);

								hall[0] += newcolor.X() * material->Specular(0);
								hall[1] += newcolor.Y() * material->Specular(1);
								hall[2] += newcolor.Z() * material->Specular(2);
							} // end if(specular)
						} // end for(int l=0;  l<m_lights.size();  l++)
						for(int p=0; p<3; p++)
						{
							if (texture)
								hall[p] /= 255.;
              if (hall[p] < 1) {
								m_rayimage[r][c*3+p] = GLbyte(hall[p] * 255);
              } else {
								m_rayimage[r][c*3+p] = GLbyte(255);
              }
						}// end for(int p=0; p<3; p++)
				  } // end if(material != NULL)

					avghallcolor[0] += hall[0];
					avghallcolor[1] += hall[1];
					avghallcolor[2] += hall[2];
				} // end if(m_intersection.Intersect(ray, NULL, nearest, t, intersect
				else
				{
					//// We hit nothing...
				}
			} // end for(int alias=0; alias<m_iterations; alias++)
			avghallcolor[0] /= m_iterations;
			avghallcolor[1] /= m_iterations;
			avghallcolor[2] /= m_iterations;

			for (int p=0; p<3; p++)
			{
        if(avghallcolor[p] < 1) {
					m_rayimage[r][c*3+p] = GLbyte(avghallcolor[p] * 255);
        } else {
					m_rayimage[r][c*3+p] = GLbyte(255);
        }
			} // end for(int p=0; p<3; p++)
		} // end for(int c=0; c<m_rayimagewidth; c++)
		m_window->Invalidate();
		m_window->UpdateWindow();
	} // end for(int r=0; r<m_rayimageheight; r++)

	return true;
}

void CMyRaytraceRenderer::GetReflect(CRay Refray, CRayIntersection::Object *nearest, CGrPoint &newcolor)
{
			double hallcolor[3];
			hallcolor[0] = hallcolor[1] = hallcolor[2] = 0;


			// Construct a Ray
			
			CRayIntersection::Object *Refnext;   // Pointer to intersecting object

			double t;                                     // Will be distance to intersection
			CGrPoint intersect;                           // Will be x,y,z location of intersection
			if(m_intersection.Intersect(Refray, nearest, Refnext, t, intersect))
			{
				// Determine information about the intersection
				CGrPoint N;             //Surface Normal
				CGrMaterial *material;
				CGrTexture *texture;
				CGrPoint texcoord;
                float texcolor[3];
			
				m_intersection.IntersectInfo(Refray, Refnext, t, N, material, texture, texcoord);

				CGrPoint V = Normalize3(-intersect);  //View direction
				
					if (texture!= NULL)
					{
						//Calculate Texinfo
						double s = double((texture->Height()-1)*fmod(texcoord.Y(),1));
						double t = double((texture->Width()-1)*fmod(texcoord.X(),1));
							
						double fs = fmod(s, 1.);
						double ft = fmod(t, 1.);
						for( int p = 0; p<3; p++)
						{
							double ct = texture->Row(int(s)+1)[int(t)*3+p] * (1.-ft) + texture->Row(int(s)+1)[(int(t)+1)*3+p]*ft;
							double cb = texture->Row(int(s))[int(t)*3+p] * (1.-ft) + texture->Row(int(s))[(int(t)+1)*3+p]*ft;
							texcolor[p] = float(ct*fs+cb*(1.-fs));
						}
					}
					else 
					{
						texcolor[0] = texcolor[1] = texcolor[2] = 1.;
					}
				
#ifdef MONTE_CARLO
        vector<double> contributions;
        vector<CGrPoint> randomLightDirections;
        contributions.reserve(m_mcRays);
        randomLightDirections.reserve(m_mcRays);
#endif
				for (int x = 0; x< int(m_lights.size()); x++)
				{
#ifdef MONTE_CARLO
          contributions.clear();
          randomLightDirections.clear();

          SphereLight light = m_lights.at(x);
#else
					Light light = m_lights.at(x);
#endif

					//Ambient Light
					for(int z=0; z<3; z++)
					{
						hallcolor[z] += double(light.m_ambient[z]*material->Ambient(z)*texcolor[z]);
					}

					// Construct a Shadow Fealer
#ifndef MONTE_CARLO
					CRay shadowray(intersect, L );
					CRayIntersection::Object *nearest2;   // Pointer to intersecting object

					double t2;                            // Will be distance to other intersection2
					CGrPoint intersect2;                  // Will be x,y,z location of intersection2
					if (m_intersection.Intersect(shadowray, Refnext, nearest2, t2, intersect2))
					{
						CGrPoint distance = (intersect - light.m_pos);
						double disttolight = sqrt(pow(distance.X(),2) + pow(distance.Y(),2) + pow(distance.Z(),2));
						if (t2 < disttolight) continue;
					}

          CGrPoint L;                 //Light direction
					if(light.m_pos[3] == 0) L = Normalize3(light.m_pos);
					else L = Normalize3(light.m_pos - intersect);
					if(Dot3(N,L)<0) continue;

          //Diffuse Light
					for(int z=0; z<3; z++)
					{
						hallcolor[z] += double(light.m_diffuse[z]*material->Diffuse(z)*Dot3(N,L)*texcolor[z]);
					}
#else
          GetMonteCarloDiffuse(contributions, randomLightDirections,
                               light, intersect, Refnext, m_mcRays);

          double lAngle = 0.0;
          for(unsigned int randomLights=0; randomLights < m_mcRays; randomLights++) {
            double contribution = contributions[randomLights];

            double cAngle = Dot3(N, randomLightDirections[randomLights]);
            if(cAngle < 0) {
              contributions[randomLights] = 0.0;
              cAngle = 0.0;
            }

            lAngle += contribution * cAngle;
          }

          if(m_mcRays > 0) {
            lAngle /= m_mcRays;
          }

          //Diffuse Light
					for(int z=0; z<3; z++)
					{
						hallcolor[z] += double(light.m_diffuse[z]*material->Diffuse(z)*lAngle*texcolor[z]);
					}
#endif

					//Specular Light
#ifndef MONTE_CARLO
					CGrPoint H = Normalize3(L + V);  //half vector between lightdir and viewdir
					double sif = pow(Dot3(N, H),(double)material->Shininess());
#else
          double sif = 0.0;
          for(unsigned int randomLights=0; randomLights < m_mcRays; randomLights++) {
            double contribution = contributions[randomLights];
            if(contribution <= 0) { continue; }

            CGrPoint half = randomLightDirections[randomLights] + intersect*-1;
            half.Normalize3();

            if(material->Shininess() > 1) {
						 sif += pow(Dot3(N, half), (double)material->Shininess()); 
            } else {
						 sif += pow(Dot3(N, half), 1.0);
            }
          }

          if(m_mcRays > 0) {
            sif /= m_mcRays;
          }
#endif
					for(int z=0; z<3; z++)
					{
						hallcolor[z] += double(light.m_specular[z]*material->Specular(z)*sif*texcolor[z]);
					}

				}

        for(int z=0; z<3; z++)
				{
					if(texture != NULL)
						hallcolor[z] /= 255.;
					if (hallcolor[z] < 0.) hallcolor[z] = 0.;
					else if (hallcolor[z] > 1.) hallcolor[z] = 1.;
				}		

			}
			else
			{
				// We hit nothing...
				hallcolor[0] = 0;
				hallcolor[1] = 0;
				hallcolor[2] = 0;
			}
			newcolor.Set(hallcolor[0], hallcolor[1], hallcolor[2]);
}


GLbyte* CMyRaytraceRenderer::ReturnColor(CRay ray)
{
	GLbyte color[3] = {GLbyte(255), GLbyte(255), GLbyte(255)};
	return color;
}
void CMyRaytraceRenderer::RendererPushMatrix()
{
	// don't want to lose what we have right now when we pop later so we
	// push it again
	m_mstack.push_back(m_mstack.back());	
}
void CMyRaytraceRenderer::RendererPopMatrix()
{
	// just remove current
	m_mstack.pop_back();
}

void CMyRaytraceRenderer::RendererRotate(double a, double x, double y, double z)
{
	// use transform class
	CGrTransform t;

	t.Identity();

	// turn point into vector
	CGrPoint p(x, y, z);

	// use transform class rotate
	t.Rotate(a, p);

	// multiply matrix by current
	t = m_mstack.back()*t;

	// replace current with new matrix
	m_mstack.pop_back();
	m_mstack.push_back(t);
}
void CMyRaytraceRenderer::RendererTranslate(double x, double y, double z)
{
	// use transform class
	CGrTransform t;

	t.Identity();

	// use transform class translate
	t.Translate(x, y, z);

	// multiply matrix by current
	//t.Compose(m_mstack.back());
	t = m_mstack.back()*t;

	// replace current with new matrix
	m_mstack.pop_back();
	m_mstack.push_back(t);
}

#ifdef MONTE_CARLO
void CMyRaytraceRenderer::GetRandomCoordSystem(CGrPoint &right, CGrPoint &up,
                                               const CGrPoint &pointOnPlane,
                                               const CGrPoint &planeNormal) const {
  double d = -1.0 * (planeNormal.X()*pointOnPlane.X() +
                     planeNormal.Y()*pointOnPlane.Y() +
                     planeNormal.Z()*pointOnPlane.Z());

  CGrPoint p(pointOnPlane.X() + 0.5, pointOnPlane.Y() + 0.5, pointOnPlane.Z() + 0.5);
  if(planeNormal.X() > planeNormal.Y() && planeNormal.X() > planeNormal.Z()){
    p.X((d + (planeNormal.Y()*p.Y()) + (planeNormal.Z()*p.Z())) / (-1.0 * planeNormal.X()));
  } else if(planeNormal.Y() > planeNormal.X() && planeNormal.Y() > planeNormal.Z()) {
    p.Y((d + (planeNormal.X()*p.X()) + (planeNormal.Z()*p.Z())) / (-1.0 * planeNormal.Y()));
  } else {
    p.Z((d + (planeNormal.Y()*p.Y()) + (planeNormal.X()*p.X())) / (-1.0 * planeNormal.Z()));
  }
  right = p - pointOnPlane;
  right.Normalize3();

  up = Cross3(planeNormal, right);
  up.Normalize3();

  return;
}

void CMyRaytraceRenderer::GetRandomPointOnDisc(CGrPoint &p, const CGrPoint &center,
                                               const CGrPoint &right,
                                               const CGrPoint &up, double radius) const {
  // This was the suggested approach for a random distributed sample points from the 
  // surface of a disc from www.mathworld.com.  r = d_rng.GetDouble * radius tends to 
  // have its points mostly about the center, but the sqrt is more in accordance with 
  // the distribution of an area
  double r = sqrt(d_rng->GetDouble() * radius);
  double theta = d_rng->GetDouble(0.0, 2.0 * M_PI);

  p = center + (right * (r * cos(theta))) + (up * (r * sin(theta)));

  return;
}

void CMyRaytraceRenderer::ApproximateLightDirection(CGrPoint &lDir, const CGrPoint &lightCenter,
                                                    const CGrPoint &discPoint,
                                                    const CGrPoint &discFacing,
                                                    double radius) const {
  CGrPoint v(discPoint - lightCenter);
  double d2 = v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z();
  double dp = sqrt(d2 + radius*radius);

  CGrPoint p(discPoint + (discFacing * dp));
  lDir = p - lightCenter;
  lDir.Normalize3();

  return;
}


double CMyRaytraceRenderer::GetSingleLightRayContribution(double lightMagnitude,
                                                     const CGrPoint &lightDirection,
                                                     const CGrPoint &lightPoint,
                                                     const CRayIntersection::Object *nearest) const {
  CRay lightray(lightPoint, lightDirection);

	double dist;
	CGrPoint intersectLight;
	CRayIntersection::Object *lightnearest;   // Pointer to intersecting object

  // Ignoring intersected surface, thus can not intersect with surface point. Illumination 
  // does not occur if and only if an intersection from the light toward the surface point 
  // occurs with a distance less than the distance from this point to the surface point.
	if (m_intersection.Intersect( lightray, nearest, lightnearest, dist, intersectLight ))
	{
    if(lightMagnitude > dist) {
			return 0.0;
    }
	}

  return 1.0;
}

void CMyRaytraceRenderer::GetMonteCarloDiffuse(vector<double> &contributions,
                                               vector<CGrPoint> &randomLightDirections,
                                               const SphereLight &light,
                                               const CGrPoint &surfacePoint,
                                               const CRayIntersection::Object *nearest,
                                               unsigned int mcRays) const {
  // Things to try
  // 1. fix specular
  // 3. Add ray number entry dialog
  // 4. Mark changes to ChildView
  // 5. 

  CGrPoint lightDirection = (light.m_pos - surfacePoint);
  double lightMagnitude = sqrt(lightDirection.X()*lightDirection.X() +
                               lightDirection.Y()*lightDirection.Y() +
                               lightDirection.Z()*lightDirection.Z());
  lightDirection.Normalize3();

  if(mcRays <= 1) {
    if(light.m_directional) {
      double cangle = Dot3(lightDirection*-1, light.m_orientation);
      double csangle = cos(light.m_sectionAngle);

      // The random light ray is outside the "cone" of light, if true
      if(csangle > cangle) {
        contributions.push_back(0.0);
        randomLightDirections.push_back(lightDirection);
        return;
      }
    }

    contributions.push_back(GetSingleLightRayContribution(lightMagnitude, lightDirection*-1, 
                                                          light.m_pos, nearest));
    randomLightDirections.push_back(lightDirection);
    return;
  }

  if(mcRays > 128) { mcRays = 128; }

  CGrPoint up, right;
  GetRandomCoordSystem(right, up, light.m_pos, Normalize3(lightDirection));

  CGrPoint p;
  for(unsigned int i=0; i<mcRays; i++) {
    GetRandomPointOnDisc(p, light.m_pos, right, up, light.m_radius);

    CGrPoint lDir = (p - surfacePoint);
    double lMag = sqrt(lDir.X()*lDir.X() + lDir.Y()*lDir.Y() + lDir.Z()*lDir.Z());
    lDir.Normalize3();

    if(light.m_directional) {
      double cangle = Dot3(lDir*-1, light.m_orientation);
      double csangle = cos(light.m_sectionAngle);

      // The random light ray is outside the "cone" of light, if true
      if(csangle > cangle) {
        contributions.push_back(0.0);
        randomLightDirections.push_back(lDir);
        continue;
      }
    }

    contributions.push_back(GetSingleLightRayContribution(lMag, lDir*-1, p, nearest));
    randomLightDirections.push_back(lDir);
  }

  return;
}

#endif
