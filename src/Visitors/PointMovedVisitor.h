/*
 <Mix-mesher: region type. This program generates a mixed-elements mesh>
 
 Copyright (C) <2013,2017>  <Claudio Lobos>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/gpl.txt>
 */

#ifndef PointMovedVisitor_h
#define PointMovedVisitor_h 1

#include "../MeshPoint.h"
#include "../OctreeEdge.h"
#include "../Point3D.h"
#include "../SurfTriangle.h"
#include "../TriMesh.h"

#include <list>
#include <set>
#include <vector>

#include "Visitor.h"
#include "EdgeVisitor.h"

using Clobscode::TriMesh;
using Clobscode::Point3D;
using Clobscode::MeshPoint;
using Clobscode::OctreeEdge;
using SurfMesh::SurfTriangle;
using std::list;
using std::vector;
using std::set;

namespace Clobscode
{
    class PointMovedVisitor : public Visitor {
    public:
        PointMovedVisitor();

        bool visit(Octant *o);

        void setPoints(vector<MeshPoint> &points);
        void setEdges(const set<OctreeEdge> &edges);
        void setMaxRefLevel(const unsigned short &max_ref_level);

    private:
        vector<MeshPoint> *points;
        const set<OctreeEdge> *edges;
        const unsigned short *max_ref_level;
    };
}


#endif //MESHER_ROI_POINTMOVEDVISITOR_H
