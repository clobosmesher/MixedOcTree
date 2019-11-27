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

    void EdgeVisitor::insertEdges(Octant *o, set<OctreeEdge> &edges) {
        for (unsigned int i=0; i<12; i++) {
            OctreeEdge ee;
            getEdge(o,i,ee);
            edges.insert(ee);
        }
    }

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