//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Planar.cpp - Author Jon Pry 

#include "Topo_Face_Planar.h"
#include "Geom_Plane.h"
#include "Topo_Edge.h"

Topo_Face_Planar::Topo_Face_Planar(Geom_Plane plane)
{
	m_plane = plane;	
}

Topo_Face_Planar::Topo_Face_Planar(const ICanAssociate *associate):Topo_Face(associate)
{
	m_plane = ((Topo_Face_Planar*)associate)->m_plane;
}

void *Topo_Face_Planar::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face *nface = new Topo_Face_Planar(this);
    std::list<Topo_Edge*>::const_iterator it;
    for(it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        nface->Add((Topo_Edge*)(*it)->MakeTranslatedCopy(dir));
    }

    return nface;
}

double Topo_Face_Planar::Area()
{
	//TODO: implement me	
}