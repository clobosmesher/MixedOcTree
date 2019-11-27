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


#ifndef FaceDiagonal_h
#define FaceDiagonal_h 3

#include <vector>
#include <iostream>

using std::vector;

namespace Clobscode
{
	class FaceDiagonal {
		
	public:
		
		FaceDiagonal(vector<unsigned int> &mypoints);
		
		virtual ~FaceDiagonal();
		
		virtual bool getDiagonal(vector<bool> &inside_state,
                                 vector<unsigned int> &diag);
        
    protected:
        
        vector<unsigned int> points;
		
	};
}
#endif
