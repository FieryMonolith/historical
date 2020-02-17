//
// Name :         FloatFactory.cpp
// Description :  The CFloatFactory class creates the scene graph
//                for a rfloat.
//

#include "stdafx.h"
#include "FloatFactory.h"
#include "Lab2.h"
//#include "WindowFactory.h"

#include "GrXYRects.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Constants
const double GRASSWIDTH = 10.;

const float PAINT[4] = {0.f, 99.f/255.f, 61.f/255.f, 1.f};
const float IPAINT[4] = {0.f, 173.f/255.f, 206.f/255.f, 1.f};
const float specular[3] = {0.f, 0.f, 0.f};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFloatFactory::CFloatFactory()
{
   
}

CFloatFactory::~CFloatFactory()
{

}


//
// Name :         CFloatFactory::Create()
// Description :  The actual function to create the rfloat.
// Returns :      true if successful.
//

bool CFloatFactory::Create()
{

	CGrPtr<CGrComposite> rfloat = new CGrComposite;
	m_float = rfloat;

	
	m_tiretex = new CGrTexture;
	//if (!m_tiretex->LoadFile("Textures/jeepbabe.bmp"))
	//	return false;
	
	m_tfrontside = new CGrTexture;
  //  if(!m_tfrontside->LoadFile("Textures/rbwithsprites2.bmp"))
  //    return false;

	m_mud = new CGrTexture;
	//if (!m_mud->LoadFile("Textures/mud01.bmp"))
	//	return false;
	
	m_concrete = new CGrTexture;
  //  if(!m_concrete->LoadFile("Textures/concrete01.bmp"))
  //    return false;

	
	m_troof = new CGrTexture;
  //  if(!m_troof->LoadFile("Textures/purple.bmp"))
  //    return false;

	
	m_tbackside = new CGrTexture;
  //  if(!m_tbackside->LoadFile("Textures/spriteonrainbow2.bmp"))
  //    return false;
	
	
	m_tleftside = new CGrTexture;
  //  if(!m_tleftside->LoadFile("Textures/rbwaving.bmp"))
  //    return false;

	
	m_trightside = new CGrTexture;
  //  if(!m_trightside->LoadFile("Textures/rbonstarlight2.bmp"))
  //    return false;

	m_seat = new CGrTexture;
	//if (!m_seat->LoadFile("Textures/rbicon2.bmp"))
	//	return false;

	m_spritetex = new CGrTexture;
	if (!m_spritetex->LoadFile("Textures/sprite2.bmp"))
		return false;
	  
	/*CGrPtr<CGrObject> window;
	CreateWin(window);
	rfloat->Child(new CGrTranslate( -20., 63., 60., new CGrRotate(90, 1., 0., 0., window)));
	rfloat->Child(new CGrTranslate( -100., 63., 60., new CGrRotate(90, 1., 0., 0., window)));*/

	/*CGrPtr<CGrPolygon> floatbottom = new CGrPolygon;
	rfloat->Child(floatbottom);

	floatbottom->AddVertex3d(-120., -1., 60.);
	floatbottom->AddVertex3d(-120., -1., -60.);
	floatbottom->AddVertex3d(120., -1., -60.);
	floatbottom->AddVertex3d(120., -1., 60.);
	floatbottom->ComputeNormal();*/

	//CGrPtr<CGrPolygon> floatcieling = new CGrPolygon;
	//rfloat->Child(floatcieling);

	//floatcieling->AddVertex3d(-120., 84., -60.);
	//floatcieling->AddVertex3d(0., 84., -60.);
	//floatcieling->AddVertex3d(0., 84., 60.);
	//floatcieling->AddVertex3d(-120., 84., 60.);
	//floatcieling->ComputeNormal();

	CGrPtr<CGrObject> sprite;
	CreateSprite(sprite);
	//rfloat->Child(new CGrTranslate(72., 1., 0., new CGrRotate(90, 0., 1., 0., sprite)));
	rfloat->Child(sprite);
	
	//CGrPtr<CGrObject> chair;
	//CreateChair(chair);
	//rfloat->Child(new CGrTranslate(38. , 1., 42. , new CGrRotate(120, 0., 1., 0., chair)));
	//rfloat->Child(new CGrTranslate(38. , 1., -42. , new CGrRotate(60, 0., 1., 0., chair)));
	//rfloat->Child(new CGrTranslate(26. , 1., 14. , new CGrRotate(90, 0., 1., 0., chair)));
	//rfloat->Child(new CGrTranslate(26. , 1., -14. , new CGrRotate(90, 0., 1., 0., chair)));

	//CGrPtr<CGrObject> tire;
	//CreateTire(tire);

	//rfloat->Child(new CGrTranslate(-80., -24., -51., tire)); 
	//rfloat->Child(new CGrTranslate(-80., -24., 51., tire)); 
	//rfloat->Child(new CGrTranslate(80., -24., 51., tire)); 
	//rfloat->Child(new CGrTranslate(80., -24., -51., tire)); 

	/*CGrPtr<CGrObject> tile;
	CreateTile(tile);
	rfloat->Child(new CGrTranslate(60., 0., 0., new CGrRotate(180., 0., 1., 0., new CGrTranslate(0., 0., 60., tile))));
	rfloat->Child(new CGrTranslate(60., 0., 0., new CGrRotate(90, 0., 1., 0., new CGrTranslate(-60., 0., 0., tile))));
	rfloat->Child(new CGrTranslate(60., 0., 60., tile));
	rfloat->Child(new CGrTranslate(60., 0., 0., new CGrRotate(-90, 0., 1., 0., new CGrTranslate(-60., 0., 0., tile))));
	rfloat->Child(new CGrTranslate(-60., 0., 0., new CGrRotate(180., 0., 1., 0., new CGrTranslate(0., 0., 60., tile))));
	rfloat->Child(new CGrTranslate(-60., 0., 0., new CGrRotate(90, 0., 1., 0., new CGrTranslate(-60., 0., 0., tile))));
	rfloat->Child(new CGrTranslate(-60., 0., 60., tile));
	rfloat->Child(new CGrTranslate(-60., 0., 0., new CGrRotate(-90, 0., 1., 0., new CGrTranslate(-60., 0., 0., tile))));
	*/
	/*CGrPtr<CGrObject> bottom;
	CreateBottom(bottom, 120, 24, 60);
	rfloat->Child(bottom);*/
	//
	//CGrPtr<CGrObject> back;
	//CreateWall(back, 2);
	//rfloat->Child(back);
	//
	//CGrPtr<CGrObject> left;
	//CreateWall(left, 3);
	//rfloat->Child(new CGrRotate(-90, 0., 1., 0., new CGrTranslate(60., 0., 60., left)));

	//CGrPtr<CGrObject> front;
	//CreateWall(front, 1);
	//rfloat->Child(new CGrTranslate(-60., 0., -60., new CGrRotate(180, 0., 1., 0., new CGrTranslate(60., 0., -60., front))));

	////// doesn't work!
	////CGrPtr<CGrObject> right;
	////CreateWall(right, 0);
	////rfloat->Child( right);

	//CGrPtr<CGrObject> roof;
	//CreateWall(roof, 4);
	//rfloat->Child(roof);

	//
	//CGrPtr<CGrObject> plant1;
	//CreatePlant(plant1, 1.f, 0.f, 0.f);
	//rfloat->Child(new CGrTranslate(110., 12.5, 50., plant1));

	//CGrPtr<CGrObject> plant2;
	//CreatePlant(plant2, 1.f, .5f, 0.f);
	//rfloat->Child(new CGrTranslate(110., 12.5, 30., plant2));

	//CGrPtr<CGrObject> plant3;
	//CreatePlant(plant3, 1.f, 1.f, 0.f);
	//rfloat->Child(new CGrTranslate(110., 12.5, 10, plant3));

	//CGrPtr<CGrObject> plant4;
	//CreatePlant(plant4, 0.f, .6f, 0.f);
	//rfloat->Child(new CGrTranslate(110., 12.5, -10., plant4));

	//CGrPtr<CGrObject> plant5;
	//CreatePlant(plant5, 0.f, 0.f, 1.f);
	//rfloat->Child(new CGrTranslate(110., 12.5, -30., plant5));

	//CGrPtr<CGrObject> plant6;
	//CreatePlant(plant6, .6f, .1f, .9f);
	//rfloat->Child(new CGrTranslate(110., 12.5, -50., plant6));
	
	return true;
}


