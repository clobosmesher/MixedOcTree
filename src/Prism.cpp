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

#include "stdafx.h"
#include "Prism.h"

namespace Clobscode
{
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	Prism::Prism(vector<unsigned int> &mypoints){
        if (mypoints.size()!=6) {
            std::cerr << "warning at Prism constructor: not 6 node element\n";
        }
        
        points = mypoints;
	}
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	Prism::~Prism(){
	
    }
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    vector<vector<unsigned int> > Prism::getSubElements(vector<bool> &inside_state){
    
        vector<vector<unsigned int> > sub_ele;
        
        //detect diagonals over
        vector<unsigned int> f1(4,0),f2(4,0),f3(4,0);
        f1[0] = 0; f1[1] = 1; f1[2] = 4; f1[3] = 3;
        f2[0] = 0; f2[1] = 3; f2[2] = 5; f2[3] = 2;
        f3[0] = 1; f3[1] = 2; f3[2] = 5; f3[3] = 4;
        
        vector<unsigned int> diag_in_1(2,0), diag_in_2(2,0), diag_in_3(2,0);
        list<unsigned int> diag_nodes, diag_nodes_sorted;
        list<unsigned int>::iterator diag_iter;
        
        FaceDiagonal fd1 (f1);
        if (fd1.getDiagonal(inside_state,diag_in_1)) {
            diag_nodes.push_back(diag_in_1[0]);
            diag_nodes.push_back(diag_in_1[1]);
        }
        
        FaceDiagonal fd2 (f2);
        if (fd2.getDiagonal(inside_state,diag_in_2)) {
            diag_nodes.push_back(diag_in_2[0]);
            diag_nodes.push_back(diag_in_2[1]);
        }
        
        FaceDiagonal fd3 (f3);
        if (fd3.getDiagonal(inside_state,diag_in_3)) {
            diag_nodes.push_back(diag_in_3[0]);
            diag_nodes.push_back(diag_in_3[1]);
        }
        
        vector<unsigned int> sorted_idx = sortDiagonals(diag_nodes);
        
        //if the number of "diags" is 4, then we have to retrun
        //a prism and tetahedron.
        if (sorted_idx.size()==3) {
            
            vector<unsigned int> pyramid (5,0), base(4,0);
            vector<unsigned int> tetahedron (4,0);
            
            oppositeFace(sorted_idx[1],base);
            
            for (unsigned int i=0; i<4; i++) {
                pyramid[i] = points[base[i]];
            }
            
            pyramid[4] = points[sorted_idx[1]];
            sub_ele.push_back(pyramid);
            
            for (unsigned int i=0; i<3; i++) {
                tetahedron[i] = points[sorted_idx[i]];
            }
            
            tetahedron[3] = points[opposite(sorted_idx[1])];
            
            sub_ele.push_back(tetahedron);
            
        }
        
        if (sorted_idx.size()==4) {
            vector<unsigned int> tet1 (4,0), tet2 (4,0), tet3 (4,0);
            
            tet1[0] = points[sorted_idx[0]];
            tet1[1] = points[sorted_idx[2]];
            tet1[2] = points[sorted_idx[1]];
            tet1[3] = points[opposite(sorted_idx[1])];
            
            sub_ele.push_back(tet1);
            
            
            tet2[0] = points[sorted_idx[1]];
            tet2[1] = points[sorted_idx[2]];
            tet2[2] = points[sorted_idx[3]];
            tet2[3] = points[opposite(sorted_idx[2])];
            
            sub_ele.push_back(tet2);
            
            tet3[0] = points[sorted_idx[0]];
            tet3[1] = points[sorted_idx[3]];
            tet3[2] = points[sorted_idx[1]];
            tet3[3] = points[sorted_idx[2]];
            
            sub_ele.push_back(tet3);
            
        }
        
        return sub_ele;
    }
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    vector<unsigned int> Prism::sortDiagonals(list<unsigned int> &original){
        list<unsigned int>::iterator iter;
        vector<unsigned int> org_vec, result;
        
        for (iter=original.begin(); iter!=original.end(); iter++) {
            org_vec.push_back(*iter);
        }
        
        org_vec.reserve(original.size());
        if (original.size()==4) {
            result.assign(3,0);
            
            if (!oneRepeated(org_vec,result)){
                std::cerr << "warning malformed prism found at Prism::sortDiagonals\n";
            }
            return result;
            
        }
        else{
            if (original.size()==6) {
                result.assign(4,0);
                
                list<unsigned int> tmp;
                vector<unsigned int> first(3,0);
                
                if (oneRepeated(org_vec,first)) {
                    if (first[0]==org_vec[4]) {
                        result[0] = org_vec[5];
                        result[1] = first[0];
                        result[2] = first[1];
                        result[3] = first[2];
                        return result;
                    }
                    if (first[0]==org_vec[5]) {
                        result[0] = org_vec[4];
                        result[1] = first[0];
                        result[2] = first[1];
                        result[3] = first[2];
                        return result;
                    }
                    if (first[2]==org_vec[4]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[5];
                        return result;
                    }
                    if (first[2]==org_vec[5]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[4];
                        return result;
                    }
                }
                else{
                    vector<unsigned int> aux(4,0);
                    for (unsigned int i=0; i<4; i++) {
                        aux[i] = org_vec[i+2];
                    }
                    if (!oneRepeated(aux,first)) {
                        std::cerr << "warning malformed prism found at Prism::sortDiagonals\n";
                    }
                    
                    if (first[2]==org_vec[0]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[1];
                        return result;
                    }
                    
                    if (first[2]==org_vec[1]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[0];
                        return result;
                    }
                    
                    else{
                        std::cerr << "warning malformed prism found at Prism::sortDiagonals\n";
                    }
                }
            }
        }
        
        return result;
        
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    bool Prism::oneRepeated(vector<unsigned int> &original, vector<unsigned int> &result){
        if (original[0] == original[2]) {
            result[0] = original[1];
            result[1] = original[0];
            result[2] = original[3];
            return true;
        }
        
        if (original[0] == original[3]) {
            result[0] = original[1];
            result[1] = original[0];
            result[2] = original[2];
            return true;
        }
        
        if (original[1] == original[2]) {
            result[0] = original[0];
            result[1] = original[1];
            result[2] = original[3];
            return true;
        }
        
        if (original[1] == original[3]) {
            result[0] = original[0];
            result[1] = original[1];
            result[2] = original[2];
            return true;
        }
        
        return false;
    }
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    unsigned int Prism::opposite(unsigned int idx){
        return (idx+3)%6;
    }

    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    void Prism::oppositeFace(unsigned int idx, vector<unsigned int> &base){
        if (idx==0 || idx==3) {
            base[0] = 1;
            base[1] = 4;
            base[2] = 5;
            base[3] = 2;
            return;
        }
        if (idx==1 || idx==4) {
            base[0] = 0;
            base[1] = 2;
            base[2] = 5;
            base[3] = 3;
            return;
        }
        if (idx==2 || idx==5) {
            base[0] = 0;
            base[1] = 3;
            base[2] = 4;
            base[3] = 1;
            return;
        }
    }

	
	/******************
	 * This class manage the different cases of 3 points inside patterns.
	 * Once the pattern is identified, the corresponding class will handle
	 * the elements to be returned (
	 *//// 
}

