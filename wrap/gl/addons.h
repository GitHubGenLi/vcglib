/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
Histoy

****************************************************************************/
#ifndef __VCG_GLADDONS
#define __VCG_GLADDONS

#include <glut.h>
#include <wrap/gl/math.h>
#include <wrap/gl/space.h>
#include <vcg/space/point3.h>
#include <map>

namespace vcg
{

	/** Class Add_Ons.
	This is class draw 3d icons on the screen
	*/

	class Add_Ons
	{
	public:
		enum DrawMode  {DMUser,DMWire,DMSolid} ;
	private:

		///used to find right trasformation in case of rotation 
		static void XAxis(  Point3d  zero,  Point3d  uno, Matrix44d & tr){
			tr.SetZero();
			*((Point3d*)&tr[0][0]) = uno-zero;
			GetUV(*((Point3d*)tr[0]),*((Point3d*)tr[1]),*((Point3d*)tr[2]));
			tr[3][3] = 1.0;
			*((Point3d*)&tr[3][0]) = zero;
		}

		//set drawingmode parameters
		static void SetGLParameters(DrawMode DM)
		{
			switch(DM)
			{
			case DMUser		  :
				break;
			case DMWire		  :	
				glDisable(GL_CULL_FACE);
				glDisable(GL_LIGHTING);
				glDisable(GL_NORMALIZE);
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case DMSolid	  :
				glDisable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT,GL_FILL);
				break;
			default : 
				break;
			}
		}

		///draw a cylinder
		static void Cylinder(int slices,double lenght,double width,bool useDisplList)
		{
			static std::map<int,GLint> Disp_listMap;
			GLint cyl_List=-1;
			std::map<int,GLint>::const_iterator it=Disp_listMap.find(slices);
			///if the diplay list is createdtake the Glint that identify it
			bool to_insert=false;
			if (useDisplList)
			{
				if (it!=Disp_listMap.end())///the list exist
					cyl_List=it->second;
				else to_insert=true;
			}

			glScaled(lenght,width,width);

			if (((!glIsList(cyl_List))&&(useDisplList))||(!useDisplList))
			{
				if (useDisplList)
				{
					cyl_List = glGenLists(1);
					glNewList(cyl_List, GL_COMPILE);
				}
				int b;
				vcg::Point3d p0;
				vcg::Point3d p1;

				double step=6.28/(double)slices;
				double angle=0;
				glBegin(GL_TRIANGLE_STRIP);
				for(b = 0; b <= slices-1; ++b){
					//double angle = 6.28*b/(double)lenght;
					//double angle = 6.28*(double)b;
					//angle+=step;
					//p0 = Point3d( 0, width * sin(angle),width * cos(angle));
					p0 = Point3d( 0, sin(angle),cos(angle));
					//p1 = p0; p1[0] = lenght;
					p1 = p0; p1[0] = 1.f;
					glNormal3f(p0[0],p0[1],p0[2]);
					glVertex3d(p0[0],p0[1],p0[2]);
					glVertex3d(p1[0],p1[1],p1[2]);
					angle+=step;
				}
				///re-conjunction with first point of cylinder
				glNormal3f(0,0,1);
				glVertex3d(0,0,1);
				glVertex3d(1,0,1);

				glEnd();

				///fill the cylinder down
				angle=0;
				p0=vcg::Point3d(0,0,0);
				glBegin(GL_TRIANGLE_FAN);
				glNormal3f(-1,0,0);
				glVertex3d(p0[0],p0[1],p0[2]);
				for(b = 0; b <= slices-1; ++b){
					glNormal3f(-1,0,0);
					p1 = Point3d( 0, sin(angle),cos(angle));
					glVertex3d(p1[0],p1[1],p1[2]);
					angle+=step;
				}
				glNormal3f(-1,0,0);
				glVertex3d(0,0,1);
				glEnd();

				angle=0;
				p0=vcg::Point3d(1,0,0);
				glBegin(GL_TRIANGLE_FAN);
				glNormal3f(1,0,0);
				glVertex3d(p0[0],p0[1],p0[2]);
				for(b = 0; b <= slices-1; ++b){
					glNormal3f(1,0,0);
					p1 = Point3d( 1, sin(angle),cos(angle));
					glVertex3d(p1[0],p1[1],p1[2]);
					angle+=step;
				}
				glNormal3f(1,0,0);
				glVertex3d(1,0,1);
				glEnd();

		
				if (useDisplList)
					glEndList();
			}
			if (useDisplList)
			{
				glCallList(cyl_List);
				///I insert key and value in the map if I need
				if (to_insert)
					Disp_listMap.insert(std::pair<int,GLint>(slices,cyl_List));
			}
		}

