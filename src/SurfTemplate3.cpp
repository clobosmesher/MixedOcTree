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

#include "SurfTemplate3.h"

namespace Clobscode
{
	SurfTemplate3::SurfTemplate3(){
	}
	
	SurfTemplate3::~SurfTemplate3(){
	}
	
	bool SurfTemplate3::getSubelements(vector<unsigned int> &all, 
									  vector<unsigned int> &in,
									  vector<vector<unsigned int> > &neweles){
		
		HexRotation hrot;
		vector<unsigned int> rotated;
		vector<unsigned int> pivotes(3,0);
		if(in.size()!=3)
			return false;
		
		bool edge2 = false;
		
		//center all at the first node:
		rotated = hrot.rotate(all,in[0]);
		
		//find out if rotating we can conform an edge
		//of inner points between (0,3)
		if(rotated[3] == all[in[1]]){
			edge2 = true;
		}
		else if(rotated[3] == all[in[2]]){
			edge2 = true;
			swap(in,1,2);
		}
		else if(rotated[4] == all[in[1]]){
			edge2 = true;
			rotated = hrot.rotatePosZ(rotated);
			swap(in,0,1);
		}
		else if(rotated[4] == all[in[2]]){
			edge2 = true;
			rotated = hrot.rotatePosZ(rotated);
			swap(in,0,2);
			swap(in,1,2);
		}
		else if(rotated[1] == all[in[1]]){
			edge2 = true;
			rotated = hrot.rotateNegY(rotated);
			swap(in,0,1);
		}
		else if(rotated[1] == all[in[2]]){
			edge2 = true;
			rotated = hrot.rotateNegY(rotated);
			swap(in,0,2);
			swap(in,1,2);
		}
		
		//if edge2 is true, then the current edge (0,3) has inner points
		//and the one to test is in[2]: possible patterns: A,B,C
		if(edge2){
			if(rotated[1] == all[in[2]]){
				//the pattern is A and no rotation is needed
				PatternA(rotated,neweles);
				return true;
			}
			else if(rotated[2] == all[in[2]]){
				//the pattern is A and Positive Y rotation is needed
				rotated = hrot.rotatePosY(rotated);
				PatternA(rotated,neweles);
				return true;
			}
			else if(rotated[4] == all[in[2]]){
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
				PatternA(rotated,neweles);
				return true;
			}
			else if(rotated[7] == all[in[2]]){
				rotated = hrot.rotateNegZ(rotated);
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
				PatternA(rotated,neweles);
				return true;
			}
			else if(rotated[5] == all[in[2]]){
				PatternB(rotated,neweles);
				return true;
			}
			else if(rotated[6] == all[in[2]]){
				PatternC(rotated,neweles);
				return true;
			}
			else{
				std::cout << " Warning: inconsistency detected at ";
				std::cout << "SurfTemplate3::getSubElements\n";
				std::cout << " configuration (0,1) and no third point.\n";
				std::cout << " nodes inside";
				for (unsigned int i=0; i<in.size(); i++) {
					std::cout << " " << in[i];
				}
				std::cout << std::endl;
				return false;
			}
		}
		//if we are here, edge2 is false, therefore nodes 1,3 and 4 aren't
		//inside.
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[6] == all[in[1]] || rotated[6] == all[in[2]])){
			rotated = hrot.rotateNegX(rotated);
			rotated = hrot.rotateNegX(rotated);
			PatternB(rotated,neweles);
			return true;
		}
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[7] == all[in[1]] || rotated[7] == all[in[2]])){
			rotated = hrot.rotateNegX(rotated);
			rotated = hrot.rotateNegX(rotated);
			PatternD(rotated,neweles);
			return true;
		}
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			PatternD(rotated,neweles);
			return true;
		}
		if((rotated[6] == all[in[1]] || rotated[6] == all[in[2]]) &&
		   (rotated[7] == all[in[1]] || rotated[7] == all[in[2]])){
			rotated = hrot.rotateNegZ(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternB(rotated,neweles);
			return true;
		}
		if((rotated[6] == all[in[1]] || rotated[6] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			rotated = hrot.rotateNegZ(rotated);
			rotated = hrot.rotatePosY(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternC(rotated,neweles);
			return true;
		}
		if((rotated[7] == all[in[1]] || rotated[7] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			rotated = hrot.rotatePosY(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternD(rotated,neweles);
			return true;
		}
		
		std::cerr << "Pattern not found for this configuration:\nin nodes:";
		for(unsigned int i=0;i<in.size();i++)
			std::cerr << " " << in[i];
		return false;
	}
    
    bool SurfTemplate3::getSubelements(vector<unsigned int> &all,
                                       vector<unsigned int> &in,
                                       vector<vector<unsigned int> > &neweles,
                                       vector<vector<unsigned int> > &possibles){
		
		HexRotation hrot;
		vector<unsigned int> rotated;
		vector<unsigned int> pivotes(3,0);
		if(in.size()!=3)
			return false;
		
		bool edge2 = false;
		
		//center all at the first node:
		rotated = hrot.rotate(all,in[0]);
		
		//find out if rotating we can conform an edge
		//of inner points between (0,3)
		if(rotated[3] == all[in[1]]){
			edge2 = true;
		}
		else if(rotated[3] == all[in[2]]){
			edge2 = true;
			swap(in,1,2);
		}
		else if(rotated[4] == all[in[1]]){
			edge2 = true;
			rotated = hrot.rotatePosZ(rotated);
			swap(in,0,1);
		}
		else if(rotated[4] == all[in[2]]){
			edge2 = true;
			rotated = hrot.rotatePosZ(rotated);
			swap(in,0,2);
			swap(in,1,2);
		}
		else if(rotated[1] == all[in[1]]){
			edge2 = true;
			rotated = hrot.rotateNegY(rotated);
			swap(in,0,1);
		}
		else if(rotated[1] == all[in[2]]){
			edge2 = true;
			rotated = hrot.rotateNegY(rotated);
			swap(in,0,2);
			swap(in,1,2);
		}
		
		//if edge2 is true, then the current edge (0,3) has inner points
		//and the one to test is in[2]: possible patterns: A,B,C
		if(edge2){
			if(rotated[1] == all[in[2]]){
				//the pattern is A and no rotation is needed
				PatternA(rotated,neweles,possibles);
				return true;
			}
			else if(rotated[2] == all[in[2]]){
				//the pattern is A and Positive Y rotation is needed
				rotated = hrot.rotatePosY(rotated);
				PatternA(rotated,neweles,possibles);
				return true;
			}
			else if(rotated[4] == all[in[2]]){
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
				PatternA(rotated,neweles,possibles);
				return true;
			}
			else if(rotated[7] == all[in[2]]){
				rotated = hrot.rotateNegZ(rotated);
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
				PatternA(rotated,neweles,possibles);
				return true;
			}
			else if(rotated[5] == all[in[2]]){
				PatternB(rotated,neweles,possibles);
				return true;
			}
			else if(rotated[6] == all[in[2]]){
				PatternC(rotated,neweles,possibles);
				return true;
			}
			else{
				std::cout << " Warning: inconsistency detected at ";
				std::cout << "SurfTemplate3::getSubElements\n";
				std::cout << " configuration (0,1) and no third point.\n";
				std::cout << " nodes inside";
				for (unsigned int i=0; i<in.size(); i++) {
					std::cout << " " << in[i];
				}
				std::cout << std::endl;
				return false;
			}
		}
		//if we are here, edge2 is false, therefore nodes 1,3 and 4 aren't
		//inside.
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[6] == all[in[1]] || rotated[6] == all[in[2]])){
			rotated = hrot.rotateNegX(rotated);
			rotated = hrot.rotateNegX(rotated);
			PatternB(rotated,neweles,possibles);
			return true;
		}
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[7] == all[in[1]] || rotated[7] == all[in[2]])){
			rotated = hrot.rotateNegX(rotated);
			rotated = hrot.rotateNegX(rotated);
			PatternD(rotated,neweles);
			return true;
		}
		if((rotated[5] == all[in[1]] || rotated[5] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			PatternD(rotated,neweles);
			return true;
		}
		if((rotated[6] == all[in[1]] || rotated[6] == all[in[2]]) &&
		   (rotated[7] == all[in[1]] || rotated[7] == all[in[2]])){
			rotated = hrot.rotateNegZ(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternB(rotated,neweles,possibles);
			return true;
		}
		if((rotated[6] == all[in[1]] || rotated[6] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			rotated = hrot.rotateNegZ(rotated);
			rotated = hrot.rotatePosY(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternC(rotated,neweles,possibles);
			return true;
		}
		if((rotated[7] == all[in[1]] || rotated[7] == all[in[2]]) &&
		   (rotated[2] == all[in[1]] || rotated[2] == all[in[2]])){
			rotated = hrot.rotatePosY(rotated);
			rotated = hrot.rotatePosY(rotated);
			PatternD(rotated,neweles);
			return true;
		}
		
		std::cerr << "Pattern not found for this configuration:\nin nodes:";
		for(unsigned int i=0;i<in.size();i++)
			std::cerr << " " << in[i];
		return false;
	}
	
	
	//Pattern C in paper
	void SurfTemplate3::PatternA(vector<unsigned int> &all, vector<vector<unsigned int> > &eles){
		
		eles.reserve(2);
		
		vector<unsigned int> ele1 (5,0);
		vector<unsigned int> ele2 (5,0);
		//vector<unsigned int> ele3 (4,-1);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[5];
		ele1[3] = all[1];
		ele1[4] = all[2];
		
		ele2[0] = all[0];
		ele2[1] = all[3];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[2];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		
	}
    
    //Pattern C in paper
	void SurfTemplate3::PatternA(vector<unsigned int> &all,
                                 vector<vector<unsigned int> > &eles,
                                 vector<vector<unsigned int> > &poss){
		
		eles.reserve(2);
        poss.reserve(1);
		
		vector<unsigned int> ele1 (5,0);
		vector<unsigned int> ele2 (5,0);
		vector<unsigned int> ele3 (4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[5];
		ele1[3] = all[1];
		ele1[4] = all[2];
		
		ele2[0] = all[0];
		ele2[1] = all[3];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[2];
		
        ele3[0] = all[4];
        ele3[1] = all[7];
        ele3[2] = all[5];
        ele3[3] = all[2];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		
        poss.push_back(ele3);
		
	}
	
	
	/*In the paper the following 2 patterns are sumarized in just one: 
	 pattern 3B */
	
	void SurfTemplate3::PatternB(vector<unsigned int> &all, 
								vector<vector<unsigned int> > &eles){
		
		eles.reserve(3);
		/* this is a complex pattern that arises when no enough refinement
		 * is presented on the region. For this reason, a pyramid is added
		 * to avoid holes
		 */
		vector<unsigned int> ele1(6,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[1];
		ele1[3] = all[3];
		ele1[4] = all[7];
		ele1[5] = all[2];
		
		ele2[0] = all[1];
		ele2[1] = all[2];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[6];
		
		ele3[0] = all[5];
		ele3[1] = all[1];
		ele3[2] = all[4];
		ele3[3] = all[6];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		
	}
    
    void SurfTemplate3::PatternB(vector<unsigned int> &all,
                                 vector<vector<unsigned int> > &eles,
                                 vector<vector<unsigned int> > &poss){
		
		eles.reserve(2);
        poss.reserve(1);
		/* this is a complex pattern that arises when no enough refinement
		 * is presented on the region. For this reason, a pyramid is added
		 * to avoid holes
		 */
		vector<unsigned int> ele1(6,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[1];
		ele1[3] = all[3];
		ele1[4] = all[7];
		ele1[5] = all[2];
		
		ele2[0] = all[1];
		ele2[1] = all[2];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[6];
		
		ele3[0] = all[5];
		ele3[1] = all[1];
		ele3[2] = all[4];
		ele3[3] = all[6];
		
		eles.push_back(ele1);
		poss.push_back(ele2);
		eles.push_back(ele3);
		
	}
	
	void SurfTemplate3::PatternC(vector<unsigned int> &all, 
								vector<vector<unsigned int> > &eles){
		
		eles.reserve(3);
		/* this is a complex pattern that arises when no enough refinement
		 * is presented on the region. For this reason, a pyramid is added
		 * to avoid holes
		 */
		vector<unsigned int> ele1(6,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[1];
		ele1[3] = all[3];
		ele1[4] = all[7];
		ele1[5] = all[2];
		
		ele2[0] = all[1];
		ele2[1] = all[2];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[5];
		
		ele3[0] = all[6];
		ele3[1] = all[7];
		ele3[2] = all[2];
		ele3[3] = all[5];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		
	}
    
    void SurfTemplate3::PatternC(vector<unsigned int> &all,
                                 vector<vector<unsigned int> > &eles,
                                 vector<vector<unsigned int> > &poss){
		
		eles.reserve(2);
        poss.reserve(1);
		/* this is a complex pattern that arises when no enough refinement
		 * is presented on the region. For this reason, a pyramid is added
		 * to avoid holes
		 */
		vector<unsigned int> ele1(6,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[4];
		ele1[2] = all[1];
		ele1[3] = all[3];
		ele1[4] = all[7];
		ele1[5] = all[2];
		
		ele2[0] = all[1];
		ele2[1] = all[2];
		ele2[2] = all[7];
		ele2[3] = all[4];
		ele2[4] = all[5];
		
		ele3[0] = all[6];
		ele3[1] = all[7];
		ele3[2] = all[2];
		ele3[3] = all[5];
		
		eles.push_back(ele1);
		poss.push_back(ele2);
		eles.push_back(ele3);
		
	}
	
	//Pattern A in paper
	void SurfTemplate3::PatternD(vector<unsigned int> &all, 
								vector<vector<unsigned int> > &eles){
		
		eles.reserve(4);
		
		vector<unsigned int> ele1(4,0);
		vector<unsigned int> ele2(4,0);
		vector<unsigned int> ele3(4,0);
		vector<unsigned int> ele4(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[1];
		ele1[2] = all[3];
		ele1[3] = all[4];
		
		ele2[0] = all[5];
		ele2[1] = all[4];
		ele2[2] = all[6];
		ele2[3] = all[1];
		
		ele3[0] = all[2];
		ele3[1] = all[3];
		ele3[2] = all[1];
		ele3[3] = all[6];
		
		/* this tet is added to avoid a hole */
		ele4[0] = all[3];
		ele4[1] = all[6];
		ele4[2] = all[4];
		ele4[3] = all[1];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		eles.push_back(ele4);
		
	}
	
	void SurfTemplate3::swap(vector<unsigned int> &vec, int pos1, int pos2){
		int aux = vec[pos1];
		vec[pos1] = vec[pos2];
		vec[pos2] = aux;
	}
	
	/******************
	 * This class manage the different cases of 3 points inside patterns.
	 * Once the pattern is identified, the corresponding class will handle
	 * the elements to be returned (
	 *//// 
}