void CFloatFactory::AddTiles(CGrPtr<CGrComposite> &p_tiles, int p_row, double p_z)
{

	CGrPtr<CGrPolygon> poly;
	int y;
	float r,g,b;

	y = (p_row%6);

	for(double x = 0;  x< 60; x++)
	{

		switch ((int(x)+y)%6)
		{
		case 0:
		   r = 1;
		   g = 0;
		   b = 0;
		   break;
	   case 1:
		   r = 1;
		   g = .5f;
		   b = 0;
		   break;
	   case 2:
		   r = 1;
		   g = 1;
		   b = 0;
		   break;
	   case 3:
		   r = 0;
		   g = .8f;
		   b = 0;
		   break;
	   case 4:
		   r = 0;
		   g = 0;
		   b = 1;
		   break;
	   case 5:
		   r = .7f;
		   g = 0;
		   b = .9f;
		   break;
	   }

      double width;
      width = 1.;
   
      poly = new CGrPolygon;
      //p_tiles->Child(poly);
		  
	  float ambient[3] = {r, g, b};

	CGrPtr<CGrMaterial> tilepaint = new CGrMaterial;
	p_tiles->Child(tilepaint);

	CGrPtr<CGrComposite> tilepainted = new CGrComposite;
    tilepaint->Child(tilepainted);

    tilepaint->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 1. );
	tilepainted->Child(poly);

      poly->AddVertex3d(x, 0., p_z);
      poly->AddVertex3d(x+width, 0., p_z);
      poly->AddVertex3d(x+width, 0., p_z-1);
      poly->AddVertex3d(x, 0., p_z-1);
      poly->ComputeNormal();

   }
}





//
// Name :         CFloatFactory::CreateLawn()
// Description :  Create the lawn display list.  I'm creating this with the back
//                left corner at (0, 0, 0).  I also make the lawn have some 
//                "thickness" just for the heck of it.
//

void CFloatFactory::CreateTile(CGrPtr<CGrObject> &p_tile)
{
	CGrPtr<CGrComposite> tile = new CGrComposite;
	p_tile = tile;
		
	CGrPtr<CGrComposite> tiles = new CGrComposite;
	tile->Child(tiles);

	double spacing = 1;
	int cnt = 60;
	for(int i=0;  i<cnt;  i++)
	{	
		AddTiles(tiles, i, -spacing * i);
	}
}



void CFloatFactory::CreateWall(CGrPtr<CGrObject> &p_bldg, int wall)
{
	 // We'll need a composite for assembly of the wall components
   CGrPtr<CGrComposite> bldg = new CGrComposite;
   p_bldg = bldg;

   CGrPtr<CGrMaterial> topmat = new CGrMaterial(CGrMaterial::texture);
   topmat->SpecularOther(0.2f, 0.2f, 0.2f);
   //p_top = topmat;
   bldg->Child(topmat);

   CGrPtr<CGrComposite> topcomp = new CGrComposite();
   topmat->Child(topcomp);

   CGrPtr<CGrPolygon> poly;

   poly = new CGrPolygon;
   topcomp->Child(poly);

	 CGrPtr<CGrPolygon> polyin;

	polyin = new CGrPolygon;
		  
	CGrPtr<CGrMaterial> tilepaint = new CGrMaterial;
	bldg->Child(tilepaint);

	CGrPtr<CGrComposite> tilepainted = new CGrComposite;
    tilepaint->Child(tilepainted);

   switch (wall)
   {
   case 1:
	   {
	   poly->Texture(m_trightside);
	   poly->AddTexVertex3d(-120., 0., 60., 0, 0);
	   poly->AddTexVertex3d(0., 0., 60., 1, 0);
	   poly->AddTexVertex3d(0., 84., 60., 1, 1);
	   poly->AddTexVertex3d(-120.,84., 60., 0, 1);
	   poly->ComputeNormal();

	   tilepaint->AmbientAndDiffuse(.5f, .8f ,.9f);
	   tilepainted->Child(polyin);
	   polyin->AddTexVertex3d(-114., 0., 54., 0, 0);
	   polyin->AddTexVertex3d(-114.,84., 54., 0, 1);
	   polyin->AddTexVertex3d(-6., 84., 54., 1, 1);
	   polyin->AddTexVertex3d(-6., 0., 54., 1, 0);


	   polyin->ComputeNormal();
	   break;
	   }

   case 2:
	   {
	   poly->Texture(m_tbackside);
	   poly->AddTexVertex3d(-120., 0., -60., 0, 0);
	   poly->AddTexVertex3d(-120., 0., 60., 1, 0);
	   poly->AddTexVertex3d(-120., 84., 60., 1, 1);
	   poly->AddTexVertex3d(-120., 84., -60., 0, 1);
	   poly->ComputeNormal();

	   tilepaint->AmbientAndDiffuse(.8f, .7f, .5f);
	   tilepainted->Child(polyin);
	   polyin->AddTexVertex3d(-114., 0., -54., 0, 0);
	   polyin->AddTexVertex3d(-114., 84., -54., 0, 1);
	   polyin->AddTexVertex3d(-114., 84., 54., 1, 1);
	   polyin->AddTexVertex3d(-114., 0., 54., 1, 0);

	  
	   polyin->ComputeNormal();
	   break;
	   }

   case 3:
	   {
	   poly->Texture(m_tleftside);
	   poly->AddTexVertex3d(0., 0., -60., 0, 0);
	   poly->AddTexVertex3d(-120., 0., -60., 1, 0);
	   poly->AddTexVertex3d(-120., 84., -60., 1, 1);
	   poly->AddTexVertex3d(0., 84., -60., 0, 1);
	   poly->ComputeNormal();

	   tilepaint->AmbientAndDiffuse(.4f, .8f, .6f);
	   tilepainted->Child(polyin);
	   polyin->AddTexVertex3d(-6., 0., -54., 0, 0);
	   polyin->AddTexVertex3d(-6., 84., -54., 0, 1);
	   polyin->AddTexVertex3d(-114., 84., -54., 1, 1);
	   polyin->AddTexVertex3d(-114., 0., -54., 1, 0);

	   
	   polyin->ComputeNormal();
	   break;
	   }
	case 0:
	   {
			tilepaint->AmbientAndDiffuse(.4f, .8f, .6f);
			tilepainted->Child(new CGrTranslate(0., 0., 120., poly));



			poly->AddTexVertex3d(0., 84., -60., 0, 1);
			poly->AddTexVertex3d(-120., 84., -60., 1, 1);
			poly->AddTexVertex3d(-120., 0., -60., 1, 0);
			poly->AddTexVertex3d(0., 0., -60., 0, 0);
			poly->ComputeNormal();

			tilepainted->Child(polyin);
		   polyin->AddTexVertex3d(-6., 0., -54., 0, 0);
		   polyin->AddTexVertex3d(-6., 84., -54., 0, 1);
		   polyin->AddTexVertex3d(-114., 84., -54., 1, 1);
		   polyin->AddTexVertex3d(-114., 0., -54., 1, 0);

		   //CGrPtr<CGrMaterial> wallpaint = new CGrMaterial;
		   //float ambient2[3] = {.5, .1, .7};
		   //wallpaint->AmbientDiffuseSpecularShininess(ambient2, ambient2, specular, 0.);

		   //CGrPtr<CGrComposite> wallpainted = new CGrComposite;
		   //wallpaint->Child(wallpainted);

		   //CGrPtr<CGrXYRects> outwall = new CGrXYRects;
		   //bldg->Child(new CGrTranslate(0., 0., 60., new CGrRotate(90., 0., 1., 0., new CGrMaterial(CGrMaterial::texture, outwall))));

		   //double winwidth = 20.0;
		   //double winleft = 20;
		   //double winright = 120 -20.;
		   //double center = 120. / 2.;

		   //outwall->Texture(m_tfrontside, 120, 90, 0, 0);
		   //outwall->AddRect(0, 0, 120, 84, 0);

		   //// Cutout for the windows
		   //outwall->CutoutCenterWidths(winleft, 63, 28., 28.);
		   //outwall->CutoutCenterWidths(winright, 63, 28., 28.);

		   //CGrPtr<CGrXYRects> inwall = new CGrXYRects;
		   //tilepaint->AmbientAndDiffuse(.6f, .4f, .9f);
		   //tilepainted->Child(new CGrTranslate(-6., 0., -60., new CGrRotate(-90., 0., 1., 0., inwall)));

		   //inwall->AddRect(6, 0, 114, 84, 0);

		   //// Cutout for the windows
		   //inwall->CutoutCenterWidths(winleft, 63, 28., 28.);
		   //inwall->CutoutCenterWidths(winright, 63, 28., 28.);

		   break;
	   }
	case 4:
		{
			poly->Texture(m_troof);
			poly->AddTexVertex3d(0., 84., 60., 0, 0);
			poly->AddTexVertex3d(0., 84., -60., 1, 0);
			poly->AddTexVertex3d(-120., 84., -60., 1, 1);
			poly->AddTexVertex3d(-120.,84., 60., 0, 1);
			poly->ComputeNormal();
			break;
		}
   }


}

