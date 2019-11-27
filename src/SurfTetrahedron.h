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

#ifndef SurfTetrahedron_h
#define SurfTetrahedron_h 3

#include "FaceDiagonal.h"
#include "MeshPoint.h"
#include <vector>
#include <list>
#include <iostream>
#include <math.h>

using std::vector;
using std::list;

namespace Clobscode
{
	class SurfTetrahedron {
		
	public:
		
		SurfTetrahedron(vector<unsigned int> &mypoints);
		
		virtual ~SurfTetrahedron();
		
		virtual vector<vector<unsigned int> > getSubElements(vector<bool> &inside_state);
        
        virtual vector<vector<unsigned int> > getTetras();
        
        virtual double getQuality(vector<MeshPoint> &allpoints);
                
    protected:
        
        vector<unsigned int> points;
        
        double scaled_jac_constant;
		
	};
}
#endif
