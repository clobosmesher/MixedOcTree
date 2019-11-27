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

#ifndef SurfPair_h
#define SurfPair_h 1

#include <iostream>

using std::ostream;

namespace SurfMesh {

	class SurfPair{
		
	public:
		
		SurfPair();
		
		SurfPair(unsigned int key, unsigned int val);
		
		virtual ~SurfPair();
		
		virtual unsigned int &operator[](unsigned int pos);
		
		virtual void operator=(SurfPair p2);
		
	protected:
		
		unsigned int key,val;
		
	};
	
	ostream& operator<<(ostream& o,SurfPair &p);
	
	bool operator==(SurfPair &p1,SurfPair &p2);
	
	bool operator!=(SurfPair &p1,SurfPair &p2);
}

#endif