		static void Diamond (double radius,bool useDisplList)
		{ 
			static GLint diam_List=-1;

			glScaled(radius,radius,radius);
			if (((!glIsList(diam_List))&&(useDisplList))||(!useDisplList))
			{
				if (useDisplList)
				{
					diam_List = glGenLists(1);
					glNewList(diam_List, GL_COMPILE);
				}

				glBegin(GL_TRIANGLE_FAN);
				glNormal3f( 0.0, 1,  0.0);
				glVertex3f(0.0,1,0.0);

				glNormal3f( 1, 0.0,  0.0);
				glVertex3f( 1, 0.0, 0.0);

				glNormal3f( 0.0, 0.0, -1);
				glVertex3f( 0.0, 0.0,-1);

				glNormal3f(-1, 0.0 , 0.0);
				glVertex3f(-1, 0.0, 0.0);

				glNormal3f( 0.0, 0.0, 1);
				glVertex3f( 0.0, 0.0, 1);

				glNormal3f( 1, 0.0,  0.0);
				glVertex3f( 1, 0.0, 0.0);

				glEnd();

				glBegin(GL_TRIANGLE_FAN);
				glNormal3f( 0.0, 1,  0.0);
				glVertex3f( 0.0,-1, 0.0);

				glNormal3f( 1, 0.0,  0.0);
				glVertex3f( 1, 0.0, 0.0);

				glNormal3f( 0.0, 0.0, 1);
				glVertex3f( 0.0, 0.0, 1);

				glNormal3f(-1,0.0 , 0.0);
				glVertex3f(-1, 0.0, 0.0);

				glNormal3f( 0.0,0.0, -1);
				glVertex3f( 0.0, 0.0,-1);

				glNormal3f( 1, 0.0,  0.0);
				glVertex3f( 1, 0.0, 0.0);
				glEnd();
				if (useDisplList)
					glEndList();
			}
			if (useDisplList)
				glCallList(diam_List);
		}

		///draw a cone
		static void Cone(int slices,double lenght,double width,bool useDisplList)
		{
			static std::map<int,GLint> Disp_listMap;
			GLint cone_List=-1;
			std::map<int,GLint>::const_iterator it=Disp_listMap.find(slices);
			///if the diplay list is createdtake the Glint that identify it
			bool to_insert=false;

			if (useDisplList)
			{
				if (it!=Disp_listMap.end())///the list exist
					cone_List=it->second;
				else to_insert=true;
			}

			glScaled(lenght,width,width);

			if (((!glIsList(cone_List))&&(useDisplList))||(!useDisplList))
			{
				int h=1;
				vcg::Point3f p0;
                vcg::Point3f P[2];
				vcg::Point3f N[2];

				glScaled(lenght,width,width);
				if (useDisplList)
				{
					cone_List = glGenLists(1);
					glNewList(cone_List, GL_COMPILE);
				}
				for(h=0; h < 2; ++h)
				{
					
					//glBegin(GL_TRIANGLE_FAN);
					p0 = Point3f(0,0,0);
					if(h==0) p0[0]+=1.f;
					//glNormal3f(1,0.0,0.0);
					//glVertex3d(p0[0],p0[1],p0[2]);
					 N[0]= Point3f( 1.f,sinf(0),cosf(0) );
					 P[0]= Point3f( 0,sinf(0),cosf(0));
					int b;
					for(b = 1; b <= slices; ++b)
					{
						double angle = -6.28*b/(double)slices;
						if (b==slices) angle=0;
						N[1] = Point3f( 1.f, sinf(angle), cosf(angle) );
						P[1] = Point3f( 0,   sinf(angle), cosf(angle));
						
						glBegin(GL_TRIANGLES);
						  Point3f n =  ( (P[0]-p0) ^ (P[2]-p0) ).Normalize();
						  glNormal3f(n[0],n[1],n[2]);
						  glVertex3f(p0[0],p0[1],p0[2]);
						  glNormal3f(N[0][0],N[0][1],N[0][2]);
						  glVertex3f(P[0][0],P[0][1],P[0][2]);
						  glNormal3f(N[1][0],N[1][1],N[1][2]);
						  glVertex3f(P[1][0],P[1][1],P[1][2]);
						glEnd();
                        N[0] = N[1];
						P[0] = P[1];
					}
					glEnd();
				}
				if (useDisplList)
					glEndList();
			}
			if (useDisplList)
			{
				glCallList(cone_List);
				///I insert key and value in the map if I need
				if (to_insert)
					Disp_listMap.insert(std::pair<int,GLint>(slices,cone_List));
			}
		}

