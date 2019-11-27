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

#ifndef SurfEdge_h
#define SurfEdge_h 1

#include <iostream>
#include <vector>
#include <list>

using std::list;
using std::vector;
using std::ostream;

namespace SurfMesh {

	class SurfEdge{
		
	public:
		
		SurfEdge();
		
		SurfEdge(unsigned int point1, unsigned int point2);
		
		virtual ~SurfEdge();
		
		virtual unsigned int getKey();
		
		virtual unsigned int getVal();
		
		virtual unsigned int numberOfFaces();
		
		virtual void addFace(unsigned int faceindex);
		
		virtual list<unsigned int> &getFaces();
		
		virtual unsigned int getOtherFace(unsigned int vf);
		
	protected:
		
		vector<unsigned int> nodes;
		list<unsigned int> faces;
		
	};
	
	inline unsigned int SurfEdge::getKey(){
		return nodes[0];
	}
	
	inline unsigned int SurfEdge::getVal(){
		return nodes[1];
	}
	
	inline unsigned int SurfEdge::numberOfFaces(){
		return faces.size();
	}
	
	inline void SurfEdge::addFace(unsigned int faceindex){
		list<unsigned int>::iterator iter;
		for(iter=faces.begin();iter!=faces.end(); iter++){
			if(faceindex==*iter)
				return;
		}
		faces.push_back(faceindex);
	}
	
	inline list<unsigned int> &SurfEdge::getFaces(){
		return faces;
	}
		
	inline unsigned int SurfEdge::getOtherFace(unsigned int vf){
		list<unsigned int>::iterator iter;
		for(iter=faces.begin();iter!=faces.end(); iter++){
			if (*iter!=vf) {
				return *iter;
			}
		}
		return 0;
	}
		
	ostream& operator<<(ostream& o,SurfEdge &p);
	
	bool operator==(SurfEdge &p1,SurfEdge &p2);
	
	bool operator!=(SurfEdge &p1,SurfEdge &p2); 
}
#endif