void CFloatFactory::CreateBottom(CGrPtr<CGrObject> &p_bottom, double x, double y, double z)
{
	float r=.3f, g=.3f, b=.9f;

	CGrPtr<CGrComposite> bottom = new CGrComposite;
	p_bottom = bottom;

	CGrPtr<CGrMaterial> bottompaint = new CGrMaterial;
	bottom->Child(bottompaint);
	bottompaint->AmbientAndDiffuse(r,g,b);

	CGrPtr<CGrComposite> bottompainted = new CGrComposite;
	bottompaint->Child(bottompainted);

	CGrPtr<CGrPolygon> right1;
	right1 = new CGrPolygon;
	bottompainted->Child(right1);

	right1->AddVertex3d(-x, 0., z);
	right1->AddVertex3d(-x-6, -y/3., z+6);
	right1->AddVertex3d(x+6, -y/3., z+6);
	right1->AddVertex3d(x, 0., z);
	right1->ComputeNormal();

	CGrPtr<CGrPolygon> right2;
	right2 = new CGrPolygon;
	bottompainted->Child(right2);

	right2->AddVertex3d(-x-6, -y/3., z+6);
	right2->AddVertex3d(-x-6, -2*y/3., z+6);
	right2->AddVertex3d(x+6, -2*y/3., z+6);
	right2->AddVertex3d(x+6, -y/3., z+6);
	right2->ComputeNormal();

	CGrPtr<CGrPolygon> right3;
	right3 = new CGrPolygon;
	bottompainted->Child(right3);

	right3->AddVertex3d(-x-6, -2*y/3., z+6);
	right3->AddVertex3d(-x, -y, z);
	right3->AddVertex3d(x, -y, z);
	right3->AddVertex3d(x+6,-2*y/3., z+6);
	right3->ComputeNormal();

	CGrPtr<CGrPolygon> right4;
	right4 = new CGrPolygon;
	bottompainted->Child(right4);

	right4->AddVertex3d(x, 0., z);
	right4->AddVertex3d(x, -y, z);
	right4->AddVertex3d(-x, -y, z);
	right4->AddVertex3d(-x, 0., z);
	right4->ComputeNormal();



	CGrPtr<CGrPolygon> back1;
	back1 = new CGrPolygon;
	bottompainted->Child(back1);

	back1->AddVertex3d(-x, 0., -z);
	back1->AddVertex3d(-x-6, -y/3., -z+6);
	back1->AddVertex3d(-x-6, -y/3., z+6);
	back1->AddVertex3d(-x, 0., z);
	back1->ComputeNormal();

	CGrPtr<CGrPolygon> back2;
	back2 = new CGrPolygon;
	bottompainted->Child(back2);

	back2->AddVertex3d(-x-6, -y/3., -z+6);
	back2->AddVertex3d(-x-6, -2*y/3., -z+6);
	back2->AddVertex3d(-x-6, -2*y/3., z+6);
	back2->AddVertex3d(-x-6, -y/3., z+6);
	back2->ComputeNormal();

	CGrPtr<CGrPolygon> back3;
	back3 = new CGrPolygon;
	bottompainted->Child(back3);

	back3->AddVertex3d(-x-6, -2*y/3., -z+6);
	back3->AddVertex3d(-x, -y, -z);
	back3->AddVertex3d(-x, -y, z);
	back3->AddVertex3d(-x-6,-2*y/3., z+6);
	back3->ComputeNormal();

	CGrPtr<CGrPolygon> back4;
	back4 = new CGrPolygon;
	bottompainted->Child(back4);

	back4->AddVertex3d(-x, 0., -z);
	back4->AddVertex3d(-x, 0, z);
	back4->AddVertex3d(-x, -y, z);
	back4->AddVertex3d(-x, -y, -z);
	back4->ComputeNormal();




	CGrPtr<CGrPolygon> left1;
	left1 = new CGrPolygon;
	bottompainted->Child(left1);

	left1->AddVertex3d(-x, 0., -z);
	left1->AddVertex3d(x, 0., -z);
	left1->AddVertex3d(x+6, -y/3., -z-6);	
	left1->AddVertex3d(-x-6, -y/3., -z-6);
	left1->ComputeNormal();

	CGrPtr<CGrPolygon> left2;
	left2 = new CGrPolygon;
	bottompainted->Child(left2);

	left2->AddVertex3d(-x-6, -y/3., -z-6);
	left2->AddVertex3d(x+6, -y/3., -z-6);
	left2->AddVertex3d(x+6, -2*y/3., -z-6);
	left2->AddVertex3d(-x-6, -2*y/3., -z-6);
	left2->ComputeNormal();

	CGrPtr<CGrPolygon> left3;
	left3 = new CGrPolygon;
	bottompainted->Child(left3);

	left3->AddVertex3d(-x-6, -2*y/3., -z-6);
	left3->AddVertex3d(x+6,-2*y/3., -z-6);
	left3->AddVertex3d(x, -y, -z);
	left3->AddVertex3d(-x, -y, -z);
	left3->ComputeNormal();

	CGrPtr<CGrPolygon> left4;
	left4 = new CGrPolygon;
	bottompainted->Child(left4);

	left4->AddVertex3d(x, 0., -z);
	left4->AddVertex3d(-x, 0., -z);
	left4->AddVertex3d(-x, -y, -z);
	left4->AddVertex3d(x, -y, -z);
	left4->ComputeNormal();


	CGrPtr<CGrPolygon> front1;
	front1 = new CGrPolygon;
	bottompainted->Child(front1);

	front1->AddVertex3d(x, 0., -z);


	front1->AddVertex3d(x, 0., z);
	front1->AddVertex3d(x+6, -y/3., z+6);
	front1->AddVertex3d(x+6, -y/3., -z-6);
	front1->ComputeNormal();

	CGrPtr<CGrPolygon> front2;
	front2 = new CGrPolygon;
	bottompainted->Child(front2);

	front2->AddVertex3d(x+6, -y/3., -z-6);

	front2->AddVertex3d(x+6, -y/3., z+6);
	front2->AddVertex3d(x+6, -2*y/3., z+6);	
	front2->AddVertex3d(x+6, -2*y/3., -z-6);

	front2->ComputeNormal();

	CGrPtr<CGrPolygon> front3;
	front3 = new CGrPolygon;
	bottompainted->Child(front3);

	front3->AddVertex3d(x+6, -2*y/3., -z-6);


	front3->AddVertex3d(x+6,-2*y/3., z+6);
	front3->AddVertex3d(x, -y, z);
	front3->AddVertex3d(x, -y, -z);
	front3->ComputeNormal();

	CGrPtr<CGrPolygon> front4;
	front4 = new CGrPolygon;
	bottompainted->Child(front4);

	front4->AddVertex3d(x, 0., -z);


	front4->AddVertex3d(x, -y, -z);
	front4->AddVertex3d(x, -y, z);
	front4->AddVertex3d(x, 0, z);
	front4->ComputeNormal();

	CGrPtr<CGrMaterial> mat = new CGrMaterial;
	bottom->Child(mat);
	mat->AmbientAndDiffuse(.3f, .3f, .3f);

	CGrPtr<CGrComposite> axispainted = new CGrComposite;
	mat->Child(axispainted);

	CGrPtr<CGrObject> axis;
	CreateCylinder(axis, 2., 2., 100.);
	axispainted->Child(new CGrTranslate(-80., -24., 50., new CGrRotate(-90, 1., 0., 0., axis)));
	axispainted->Child(new CGrTranslate(80., -24., 50., new CGrRotate(-90, 1., 0., 0., axis)));

	CGrPtr<CGrObject> centeraxis;
	CreateCylinder(centeraxis, 2., 2., 160.);
	axispainted->Child(new CGrTranslate(-80., -24., 0., new CGrRotate(-90, 0., 0., 1., centeraxis)));
}


