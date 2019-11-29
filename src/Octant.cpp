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

#include "Octant.h"


namespace Clobscode
{
	
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	Octant::Octant(vector<unsigned int> &epts, const unsigned short &ref_level,
                   const unsigned int &o_id):o_id(o_id), ref_level(ref_level),
                   n_influences(0), influence_commit(false) {
		
        pointindex.assign(8,0);
		//for optimization
		//(*this).ref_level = ref_level;
        
        for (unsigned int i=0; i<8; i++) {
            pointindex[i] = epts[i];
        }
        
		sub_elements.assign(1,pointindex);
		//n_influences = 0;
		//influence_commit = false;
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	Octant::~Octant(){
		
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------


    bool Octant::accept(Visitor *v)
    {
        return v->visit(this);
    }

	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	std::ostream& operator<<(std::ostream& o,Octant &e){
		std::vector<unsigned int> points = e.getPoints();
		for (unsigned int i=0; i<points.size(); i++)
			o << " " << points[i];
		return o;
	}

}
