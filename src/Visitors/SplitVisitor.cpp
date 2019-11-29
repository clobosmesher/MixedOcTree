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

#include "SplitVisitor.h"
#include "../Octant.h"

namespace Clobscode
{
    SplitVisitor::SplitVisitor():points(NULL), new_pts(NULL), MapEdges(NULL), new_eles(NULL),clipping(NULL)
    { idx = 0; }

    void SplitVisitor::setPoints(vector<MeshPoint> &points) {
        this->points = &points;
    }
    
    void SplitVisitor::setNewPts(list<Point3D> &new_pts) {
        this->new_pts = &new_pts;
    }
    
    void SplitVisitor::setMapEdges(map<OctreeEdge, EdgeInfo> &MapEdges) {
        this->MapEdges = &MapEdges;
    }
    
    void SplitVisitor::setNewEles(vector<vector<unsigned int> > &new_eles) {
        this->new_eles = &new_eles;
    }
    
    void SplitVisitor::setClipping(vector<vector<Point3D> > &clipping) {
        this->clipping = &clipping;
    }
    
    void SplitVisitor::setStartIndex(const unsigned int &sidx) {
        this->idx = sidx;
    }
    
    void SplitVisitor::setProcessedOctVector(vector<Octant> &proOctVec) {
        this->proOctVec = &proOctVec;
    }
    
    void SplitVisitor::setMapProcessed(map<unsigned int, unsigned int> &proOctMap) {
        this->proOctMap = &proOctMap;
    }
    
    void SplitVisitor::setToBalanceList(list<pair<unsigned int,unsigned int> > &unBalanced) {
        this->unBalanced = &unBalanced;
    }

