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

#ifndef EdgeVisitor_h
#define EdgeVisitor_h 1

#include "../OctreeEdge.h"
#include "../Point3D.h"

#include <set>
#include <vector>

#include "Visitor.h"

using Clobscode::OctreeEdge;
using Clobscode::Point3D;
using std::set;
using std::vector;

namespace Clobscode
{
    class EdgeVisitor : public Visitor{
    public:
        static void insertEdges(Octant *o, set<OctreeEdge> &edges);
        static void getEdge(Octant *o, const unsigned int &idx, OctreeEdge &e);
    };

}


#endif //EdgeVisitor_h