void CFloatFactory::CreateTire(CGrPtr<CGrObject> &p_tire)
{

	CGrPtr<CGrComposite> tire = new CGrComposite;
	p_tire = tire;

	CGrPtr<CGrMaterial> tirepaint = new CGrMaterial;
	tire->Child(tirepaint);
	tirepaint->AmbientAndDiffuse(.3f, .3f, .3f);

	CGrPtr<CGrComposite> tirepainted = new CGrComposite;
	tirepaint->Child(tirepainted);

	CGrPtr<CGrObject> cyl;
	CreateCylinder(cyl, 18, 18, 16, NULL, m_tiretex);
	tirepainted->Child(new CGrRotate(-90, 1., 0., 0., new CGrTranslate(0., -8., 0., cyl)));
}

void CFloatFactory::CreateCylinder(CGrPtr<CGrObject> &p_cyl, double brad, double trad, double height, 
								   CGrTexture *tex1, CGrTexture *tex2)
{

	CGrPtr<CGrComposite> cyl = new CGrComposite;
	p_cyl = cyl;

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

		top->Texture(tex2);
		bottom->Texture(tex2);
		top->AddTexVertex3d(trad*cos(theta1), height, -trad*sin(theta1), .5*cos(theta1)+.5, -.5*sin(theta1)+.5 );
		bottom->AddTexVertex3d(brad*cos(6.32317-theta1), 0, -brad*sin(6.32317-theta1), .5*cos(6.32317-theta1)+.5, -.5*sin(6.32317-theta1)+.5 );
	}

	top->ComputeNormal();
	bottom->ComputeNormal();

}

inline void Normalize3(GLdouble *v)
{
	GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void CFloatFactory::CreateChair(CGrPtr<CGrObject> &p_chair)
{
	CGrPtr<CGrComposite> chair = new CGrComposite;
	p_chair = chair;

	CGrPtr<CGrObject> leg;
	CreateLeg(leg);
	chair->Child(new CGrTranslate(-5., 0., -5., leg));
	chair->Child(new CGrTranslate(5., 0., -5., leg));
	chair->Child(new CGrTranslate(-7.5, 0., 5., leg));
	chair->Child(new CGrTranslate(7.5, 0., 5., leg));

	CGrPtr<CGrObject> seat;
	CreateSeat(seat);
	chair->Child(new CGrTranslate(0., 14., 0., seat));

	CGrPtr<CGrObject> back;
	CreateBack(back);
	chair->Child(new CGrTranslate(0., 15., -5., new CGrRotate(-10, 1., 0., 0., back)));

	CGrPtr<CGrObject> arm;
	CreateArm(arm);
	chair->Child(new CGrTranslate( 7., 15., 4.2, new CGrRotate(11.5, 0., 1., 0., arm)));
	chair->Child(new CGrTranslate( -7., 15., 4.2, new CGrRotate(-11.5, 0., 1., 0., arm)));

}

void CFloatFactory::CreateLeg(CGrPtr<CGrObject> &p_leg)
{
	CGrPtr<CGrComposite> leg = new CGrComposite;
	p_leg = leg;

	float ambient[3] = {.5, .5, 1.};

	CGrPtr<CGrMaterial> cyl1paint = new CGrMaterial;
	leg->Child(cyl1paint);
	cyl1paint->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 1.);

	CGrPtr<CGrComposite> cyl1painted = new CGrComposite;
	cyl1paint->Child(cyl1painted);

	CGrPtr<CGrObject> cyl1;
	CreateCylinder(cyl1, .5, 1, 4.5);
	cyl1painted->Child(new CGrTranslate(0., 1., 0.,cyl1));
	cyl1painted->Child(new CGrTranslate(0., 13, 0., new CGrRotate(180, 1., 0., 0., cyl1)));

	CGrPtr<CGrMaterial> cyl2paint = new CGrMaterial;
	leg->Child(cyl2paint);
	cyl2paint->AmbientAndDiffuse(.1f, .1f, .9f);

	CGrPtr<CGrComposite> cyl2painted = new CGrComposite;
	cyl2paint->Child(cyl2painted);

	CGrPtr<CGrObject> cyl2;
	CreateCylinder(cyl2, .5, .5, 3.);
	cyl2painted->Child(new CGrTranslate(0., 5.5, 0., cyl2));

	CGrPtr<CGrObject> cyl3;
	CreateCylinder(cyl3, 1.1, 1., 1.);
	leg->Child(cyl3);

}