    bool SplitVisitor::visit(Octant *o)
    {
        
        
        return true;
        
        
        
        //Must update structure
        
        
        
        
        
        
        
        
        //getting variables for modification
        //preferably by reference, to avoid unnecesary copying
        /*vector<unsigned int>&pi = o->pointindex;

        //cout << "Visitor accepted." << endl;

        //from now on, it's all old code, verbatim

        new_eles->reserve(8);

        unsigned int n_pts = points->size() + new_pts->size();
        //the vector containing all nodes of this octant (and sons)
        vector<unsigned int> all_pts(27,0);

        //save the eight nodes of this cube first
        for (unsigned int i=0; i< pi.size(); i++) {
            all_pts[i] = pi[i];
        }

        const Point3D min = points->at(pi[0]).getPoint();
        const Point3D max = points->at(pi[6]).getPoint();
        const Point3D avg = (max-min)/2 + min;

        //inserting node 8 between nodes 0 and 1
        if (splitEdge(all_pts[0],all_pts[1],n_pts,all_pts[8])) {
            //the coordinates of node 8 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],min[1],avg[2]));
        }
        //inserting node 9 between nodes 1 and 2
        if (splitEdge(all_pts[1],all_pts[2],n_pts,all_pts[9])) {
            //the coordinates of node 9 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],max[2]));
        }
        //inserting node 10 between nodes 2 and 3
        if (splitEdge(all_pts[2],all_pts[3],n_pts,all_pts[10])) {
            //the coordinates of node 10 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],min[1],avg[2]));
        }
        //inserting node 11 between nodes 3 and 0
        if (splitEdge(all_pts[0],all_pts[3],n_pts,all_pts[11])) {
            //the coordinates of node 11 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],min[2]));
        }

        //inserting node 12 between nodes 0 and 4
        if (splitEdge(all_pts[0],all_pts[4],n_pts,all_pts[12])) {
            //the coordinates of node 12 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],min[2]));
        }
        //inserting node 13 between nodes 1 and 5
        if (splitEdge(all_pts[1],all_pts[5],n_pts,all_pts[13])) {
            //the coordinates of node 13 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],max[2]));
        }
        //inserting node 14 between nodes 2 and 6
        if (splitEdge(all_pts[2],all_pts[6],n_pts,all_pts[14])) {
            //the coordinates of node 14 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],max[2]));
        }
        //inserting node 15 between nodes 3 and 7
        if (splitEdge(all_pts[3],all_pts[7],n_pts,all_pts[15])) {
            //the coordinates of node 15 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],min[2]));
        }

        //inserting node 16 between nodes 4 and 5
        if (splitEdge(all_pts[4],all_pts[5],n_pts,all_pts[16])) {
            //the coordinates of node 16 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],max[1],avg[2]));
        }
        //inserting node 17 between nodes 5 and 6
        if (splitEdge(all_pts[5],all_pts[6],n_pts,all_pts[17])) {
            //the coordinates of node 17 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],max[2]));
        }
        //inserting node 18 between nodes 6 and 7
        if (splitEdge(all_pts[6],all_pts[7],n_pts,all_pts[18])) {
            //the coordinates of node 18 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],max[1],avg[2]));
        }
        //inserting node 19 between nodes 7 and 0
        if (splitEdge(all_pts[4],all_pts[7],n_pts,all_pts[19])) {
            //the coordinates of node 19 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],min[2]));
        }


        //inserting mid point of faces

        //inserting node 20 between 8-10 and 9-11
        if (splitFace(all_pts[8],all_pts[9],all_pts[10],all_pts[11],
                      n_pts,all_pts[20])) {
            //the coordinates of node 20 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],avg[2]));
        }
        //inserting node 21 between 8-16 and 12-13
        if (splitFace(all_pts[8],all_pts[12],all_pts[16],all_pts[13],
                      n_pts,all_pts[21])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],avg[2]));
        }
        //inserting node 22 between 9-17 and 13-14
        if (splitFace(all_pts[9],all_pts[13],all_pts[17],all_pts[14],
                      n_pts,all_pts[22])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],avg[1],max[2]));
        }
        //inserting node 23 between 10-18 and 14-15
        if (splitFace(all_pts[10],all_pts[14],all_pts[18],all_pts[15],
                      n_pts,all_pts[23])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],avg[2]));
        }
        //inserting node 24 between 11-19 and 12-15
        if (splitFace(all_pts[11],all_pts[15],all_pts[19],all_pts[12],
                      n_pts,all_pts[24])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],avg[1],min[2]));
        }
        //inserting node 25 between 16-18 and 17-19
        if (splitFace(all_pts[16],all_pts[19],all_pts[18],all_pts[17],
                      n_pts,all_pts[25])) {
            //the coordinates of node 20 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],avg[2]));
        }

        //of course all the intern edges and mid point were never inserted
        //before, so this task is performed without asking
        new_pts->push_back(Point3D (avg[0],avg[1],avg[2]));
        all_pts[26] = n_pts;

        OctreeEdge intern_edge1 (all_pts[20],all_pts[25]);
        intern_edge1.updateMidPoint(all_pts[26]);
        OctreeEdge intern_edge2 (all_pts[21],all_pts[23]);
        intern_edge2.updateMidPoint(all_pts[26]);
        OctreeEdge intern_edge3 (all_pts[22],all_pts[24]);
        intern_edge3.updateMidPoint(all_pts[26]);
        edges->insert(intern_edge1);
        edges->insert(intern_edge2);
        edges->insert(intern_edge3);

        edges->insert(OctreeEdge (all_pts[20],all_pts[26]));
        edges->insert(OctreeEdge (all_pts[25],all_pts[26]));
        edges->insert(OctreeEdge (all_pts[21],all_pts[26]));
        edges->insert(OctreeEdge (all_pts[23],all_pts[26]));
        edges->insert(OctreeEdge (all_pts[22],all_pts[26]));
        edges->insert(OctreeEdge (all_pts[24],all_pts[26]));

        //now that all edges were inserted, the elements can be easily built
        vector<unsigned int> son_element (8,0);
        son_element[0]=all_pts[0];
        son_element[1]=all_pts[8];
        son_element[2]=all_pts[20];
        son_element[3]=all_pts[11];

        son_element[4]=all_pts[12];
        son_element[5]=all_pts[21];
        son_element[6]=all_pts[26];
        son_element[7]=all_pts[24];
        new_eles->push_back(son_element);

        son_element[0]=all_pts[8];
        son_element[1]=all_pts[1];
        son_element[2]=all_pts[9];
        son_element[3]=all_pts[20];

        son_element[4]=all_pts[21];
        son_element[5]=all_pts[13];
        son_element[6]=all_pts[22];
        son_element[7]=all_pts[26];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[20];
        son_element[1]=all_pts[9];
        son_element[2]=all_pts[2];
        son_element[3]=all_pts[10];

        son_element[4]=all_pts[26];
        son_element[5]=all_pts[22];
        son_element[6]=all_pts[14];
        son_element[7]=all_pts[23];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[11];
        son_element[1]=all_pts[20];
        son_element[2]=all_pts[10];
        son_element[3]=all_pts[3];

        son_element[4]=all_pts[24];
        son_element[5]=all_pts[26];
        son_element[6]=all_pts[23];
        son_element[7]=all_pts[15];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[12];
        son_element[1]=all_pts[21];
        son_element[2]=all_pts[26];
        son_element[3]=all_pts[24];

        son_element[4]=all_pts[4];
        son_element[5]=all_pts[16];
        son_element[6]=all_pts[25];
        son_element[7]=all_pts[19];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[21];
        son_element[1]=all_pts[13];
        son_element[2]=all_pts[22];
        son_element[3]=all_pts[26];

        son_element[4]=all_pts[16];
        son_element[5]=all_pts[5];
        son_element[6]=all_pts[17];
        son_element[7]=all_pts[25];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[26];
        son_element[1]=all_pts[22];
        son_element[2]=all_pts[14];
        son_element[3]=all_pts[23];

        son_element[4]=all_pts[25];
        son_element[5]=all_pts[17];
        son_element[6]=all_pts[6];
        son_element[7]=all_pts[18];
        new_eles->push_back(son_element);


        son_element[0]=all_pts[24];
        son_element[1]=all_pts[26];
        son_element[2]=all_pts[23];
        son_element[3]=all_pts[15];

        son_element[4]=all_pts[19];
        son_element[5]=all_pts[25];
        son_element[6]=all_pts[18];
        son_element[7]=all_pts[7];
        new_eles->push_back(son_element);

        //extreme nodes of each son to be used by clipping
        //method.
        clipping->reserve(8);
        vector<Point3D> extreme_nodes(2, Point3D ());

        //bottom/left/back son is defined by nodes 0 and 26
        extreme_nodes[0] = Point3D (min[0],min[1],min[2]);
        extreme_nodes[1] = Point3D (avg[0],avg[1],avg[2]);
        clipping->push_back(extreme_nodes);

        //bottom/left/front son is defined by nodes 8 and 22
        extreme_nodes[0] = Point3D (min[0],min[1],avg[2]);
        extreme_nodes[1] = Point3D (avg[0],avg[1],max[2]);
        clipping->push_back(extreme_nodes);

        //bottom/right/front son is defined by nodes 20 and 14
        extreme_nodes[0] = Point3D (avg[0],min[1],avg[2]);
        extreme_nodes[1] = Point3D (max[0],avg[1],max[2]);
        clipping->push_back(extreme_nodes);

        //bottom/right/back son is defined by nodes 11 and 23
        extreme_nodes[0] = Point3D (avg[0],min[1],min[2]);
        extreme_nodes[1] = Point3D (max[0],avg[1],avg[2]);
        clipping->push_back(extreme_nodes);

        //top/left/back son is defined by nodes 12 and 25
        extreme_nodes[0] = Point3D (min[0],avg[1],min[2]);
        extreme_nodes[1] = Point3D (avg[0],max[1],avg[2]);
        clipping->push_back(extreme_nodes);

        //top/left/front son is defined by nodes 21 and 17
        extreme_nodes[0] = Point3D (min[0],avg[1],avg[2]);
        extreme_nodes[1] = Point3D (avg[0],max[1],max[2]);
        clipping->push_back(extreme_nodes);

        //top/right/front son is defined by nodes 26 and 6
        extreme_nodes[0] = Point3D (avg[0],avg[1],avg[2]);
        extreme_nodes[1] = Point3D (max[0],max[1],max[2]);
        clipping->push_back(extreme_nodes);

        //top/right/back son is defined by nodes 24 and 18
        extreme_nodes[0] = Point3D (avg[0],avg[1],min[2]);
        extreme_nodes[1] = Point3D (max[0],max[1],avg[2]);
        clipping->push_back(extreme_nodes);

        return true;*/
    }


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


