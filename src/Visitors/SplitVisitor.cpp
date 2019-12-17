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

    bool SplitVisitor::visit(Octant *o) {
        
        //getting variables for modification
        //preferably by reference, to avoid unnecesary copying
        vector<unsigned int> &pi = o->pointindex;
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
        
        const unsigned int myrl = o->getRefinementLevel()+1;
        
        //inserting node 8 between nodes 0 and 1
        if (splitEdge(all_pts[0],all_pts[1],idx,idx+1,1,myrl,n_pts,all_pts[8])) {
            //the coordinates of node 8 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],min[1],avg[2]));
        }
        
        //inserting node 9 between nodes 1 and 2
        if (splitEdge(all_pts[1],all_pts[2],idx+1,idx+2,4,myrl,n_pts,all_pts[9])) {
            //the coordinates of node 9 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],max[2]));
        }
        //inserting node 10 between nodes 2 and 3
        if (splitEdge(all_pts[3],all_pts[2],idx+3,idx+2,2,myrl,n_pts,all_pts[10])) {
            //the coordinates of node 10 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],min[1],avg[2]));
        }
        //inserting node 11 between nodes 3 and 0
        if (splitEdge(all_pts[0],all_pts[3],idx,idx+3,1,myrl,n_pts,all_pts[11])) {
            //the coordinates of node 11 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],min[2]));
        }

        //inserting node 12 between nodes 0 and 4
        if (splitEdge(all_pts[0],all_pts[4],idx,idx+4,1,myrl,n_pts,all_pts[12])) {
            //the coordinates of node 12 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],min[2]));
        }
        //inserting node 13 between nodes 1 and 5
        if (splitEdge(all_pts[1],all_pts[5],idx+1,idx+5,2,myrl,n_pts,all_pts[13])) {
            //the coordinates of node 13 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],max[2]));
        }
        //inserting node 14 between nodes 2 and 6
        if (splitEdge(all_pts[2],all_pts[6],idx+2,idx+6,3,myrl,n_pts,all_pts[14])) {
            //the coordinates of node 14 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],max[2]));
        }
        //inserting node 15 between nodes 3 and 7
        if (splitEdge(all_pts[3],all_pts[7],idx+3,idx+7,4,myrl,n_pts,all_pts[15])) {
            //the coordinates of node 15 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],min[2]));
        }

        //inserting node 16 between nodes 4 and 5
        if (splitEdge(all_pts[4],all_pts[5],idx+4,idx+5,4,myrl,n_pts,all_pts[16])) {
            //the coordinates of node 16 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],max[1],avg[2]));
        }
        //inserting node 17 between nodes 5 and 6
        if (splitEdge(all_pts[5],all_pts[6],idx+5,idx+6,3,myrl,n_pts,all_pts[17])) {
            //the coordinates of node 17 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],max[2]));
        }
        //inserting node 18 between nodes 6 and 7
        if (splitEdge(all_pts[7],all_pts[6],idx+7,idx+6,3,myrl,n_pts,all_pts[18])) {
            //the coordinates of node 18 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],max[1],avg[2]));
        }
        //inserting node 19 between nodes 7 and 0
        if (splitEdge(all_pts[4],all_pts[7],idx+4,idx+7,2,myrl,n_pts,all_pts[19])) {
            //the coordinates of node 19 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],min[2]));
        }
        
        
        
        //inserting mid point of faces

        //inserting node 20 between 8-10 and 9-11
        vector<unsigned int> mide1 ({all_pts[8],all_pts[9],all_pts[10],all_pts[11]});
        vector<unsigned int> octs1 ({idx,idx+1,idx+2,idx+3});
        auto ne0 = MapEdges->find(OctreeEdge (all_pts[0],all_pts[1]));
        //ne0->second[4] contains the index of the neighbor Octant that is
        //below the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType1(mide1,octs1,ne0->second[4],n_pts,all_pts[20])) {
            //the coordinates of node 20 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],avg[2]));
        }
        
        //inserting node 21 between 8-16 and 12-13
        vector<unsigned int> mide2 ({all_pts[12],all_pts[16],all_pts[13],all_pts[8]});
        vector<unsigned int> octs2 ({idx,idx+4,idx+5,idx+1});
        //ne0->second[2] contains the index of the neighbor Octant that is
        //to the left of the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType1(mide2,octs2,ne0->second[2],n_pts,all_pts[21])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],avg[2]));
        }
        
        //inserting node 22 between 9-17 and 13-14
        vector<unsigned int> mide3 ({all_pts[9],all_pts[14],all_pts[17],all_pts[13]});
        vector<unsigned int> octs3 ({idx+1,idx+2,idx+6,idx+5});
        auto ne6 = MapEdges->find(OctreeEdge (all_pts[2],all_pts[6]));
        //ne6->second[4] contains the index of the neighbor Octant that is
        //in front of the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType2(mide3,octs3,ne6->second[4],n_pts,all_pts[22])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],avg[1],max[2]));
        }
        //inserting node 23 between 10-18 and 14-15
        vector<unsigned int> mide4 ({all_pts[15],all_pts[18],all_pts[14],all_pts[10]});
        vector<unsigned int> octs4 ({idx+3,idx+7,idx+6,idx+2});
        //ne6->second[2] contains the index of the neighbor Octant that is
        //to the right of the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType2(mide4,octs4,ne6->second[2],n_pts,all_pts[23])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],avg[2]));
        }
        
        //inserting node 24 between 11-19 and 12-15
        vector<unsigned int> mide5 ({all_pts[11],all_pts[15],all_pts[19],all_pts[12]});
        vector<unsigned int> octs5 ({idx,idx+3,idx+7,idx+4});
        auto ne11 = MapEdges->find(OctreeEdge (all_pts[7],all_pts[4]));
        //ne11->second[3] contains the index of the neighbor Octant that is
        //at the back of the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType1(mide5,octs5,ne11->second[3],n_pts,all_pts[24])) {
            //the coordinates of node 21 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],avg[1],min[2]));
        }
        
        //inserting node 25 between 16-18 and 17-19
        vector<unsigned int> mide6 ({all_pts[16],all_pts[17],all_pts[18],all_pts[19]});
        vector<unsigned int> octs6 ({idx+4,idx+5,idx+6,idx+7});
        //ne11->second[1] contains the index of the neighbor Octant that is
        //in top of the current one. This info will be used only in the case that
        //octant haven't been split.
        if (splitFaceType2(mide6,octs6,ne11->second[1],n_pts,all_pts[25])) {
            //the coordinates of node 20 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],avg[2]));
        }
        
        //of course all the intern edges and mid point were never inserted
        //before, so this task is performed without asking
        new_pts->push_back(Point3D (avg[0],avg[1],avg[2]));
        all_pts[26] = n_pts;
        
        //Insert internal parent edges
        vector<unsigned int> nes1 ({0,idx+2,idx+3,idx,idx+1});
        MapEdges->emplace(OctreeEdge (all_pts[20],all_pts[26],true), EdgeInfo (nes1));
        
        vector<unsigned int> nes2 ({0,idx+5,idx+1,idx,idx+4});
        MapEdges->emplace(OctreeEdge (all_pts[21],all_pts[26],true), EdgeInfo (nes2));
        
        vector<unsigned int> nes3({0,idx+6,idx+5,idx+1,idx+2});
        MapEdges->emplace(OctreeEdge (all_pts[22],all_pts[26],true), EdgeInfo (nes3));

        vector<unsigned int> nes4 ({0,idx+6,idx+2,idx+3,idx+7});
        MapEdges->emplace(OctreeEdge (all_pts[23],all_pts[26],true), EdgeInfo (nes4));
        
        vector<unsigned int> nes5 ({0,idx+7,idx+4,idx,idx+3});
        MapEdges->emplace(OctreeEdge (all_pts[24],all_pts[26],true), EdgeInfo (nes5));
        
        vector<unsigned int> nes6 ({0,idx+6,idx+7,idx+4,idx+5});
        MapEdges->emplace(OctreeEdge (all_pts[25],all_pts[26],true), EdgeInfo (nes6));
        
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
        
        return true;
    }


    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    bool SplitVisitor::splitEdge(const unsigned int &idx1, const unsigned int &idx2,
                                 const unsigned int &sub1, const unsigned int &sub2,
                                 const unsigned int &pos, const unsigned int &myrl,
                                 unsigned int &c_n_pts, unsigned int &mid_idx){
        
        
        auto help = MapEdges->find(OctreeEdge (idx1,idx2));
        mid_idx = help->second[0];
        
        //if the edge was already split
        if (mid_idx!=0) {
            
            //update neighbor info in EdgeInfo
            auto e1 = MapEdges->find(OctreeEdge (idx1,mid_idx));
            e1->second[pos] = sub1;
            
            auto e2 = MapEdges->find(OctreeEdge (idx2,mid_idx));
            e2->second[pos] = sub2;
            
            //Important note. It is possible that edges (idxX,mid_idx) have been already
            //split. In order to mantain updated info, it is necessary to check for
            //evetual edge sons and make them congruent.
            unsigned int sub_mid = (e1->second)[0];
            if (sub_mid!=0) {
                (MapEdges->find(OctreeEdge (idx1,sub_mid))->second)[pos] = sub1;
                (MapEdges->find(OctreeEdge (mid_idx,sub_mid))->second)[pos] = sub1;
            }
            
            sub_mid = (e2->second)[0];
            if (sub_mid!=0) {
                (MapEdges->find(OctreeEdge (idx2,sub_mid))->second)[pos] = sub2;
                (MapEdges->find(OctreeEdge (mid_idx,sub_mid))->second)[pos] = sub2;
            }
            
            //return false (the current process didn't split the edge)
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

        help->second[0] = c_n_pts;
        mid_idx = c_n_pts++;
        //get initial edge neighbor octants
        vector<unsigned int> neighbors = help->second.getNeighborOcts();
        
        //the sub-edges are not split:
        neighbors[0] = 0;
        //the neighbors of sub-edges should be the same as parent, changing only
        //in pos with sub1 and sub2 respectively.
        neighbors[pos] = sub1;
        MapEdges->emplace(OctreeEdge (idx1,mid_idx,true), EdgeInfo (neighbors));
        
        neighbors[pos] = sub2;
        MapEdges->emplace(OctreeEdge (idx2,mid_idx,true), EdgeInfo (neighbors));
        
        unsigned int nn = std::numeric_limits<unsigned int>::max();
        
        //now we should check if neighbors were turned unbalanced.
        for (unsigned int i=1; i<5; i++) {
            if (i==pos || neighbors[i]==nn) {
                continue;
            }
            auto found = proOctMap->find(neighbors[i]);
            if (found!=proOctMap->end()) {
                unsigned int nrl = (proOctVec->at(found->second)).getRefinementLevel();
                int diff = nrl - myrl;
                
                if (diff>1 or diff<-1) {
                    unBalanced->push_back(*found);
                }
            }
        }

        return true;
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    bool SplitVisitor::splitFaceType1(const vector<unsigned int> &fpts,
                                      const vector<unsigned int> &sons,
                                      const unsigned int &neighbor,
                                      unsigned int &c_n_pts, unsigned int &mid_idx){
        
        
        auto fe1 = MapEdges->find(OctreeEdge (fpts[0],fpts[2]));

        if (fe1==MapEdges->end()) {
            
            unsigned int nn = std::numeric_limits<unsigned int>::max();
            
            vector<unsigned int> eni1 ({0,sons[1],neighbor,nn,sons[0]});
            MapEdges->emplace(OctreeEdge (fpts[0],c_n_pts,true), EdgeInfo (eni1));
            
            vector<unsigned int> eni2 ({0,sons[2],sons[1],neighbor,nn});
            MapEdges->emplace(OctreeEdge (fpts[1],c_n_pts,true), EdgeInfo (eni2));
            
            vector<unsigned int> eni3 ({0,sons[2],neighbor,nn,sons[3]});
            MapEdges->emplace(OctreeEdge (fpts[2],c_n_pts,true), EdgeInfo (eni3));
            
            vector<unsigned int> eni4 ({0,sons[3],sons[0],neighbor,nn});
            MapEdges->emplace(OctreeEdge (fpts[3],c_n_pts,true), EdgeInfo (eni4));

            //we need to insert an edge that won't be used so if the neighbor
            //is refined, it will know this face was already split.
            MapEdges->emplace(OctreeEdge (fpts[0],fpts[2]), EdgeInfo (0,c_n_pts));
            MapEdges->emplace(OctreeEdge (fpts[1],fpts[3]), EdgeInfo (0,c_n_pts));
            
            //increase the number fo total points
            mid_idx = c_n_pts++;
            
            return true;
        }

        mid_idx = fe1->second[0];
        
        //at this point, the face was already split. Update the mid index
        auto ed = MapEdges->find(OctreeEdge (fpts[0],mid_idx));
        ed->second[4] = sons[0];
        ed->second[1] = sons[1];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[0],ed->second[0]));
            subed->second[4] = sons[0];
            subed->second[1] = sons[1];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[4] = sons[0];
            subed->second[1] = sons[1];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[1],mid_idx));
        ed->second[2] = sons[1];
        ed->second[1] = sons[2];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[1],ed->second[0]));
            subed->second[2] = sons[1];
            subed->second[1] = sons[2];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[2] = sons[1];
            subed->second[1] = sons[2];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[2],mid_idx));
        ed->second[1] = sons[2];
        ed->second[4] = sons[3];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[2],ed->second[0]));
            subed->second[1] = sons[2];
            subed->second[4] = sons[3];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[1] = sons[2];
            subed->second[4] = sons[3];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[3],mid_idx));
        ed->second[1] = sons[3];
        ed->second[2] = sons[0];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[3],ed->second[0]));
            subed->second[1] = sons[3];
            subed->second[2] = sons[0];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[1] = sons[3];
            subed->second[2] = sons[0];
        }

        return false;
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    bool SplitVisitor::splitFaceType2(const vector<unsigned int> &fpts,
                                      const vector<unsigned int> &sons,
                                      const unsigned int &neighbor,
                                      unsigned int &c_n_pts, unsigned int &mid_idx){
        
        
        auto fe1 = MapEdges->find(OctreeEdge (fpts[0],fpts[2]));
        
        if (fe1==MapEdges->end()) {
            
            unsigned int nn = std::numeric_limits<unsigned int>::max();
            
            vector<unsigned int> eni1 ({0,neighbor,sons[1],sons[0],nn});
            MapEdges->emplace(OctreeEdge (fpts[0],c_n_pts,true), EdgeInfo (eni1));
            
            vector<unsigned int> eni2 ({0,neighbor,nn,sons[1],sons[2]});
            MapEdges->emplace(OctreeEdge (fpts[1],c_n_pts,true), EdgeInfo (eni2));
            
            vector<unsigned int> eni3 ({0,neighbor,sons[2],sons[3],nn});
            MapEdges->emplace(OctreeEdge (fpts[2],c_n_pts,true), EdgeInfo (eni3));
            
            vector<unsigned int> eni4 ({0,neighbor,nn,sons[0],sons[3]});
            MapEdges->emplace(OctreeEdge (fpts[3],c_n_pts,true), EdgeInfo (eni4));
            
            //we need to insert an edge that won't be used so if the neighbor
            //is refined, it will know this face was already split.
            MapEdges->emplace(OctreeEdge (fpts[0],fpts[2]), EdgeInfo (0,c_n_pts));
            MapEdges->emplace(OctreeEdge (fpts[1],fpts[3]), EdgeInfo (0,c_n_pts));
            
            //increase the number fo total points
            mid_idx = c_n_pts++;
            
            return true;
        }
        
        mid_idx = fe1->second[0];
        
        //at this point, the face was already split. Update the mid index
        auto ed = MapEdges->find(OctreeEdge (fpts[0],mid_idx));
        ed->second[3] = sons[0];
        ed->second[2] = sons[1];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[0],ed->second[0]));
            subed->second[3] = sons[0];
            subed->second[2] = sons[1];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[3] = sons[0];
            subed->second[2] = sons[1];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[1],mid_idx));
        ed->second[3] = sons[1];
        ed->second[4] = sons[2];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[1],ed->second[0]));
            subed->second[3] = sons[1];
            subed->second[4] = sons[2];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[3] = sons[1];
            subed->second[4] = sons[2];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[2],mid_idx));
        ed->second[2] = sons[2];
        ed->second[3] = sons[3];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[2],ed->second[0]));
            subed->second[2] = sons[2];
            subed->second[3] = sons[3];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[2] = sons[2];
            subed->second[3] = sons[3];
        }
        
        ed = MapEdges->find(OctreeEdge (fpts[3],mid_idx));
        ed->second[4] = sons[3];
        ed->second[3] = sons[0];
        
        //chek if there are sub-edges and uptade them too.
        if (ed->second[0]!=0) {
            auto subed = MapEdges->find(OctreeEdge (fpts[3],ed->second[0]));
            subed->second[4] = sons[3];
            subed->second[3] = sons[0];
            subed = MapEdges->find(OctreeEdge (mid_idx,ed->second[0]));
            subed->second[4] = sons[3];
            subed->second[3] = sons[0];
        }
        
        return false;
    }


}


