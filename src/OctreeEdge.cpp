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

#include "OctreeEdge.h"

namespace Clobscode
{
	
	OctreeEdge::OctreeEdge(){
        info[0] = info[1] = 0;
	}
	
	OctreeEdge::OctreeEdge(const unsigned int &point1, const unsigned int &point2){
        assign(point1,point2);
	}
    
    OctreeEdge::OctreeEdge(const unsigned int &p1, const unsigned int &p2, const bool &forced) {
        info[0] = p1;
        info[1] = p2;
    }
	
	OctreeEdge::~OctreeEdge(){
		
	}
	
	void OctreeEdge::assign(const unsigned int &point1, const unsigned int &point2){
		if (point1<point2) {
			info[0]=point1;
			info[1]=point2;
		}
		else {
			info[1]=point1;
			info[0]=point2;
		}
	}
	
	ostream& operator<<(ostream& o, const OctreeEdge &e){
		o << e[0] << " ";
        o << e[1];
		return o;
	}
	
	bool operator==(const OctreeEdge &e1, const OctreeEdge &e2) {
		//this possible as OctreeEdges are sorted by min index
		if (e1[0]==e2[0] && e1[1]==e2[1])
			return true;
		return false;
	}
	
	bool operator!=(const OctreeEdge &e1, const OctreeEdge &e2) {
		return !(e1==e2);
	}

	bool operator<(const OctreeEdge &e1, const OctreeEdge &e2) {
		if (e1[0]!=e2[0]){
			return e1[0]<e2[0];
		}
		return e1[1] < e2[1];
	}
	
	void OctreeEdge::operator=(const OctreeEdge &e){
		info[0]=e[0];
		info[1]=e[1];
	}
	
}