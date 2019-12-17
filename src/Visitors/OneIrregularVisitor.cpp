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

#include "OneIrregularVisitor.h"

#include "../Octant.h"

namespace Clobscode
{

    OneIrregularVisitor::OneIrregularVisitor() {
        edges = NULL;
        max_ref_level = NULL;
    }

    void OneIrregularVisitor::setMapEdges(map<OctreeEdge, EdgeInfo> &MapEdges) {
        this->edges = &MapEdges;
    }

    void OneIrregularVisitor::setMaxRefLevel(const unsigned short &max_ref_level) {
        this->max_ref_level = &max_ref_level;
    }

    bool OneIrregularVisitor::visit(Octant *o) {
        //cout << "OneIrregular visit" << endl;
        if (*max_ref_level==o->ref_level) {
            return true;
        }

        unsigned int oidx = o->getIndex();
        
        //EdgeVisitor ev;
        for (unsigned int i=0; i<12; i++) {
            OctreeEdge ee;
            EdgeVisitor::getEdge(o,i,ee);
            auto my_edge = edges->find(ee);
            
            if (my_edge==edges->end()) {
                cerr << "Error at Quadrant::isOneIrregular!!!\n";
                cerr << "Edge not found\n";
                std::abort();
            }
            
            unsigned int mid_idx = (my_edge->second)[0];
            
            //if the edge is not split, check the others
            if (mid_idx==0) {
                continue;
            }
            //At this point, the edge is split so both
            //"sub-edges" must be checked. If at least one of
            //them is also split, then this element is not
            //one-irregular
            
            auto sub_edge = edges->find(OctreeEdge (ee[0],mid_idx));
            if ((sub_edge->second)[0]!=0) {
                
                cout << "    * Octant " << oidx << " not balanced (RL " << o->ref_level << ")\n";
                cout << "    * Main edge (" << ee[0] << "," << ee[1] << ")";
                cout << " local idx " << i << " Info:";
                for (auto n: my_edge->second.getNeighborOcts()) {
                    cout << " " << n;
                }
                cout << "\n      * Sub-edge info:";
                for (auto n: sub_edge->second.getNeighborOcts()) {
                    cout << " " << n;
                }
                cout << "\n    * Octant point index";
                for (unsigned int k=0; k<8; k++) {
                    cout << " " << o->getPoints()[k];
                }
                
                cout << "\n\n";
                
                return false;
            }
            
            sub_edge = edges->find(OctreeEdge (ee[1],mid_idx));
            if ((sub_edge->second)[0]!=0) {
                
                cout << "    * Octant " << oidx << " not balanced\n";
                
                return false;
            }
        }
        return true;
    }
}
