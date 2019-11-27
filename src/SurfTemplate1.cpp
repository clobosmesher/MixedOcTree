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

#include "SurfTemplate1.h"

namespace Clobscode
{
	SurfTemplate1::SurfTemplate1(){
	}
	
	SurfTemplate1::~SurfTemplate1(){
	}
	
	bool SurfTemplate1::getSubelements(vector<unsigned int> &all, 
									  vector<unsigned int> &in,
									  vector<vector<unsigned int> > &newsubs){
		
		newsubs.reserve(1);
		vector<unsigned int> tetra,rotated;
		HexRotation hrot;
		tetra.reserve(4);
		
		rotated = hrot.rotate(all,in[0]);
		
		tetra.push_back(rotated[0]);
		tetra.push_back(rotated[1]);
		tetra.push_back(rotated[3]);
		tetra.push_back(rotated[4]);
		
		newsubs.push_back(tetra);
		
		return true;
	}
}
