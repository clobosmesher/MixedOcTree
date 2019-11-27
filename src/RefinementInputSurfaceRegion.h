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

#ifndef RefinementInputSurfaceRegion_h
#define RefinementInputSurfaceRegion_h 1

#include <vector>
#include <set>
#include <iostream>
#include "RefinementRegion.h"
#include "TriMesh.h"
#include "Octant.h"

using Clobscode::Point3D;
using std::vector;
using std::cout;
using std::set;
using Clobscode::RefinementRegion;
using Clobscode::TriMesh;
using Clobscode::Octant;

namespace Clobscode
{
	class RefinementInputSurfaceRegion : public RefinementRegion
	{
	public:
		
		// Construction / destruction
		RefinementInputSurfaceRegion(const unsigned short &level);
		
		virtual ~RefinementInputSurfaceRegion();
        
        virtual GeometricTransform rotateWithinYou(TriMesh &input);
		
        virtual void rotate(GeometricTransform &gt);
		
        virtual bool intersectsOctant(vector<MeshPoint> &points, Octant &oct);
	};
	
}

#endif
