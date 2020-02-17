//
// Name :         FloatFactory.h
// Description :  Header for FloatFactory class.
//                See FloatFactory.cpp for details.
// Author :       Charles B. Owen
//


#ifndef _COTTAGEFACTORY_H
#define _COTTAGEFACTORY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrObject.h"
#include "ShapeFactory.h"
#include "GrTexture.h"	   // Added by ClassView

class CFloatFactory  
{
public:
	CFloatFactory();
	virtual ~CFloatFactory();

	bool Create();
	CGrObject *SceneGraph() {return m_float;}
   
	CGrRotate* RotateStar(){return m_starrotate;}

	CGrRotate* WheelRotate() {return m_wheelangle;}
private:

	CGrPtr<CGrTexture> m_tfrontside;
	CGrPtr<CGrTexture> m_tbackside;
	CGrPtr<CGrTexture> m_jellybeans;
	CGrPtr<CGrTexture> m_concrete;
	CGrPtr<CGrTexture> m_mud;
	CGrPtr<CGrTexture> m_purple;
	CGrPtr<CGrTexture> m_tleftside;
	CGrPtr<CGrTexture> m_trightside;
	CGrPtr<CGrTexture> m_seat;
	CGrPtr<CGrTexture> m_tiretex;
	CGrPtr<CGrTexture> m_spritetex;
	CGrPtr<CGrTexture> m_troof;
	

	CGrPtr<CGrObject> m_float;
	CGrPtr<CGrObject> m_bldg;

	CGrPtr<CGrObject> m_door;
	CGrPtr<CGrRotate> m_starrotate;
	CGrPtr<CGrRotate> m_wheelangle;

	CGrPtr<CGrObject> m_window;
	CGrPtr<CGrObject> m_window1;
	CGrPtr<CGrTranslate> m_window1lower;
	CGrPtr<CGrTranslate> m_window1upper;

   // The lawn
   void CreateTile(CGrPtr<CGrObject> &p_tile);
	void CreateStar(CGrPtr<CGrComposite> &star, double radius, double width);
   void CreateTire(CGrPtr<CGrObject> &p_tire);
   void CreateCylinder(CGrPtr<CGrObject> &p_cyl, double brad, double trad, double height,
	   CGrTexture *tex1=NULL, CGrTexture *tex2=NULL);
   void CreateChair(CGrPtr<CGrObject> &p_chair);
   void CreateLeg(CGrPtr<CGrObject> &p_leg);
   void CreateSeat(CGrPtr<CGrObject> &p_seat);
   void CreateBack(CGrPtr<CGrObject> &p_back);
	void CreateArm(CGrPtr<CGrObject> &p_arm);
	void CreateSpriteLeg(CGrPtr<CGrObject> &p_sprite_leg, bool arm);
	void CreateSprite(CGrPtr<CGrObject> &p_sprite);
	void CreateWin(CGrPtr<CGrObject> &p_window);
	void CreateSphere(CGrPtr<CGrObject> &p_sphere, double p_radius, int part, CGrTexture *tex=NULL);
	void SphereFace(CGrPtr<CGrComposite> &comp, int p_recurse, double p_radius, GLdouble *a, 
                                               GLdouble *b, GLdouble *c, CGrTexture *tex);
	void CFloatFactory::CreateFlatTrim(CGrPtr<CGrComposite> &trim, double lenout, double lenin,double width);
	void CreatePlant(CGrPtr<CGrObject> &plant, float, float, float);
   // The house
   bool CreateHouse(CGrPtr<CGrObject> &p_house);
   void CreateWall(CGrPtr<CGrObject> &p_bldg, int p_which);
   void CreateRoof(CGrPtr<CGrObject> &p_roof);
   void CreateBottom(CGrPtr<CGrObject> &p_bottom, double x, double y, double z);
   void AddTiles(CGrPtr<CGrComposite> &p_tiles, int p_row, double p_z);
	
};



#endif // !defined(AFX_COTTAGE1_H__DC5BC293_726F_4898_B420_A22789D050BF__INCLUDED_)
