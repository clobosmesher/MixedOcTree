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

#include "MeshPoint.h"

namespace Clobscode
{
	MeshPoint::MeshPoint(){
		//we assume that every point is outisde by default.
		inside = false;
		//checking if a point is outside or not is a very expensive 
		//operation, so we try to do it only once
		outsidechecked = false;
		projected = false;
		maxdistance = 0;
	}
	
	MeshPoint::MeshPoint(const Point3D &p){
		point = p;
		//we assume that every point is outisde by default.
		inside = false;
		//checking if a point is outside or not is a very expensive 
		//operation, so we try to do it only once
		outsidechecked = false;
		projected = false;
		maxdistance = 0;
	}
	
	MeshPoint::~MeshPoint(){
		
	}
		
	std::ostream& operator<<(std::ostream& o,MeshPoint &p){
		o << p.getPoint();
		return o;
	}
	
}
