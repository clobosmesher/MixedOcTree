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
* @file EdgeInfo.cpp
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#include "EdgeInfo.h"

namespace Clobscode
{

	EdgeInfo::EdgeInfo(){
        info.assign(5,std::numeric_limits<unsigned int>::max());
        info[0] = 0;
	}
    
    EdgeInfo::EdgeInfo(const vector<unsigned int> &info) {
        this->info = info;
    }
	
    EdgeInfo::EdgeInfo(const unsigned int &mid_idx, const unsigned int &q1,
                       const unsigned int &q2, const unsigned int &q3,
                       const unsigned int &q4) {
        info.reserve(5);
        info.push_back(mid_idx);
        info.push_back(q1);
        info.push_back(q2);
        info.push_back(q3);
        info.push_back(q4);
    }
    
    EdgeInfo::EdgeInfo(const unsigned int &pos, const unsigned int &value) {
        info.assign(5,std::numeric_limits<unsigned int>::max());
        info[0] = 0;
        info[pos] = value;
    }
    
    EdgeInfo::~EdgeInfo() {
    
    }
	
	ostream& operator<<(ostream& o, const EdgeInfo &e){
		o << e[0] << " ";
        o << e[1] << " ";
        o << e[2] << " ";
        o << e[3] << " ";
        o << e[4];
		return o;
	}
}
