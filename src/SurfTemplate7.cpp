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

#include "SurfTemplate7.h"

namespace Clobscode
{
	SurfTemplate7::SurfTemplate7(){
	}
	
	SurfTemplate7::~SurfTemplate7(){
	}
	
	bool SurfTemplate7::getSubelements(vector<unsigned int> &all, 
									  vector<unsigned int> &out,
									  vector<vector<unsigned int> > &newsubs){
		
		newsubs.reserve(3);
		vector<unsigned int> rotated,py1(5,0),py2(5,0),py3(5,0);
		HexRotation hrot;
		if(out.size()!=1)
			return false;
		
		rotated = hrot.rotate(all,out[0]);
		
		py1[0] = rotated[1];
		py1[1] = rotated[5];
		py1[2] = rotated[6];
		py1[3] = rotated[2];
		py1[4] = rotated[0];
		
		py2[0] = rotated[2];
		py2[1] = rotated[6];
		py2[2] = rotated[7];
		py2[3] = rotated[3];
		py2[4] = rotated[0];
		
		py3[0] = rotated[4];
		py3[1] = rotated[7];
		py3[2] = rotated[6];
		py3[3] = rotated[5];
		py3[4] = rotated[0];
		
		newsubs.push_back(py1);
		newsubs.push_back(py2);
		newsubs.push_back(py3);
		
		return true;
	}
}
