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

#include "RefinementInputSurfaceRegion.h"

namespace Clobscode
{	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementInputSurfaceRegion::RefinementInputSurfaceRegion(const unsigned short &level)
	{
		refine_level = level;
        
        local_rot = false;
        input_rot = false;
        
        //Use force input rotation (Defined in parent class) if the input domain
        //should be re-aligned.
	}

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementInputSurfaceRegion::~RefinementInputSurfaceRegion()
	{
		
	}

    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    GeometricTransform RefinementInputSurfaceRegion::rotateWithinYou(TriMesh &input) {
		GeometricTransform gt;
        gt.rotateSurfaceMesh(input);
        //rotate input and save it in gt variable (see .h). then return gt.
        return gt;
        //gt defined in parent class
    }
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    
    void RefinementInputSurfaceRegion::rotate(GeometricTransform &gt) {
        //nothing to do
    }
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	bool RefinementInputSurfaceRegion::intersectsOctant(vector<MeshPoint> &points, Octant &oct)
    {
        return !oct.getIntersectedFaces().empty();
	}
}
