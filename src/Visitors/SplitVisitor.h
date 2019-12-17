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
#include <map>
#include <vector>
#include <limits>

#include "../MeshPoint.h"
#include "../Point3D.h"
#include "../OctreeEdge.h"
#include "../EdgeInfo.h"

#include "Visitor.h"


using Clobscode::MeshPoint;
using Clobscode::OctreeEdge;
using Clobscode::EdgeInfo;
using Clobscode::Point3D;
using std::vector;
using std::list;
using std::map;


namespace Clobscode
{
    class SplitVisitor : public Visitor {

    public:
        SplitVisitor();

        bool visit(Octant *o);

        void setPoints(vector<MeshPoint> &points);
        void setNewPts(list<Point3D> &new_pts);
        void setMapEdges(map<OctreeEdge, EdgeInfo> &MapEdges);
        void setNewEles(vector<vector<unsigned int> > &new_eles);
        void setStartIndex(const unsigned int &sidx);
        void setClipping(vector<vector<Point3D> > &clipping);
        void setProcessedOctVector(vector<Octant> &proOctVec);
        void setMapProcessed(map<unsigned int, unsigned int> &proOctMap);
        void setToBalanceList(list<pair<unsigned int,unsigned int> > &unBalanced);

    protected:
        //references
        vector<MeshPoint> *points;
        list<Point3D> *new_pts;
        map<OctreeEdge, EdgeInfo> *MapEdges;
        vector<vector<unsigned int> > *new_eles;
        vector<vector<Point3D> > *clipping;
        vector<Octant> *proOctVec;
        map<unsigned int, unsigned int> *proOctMap;
        list<pair<unsigned int,unsigned int> > *unBalanced;
        
        unsigned int idx;
        

        bool splitEdge(const unsigned int &idx1, const unsigned int &idx2,
                       const unsigned int &sub1, const unsigned int &sub2,
                       const unsigned int &pos, const unsigned int &myrl,
                       unsigned int &c_n_pts, unsigned int &mid_idx,
                       unsigned int &o_idx);

        bool splitFaceType1(const vector<unsigned int> &fpts,
                            const vector<unsigned int> &sons,
                            const unsigned int &neighbor,
                            unsigned int &c_n_pts, unsigned int &mid_idx);
        
        bool splitFaceType2(const vector<unsigned int> &fpts,
                            const vector<unsigned int> &sons,
                            const unsigned int &neighbor,
                            unsigned int &c_n_pts, unsigned int &mid_idx);


    };
}



#endif //MESHER_ROI_SPLITVISITOR_H
