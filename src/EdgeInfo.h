/*
<Mix-mesher: region type. This program generates a mixed-elements 2D mesh>

Copyright (C) <2013,2018>  <Claudio Lobos> All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
*/
/**
* @file EdgeInfo.h
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#ifndef EdgeInfo_h
#define EdgeInfo_h 1

#include <iostream>
#include <vector>
#include <set>
#include <limits>

using std::vector;
using std::ostream;
using std::set;
using std::cout;
using std::cerr;
using std::pair;

namespace Clobscode
{
	
	class EdgeInfo{
		
	public:
        
		EdgeInfo();
		
        EdgeInfo(const unsigned int &mid_idx, const unsigned int &q1, const unsigned int &q2);
        
        EdgeInfo(const unsigned int &pos, const unsigned int &value);
        
		virtual ~EdgeInfo();
		
        virtual unsigned int operator[](const unsigned int &pos) const;
        
        virtual unsigned int &operator[](const unsigned int &pos);

		friend ostream& operator<<(ostream& o, const EdgeInfo &e);
		
		
	protected:
		
        unsigned int info [3];

	};

    inline unsigned int EdgeInfo::operator[](const unsigned int &pos) const{
        return info[pos];
    }
	
    inline unsigned int &EdgeInfo::operator[](const unsigned int &pos){
        return info[pos];
    }
}
#endif
