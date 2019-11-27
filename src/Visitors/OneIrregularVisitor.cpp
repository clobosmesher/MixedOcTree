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

    void OneIrregularVisitor::setEdges(set<OctreeEdge> &edges) {
        this->edges = &edges;
    }

    void OneIrregularVisitor::setMaxRefLevel(const unsigned short &max_ref_level) {
        this->max_ref_level = &max_ref_level;
    }

    bool OneIrregularVisitor::visit(Octant *o) {
        //cout << "OneIrregular visit" << endl;
        if (*max_ref_level==o->ref_level) {
            return true;
        }

        //EdgeVisitor ev;
        for (unsigned int i=0; i<12; i++) {
            OctreeEdge ee;
            EdgeVisitor::getEdge(o,i,ee);
            set<OctreeEdge>::iterator my_edge, sub_edge;
            my_edge = edges->find(ee);
            if (my_edge==edges->end()) {
                cout << "Octant::isOneIrregular wtf!!!\n";
                return false;
            }

            unsigned int mid_idx = (*my_edge)[2];

            //if the edge is not split, check the others
            if (mid_idx==0) {
                continue;
            }
            //At this point, the edge is split so both
            //"sub-edges" must be checked. If at least one of
            //them is also split, then this element is not
            //one-irregular

            OctreeEdge sub1(ee[0],mid_idx);
            sub_edge = edges->find(sub1);
            if ((*sub_edge)[2]!=0) {
                return false;
            }
            OctreeEdge sub2(ee[1],mid_idx);
            sub_edge = edges->find(sub2);
            if ((*sub_edge)[2]!=0) {
                return false;
            }
        }
        return true;
    }
}
