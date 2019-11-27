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

#ifndef SurfHexahedron_h
#define SurfHexahedron_h 3

#include <vector>
#include <list>
#include <iostream>
#include "MeshPoint.h"
#include "TriMesh.h"
#include "SurfTemplate1.h"
#include "SurfTemplate2.h"
#include "SurfTemplate3.h"
#include "SurfTemplate4.h"
#include "SurfTemplate5.h"
#include "SurfTemplate6.h"
#include "SurfTemplate7.h"


using std::vector;
using std::list;
using Clobscode::MeshPoint;
using Clobscode::TriMesh;

namespace Clobscode
{
	class SurfHexahedron {
		
	public:
		
		SurfHexahedron(vector<unsigned int> &mypoints);
		
		virtual ~SurfHexahedron();
		
        virtual vector<vector<unsigned int> > getSubElements(vector<MeshPoint> &meshpoints, list<MeshPoint> &newpts,
                                                             TriMesh &input, unsigned int &e_idx,
                                                             vector<vector<unsigned int> > &possibles,
                                                             vector<vector<unsigned int> > &continuity,
                                                             list<unsigned int> &intersected_faces);        
    protected:
        
        vector<unsigned int> points;
		
	};
}
#endif