    bool SplitVisitor::splitEdge(const unsigned int &idx1, const unsigned int &idx2,
                                 unsigned int &c_n_pts, unsigned int &mid_idx){
        
        
        /*OctreeEdge this_edge (idx1,idx2);
        set<OctreeEdge>::iterator found = edges->find(this_edge);

        if ((*found)[2]!=0) {
            //if the edge was already split, then save its mid_point and
            //return false (the current process didn't split the edge)
            mid_idx = (*found)[2];
            return false;
        }

        //this edge is about to be split. Note that no edge can have point index
        //0 as its mid_point. For this reason, we know that the edge was not
        //split by other octant before. The current edge must be replaced in the
        //set by the same one plus a mid_point equal to c_n_pts (current number
        //of points). The coordinates of this new point will be inserted by the
        //split method above. The splitEdge method will only compute the index
        //for this new point and will insert the two new edges (derived from the
        //splitting process of the current edge). Note that c_n_pts will be
        //increased for next splitting process of another edge.

        this_edge.updateMidPoint(c_n_pts++);
        edges->erase(found);
        OctreeEdge e1(this_edge[0],this_edge[2]), e2 (this_edge[2],this_edge[1]);
        edges->insert(this_edge);
        edges->insert(e1);
        edges->insert(e2);
        mid_idx = this_edge[2];*/
        return true;
    }

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

    bool SplitVisitor::splitFace(const unsigned int &idx1, const unsigned int &idx2,
                                 const unsigned int &idx3, const unsigned int &idx4,
                                 unsigned int &c_n_pts, unsigned int &mid_idx){
        
        
        /*OctreeEdge e1 (idx1,idx3);
        set<OctreeEdge>::iterator found = edges->find(e1);

        if (found==edges->end()) {
            //this face wasn't split before->
            e1.updateMidPoint(c_n_pts);
            edges->insert(e1);

            OctreeEdge e2 (idx2, idx4);
            e2.updateMidPoint(c_n_pts);
            edges->insert(e2);

            //splitting edge e1
            edges->insert(OctreeEdge (idx1,c_n_pts));
            edges->insert(OctreeEdge (idx3,c_n_pts));
            //splitting edge e2
            edges->insert(OctreeEdge (idx2,c_n_pts));
            edges->insert(OctreeEdge (idx4,c_n_pts));

            //increase the number fo total points
            mid_idx = c_n_pts++;

            return true;
        }

        //at this point, the face was already split. Update the mid index
        mid_idx = (*found)[2];*/
        return false;
    }

}


