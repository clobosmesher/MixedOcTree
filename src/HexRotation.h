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

#ifndef HexRotation_h
#define HexRotation_h 1

#include <vector>

using std::vector;

namespace Clobscode
{
	class HexRotation {
		
	public:
		HexRotation();
		
		virtual ~HexRotation();
		
		virtual vector<unsigned int> rotate(vector<unsigned int> original,
											int index);
		
		virtual vector<unsigned int> rotatePosX(vector<unsigned int> original);
		
		virtual vector<unsigned int> rotateNegX(vector<unsigned int> original);
		
		virtual vector<unsigned int> rotatePosY(vector<unsigned int> original);
		
		virtual vector<unsigned int> rotateNegY(vector<unsigned int> original);
		
		virtual vector<unsigned int> rotatePosZ(vector<unsigned int> original);
		
		virtual vector<unsigned int> rotateNegZ(vector<unsigned int> original);
		
	};
}
#endif
