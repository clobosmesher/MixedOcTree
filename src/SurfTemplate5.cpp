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

#include "SurfTemplate5.h"

namespace Clobscode
{
	SurfTemplate5::SurfTemplate5(){
	}
	
	SurfTemplate5::~SurfTemplate5(){
	}
	
	bool SurfTemplate5::getSubelements(vector<unsigned int> &all, 
									   vector<unsigned int> &in,
									   vector<MeshPoint> &pts,
									   list<MeshPoint> &newpts,
									   vector<vector<unsigned int> > &neweles,
									   TriMesh &input,
									   list<unsigned int> &intersectedfaces,
									   unsigned int &e_idx){
		
		HexRotation hrot;
		vector<unsigned int> rotated,number (5,0);
		if(in.size()!=5)
			return false;
		rotated.reserve(8);
		
		//searh by edges:
		int total=0;
		for (unsigned int i=0;i<5;i++){
			vector<unsigned int> edsi = getEdges(in[i]);
			for (unsigned int j=0;j<edsi.size();j++){
				for (unsigned int k=0;k<5;k++){
					if(in[k]==edsi[j]){
						total++;
						number[i]++;
						continue;
					}
				}	
			}
		}
		//pattern A will send back the same Hexahedra
		if(total==10){
			//find the 3-connected and 1-connected node.
			unsigned int con3=0,con1=0;
			for (unsigned int i=0;i<number.size();i++){
				if(number[i]==3)
					con3=in[i];
				else if(number[i]==1)
					con1=in[i];
			}
			rotated = hrot.rotate(all,con3);
			if(rotated[1]==all[con1]){
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
			}
			else if(rotated[3]==all[con1]){
				rotated = hrot.rotatePosZ(rotated);
				rotated = hrot.rotatePosY(rotated);
			}
			
			PatternA(rotated,pts,newpts,neweles,input,intersectedfaces,e_idx);
			return true;
		}
		else if(total==8){
			vector<unsigned int> doubles;
			doubles.reserve(3);
			for (unsigned int i=0;i<number.size();i++){
				if(number[i]==2)
					doubles.push_back(in[i]);
			}
			doubles = searchPivote(doubles);
			rotated = hrot.rotate(all,doubles[0]);
			if((rotated[4]==all[doubles[1]] && rotated[3]==all[doubles[2]]) || 
			   (rotated[3]==all[doubles[1]] && rotated[4]==all[doubles[2]])){
				rotated = hrot.rotateNegX(rotated);
				rotated = hrot.rotateNegY(rotated);
			}
			else if((rotated[4]==all[doubles[1]] && rotated[1]==all[doubles[2]]) || 
					(rotated[1]==all[doubles[1]] && rotated[4]==all[doubles[2]])){
				rotated = hrot.rotatePosZ(rotated);
				rotated = hrot.rotatePosY(rotated);
			}
			PatternB(rotated,neweles);
			return true;
		}
		else if(total==6){
			int pivote=0;
			for (unsigned int i=0;i<number.size();i++)
				if(number[i]==3){
					pivote=in[i];
					break;
				}
			rotated = hrot.rotate(all,pivote);
			PatternC(rotated,neweles);
			return true;
		}
		else{
			std::cerr << "Pattern not found for this configuration:\nin nodes:";
			for(unsigned int i=0;i<in.size();i++)
				std::cerr << " " << in[i];
		}
		
		return false;
	}
	
	vector<unsigned int> SurfTemplate5::getEdges(int p){
		vector<unsigned int> edges (3,0);
		if(p==0){
			edges[0]=1;
			edges[1]=3;
			edges[2]=4;
		}
		else if(p==1){
			edges[0]=0;
			edges[1]=2;
			edges[2]=5;
		}
		else if(p==2){
			edges[0]=1;
			edges[1]=3;
			edges[2]=6;
		}
		else if(p==3){
			edges[0]=2;
			edges[1]=0;
			edges[2]=7;
		}
		else if(p==4){
			edges[0]=0;
			edges[1]=5;
			edges[2]=7;
		}
		else if(p==5){
			edges[0]=1;
			edges[1]=4;
			edges[2]=6;
		}
		else if(p==6){
			edges[0]=2;
			edges[1]=7;
			edges[2]=5;
		}
		else if(p==7){
			edges[0]=3;
			edges[1]=6;
			edges[2]=4;
		}
		return edges;
	}
	
