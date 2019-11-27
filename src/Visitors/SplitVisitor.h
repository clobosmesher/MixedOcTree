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

#ifndef SplitVisitor_h
#define SplitVisitor_h 1

#include <list>
#include <set>
#include <vector>

#include "../MeshPoint.h"
#include "../Point3D.h"
#include "../OctreeEdge.h"

#include "Visitor.h"


using Clobscode::MeshPoint;
using Clobscode::OctreeEdge;
using Clobscode::Point3D;
using std::vector;
using std::list;
using std::set;


namespace Clobscode
{
    class SplitVisitor : public Visitor {

    public:
        SplitVisitor();
        /*void VisitOctant(Octant *o,
                         vector<MeshPoint> &points,
                         list<Point3D> &new_pts,
                         set<OctreeEdge> &edges,
                         vector< vector<unsigned int> > &new_eles,
                         vector<vector<Point3D> > &clipping);*/

        bool visit(Octant *o);

        void setPoints(vector<MeshPoint> &points);
        void setNewPts(list<Point3D> &new_pts);
        void setEdges(set<OctreeEdge> &edges);
        void setNewEles(vector<vector<unsigned int> > &new_eles);
        void setClipping(vector<vector<Point3D> > &clipping);

    protected:
        //references
        vector<MeshPoint> *points;
        list<Point3D> *new_pts;
        set<OctreeEdge> *edges;
        vector<vector<unsigned int> > *new_eles;
        vector<vector<Point3D> > *clipping;

        bool splitEdge(const unsigned int &idx1,
                       const unsigned int &idx2,
                       unsigned int &c_n_pts,
                       unsigned int &mid_idx);

        bool splitFace(const unsigned int &idx1,
                       const unsigned int &idx2,
                       const unsigned int &idx3,
                       const unsigned int &idx4,
                       unsigned int &c_n_pts,
                       unsigned int &mid_idx);


    };
}



#endif //MESHER_ROI_SPLITVISITOR_H
