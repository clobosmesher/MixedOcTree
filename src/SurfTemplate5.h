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

#ifndef SurfTemplate5_h
#define SurfTemplate5_h 3

#include <vector>
#include <list>
#include <iostream>
#include "HexRotation.h"
#include "MeshPoint.h"
#include "TriMesh.h"

using std::vector;
using std::list;
using Clobscode::MeshPoint;
using Clobscode::TriMesh;

namespace Clobscode
{
	class VolumePointContainer;
	
	class SurfTemplate5 {
		
	public:
		
		SurfTemplate5();
		
		virtual ~SurfTemplate5();
		
		//eventually, this pattern will add new MeshPoints. If 
		//this is the case they will be included in the list newpts.
		virtual bool getSubelements(vector<unsigned int> &all, 
									vector<unsigned int> &in,
									vector<MeshPoint> &pts,
									list<MeshPoint> &newpts,
									vector<vector<unsigned int> > &neweles,
									TriMesh &input,
									list<unsigned int> &intersectedfaces,
									unsigned int &e_idx);
		
	protected:
		
		virtual void PatternA(vector<unsigned int> &all, 
							  vector<MeshPoint> &pts,
							  list<MeshPoint> &newpts,
							  vector<vector<unsigned int> > &neweles,
							  TriMesh &inputs,
							  list<unsigned int> &intersectedfaces,
							  unsigned int &e_idx);
		
		virtual void PatternB(vector<unsigned int> &all, 
							  vector<vector<unsigned int> > &neweles);
		
		virtual void PatternC(vector<unsigned int> &all, 
							  vector<vector<unsigned int> > &neweles);
		
		virtual vector<unsigned int> searchPivote(vector<unsigned int> &doubles);
		
		virtual vector<unsigned int> getEdges(int p);
		
	};
}
#endif
