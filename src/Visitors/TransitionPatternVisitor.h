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

#ifndef TransitionPatternVisitor_h
#define TransitionPatternVisitor_h 1

#include "../OctreeEdge.h"
#include "../MeshPoint.h"
#include "../TriMesh.h"

#include "../TransitionTemplate.h"

//#include "../SurfTemplate1.h"
//#include "../SurfTemplate2.h"
//#include "../SurfTemplate3.h"
//#include "../SurfTemplate4.h"
//#include "../SurfTemplate5.h"
//#include "../SurfTemplate6.h"
//#include "../SurfTemplate7.h"
//#include "../SurfPrism.h"
//#include "../SurfPyramid.h"
//#include "../SurfTetrahedron.h"
//#include "../SurfHexahedron.h"

#include "Visitor.h"
#include "EdgeVisitor.h"

#include <list>
#include <set>
#include <vector>

using Clobscode::MeshPoint;
using Clobscode::OctreeEdge;
using Clobscode::TriMesh;
using std::list;
using std::set;
using std::vector;

namespace Clobscode
{
    class TransitionPatternVisitor : public Visitor{
    public:
        TransitionPatternVisitor();
        TransitionPatternVisitor(bool apply_pattern);

        bool visit(Octant *o);

        void setPoints(vector<MeshPoint> &points);
        void setNewPoints(list<Point3D> &new_pts);
        void setEdges(const set<OctreeEdge> &edges);
        void setMaxRefLevel(const unsigned short &max_ref_level);
        void setApplyMode(bool apply_pattern);

    private:
        vector<MeshPoint> *points;
        list<Point3D> *new_pts;
        const set<OctreeEdge> *edges;
        const unsigned short *max_ref_level;
        bool apply_pattern;

    };
}


#endif //MESHER_ROI_TRANSITIONPATTERNVISITOR_H
