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

#ifndef OctreeEdge_h
#define OctreeEdge_h 1

#include <iostream>
#include <vector>
#include <set>

using std::vector;
using std::ostream;
using std::set;
using std::cout;
using std::pair;

namespace Clobscode
{
	
	//class OctreeEdge;
	
	class OctreeEdge{
		
	public:
        
		OctreeEdge();
		
		OctreeEdge(const unsigned int &idx1, const unsigned int &idx2);
		
		virtual ~OctreeEdge();
		
		virtual void assign(const unsigned int &idx1, const unsigned int &idx2);
		
		virtual void updateMidPoint(const unsigned int &idx);
		
		virtual bool split(set<OctreeEdge> &allOctreeEdges, unsigned int maxp);
		
		virtual void setMidPoint(unsigned int &mid);
		
		//virtual unsigned int &operator[](unsigned const int &pos);
		
		virtual unsigned int operator[](unsigned const int &pos) const;
		
		virtual void operator=(const OctreeEdge &e);
		
		friend ostream& operator<<(ostream& o, const OctreeEdge &e);
		
		friend bool operator==(const OctreeEdge &e1, const OctreeEdge &e2);
		
		friend bool operator!=(const OctreeEdge &e1, const OctreeEdge &e2);
		
		friend bool operator<(const OctreeEdge &e1, const OctreeEdge &e2);
		
		
	protected:
		
		vector<unsigned int> info;
		
		//set<OctreeEdge>::iterator mypointer;
	};
	
	/*inline unsigned int &OctreeEdge::operator[](unsigned const int &pos){
		return info.at(pos);
	}*/
	
	inline void OctreeEdge::updateMidPoint(const unsigned int &idx){
		info[2] = idx;
	}

	inline unsigned int OctreeEdge::operator[](unsigned const int &pos) const{
		return info.at(pos);
	}
	
	inline void OctreeEdge::setMidPoint(unsigned int &mid){
		info[2] = mid;
	}
	
}
#endif
