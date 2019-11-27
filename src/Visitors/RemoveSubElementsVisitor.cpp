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

#include "RemoveSubElementsVisitor.h"
#include "../Octant.h"

namespace Clobscode
{

    RemoveSubElementsVisitor::RemoveSubElementsVisitor() {
        points = NULL;
    }

    void RemoveSubElementsVisitor::setPoints(vector<MeshPoint> &points) {
        this->points = &points;
    }

    bool RemoveSubElementsVisitor::visit(Octant *o) {
        //cout << "RemoveElements" << endl;
        vector<vector<unsigned int>> &sub_elements = o->sub_elements;

        list<vector<unsigned int> > still_in;
        list<vector<unsigned int> >::iterator iter;

        for (unsigned int i=0; i<sub_elements.size(); i++) {

            bool onein = false;
            vector<unsigned int> e_pts = sub_elements[i];
            for (unsigned int j=0; j<e_pts.size(); j++) {
                if (points->at(e_pts[j]).isInside()) {
                    onein = true;
                    break;
                }
            }
            if (onein) {
                still_in.push_back(sub_elements[i]);
            }
        }

        if (still_in.size()==sub_elements.size()) {
            return false;
        }
        if (still_in.empty()) {
            return true;
        }

        sub_elements.clear();
        sub_elements.reserve(still_in.size());
        for (iter=still_in.begin(); iter!=still_in.end(); iter++) {
            sub_elements.push_back(*iter);
        }
        return false;
    }
}