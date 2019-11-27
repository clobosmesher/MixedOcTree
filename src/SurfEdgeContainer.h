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

#ifndef SurfEdgeContainer_h
#define SurfEdgeContainer_h 1

#include <iostream>
#include <vector>
#include <list>
#include "SurfEdge.h"
#include "SurfPair.h"

using std::vector;
using std::list;
using std::ostream;

namespace SurfMesh
{
	class SurfEdgeContainer{
		
	public:
		
		SurfEdgeContainer(unsigned int size);
		
		virtual ~SurfEdgeContainer();
		
		virtual unsigned int addEdge(unsigned int point1,
									 unsigned int point2,
									 unsigned int faceindex);
		
		virtual SurfEdge &getEdge(unsigned int index);
		
		virtual unsigned int length();
		
	protected:
		
		virtual unsigned int addEdge(SurfEdge &e);
		
	protected:
		
		vector<list<SurfPair> > edgeindexer;
		vector<SurfEdge> indexed;
		unsigned int size;
		
	};
	
	inline unsigned int SurfEdgeContainer::length(){
		return size;
	}
	
	ostream& operator<<(ostream& o,SurfEdgeContainer &ec);
}
#endif
