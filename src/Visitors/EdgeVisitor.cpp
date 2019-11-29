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

#include "EdgeVisitor.h"
#include "../Octant.h"


namespace Clobscode
{

    void EdgeVisitor::insertEdges(Octant *o, map<OctreeEdge, EdgeInfo> &MapEdges) {
        
        unsigned int oi = o->getIndex();
        vector<unsigned int> pointindex = o->getPoints();
        
        //Note: EdgeIndex at pos 0 is reserved for mid edge index.
        //Regarding the edge, this octant will be set in one of the
        //4 possible positions following the convention. It follows
        //the right hand rule aligned with x,y,z
        
        
        //X axis
        
        OctreeEdge oe3(pointindex[3],pointindex[0]);
        auto found = MapEdges.find(oe3);
        if (found!=MapEdges.end()) {
            (found->second)[1] = oi;
        }
        else {
            MapEdges.emplace(oe3, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe11(pointindex[7],pointindex[4]);
        found = MapEdges.find(oe11);
        if (found!=MapEdges.end()) {
            (found->second)[2] = oi;
        }
        else {
            MapEdges.emplace(oe11, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe9(pointindex[5],pointindex[6]);
        found = MapEdges.find(oe9);
        if (found!=MapEdges.end()) {
            (found->second)[3] = oi;
        }
        else {
            MapEdges.emplace(oe9, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe1(pointindex[1],pointindex[2]);
        found = MapEdges.find(oe1);
        if (found!=MapEdges.end()) {
            (found->second)[4] = oi;
        }
        else {
            MapEdges.emplace(oe1, EdgeInfo (4,oi));
        }
        
        //Y axis
        
        OctreeEdge oe4(pointindex[0],pointindex[4]);
        found = MapEdges.find(oe4);
        if (found!=MapEdges.end()) {
            (found->second)[1] = oi;
        }
        else {
            MapEdges.emplace(oe4, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe5(pointindex[1],pointindex[5]);
        found = MapEdges.find(oe5);
        if (found!=MapEdges.end()) {
            (found->second)[2] = oi;
        }
        else {
            MapEdges.emplace(oe5, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe6(pointindex[2],pointindex[6]);
        found = MapEdges.find(oe6);
        if (found!=MapEdges.end()) {
            (found->second)[3] = oi;
        }
        else {
            MapEdges.emplace(oe6, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe7(pointindex[3],pointindex[7]);
        found = MapEdges.find(oe7);
        if (found!=MapEdges.end()) {
            (found->second)[4] = oi;
        }
        else {
            MapEdges.emplace(oe7, EdgeInfo (4,oi));
        }
        
        //Z axis
        
        OctreeEdge oe0(pointindex[0],pointindex[1]);
        found = MapEdges.find(oe0);
        if (found!=MapEdges.end()) {
            (found->second)[1] = oi;
        }
        else {
            MapEdges.emplace(oe0, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe2(pointindex[2],pointindex[3]);
        found = MapEdges.find(oe2);
        if (found!=MapEdges.end()) {
            (found->second)[2] = oi;
        }
        else {
            MapEdges.emplace(oe2, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe10(pointindex[6],pointindex[7]);
        found = MapEdges.find(oe10);
        if (found!=MapEdges.end()) {
            (found->second)[3] = oi;
        }
        else {
            MapEdges.emplace(oe10, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe8(pointindex[4],pointindex[5]);
        found = MapEdges.find(oe8);
        if (found!=MapEdges.end()) {
            (found->second)[4] = oi;
        }
        else {
            MapEdges.emplace(oe8, EdgeInfo (4,oi));
        }
        
    }
    
    void EdgeVisitor::removeOctFromEdges(Octant *o, map<OctreeEdge, EdgeInfo> &MapEdges) {
        
        //This is a copy of above process, but it replace the octant with max nuemric
        //limits in the corresponding position at each edge.
        
        vector<unsigned int> pointindex = o->getPoints();
        unsigned int removed = std::numeric_limits<unsigned int>::max();
        
        //Note: EdgeIndex at pos 0 is reserved for mid edge index.
        //Regarding the edge, this octant will be set in one of the
        //4 possible positions following the convention. It follows
        //the right hand rule aligned with x,y,z
        
        
        //X axis
        
        OctreeEdge oe3(pointindex[3],pointindex[0]);
        auto found = MapEdges.find(oe3);
        if (found!=MapEdges.end()) {
            (found->second)[1] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (3,0) not found\n";
        }
        
        OctreeEdge oe11(pointindex[7],pointindex[4]);
        found = MapEdges.find(oe11);
        if (found!=MapEdges.end()) {
            (found->second)[2] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (7,4) not found\n";
        }
        
        OctreeEdge oe9(pointindex[5],pointindex[6]);
        found = MapEdges.find(oe9);
        if (found!=MapEdges.end()) {
            (found->second)[3] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (5,6) not found\n";
        }
        
        OctreeEdge oe1(pointindex[1],pointindex[2]);
        found = MapEdges.find(oe1);
        if (found!=MapEdges.end()) {
            (found->second)[4] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (1,2) not found\n";
        }
        
        //Y axis
        
        OctreeEdge oe4(pointindex[0],pointindex[4]);
        found = MapEdges.find(oe4);
        if (found!=MapEdges.end()) {
            (found->second)[1] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (0,4) not found\n";
        }
        
        OctreeEdge oe5(pointindex[1],pointindex[5]);
        found = MapEdges.find(oe5);
        if (found!=MapEdges.end()) {
            (found->second)[2] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (1,5) not found\n";
        }
        
        OctreeEdge oe6(pointindex[2],pointindex[6]);
        found = MapEdges.find(oe6);
        if (found!=MapEdges.end()) {
            (found->second)[3] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (2,6) not found\n";
        }
        
        OctreeEdge oe7(pointindex[3],pointindex[7]);
        found = MapEdges.find(oe7);
        if (found!=MapEdges.end()) {
            (found->second)[4] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (3,7) not found\n";
        }
        
        //Z axis
        
        OctreeEdge oe0(pointindex[0],pointindex[1]);
        found = MapEdges.find(oe0);
        if (found!=MapEdges.end()) {
            (found->second)[1] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (0,1) not found\n";
        }
        
        OctreeEdge oe2(pointindex[2],pointindex[3]);
        found = MapEdges.find(oe2);
        if (found!=MapEdges.end()) {
            (found->second)[2] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (2,3) not found\n";
        }
        
        OctreeEdge oe10(pointindex[6],pointindex[7]);
        found = MapEdges.find(oe10);
        if (found!=MapEdges.end()) {
            (found->second)[3] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (6,7) not found\n";
        }
        
        OctreeEdge oe8(pointindex[4],pointindex[5]);
        found = MapEdges.find(oe8);
        if (found!=MapEdges.end()) {
            (found->second)[4] = removed;
        }
        else {
            cerr << "Error at EdgeVisitor::removeOctFromEdges edge (4,5) not found\n";
        }
        
    }
    

}