void CFloatFactory::CreateSeat(CGrPtr<CGrObject> &p_seat)
{
	CGrPtr<CGrComposite> seat = new CGrComposite;
	p_seat = seat;

	float ambient[3] = {.1, .1, .9};

	CGrPtr<CGrComposite> layer = new CGrComposite;
	seat->Child(new CGrTranslate(0., -.5, 0., layer));
	seat->Child(new CGrTranslate(0., .75, 0., layer));	

	CGrPtr<CGrMaterial> topmat = new CGrMaterial(CGrMaterial::texture);
	topmat->SpecularOther(0.2f, 0.2f, 0.2f);
	layer->Child(topmat);

	CGrPtr<CGrComposite> topcomp = new CGrComposite();
	topmat->Child(topcomp);

	// We'll use textured polygons for the top and sides

	CGrPtr<CGrPolygon> top;
	top = new CGrPolygon;
	topcomp->Child(top);

	top->Texture(m_seat);
	top->AddTexVertex3d( -8.5, .5, 6., 0., 0.);
	top->AddTexVertex3d( 8.5, .5, 6., 1., 0. );
	top->AddTexVertex3d( 6., .5, -6., 1., 1. );
	top->AddTexVertex3d( -6., .5, -6., 0., 1. );
	top->ComputeNormal();

	CGrPtr<CGrMaterial> mat = new CGrMaterial;
	layer->Child(mat);
	mat->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 0.);

	CGrPtr<CGrComposite> matpainted = new CGrComposite;
	mat->Child(matpainted);

	CGrPtr<CGrPolygon> left;
	left = new CGrPolygon;
	matpainted->Child(left);

	left->AddVertex3d( -8.5, .5, 6. );
	left->AddVertex3d( -6., .5, -6. );
	left->AddVertex3d( -6., -.5, -6. );
	left->AddVertex3d( -8.5, -.5, 6. );
	left->ComputeNormal();

	CGrPtr<CGrPolygon> back;
	back = new CGrPolygon;
	matpainted->Child(back);

	back->AddVertex3d( -6., .5, -6. );
	back->AddVertex3d( 6., .5, -6. );
	back->AddVertex3d( 6., -.5, -6. );
	back->AddVertex3d( -6., -.5, -6. );
	back->ComputeNormal();

	CGrPtr<CGrPolygon> right;
	right = new CGrPolygon;
	matpainted->Child(right);

	right->AddVertex3d( 6., .5, -6. );
	right->AddVertex3d( 8.5, .5, 6. );
	right->AddVertex3d( 8.5, -.5, 6. );
	right->AddVertex3d( 6., -.5, -6. );
	right->ComputeNormal();

	CGrPtr<CGrPolygon> front;
	front = new CGrPolygon;
	matpainted->Child(front);

	front->AddVertex3d( 8.5, -.5, 6. );
	front->AddVertex3d( 8.5, .5, 6. );
	front->AddVertex3d( -8.5, .5, 6. );
	front->AddVertex3d( -8.5, -.5, 6. );
	front->ComputeNormal();

	CGrPtr<CGrPolygon> bottom;
	bottom = new CGrPolygon;
	matpainted->Child(bottom);

	bottom->AddVertex3d( 6., -.5, -6. );
	bottom->AddVertex3d( 8.5, -.5, 6. );
	bottom->AddVertex3d( -8.5, -.5, 6. );
	bottom->AddVertex3d( -6., -.5, -6. );
	bottom->ComputeNormal();

}

void CFloatFactory::CreateBack(CGrPtr<CGrObject> &p_back)
{
	CGrPtr<CGrComposite> back = new CGrComposite;
	p_back = back;

	float ambient[3] = {.5, .5, 1.};

	CGrPtr<CGrMaterial> cyl1paint = new CGrMaterial;
	back->Child(cyl1paint);
	cyl1paint->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 0.);

	CGrPtr<CGrComposite> cyl1painted = new CGrComposite;
	cyl1paint->Child(cyl1painted);

	CGrPtr<CGrObject> cyl1;
	CreateCylinder(cyl1, .75, .75, 18.);
	cyl1painted->Child(new CGrTranslate(5., 0., 0., cyl1));
	cyl1painted->Child(new CGrTranslate(-5., 0., 0., cyl1));

	CGrPtr<CGrMaterial> cyl2paint = new CGrMaterial;
	back->Child(cyl2paint);
	float ambient1[3] = {.1, .1, .9};
	cyl2paint->AmbientDiffuseSpecularShininess(ambient1, ambient1, specular, 0.);

	CGrPtr<CGrComposite> cyl2painted = new CGrComposite;
	cyl2paint->Child(cyl2painted);


	CGrPtr<CGrObject> cyl2;
	CreateCylinder(cyl2, 1., 1., 1.);
	cyl2painted->Child(new CGrTranslate(5., 18., 0., cyl2));
	cyl2painted->Child(new CGrTranslate(-5., 18., 0., cyl2));

	float r, g, b;
	for(int x = 0;  x< 6; x++)
	{
		switch(x)
		{
		case 0:
			r = 1;
			g = 0;
			b = 0;
			break;
		case 1:
			r = 1;
			g = .5f;
			b = 0;
			break;
		case 2:
			r = 1;
			g = 1;
			b = 0;
			break;
		case 3:
			r = 0;
			g = .6f;
			b = 0;
			break;
		case 4:
			r = 0;
			g = 0;
			b = 1;
			break;
		case 5:
			r = .6f;
			g = .1f;
			b = .9f;
			break;
		}

		CGrPtr<CGrMaterial> secpaint = new CGrMaterial;
		back->Child(secpaint);

		CGrPtr<CGrComposite> secpainted = new CGrComposite;
		secpaint->Child(secpainted);
		secpaint->AmbientAndDiffuse(r,g,b);

		CGrPtr<CGrComposite> slat = new CGrComposite;

		secpainted->Child(new CGrTranslate(0., 2.+3.*x, 0., slat));	
		slat->Box(-4.25, -1., -.25, 8.5, 2., .5);
	}

}

void CFloatFactory::CreateArm(CGrPtr<CGrObject> &p_arm)
{
	CGrPtr<CGrComposite> arm = new CGrComposite;
	p_arm = arm;

	float ambient[3] = {.1, .1, .9};

	CGrPtr<CGrMaterial> paint = new CGrMaterial;
	arm->Child(paint);
	paint->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 1.);

	CGrPtr<CGrComposite> painted = new CGrComposite;
	paint->Child(painted);

	CGrPtr<CGrComposite> rect = new CGrComposite;
	rect->Box(-.75, 0., 0., 1.5, 1., 8.5);

	painted->Child(new CGrTranslate(0., 4., -9.7, rect)); 

	CGrPtr<CGrObject> cyl1;
	CreateCylinder(cyl1, 1.5, 1.5, 1.);
	painted->Child(new CGrTranslate(0., 4., 0., cyl1));

	CGrPtr<CGrObject> cyl3;
	CreateCylinder(cyl3, .5, .5, 2.);
	painted->Child(new CGrTranslate(0., 1., 0., cyl3));

	CGrPtr<CGrMaterial> cyl2paint = new CGrMaterial;
	arm->Child(cyl2paint);
	cyl2paint->AmbientAndDiffuse(.5, .5, 1.);

	CGrPtr<CGrComposite> cyl2painted = new CGrComposite;
	cyl2paint->Child(cyl2painted);

	CGrPtr<CGrObject> cyl2;
	CreateCylinder(cyl2, 1., 1., 1.);
	cyl2painted->Child(cyl2);
	cyl2painted->Child(new CGrTranslate(0., 3., 0., cyl2));

}