	public:

		/// draw an arrow from tail to head
		/// body_width = width of the body of arrow
		/// head_lenght = lenght of the head of arrow
		/// head_width = width of the head of arrow
		/// body_slice = number of slices on the body
		/// head_slice = number of slices on the head
		template <DrawMode dm>
			static void glArrow(Point3d tail, Point3d head,double body_width,double head_lenght,
			double head_width,int body_slice=10,int head_slice=10,bool useDisplList=true)
		{
			if (tail!=head)
			{
				//assert(!glGetError());
				Matrix44d tr;
				XAxis(tail,head,tr);
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				SetGLParameters(dm);
				glPushMatrix();		
				glMultMatrixd(&tr[0][0]);
				vcg::Point3d Direct=(head-tail);
				double l_body=Direct.Norm()-head_lenght;
				glPushMatrix();
				glTranslate(vcg::Point3d(tail.Norm(),0,0));
				Cylinder(body_slice,l_body,body_width,useDisplList);
				glPopMatrix();
				glTranslate(vcg::Point3d(l_body,0,0));
				Cone(head_slice,head_lenght,head_width,useDisplList);
				glPopMatrix();
				//assert(!glGetError());
				glPopAttrib();
				//assert(!glGetError());
			}
		}

		/// draw a cone from tail to head
		/// width = width of the base of the cone
		/// slice = number of slices on the cone
		template <DrawMode dm>
			static void glCone(Point3d tail, Point3d head,double width,int slice=10,bool useDisplList=true)
		{
			if (tail!=head)
			{
				Matrix44d tr;								   
				XAxis(tail,head,tr);
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				SetGLParameters(dm);
				glPushMatrix();
				glMultMatrixd(&tr[0][0]);
				vcg::Point3d Direct=(head-tail);
				double l_body=Direct.Norm();
				glTranslate(vcg::Point3d(tail.Norm(),0,0));
				Cone(slice,l_body,width,useDisplList);	
				glPopMatrix();
				glPopAttrib();
			}
		}

		/// draw a cylinder from tail to head
		/// width = width of the base of the cylinder
		/// slice = number of slices on the cylinder
		template <DrawMode dm>
			static void glCylinder(Point3d tail, Point3d head,double width,int slice=10,bool useDisplList=true)
		{
			if (tail!=head)
			{
				Matrix44d tr;								   
				XAxis(tail,head,tr);						   
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				SetGLParameters(dm);
				glPushMatrix();
				glMultMatrixd(&tr[0][0]);
				vcg::Point3d Direct=(head-tail);
				double l_body=Direct.Norm();
				glTranslate(vcg::Point3d(tail.Norm(),0,0));
				Cylinder(slice,l_body,width,useDisplList);
				glPopMatrix();
				glPopAttrib();
			}

		}

		/// draw a point in Center
		/// size   = Radius of the point
		/// slices = The number of subdivisions around the Z axis (similar to lines of longitude). 
		/// stacks = The number of subdivisions along the Z axis (similar to lines of latitude). 
		template <DrawMode dm>
			static void glPoint(vcg::Point3f Center,float size,int slices =16,int stacks =16)
		{
			if (size!=0){
				glPushMatrix();
				glTranslate(Center);
				if (dm==DMWire)
					glutWireSphere(size,slices,stacks);
				else
					if (dm==DMSolid)
						glutSolidSphere(size,slices,stacks);
					else
						glutSolidSphere(size,slices,stacks);
				glPopMatrix();
			}
		}

		/// draw a point in Center
		/// size   = Radius of the point
		/// slices = The number of subdivisions around the Z axis (similar to lines of longitude). 
		/// stacks = The number of subdivisions along the Z axis (similar to lines of latitude). 
		template <DrawMode dm>
			static void glDiamond (Point3f Center, double size,bool useDisplList=true)
		{
			if (size!=0){
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				SetGLParameters(dm);
				glPushMatrix();
				glTranslated(Center[0],Center[1],Center[2]);
				Diamond(size,useDisplList);
				glPopMatrix();
				glPopAttrib();
			}
		}
	};
}
#endif