	//Pattern C in paper
	/*Note 2011: this is different to paper illustration, it's computed
	 as four nodes in the base square face (0,1,2,3) and the other one
	 is on top of node 0 (this is node 4). In paper figure this last
	 node correspond to node 6*/
	
	void SurfTemplate5::PatternA(vector<unsigned int> &all, vector<MeshPoint> &pts,
								 list<MeshPoint> &newpts,
								 vector<vector<unsigned int> > &eles,
								 TriMesh &input,
								 list<unsigned int> &intersectedfaces,
								 unsigned int &e_idx){
		
		eles.clear();
		eles.reserve(9);
		
		/*  this pattern needs ipc has the hexahedra must be split in 6 piramids first,
		 *  then 3 of them will be replaced by 6 tetras. 
		 */
		
		Point3D middle;
		unsigned int mid = 0;
		for (unsigned int i=0;i<all.size();i++){
			middle+=pts.at(all[i]).getPoint();
		}
		middle/=8;
		MeshPoint mp(middle);
		
		bool mid_projected = false;
		
		mp.outsideChecked();
		mp.addElement(e_idx);
		
            //Detect if new node is inside the input mesh. And if so,
            //try to project it into the surface.
            
        if (intersectedfaces.empty()) {
            mp.setIOState(pts[all[0]].getIOState());
        }
        else {
            if (input.pointIsInMesh(middle,intersectedfaces)) {
                mp.setInside();
                mp.setMaxDistance(pts[all[0]].getMaxDistance()*0.3);
                
                Point3D projected = input.getProjection(middle, intersectedfaces);
                double dis = (middle - projected).Norm();
                
                if(dis<mp.getMaxDistance()){
                    //this node have been moved to boundary, thus every element
                    //sharing this node must be set as a border element in order
                    //to avoid topological problems.
                    mp.setIOState(false);
                    //mp.setPoint(projected);
                    mid_projected = true;
                }
            }
        }

    
		mid = pts.size() + newpts.size();
		newpts.push_back(mp);
		
		vector<unsigned int> ele1(5,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(5,0);
		vector<unsigned int> ele4(4,0);
		vector<unsigned int> ele5(4,0);
		vector<unsigned int> ele6(4,0);
		vector<unsigned int> ele7(4,0);
		vector<unsigned int> ele8(4,0);
		vector<unsigned int> ele9(4,0);
		
		ele1[0] = all[0];
		ele1[1] = all[1];
		ele1[2] = all[2];
		ele1[3] = all[3];
		ele1[4] = mid;
		
		ele2[0] = all[1];
		ele2[1] = all[5];
		ele2[2] = all[6];
		ele2[3] = all[2];
		ele2[4] = mid;
		
		ele3[0] = all[2];
		ele3[1] = all[6];
		ele3[2] = all[7];
		ele3[3] = all[3];
		ele3[4] = mid;
		
		ele4[0] = all[0];
		ele4[1] = all[7];
		ele4[2] = all[4];
		ele4[3] = mid;
		
		ele5[0] = all[0];
		ele5[1] = all[3];
		ele5[2] = all[7];
		ele5[3] = mid;
		
		ele6[0] = all[0];
		ele6[1] = all[4];
		ele6[2] = all[5];
		ele6[3] = mid;
		
		ele7[0] = all[0];
		ele7[1] = all[5];
		ele7[2] = all[1];
		ele7[3] = mid;
		
		ele8[0] = all[4];
		ele8[1] = all[7];
		ele8[2] = all[5];
		ele8[3] = mid;
		
		ele9[0] = all[6];
		ele9[1] = all[5];
		ele9[2] = all[7];
		ele9[3] = mid;
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		eles.push_back(ele4);
		eles.push_back(ele5);
		eles.push_back(ele6);
		eles.push_back(ele7);
		eles.push_back(ele8);
		
		//if the middle node was close to input domain (meaning
		//it was projected) this tetrahedron must no be included.
		if (!mp.isOutside()) {
			eles.push_back(ele9);
		}
		
		//eles.push_back(ele9);

	}
	
	//Pattern B in paper
	void SurfTemplate5::PatternB(vector<unsigned int> &all,
								vector<vector<unsigned int> > &eles){
		
		eles.reserve(4);
		vector<unsigned int> ele1(5,0);
		vector<unsigned int> ele2(5,0);
		vector<unsigned int> ele3(4,0);
		vector<unsigned int> ele4(4,0);
		
		ele1[0] = all[5];
		ele1[1] = all[6];
		ele1[2] = all[2];
		ele1[3] = all[1];
		ele1[4] = all[4];
		
		ele2[0] = all[7];
		ele2[1] = all[3];
		ele2[2] = all[2];
		ele2[3] = all[6];
		ele2[4] = all[4];
		
		ele3[0] = all[0];
		ele3[1] = all[1];
		ele3[2] = all[2];
		ele3[3] = all[4];
		
		ele4[0] = all[0];
		ele4[1] = all[2];
		ele4[2] = all[3];
		ele4[3] = all[4];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		eles.push_back(ele4);
		
	}
	
	//Pattern A in paper
	void SurfTemplate5::PatternC(vector<unsigned int> &all, 
								vector<vector<unsigned int> > &eles){
		
		eles.reserve(5);
		
		vector<unsigned int> ele1(4,0);
		vector<unsigned int> ele2(4,0);
		vector<unsigned int> ele3(4,0);
		vector<unsigned int> ele4(4,0);
		vector<unsigned int> ele5(4,0);
		
		ele1[0] = all[4];
		ele1[1] = all[7];
		ele1[2] = all[5];
		ele1[3] = all[0];
		
		ele2[0] = all[6];
		ele2[1] = all[5];
		ele2[2] = all[7];
		ele2[3] = all[2];
		
		ele3[0] = all[1];
		ele3[1] = all[2];
		ele3[2] = all[0];
		ele3[3] = all[5];
		
		ele4[0] = all[3];
		ele4[1] = all[0];
		ele4[2] = all[2];
		ele4[3] = all[7];
		
		ele5[0] = all[0];
		ele5[1] = all[2];
		ele5[2] = all[7];
		ele5[3] = all[5];
		
		eles.push_back(ele1);
		eles.push_back(ele2);
		eles.push_back(ele3);
		eles.push_back(ele4);
		eles.push_back(ele5);
		
	}
	
	//between 3 nodes, search the one that is connected to the other two.
	//This last node will be at the first position of the argument array.
	vector<unsigned int> SurfTemplate5::searchPivote(vector<unsigned int> &doubles){
		vector<unsigned int> edges = getEdges(doubles[0]),order(3,0);
		int con1=0,con2=0;
		for (unsigned int i=0;i<edges.size();i++){
			if(edges[i]==doubles[1]) 
				con1=doubles[1];
			else if(edges[i]==doubles[2])
				con2=doubles[2];
		}
		if(con1==0){
			order[0] = doubles[2];
			order[1] = doubles[1];
			order[2] = doubles[0];
		}
		else if(con2==0){
			order[0] = doubles[1];
			order[1] = doubles[2];
			order[2] = doubles[0];
		}
		else{
			order[0] = doubles[0];
			order[1] = doubles[1];
			order[2] = doubles[2];
		}
		return order;
	}
}