void CFloatFactory::CreateSprite(CGrPtr<CGrObject> &p_sprite)
{
	CGrPtr<CGrComposite> sprite = new CGrComposite;
	p_sprite = sprite;
	
	CGrPtr<CGrMaterial> spritebody = new CGrMaterial(CGrMaterial::texture);
	spritebody->SpecularOther(0.2f, 0.2f, 0.2f);
	spritebody->Specular(1., 1., 1.);
	spritebody->Shininess(100.);
	sprite->Child(spritebody);

	CGrPtr<CGrComposite> spritetextured = new CGrComposite();
	spritebody->Child(spritetextured);

	CShapeFactory body;
	body.Sphere( 12., 0, m_spritetex);
	CGrPtr<CGrObject> bod = body.SceneGraph();
	spritetextured->Child(new CGrTranslate(0., 36., 0., bod));

	CGrPtr<CGrMaterial> podpaint = new CGrMaterial;
	sprite->Child(podpaint);

	float specularpod[3] = {1.f, 1.f, 1.f};
	float ambient[3] = {.1f, .1f, .9f};
	podpaint->AmbientDiffuseSpecularShininess(ambient, ambient, specularpod, 120.);

	CGrPtr<CGrComposite> podpainted = new CGrComposite;
	podpaint->Child(podpainted);
	podpainted->Box(-18., 0., -18., 36., 12., 36.);

	CGrPtr<CGrObject> leg;
	CreateSpriteLeg(leg, false);
	sprite->Child(new CGrTranslate(-8., 13., 0., new CGrRotate(-15, 0., 0., 1., leg)));
	sprite->Child(new CGrTranslate(8., 13., 0., new CGrRotate(15, 0., 0., 1., leg)));

	CGrPtr<CGrObject> arm;
	CreateSpriteLeg(arm, true);
	sprite->Child(new CGrTranslate(20.5, 38., 0., new CGrRotate(105, 0., 0., 1., arm)));
	sprite->Child(new CGrTranslate(-20.5, 38., 0., new CGrRotate(-105, 0., 0., 1., arm)));

	CGrPtr<CGrComposite> antenna = new CGrComposite;
	sprite->Child(new CGrTranslate(-8., 55., 0., new CGrRotate(15, 0., 0., 1., antenna)));
	sprite->Child(new CGrTranslate(8., 55., 0., new CGrRotate(-15, 0., 0., 1., antenna)));

	CGrPtr<CGrMaterial> antennapaint = new CGrMaterial;
	antenna->Child(antennapaint);
	antennapaint->AmbientAndDiffuse(1., 1., 0.);

	CGrPtr<CGrComposite> antennapainted = new CGrComposite;
	CreateStar(antennapainted, 5., 2.);
	antennapaint->Child(antennapainted);

	CGrPtr<CGrMaterial> rodpaint = new CGrMaterial;
	antenna->Child(new CGrTranslate(0., -10., 0., rodpaint));

	float ambientrod[3] = {1., 0., 0.};

	rodpaint->AmbientDiffuseSpecularShininess(ambientrod, ambientrod, specular, 0.);

	CGrPtr<CGrObject> rodpainted;
	CreateCylinder(rodpainted, .5, .5, 10.);
	rodpaint->Child(rodpainted);

}

void CFloatFactory::CreateSpriteLeg(CGrPtr<CGrObject> &p_sprite_leg, bool arm)
{
	CGrPtr<CGrComposite> leg = new CGrComposite;
	p_sprite_leg = leg;

	CGrPtr<CGrPolygon> poly;
	float r, g, b;

	for(int x = 1;  x< 6; x++)
	{
		switch(x)
		{
		case 0:
			r = 1;
			g = 0;
			b = 0;
			break;
		case 1:
			r = 1;
			g = .5f;
			b = 0;
			break;
		case 2:
			r = 1;
			g = 1;
			b = 0;
			break;
		case 3:
			r = 0;
			g = .6f;
			b = 0;
			break;
		case 4:
			r = 0;
			g = 0;
			b = 1;
			break;
		case 5:
			r = .6f;
			g = .1f;
			b = .9f;
			break;
		}

		poly = new CGrPolygon;
		  
		CGrPtr<CGrMaterial> secpaint = new CGrMaterial;
		leg->Child(secpaint);

		CGrPtr<CGrComposite> secpainted = new CGrComposite;
		secpaint->Child(secpainted);

		float ambient[3] = {r, g, b};
		secpaint->AmbientDiffuseSpecularShininess(ambient, ambient, specular, 0.);
		
		CGrPtr<CGrObject> cyl;

		if(arm)
		{
			CreateCylinder(cyl, 2., 2., 1.5);
			secpainted->Child(new CGrTranslate(0., 1.5*double(x)+.7, 0., cyl));
		}
		else
		{
			CreateCylinder(cyl, 2.5, 2.5, 2.);
			secpainted->Child(new CGrTranslate(0., 2*double(x)+1., 0., cyl));
		}
	}
	
	CGrPtr<CGrMaterial> secpaint = new CGrMaterial;
	leg->Child(secpaint);

	CGrPtr<CGrComposite> secpainted = new CGrComposite;
	secpaint->Child(secpainted);
	secpaint->AmbientAndDiffuse(1., 0., 0.);
	CShapeFactory foot; 

	if(arm)
		foot.Sphere(2.8, 0);
	else
		foot.Sphere(3.7, 0);
	CGrPtr<CGrObject> foo = foot.SceneGraph();
	secpainted->Child(foo);

}
void CFloatFactory::CreateWin(CGrPtr<CGrObject> &p_window)
{
	CGrPtr<CGrComposite> window = new CGrComposite;
	p_window = window;

	float r, g, b;

	for(int x = 0;  x< 6; x++)
	{
		switch(x)
		{
		case 0:
			r = 1;
			g = 0;
			b = 0;
			break;
		case 1:
			r = 1;
			g = .5f;
			b = 0;
			break;
		case 2:
			r = 1;
			g = 1;
			b = 0;
			break;
		case 3:
			r = 0;
			g = .6f;
			b = 0;
			break;
		case 4:
			r = 0;
			g = 0;
			b = 1;
			break;
		case 5:
			r = .6f;
			g = .1f;
			b = .9f;
			break;
		}
		  
		CGrPtr<CGrMaterial> trimpaint = new CGrMaterial;
		window->Child(trimpaint);

		CGrPtr<CGrComposite> trimpainted = new CGrComposite;
		trimpaint->Child(trimpainted);
		trimpaint->AmbientAndDiffuse(r,g,b);

		CGrPtr<CGrComposite> trim1 = new CGrComposite;
		CreateFlatTrim(trim1, 28.-2*x, 26.-2*x, 1);
		trimpainted->Child(new CGrTranslate(0., -x, 0., trim1));
	}
	
	CGrPtr<CGrMaterial> trimpaint = new CGrMaterial;
	window->Child(trimpaint);

	CGrPtr<CGrComposite> trimpainted = new CGrComposite;
	trimpaint->Child(trimpainted);
	trimpaint->AmbientAndDiffuse(.6f, .1f, .9f);	

	CGrPtr<CGrObject> cyl;
	CreateCylinder(cyl, .5, .5, 16.);
	trimpainted->Child(new CGrTranslate(0., -5.5, -8., new CGrRotate(90, 1., 0., 0., cyl)));
	trimpainted->Child(new CGrTranslate(-8., -5.5, 0., new CGrRotate(-90, 0., 0., 1., cyl)));

	CGrPtr<CGrMaterial> sillpaint = new CGrMaterial;
	window->Child(sillpaint);

	CGrPtr<CGrComposite> sillpainted = new CGrComposite;
	sillpaint->Child(sillpainted);
	sillpaint->AmbientAndDiffuse(1, 0., 0.);

	CGrPtr<CGrComposite> sill = new CGrComposite;
	sill->Box(-14., 0., 14., 28., 2., 2.);
	sillpainted->Child( sill);
	sillpainted->Child( sill);

}
void CFloatFactory::CreateFlatTrim(CGrPtr<CGrComposite> &trim, double lenout, double lenin, double width)
{
	double out = lenout/2.;
	double in = lenin/2.;

	CGrPtr<CGrPolygon> poly1;
	poly1 = new CGrPolygon;
	trim->Child(poly1);
	poly1->AddVertex3d(-out, 0., -out);
	poly1->AddVertex3d(-out, 0., out);
	poly1->AddVertex3d(-in, 0., in);
	poly1->AddVertex3d(-in, 0., -in);
	poly1->ComputeNormal();

	CGrPtr<CGrPolygon> poly2;
	poly2 = new CGrPolygon;
	trim->Child(poly2);
	poly2->AddVertex3d(out, 0., -out);
	poly2->AddVertex3d(-out, 0., -out);
	poly2->AddVertex3d(-in, 0., -in);
	poly2->AddVertex3d(in, 0., -in);
	poly2->ComputeNormal();

	CGrPtr<CGrPolygon> poly3;
	poly3 = new CGrPolygon;
	trim->Child(poly3);
	poly3->AddVertex3d(out, 0., out);
	poly3->AddVertex3d(out, 0., -out);
	poly3->AddVertex3d(in, 0., -in);
	poly3->AddVertex3d(in, 0., in);
	poly3->ComputeNormal();

	CGrPtr<CGrPolygon> poly4;
	poly4 = new CGrPolygon;
	trim->Child(poly4);
	poly4->AddVertex3d(-out, 0., out);
	poly4->AddVertex3d(out, 0., out);
	poly4->AddVertex3d(in, 0., in);
	poly4->AddVertex3d(-in, 0., in);
	poly4->ComputeNormal();

	CGrPtr<CGrPolygon> poly5;
	poly5 = new CGrPolygon;
	trim->Child(poly5);
	poly5->AddVertex3d(-in, 0., -in);
	poly5->AddVertex3d(-in, 0., in);
	poly5->AddVertex3d(-in, -width, in);
	poly5->AddVertex3d(-in, -width, -in);
	poly5->ComputeNormal();

	CGrPtr<CGrPolygon> poly6;
	poly6 = new CGrPolygon;
	trim->Child(poly6);
	poly6->AddVertex3d(in, 0., -in);
	poly6->AddVertex3d(-in, 0., -in);
	poly6->AddVertex3d(-in, -width, -in);
	poly6->AddVertex3d(in, -width, -in);
	poly6->ComputeNormal();

	CGrPtr<CGrPolygon> poly7;
	poly7 = new CGrPolygon;
	trim->Child(poly7);
	poly7->AddVertex3d(in, 0.,in);
	poly7->AddVertex3d(in, 0., -in);
	poly7->AddVertex3d(in, -width, -in);
	poly7->AddVertex3d(in, -width, in);
	poly7->ComputeNormal();

	CGrPtr<CGrPolygon> poly8;
	poly8 = new CGrPolygon;
	trim->Child(poly8);
	poly8->AddVertex3d(-in, 0., in);
	poly8->AddVertex3d(in, 0., in);
	poly8->AddVertex3d(in, -width, in);
	poly8->AddVertex3d(-in, -width, in);
	poly8->ComputeNormal();

}

