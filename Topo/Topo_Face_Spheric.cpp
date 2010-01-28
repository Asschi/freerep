//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Spheric.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"

#include <math.h>

#define tao 1.61803399

Geom_Vec3 icosahedron_verts[] = {
	Geom_Vec3(1,tao,0),Geom_Vec3(-1,tao,0),Geom_Vec3(1,-tao,0),
	Geom_Vec3(-1,-tao,0),Geom_Vec3(0,1,tao),Geom_Vec3(0,-1,tao),
	Geom_Vec3(0,1,-tao),Geom_Vec3(0,-1,-tao),Geom_Vec3(tao,0,1),
	Geom_Vec3(-tao,0,1),Geom_Vec3(tao,0,-1),Geom_Vec3(-tao,0,-1)};

int icosahedron_faces[][3] = {
	{0,1,4},{1,9,4},{4,9,5},
	{5,9,3},{2,3,7},{3,2,5},
	{7,10,2},{0,8,10},{0,4,8},
	{8,2,10},{8,4,5},{8,5,2},
	{1,0,6},{11,1,6},{3,9,11},
	{6,10,7},{3,11,7},{11,6,7},
	{6,0,10},{9,1,11}};
	
Topo_Face_Spheric::Topo_Face_Spheric()
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(Geom_Vec3 c, double radius)
{
	m_C = c; m_radius = radius;	
}

Topo_Face_Spheric::Topo_Face_Spheric(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(Geom_Plane plane)
{
	m_plane = plane;
	//TODO: get the radius from somewhere
	m_radius = 1;
	m_C = Geom_Vec3(0,0,0);	
}

void Topo_Face_Spheric::ProjectPoint(const Geom_Vec3 &pnt, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	Geom_Vec3 norm = (pnt - m_C).Normalized();
	pRet((norm * m_radius) + m_C, norm);
}

void Topo_Face_Spheric::OutputTri(Geom_Vec3 &pnt1, Geom_Vec3 &pnt2, Geom_Vec3 &pnt3, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	ProjectPoint(pnt1,pRet);
	ProjectPoint(pnt2,pRet);
	ProjectPoint(pnt3,pRet);
}

void Topo_Face_Spheric::SplitFace(Geom_Vec3 &pnt1, Geom_Vec3 &pnt2, Geom_Vec3 &pnt3, int ndivisions, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	//TODO: this is just an implementation of loop subdivision. Should be moved
	//another s somewhere as a utillity. 
	if(ndivisions == 1)
	{
		OutputTri(pnt1,pnt2,pnt3,pRet);
		return;
	}
		
	Geom_Vec3 npnt2 = ((pnt1 - pnt2) / ndivisions) + pnt2;
	Geom_Vec3 npnt3 = ((pnt1 - pnt3) / ndivisions) + pnt3;
	
	Geom_Vec3 vec = (npnt3 - npnt2) / (ndivisions - 1);
	
	Geom_Vec3 cbpnt = pnt2;
	Geom_Vec3 ctpnt = npnt2;
	
	for(int i=0; i < ndivisions-1;i++)
	{
		Geom_Vec3 ncbpnt = cbpnt+vec;
		Geom_Vec3 nctpnt = ctpnt+vec;
		OutputTri(cbpnt,ctpnt,ncbpnt,pRet);
		OutputTri(ctpnt,nctpnt,ncbpnt,pRet);
		cbpnt = ncbpnt;
		ctpnt = nctpnt;
	} 
	
	OutputTri(ctpnt,pnt3,cbpnt,pRet);
	
	SplitFace(pnt1,npnt2,npnt3, ndivisions - 1, pRet);
}

const Topo_Face_Spheric *sphere;
void (*pTopoFaceSphericRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm);
Geom_Plane plane;

void TopoFaceSphericVertexAbsorber(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = plane.UnmapPoint(pnt);
	sphere->ProjectPoint(p,pTopoFaceSphericRet);
}

double TopoFaceSphericMetric(const Geom_Vec3 &a, const Geom_Vec3 &b)
{
	//TODO: get radius from the sphere
	double radius=1;
	
	Geom_Vec3 start = a;
	Geom_Vec3 end = b;
	start.m_z = sqrt(radius * radius - start.m_x * start.m_x - start.m_y * start.m_y);
	end.m_z = sqrt(radius * radius - end.m_x * end.m_x - end.m_y * end.m_y);
	
	return (start - end).Norm();
}

Geom_Vec3 TopoFaceSphericSubdivide(const Geom_Vec3 &a, const Geom_Vec3 &b)
{
	return (a+b).Normalized();
}

void TopoFaceSphericVertexMapper(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = plane.MapPoint(pnt);
	MaxEdgeLengthVertexAbsorber(p,argh);
}

void Topo_Face_Spheric::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	double n = M_PI / acos((m_radius - dDeviation) / m_radius);
	double s = 2 * dDeviation / tan(M_PI * (n-2)/ (2 * n));
	
	sphere = this;
	plane = GetPlane();
	pTopoFaceSphericRet = pRet;
	SetupMaxEdgeLength(s,TopoFaceSphericVertexAbsorber,TopoFaceSphericMetric,TopoFaceSphericSubdivide);
	Topo_Face::Triangulate(dDeviation,TopoFaceSphericVertexMapper);
}

void *Topo_Face_Spheric::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Spheric *nface = new Topo_Face_Spheric(this);

	nface->m_C = nface->m_C + dir;
    return nface;
}

