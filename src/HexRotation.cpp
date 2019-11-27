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

#include "HexRotation.h"

namespace Clobscode
{
HexRotation::HexRotation(){
}

HexRotation::~HexRotation(){
}

//class idea:
//The original points will be rearrenged as if index were now 
//point 0 in the hexa node cofiguration.
vector<unsigned int> HexRotation::rotate(vector<unsigned int> original, 
										 int index){
  vector<unsigned int> rotated(8,0);
  switch(index){
    case 0:
      return original;		  
	/*node 1 is now node 0*/
    case 1:
      rotated[0] = original[1];
      rotated[1] = original[2];
      rotated[2] = original[3];
      rotated[3] = original[0];
      rotated[4] = original[5];
      rotated[5] = original[6];
      rotated[6] = original[7];
      rotated[7] = original[4];
      return rotated;
    case 2:
	/*node 2 is now node 0*/
      rotated[0] = original[2];
      rotated[1] = original[3];
      rotated[2] = original[0];
      rotated[3] = original[1];
      rotated[4] = original[6];
      rotated[5] = original[7];
      rotated[6] = original[4];
      rotated[7] = original[5];
      return rotated;
	/*node 3 is now node 0*/
	case 3:
      rotated[0] = original[3];
      rotated[1] = original[0];
      rotated[2] = original[1];
      rotated[3] = original[2];
      rotated[4] = original[7];
      rotated[5] = original[4];
      rotated[6] = original[5];
      rotated[7] = original[6];
      return rotated;
	/*node 4 is now node 0*/
    case 4:
      rotated[0] = original[4];
      rotated[1] = original[5];
      rotated[2] = original[1];
      rotated[3] = original[0];
      rotated[4] = original[7];
      rotated[5] = original[6];
      rotated[6] = original[2];
      rotated[7] = original[3];
      return rotated;
	/*node 5 is now node 0*/
    case 5:
      rotated[0] = original[5];
      rotated[1] = original[1];
      rotated[2] = original[0];
      rotated[3] = original[4];
      rotated[4] = original[6];
      rotated[5] = original[2];
      rotated[6] = original[3];
      rotated[7] = original[7];
      return rotated;
	/*node 6 is now node 0*/
	case 6:
      rotated[0] = original[6];
      rotated[1] = original[5];
      rotated[2] = original[4];
      rotated[3] = original[7];
      rotated[4] = original[2];
      rotated[5] = original[1];
      rotated[6] = original[0];
      rotated[7] = original[3];
      return rotated;
	/*node 7 is now node 0*/
    case 7:
      rotated[0] = original[7];
      rotated[1] = original[6];
      rotated[2] = original[5];
      rotated[3] = original[4];
      rotated[4] = original[3];
      rotated[5] = original[2];
      rotated[6] = original[1];
      rotated[7] = original[0];
      return rotated;
    default:
      return rotated;
  }
}

vector<unsigned int> HexRotation::rotatePosX(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[1];
  rotated[1] = original[5];
  rotated[2] = original[6];
  rotated[3] = original[2];
  rotated[4] = original[0];
  rotated[5] = original[4];
  rotated[6] = original[7];
  rotated[7] = original[3];
  return rotated;
}

vector<unsigned int> HexRotation::rotateNegX(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[4];
  rotated[1] = original[0];
  rotated[2] = original[3];
  rotated[3] = original[7];
  rotated[4] = original[5];
  rotated[5] = original[1];
  rotated[6] = original[2];
  rotated[7] = original[6];
  return rotated;
}

vector<unsigned int> HexRotation::rotatePosY(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[3];
  rotated[1] = original[0];
  rotated[2] = original[1];
  rotated[3] = original[2];
  rotated[4] = original[7];
  rotated[5] = original[4];
  rotated[6] = original[5];
  rotated[7] = original[6];
  return rotated;
}

vector<unsigned int> HexRotation::rotateNegY(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[1];
  rotated[1] = original[2];
  rotated[2] = original[3];
  rotated[3] = original[0];
  rotated[4] = original[5];
  rotated[5] = original[6];
  rotated[6] = original[7];
  rotated[7] = original[4];
  return rotated;
}

vector<unsigned int> HexRotation::rotatePosZ(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[4];
  rotated[1] = original[5];
  rotated[2] = original[1];
  rotated[3] = original[0];
  rotated[4] = original[7];
  rotated[5] = original[6];
  rotated[6] = original[2];
  rotated[7] = original[3];
  return rotated;
}

vector<unsigned int> HexRotation::rotateNegZ(vector<unsigned int> original){
  vector<unsigned int> rotated(8,0);
  rotated[0] = original[3];
  rotated[1] = original[2];
  rotated[2] = original[6];
  rotated[3] = original[7];
  rotated[4] = original[0];
  rotated[5] = original[1];
  rotated[6] = original[5];
  rotated[7] = original[4];
  return rotated;
}
}