void CFloatFactory::CreateStar(CGrPtr<CGrComposite> &star, double radius, double width)
{
	double angle = .3142;
	double lilrad = radius/2.618;
	double angle2 = angle+.628;

	CGrPtr<CGrPolygon> poly1;
	poly1 = new CGrPolygon;
	star->Child(poly1);

	poly1->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), width/2.);
	poly1->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), width/2.);
	poly1->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), width/2.);
	poly1->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), width/2.);
	poly1->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), width/2.);
	poly1->ComputeNormal();

	CGrPtr<CGrPolygon> poly2;
	poly2 = new CGrPolygon;
	star->Child(poly2);
	
	poly2->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), width/2.);
	poly2->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), width/2.);
	poly2->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), width/2.);
	poly2->ComputeNormal();

	CGrPtr<CGrPolygon> poly3;
	poly3 = new CGrPolygon;
	star->Child(poly3);

	glBegin(GL_POLYGON);
	poly3->AddVertex3d(0.+lilrad*cos(1.257+angle2), 0.+lilrad*sin(1.257+angle2), width/2.);
	poly3->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), width/2.);
	poly3->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), width/2.);
	poly3->ComputeNormal();

	CGrPtr<CGrPolygon> poly4;
	poly4 = new CGrPolygon;
	star->Child(poly4);

	poly4->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), width/2.);
	poly4->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), width/2.);
	poly4->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), width/2.);
	poly4->ComputeNormal();


	CGrPtr<CGrPolygon> poly5;
	poly5 = new CGrPolygon;
	star->Child(poly5);

	poly5->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), width/2.);
	poly5->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), width/2.);
	poly5->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), width/2.);
	poly5->ComputeNormal();

	CGrPtr<CGrPolygon> poly6;
	poly6 = new CGrPolygon;
	star->Child(poly6);

	poly6->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), width/2.);
	poly6->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), width/2.);
	poly6->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), width/2.);
	poly6->ComputeNormal();

	CGrPtr<CGrPolygon> poly7;
	poly7 = new CGrPolygon;
	star->Child(poly7);
	
	poly7->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), -width/2.);
	poly7->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), -width/2.);
	poly7->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), -width/2.);
	poly7->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), -width/2.);
	poly7->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), -width/2.);
	poly7->ComputeNormal();

	CGrPtr<CGrPolygon> poly8;
	poly8 = new CGrPolygon;
	star->Child(poly8);
	
	poly8->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), -width/2.);
	poly8->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), -width/2.);
	poly8->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), -width/2.);
	poly8->ComputeNormal();

	CGrPtr<CGrPolygon> poly9;
	poly9 = new CGrPolygon;
	star->Child(poly9);

	glBegin(GL_POLYGON);
	poly9->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), -width/2.);
	poly9->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), -width/2.);
	poly9->AddVertex3d(0.+lilrad*cos(1.257+angle2), 0.+lilrad*sin(1.257+angle2), -width/2.);
	poly9->ComputeNormal();

	CGrPtr<CGrPolygon> poly10;
	poly10 = new CGrPolygon;
	star->Child(poly10);

	poly10->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), -width/2.);	
	poly10->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), -width/2.);
	poly10->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), -width/2.);
	poly10->ComputeNormal();


	CGrPtr<CGrPolygon> poly11;
	poly11 = new CGrPolygon;
	star->Child(poly11);

	poly11->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), -width/2.);
	poly11->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), -width/2.);
	poly11->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), -width/2.);
	poly11->ComputeNormal();

	CGrPtr<CGrPolygon> poly12;
	poly12 = new CGrPolygon;
	star->Child(poly12);

	poly12->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), -width/2.);
	poly12->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), -width/2.);
	poly12->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), -width/2.);
	poly12->ComputeNormal();

	CGrPtr<CGrPolygon> poly13;
	poly13 = new CGrPolygon;
	star->Child(poly13);

	poly13->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), width/2.);
	poly13->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), width/2.);
	poly13->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), -width/2.);
	poly13->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), -width/2.);
	poly13->ComputeNormal();

CGrPtr<CGrPolygon> poly14;
	poly14 = new CGrPolygon;
	star->Child(poly14);

	poly14->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), width/2.);
	poly14->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), width/2.);
	poly14->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), -width/2.);
	poly14->AddVertex3d(0.+lilrad*cos(4*1.257+angle2), 0.+lilrad*sin(4*1.257+angle2), -width/2.);
	poly14->ComputeNormal();

