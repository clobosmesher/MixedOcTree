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

#ifndef Prism_h
#define Prism_h 3

#include "FaceDiagonal.h"
#include <vector>
#include <list>
#include <iostream>

using std::vector;
using std::list;

namespace Clobscode
{
	class Prism {
		
	public:
		
		Prism(vector<unsigned int> &mypoints);
		
		virtual ~Prism();
		
		virtual vector<vector<unsigned int> > getSubElements(vector<bool> &inside_state);
        
    protected:
        
        virtual unsigned int opposite(unsigned int idx);
        
        virtual void oppositeFace(unsigned int idx, vector<unsigned int> &base);
        
        virtual bool oneRepeated(vector<unsigned int> &original, vector<unsigned int> &result);
        
        virtual vector<unsigned int> sortDiagonals(list<unsigned int> &original);
        
    protected:
        
        vector<unsigned int> points;
		
	};
}
#endif
