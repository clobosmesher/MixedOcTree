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

#include "SurfEdgeContainer.h"

namespace SurfMesh {
	
	SurfEdgeContainer::SurfEdgeContainer(unsigned int size){
		edgeindexer.reserve(size);
		indexed.reserve(size);
		this->size=0;
	}
	
	SurfEdgeContainer::~SurfEdgeContainer(){
		
	}
	
	unsigned int SurfEdgeContainer::addEdge(unsigned int point1,
										unsigned int point2,
										unsigned int faceindex){
		SurfEdge ne(point1,point2);
		
		if(edgeindexer.size()<ne.getKey()+1){
			//Edge first insertion.
			for(unsigned int i=edgeindexer.size(); i<ne.getKey()+1; i++){
				list<SurfPair> dummy;
				edgeindexer.push_back(dummy);
			}
			ne.addFace(faceindex);
			return addEdge(ne);
		}
		else{
			//at this point the edge key exists in the vector
			list<SurfPair> pairs = edgeindexer[ne.getKey()];
			list<SurfPair>::iterator iter;
			for(iter=pairs.begin();iter!=pairs.end();iter++){
				if((*iter)[0]==ne.getVal()){
					//the edge was found, so the list of faces of it is updated
					indexed[(*iter)[1]].addFace(faceindex);
					return (*iter)[1];
				}
			}
			//the edge was not found, so it must be inserted;
			ne.addFace(faceindex);
			return addEdge(ne);
		}
	}
	
	unsigned int SurfEdgeContainer::addEdge(SurfEdge &e){
		SurfPair np(e.getVal(),size);
		edgeindexer[e.getKey()].push_back(np);
		indexed.push_back(e);
		return size++;
	}
	
	
	SurfEdge &SurfEdgeContainer::getEdge(unsigned int index){
		if(index>=size)
			std::cerr << "Index out of boundary in EdgeContainer\n";
		return indexed[index];
	}
	
	ostream& operator<<(ostream& o,SurfEdgeContainer &ec){
		for(int i=0;i<ec.length();i++){
			o << ec.getEdge(i) << " [";
			list<unsigned int> ef = ec.getEdge(i).getFaces();
			list<unsigned int>::iterator iter;
			int semicolon=0;
			for(iter=ef.begin();iter!=ef.end();iter++){
				o << *iter;
				if(semicolon++<ef.size()-1)
					o << ",";
			}
			o << "]\n";
		}
		return o;
	}
}
