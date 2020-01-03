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
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    void EdgeVisitor::insertAllEdges(Octant *o, map<OctreeEdge, EdgeInfo> &MapEdges) {
        
        unsigned int oi = o->getIndex();
        vector<unsigned int> pidxs = o->getPoints();
        
        //Note: EdgeIndex at pos 0 is reserved for mid edge index.
        //Regarding the edge, this octant will be set in one of the
        //4 possible positions following the convention. It follows
        //the right hand rule aligned with x,y,z
        
        //This array counts the number of edges split for each octant's face.
        unsigned int faces [] = {0,0,0,0,0,0};
        
        //X axis
        
        //Edge 3
        auto found = MapEdges.find(OctreeEdge (pidxs[3],pidxs[0]));
        if (found!=MapEdges.end()) {
            //Test mid node
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(1,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[3],mid))->second[1] = oi;
                MapEdges.find(OctreeEdge (pidxs[0],mid))->second[1] = oi;
                faces[0]++;
                faces[4]++;
            }
            
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 3 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        //Edge 11
        found = MapEdges.find(OctreeEdge (pidxs[7],pidxs[4]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(2,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[7],mid))->second[2] = oi;
                MapEdges.find(OctreeEdge (pidxs[4],mid))->second[2] = oi;
                faces[4]++;
                faces[5]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 11 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 9
        found = MapEdges.find(OctreeEdge (pidxs[5],pidxs[6]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(3,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[5],mid))->second[3] = oi;
                MapEdges.find(OctreeEdge (pidxs[6],mid))->second[3] = oi;
                faces[2]++;
                faces[5]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 9 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 1
        found = MapEdges.find(OctreeEdge (pidxs[1],pidxs[2]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(4,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[1],mid))->second[4] = oi;
                MapEdges.find(OctreeEdge (pidxs[2],mid))->second[4] = oi;
                faces[0]++;
                faces[2]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 1 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        //Y axis
        
        
        //Edge 4
        found = MapEdges.find(OctreeEdge (pidxs[0],pidxs[4]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(1,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[0],mid))->second[1] = oi;
                MapEdges.find(OctreeEdge (pidxs[4],mid))->second[1] = oi;
                faces[1]++;
                faces[4]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 4 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 5
        found = MapEdges.find(OctreeEdge (pidxs[1],pidxs[5]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(2,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[1],mid))->second[2] = oi;
                MapEdges.find(OctreeEdge (pidxs[5],mid))->second[2] = oi;
                faces[1]++;
                faces[2]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 5 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 6
        found = MapEdges.find(OctreeEdge (pidxs[2],pidxs[6]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(3,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[2],mid))->second[3] = oi;
                MapEdges.find(OctreeEdge (pidxs[6],mid))->second[3] = oi;
                faces[2]++;
                faces[3]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 6 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 7
        found = MapEdges.find(OctreeEdge (pidxs[3],pidxs[7]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(4,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[3],mid))->second[4] = oi;
                MapEdges.find(OctreeEdge (pidxs[7],mid))->second[4] = oi;
                faces[3]++;
                faces[4]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 7 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        //Z axis
        
        
        //Edge 0
        found = MapEdges.find(OctreeEdge (pidxs[0],pidxs[1]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(1,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[0],mid))->second[1] = oi;
                MapEdges.find(OctreeEdge (pidxs[1],mid))->second[1] = oi;
                faces[0]++;
                faces[1]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 0 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 2
        found = MapEdges.find(OctreeEdge (pidxs[2],pidxs[3]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(2,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[2],mid))->second[2] = oi;
                MapEdges.find(OctreeEdge (pidxs[3],mid))->second[2] = oi;
                faces[0]++;
                faces[3]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 2 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 10
        found = MapEdges.find(OctreeEdge (pidxs[6],pidxs[7]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(3,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[6],mid))->second[3] = oi;
                MapEdges.find(OctreeEdge (pidxs[7],mid))->second[3] = oi;
                faces[3]++;
                faces[5]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 10 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        //Edge 8
        found = MapEdges.find(OctreeEdge (pidxs[4],pidxs[5]));
        if (found!=MapEdges.end()) {
            unsigned int mid = found->second[0];
            if (mid==0) {
                found->second.update(4,oi);
            }
            else {
                //only update two mid-edges. The big one is already deprecated
                MapEdges.find(OctreeEdge (pidxs[4],mid))->second[4] = oi;
                MapEdges.find(OctreeEdge (pidxs[5],mid))->second[4] = oi;
                faces[1]++;
                faces[5]++;
            }
        }
        else {
            cerr << "Error in EdgeVisitor::insertAllEdges edge 8 not found";
            cerr << " for octant " << oi << "\n";
        }
        
        
        
        
        //Now the edges inserted at the split faces must be updated. This task will be
        //performed only for faces with 4 split edges.
        
        if (faces[0]==4) {
            
            //Verificar que el arco con los indices 8-16 y 12-13 fueron almacenados en
            //el archivo .oct. Esto es necesario para obtener el vertice central de la
            //cara y lo mismo para el resto de las caras.
            
            
        }
        
        
        
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
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
            found->second.update(1,oi);
        }
        else {
            MapEdges.emplace(oe3, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe11(pointindex[7],pointindex[4]);
        found = MapEdges.find(oe11);
        if (found!=MapEdges.end()) {
            found->second.update(2,oi);
        }
        else {
            MapEdges.emplace(oe11, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe9(pointindex[5],pointindex[6]);
        found = MapEdges.find(oe9);
        if (found!=MapEdges.end()) {
            found->second.update(3,oi);
        }
        else {
            MapEdges.emplace(oe9, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe1(pointindex[1],pointindex[2]);
        found = MapEdges.find(oe1);
        if (found!=MapEdges.end()) {
            found->second.update(4,oi);
        }
        else {
            MapEdges.emplace(oe1, EdgeInfo (4,oi));
        }
        
        //Y axis
        
        OctreeEdge oe4(pointindex[0],pointindex[4]);
        found = MapEdges.find(oe4);
        if (found!=MapEdges.end()) {
            found->second.update(1,oi);
        }
        else {
            MapEdges.emplace(oe4, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe5(pointindex[1],pointindex[5]);
        found = MapEdges.find(oe5);
        if (found!=MapEdges.end()) {
            found->second.update(2,oi);
        }
        else {
            MapEdges.emplace(oe5, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe6(pointindex[2],pointindex[6]);
        found = MapEdges.find(oe6);
        if (found!=MapEdges.end()) {
            found->second.update(3,oi);
        }
        else {
            MapEdges.emplace(oe6, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe7(pointindex[3],pointindex[7]);
        found = MapEdges.find(oe7);
        if (found!=MapEdges.end()) {
            found->second.update(4,oi);
        }
        else {
            MapEdges.emplace(oe7, EdgeInfo (4,oi));
        }
        
        //Z axis
        
        OctreeEdge oe0(pointindex[0],pointindex[1]);
        found = MapEdges.find(oe0);
        if (found!=MapEdges.end()) {
            found->second.update(1,oi);
        }
        else {
            MapEdges.emplace(oe0, EdgeInfo (1,oi));
        }
        
        OctreeEdge oe2(pointindex[2],pointindex[3]);
        found = MapEdges.find(oe2);
        if (found!=MapEdges.end()) {
            found->second.update(2,oi);
        }
        else {
            MapEdges.emplace(oe2, EdgeInfo (2,oi));
        }
        
        OctreeEdge oe10(pointindex[6],pointindex[7]);
        found = MapEdges.find(oe10);
        if (found!=MapEdges.end()) {
            found->second.update(3,oi);
        }
        else {
            MapEdges.emplace(oe10, EdgeInfo (3,oi));
        }
        
        OctreeEdge oe8(pointindex[4],pointindex[5]);
        found = MapEdges.find(oe8);
        if (found!=MapEdges.end()) {
            found->second.update(4,oi);
        }
        else {
            MapEdges.emplace(oe8, EdgeInfo (4,oi));
        }
        
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
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
        auto found = MapEdges.find(OctreeEdge (pointindex[3],pointindex[0]));
        found->second[1] = removed;
        
        found = MapEdges.find(OctreeEdge (pointindex[7],pointindex[4]));
        found->second[2] = removed;
        
        found = MapEdges.find(OctreeEdge (pointindex[5],pointindex[6]));
        found->second[3] = removed;

        found = MapEdges.find(OctreeEdge (pointindex[1],pointindex[2]));
        found->second[4] = removed;

        //Y axis
        found = MapEdges.find(OctreeEdge (pointindex[0],pointindex[4]));
        found->second[1] = removed;
        
        found = MapEdges.find(OctreeEdge (pointindex[1],pointindex[5]));
        found->second[2] = removed;

        found = MapEdges.find(OctreeEdge (pointindex[2],pointindex[6]));
        found->second[3] = removed;
        
        found = MapEdges.find(OctreeEdge (pointindex[3],pointindex[7]));
        found->second[4] = removed;

        //Z axis
        found = MapEdges.find(OctreeEdge (pointindex[0],pointindex[1]));
        found->second[1] = removed;

        found = MapEdges.find(OctreeEdge (pointindex[2],pointindex[3]));
        found->second[2] = removed;

        found = MapEdges.find(OctreeEdge (pointindex[6],pointindex[7]));
        found->second[3] = removed;

        found = MapEdges.find(OctreeEdge (pointindex[4],pointindex[5]));
        found->second[4] = removed;        
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    //Used by other visitors.
    void EdgeVisitor::getEdge(Octant *o, const unsigned int &idx, OctreeEdge &e) {
        vector<unsigned int> pointindex = o->pointindex;
        unsigned int e0,e1;
        switch (idx) {
            case 0:
                e0 = pointindex[0];
                e1 = pointindex[1];
                break;
            case 1:
                e0 = pointindex[1];
                e1 = pointindex[2];
                break;
            case 2:
                e0 = pointindex[2];
                e1 = pointindex[3];
                break;
            case 3:
                e0 = pointindex[3];
                e1 = pointindex[0];
                break;
            case 4:
                e0 = pointindex[0];
                e1 = pointindex[4];
                break;
            case 5:
                e0 = pointindex[1];
                e1 = pointindex[5];
                break;
            case 6:
                e0 = pointindex[2];
                e1 = pointindex[6];
                break;
            case 7:
                e0 = pointindex[3];
                e1 = pointindex[7];
                break;
            case 8:
                e0 = pointindex[4];
                e1 = pointindex[5];
                break;
            case 9:
                e0 = pointindex[5];
                e1 = pointindex[6];
                break;
            case 10:
                e0 = pointindex[6];
                e1 = pointindex[7];
                break;
            case 11:
                e0 = pointindex[7];
                e1 = pointindex[4];
                break;
            default:
                break;
        }
        e.assign(e0,e1);
    }
    

}