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

#include "SurfEdge.h"

namespace SurfMesh {
	
	SurfEdge::SurfEdge(unsigned int point1, unsigned int point2){
		nodes.reserve(2);
		if(point1 < point2){
			nodes.push_back(point1);
			nodes.push_back(point2);
		}
		else{ 
			nodes.push_back(point2);
			nodes.push_back(point1);
		}
	}
	
	SurfEdge::~SurfEdge(){
		
	}
	
	ostream& operator<<(ostream& o,SurfEdge &e){
		o << "e(" << e.getKey() << ",";
		o << e.getVal() << ")" << " f(";
		list<unsigned int> faces = e.getFaces();
		for (list<unsigned int>::iterator iter=faces.begin(); iter!=faces.end(); iter++) {
			o << " " << *iter;
		}
		o << " )";
		return o;
	}
	
	bool operator==(SurfEdge &e1,SurfEdge &e2){
		if(e1.getKey()==e2.getKey() && e1.getVal()==e2.getVal())
			return true;
		return false;
	}
	
	bool operator!=(SurfEdge &p1,SurfEdge &p2){
		return !(p1==p2);
	}
}
