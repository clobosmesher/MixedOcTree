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

#include "SurfPair.h"

namespace SurfMesh {
	
	SurfPair::SurfPair(){
		key=val=-1;
	}
	
	SurfPair::SurfPair(unsigned int key, unsigned int val){
		this->key=key;
		this->val=val;
	}
	
	SurfPair::~SurfPair(){
		
	}
	
	void SurfPair::operator=(SurfPair p2){
		key=p2[0];
		val=p2[1];
	}
	
	ostream& operator<<(ostream& o,SurfPair &p){
		o << p[0] << " ";
		o << p[1];
		return o;
	}
	
	bool operator==(SurfPair &p1,SurfPair &p2){
		if(p1[0]==p2[0] && p1[1]==p2[1])
			return true;
		return false;
	}
	
	bool operator!=(SurfPair &p1,SurfPair &p2){
		return !(p1==p2);
	}
	
	unsigned int &SurfPair::operator[](unsigned int pos){
		if(pos==0)
			return key;
		return val;
	}
	
}