CGrPtr<CGrPolygon> poly15;
	poly15 = new CGrPolygon;
	star->Child(poly15);

	poly15->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), width/2.);
	poly15->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), width/2.);
	poly15->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), -width/2.);
	poly15->AddVertex3d(0.+radius*cos(4*1.257+angle), 0.+radius*sin(4*1.257+angle), -width/2.);
	poly15->ComputeNormal();

CGrPtr<CGrPolygon> poly16;
	poly16= new CGrPolygon;
	star->Child(poly16);

	poly16->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), width/2.);
	poly16->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), width/2.);
	poly16->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), -width/2.);
	poly16->AddVertex3d(0.+lilrad*cos(3*1.257+angle2), 0.+lilrad*sin(3*1.257+angle2), -width/2.);
	poly16->ComputeNormal();

CGrPtr<CGrPolygon> poly17;
	poly17 = new CGrPolygon;
	star->Child(poly17);

	poly17->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), width/2.);
	poly17->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), width/2.);
	poly17->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), -width/2.);
	poly17->AddVertex3d(0.+radius*cos(3*1.257+angle), 0.+radius*sin(3*1.257+angle), -width/2.);
	poly17->ComputeNormal();

CGrPtr<CGrPolygon> poly18;
	poly18 = new CGrPolygon;
	star->Child(poly18);

	poly18->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), width/2.);
	poly18->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), width/2.);
	poly18->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), -width/2.);
	poly18->AddVertex3d(0.+lilrad*cos(2*1.257+angle2), 0.+lilrad*sin(2*1.257+angle2), -width/2.);
	poly18->ComputeNormal();

CGrPtr<CGrPolygon> poly19;
	poly19 = new CGrPolygon;
	star->Child(poly19);

	poly19->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), width/2.);
	poly19->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), width/2.);
	poly19->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), -width/2.);
	poly19->AddVertex3d(0.+radius*cos(2*1.257+angle), 0.+radius*sin(2*1.257+angle), -width/2.);
	poly19->ComputeNormal();

CGrPtr<CGrPolygon> poly20;
	poly20 = new CGrPolygon;
	star->Child(poly20);

	poly20->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), width/2.);
	poly20->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), width/2.);
	poly20->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), -width/2.);
	poly20->AddVertex3d(0.+lilrad*cos(1*1.257+angle2), 0.+lilrad*sin(1*1.257+angle2), -width/2.);
	poly20->ComputeNormal();

CGrPtr<CGrPolygon> poly21;
	poly21 = new CGrPolygon;
	star->Child(poly21);

	poly21->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), width/2.);	
	poly21->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), width/2.);
	poly21->AddVertex3d(0.+radius*cos(angle), 0.+radius*sin(angle), -width/2.);
	poly21->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), -width/2.);	
	poly21->ComputeNormal();

CGrPtr<CGrPolygon> poly22;
	poly22 = new CGrPolygon;
	star->Child(poly22);

	poly22->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), width/2.);
	poly22->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), width/2.);	
	poly22->AddVertex3d(0.+lilrad*cos(angle2), 0.+lilrad*sin(angle2), -width/2.);	
	poly22->AddVertex3d(0.+radius*cos(1.257+angle), 0.+radius*sin(1.257+angle), -width/2.);
	poly22->ComputeNormal();
}


void CFloatFactory::CreatePlant(CGrPtr<CGrObject> &p_plant, float r, float g, float b)
{
	CGrPtr<CGrComposite> plant = new CGrComposite;
	p_plant = plant;

	CGrPtr<CGrMaterial> berrypaint = new CGrMaterial;
	plant->Child(berrypaint);
	berrypaint->AmbientAndDiffuse(r, g, b);

	CGrPtr<CGrComposite> berrypainted = new CGrComposite;
	berrypaint->Child(berrypainted);

	CShapeFactory ring;
	ring.RingofSpheres(1., 3.5, 0);
	CGrPtr<CGrObject> ring1 = ring.SceneGraph();
	berrypainted->Child(ring1);

	ring.RingofSpheres(1., 2.6, 0);
	CGrPtr<CGrObject> ring2 = ring.SceneGraph();
	berrypainted->Child(new CGrTranslate(0., 1.8, 0., ring2));

	ring.RingofSpheres(1., 1.7, 0);
	CGrPtr<CGrObject> ring3 = ring.SceneGraph();
	berrypainted->Child(new CGrTranslate(0., 3.6, 0., ring3));

	ring.RingofSpheres(1., 1., 0);
	CGrPtr<CGrObject> ring4 = ring.SceneGraph();
	berrypainted->Child(new CGrTranslate(0., 5.4, 0., ring4));

	ring.RingofSpheres(1., .4, 0);
	CGrPtr<CGrObject> ring5 = ring.SceneGraph();
	berrypainted->Child(new CGrTranslate(0., 7.2, 0.,ring5));

	 CGrPtr<CGrMaterial> branchpaint = new CGrMaterial;
	 plant->Child(branchpaint);
	 branchpaint->AmbientAndDiffuse(0., .5, 0.);

	 CGrPtr<CGrComposite> branchpainted = new CGrComposite;
	 branchpaint->Child(branchpainted);

	 ring.RingofCylinders(.2, .1, 3.5, -15., int(3.14159*3.5));
	 CGrPtr<CGrObject> ring6 = ring.SceneGraph();
	 branchpainted->Child(new CGrTranslate(0., 1., 0., new CGrRotate(18, 0., 1., 0., ring6)));
	
	 ring.RingofCylinders(.18, .9, 2.6, 0., int(3.14159*2.6));
	 CGrPtr<CGrObject> ring7 = ring.SceneGraph();
	 branchpainted->Child(new CGrTranslate(0., 1.8, 0., ring7));

	 ring.RingofCylinders(.15, .075, 1.7, 10., int(3.14159*1.7));
	 CGrPtr<CGrObject> ring8 = ring.SceneGraph();
	 branchpainted->Child(new CGrTranslate(0., 3.2, 0., ring8));

	 ring.RingofCylinders(.12, .06, 1., 20., int(3.14159));
	 CGrPtr<CGrObject> ring9 = ring.SceneGraph();
	 branchpainted->Child(new CGrTranslate(0., 5.0, 0., ring9));

	 ring.CreateCylinder(.5, .1, 12.);
	 CGrPtr<CGrObject> stem = ring.SceneGraph();
	 branchpainted->Child(new CGrTranslate(0., -5., 0., stem));

	CGrPtr<CGrMaterial> potpaint = new CGrMaterial;
	plant->Child(potpaint);
	float ambientpot[3] = {.5, .5, .5};
	float specularwhite[3] = {1., 1., 1.};
	potpaint->AmbientDiffuseSpecularShininess(ambientpot, ambientpot, specularwhite, 1.);

	CGrPtr<CGrComposite> potpainted = new CGrComposite;
	potpaint->Child(potpainted);
	
	 ring.CreateCylinder(4., 5.5, 6., true, m_concrete, m_mud);
	 CGrPtr<CGrObject> pot = ring.SceneGraph();
	 berrypainted->Child(new CGrTranslate(0., -12., 0., pot));	

	 ring.CreateHollowCylinder(6., 6., 4.0, 4., 2., m_concrete);
	 CGrPtr<CGrObject> toppot = ring.SceneGraph();
	 potpainted->Child(new CGrTranslate(0., -6., 0., toppot));

}