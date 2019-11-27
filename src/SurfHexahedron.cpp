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

#include "SurfHexahedron.h"

namespace Clobscode
{
 
    /*
     NOTA IMPORTANTE:
     mejorar esta clase y Octant para que la segunda no tenga estos mismos métodos,
     Además se pueden mejorar varias cosas en esta clase: imitar el tratamiento de 
     puntos in/out en un solo vector, como se hace para prismas, etc.
     
     */
    
    
    
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfHexahedron::SurfHexahedron(vector<unsigned int> &mypoints){
        if (mypoints.size()!=8) {
            std::cerr << "warning at SurfHexahedron constructor: not 6 node element\n";
        }
        
        points = mypoints;
	}
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfHexahedron::~SurfHexahedron(){
	
    }
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    vector<vector<unsigned int> > SurfHexahedron::getSubElements(vector<MeshPoint> &meshpoints, list<MeshPoint> &newpts,
                                                                 TriMesh &input, unsigned int &e_idx,
                                                                 vector<vector<unsigned int> > &possibles,
                                                                 vector<vector<unsigned int> > &continuity,
                                                                 list<unsigned int> &intersected_faces){
    
        vector<vector<unsigned int> > newsubs;
        
        
        vector<unsigned int> inpts, outpts;
        list<unsigned int> tmpin,tmpout;
        list<unsigned int>::iterator piter;
        
        for (unsigned int i=0; i<points.size(); i++) {
            
            if (meshpoints.at(points[i]).isOutside()) {
                tmpout.push_back(i);
            }
            else {
                tmpin.push_back(i);
            }
        }
        
        //save the points in a std::vector for quick acces
        inpts.reserve(tmpin.size());
        for(piter=tmpin.begin();piter!=tmpin.end();piter++)
            inpts.push_back(*piter);
        outpts.reserve(tmpout.size());
        for(piter=tmpout.begin();piter!=tmpout.end();piter++)
            outpts.push_back(*piter);
        
        
        //select the patter to apply
        switch (inpts.size()) {
            case 0:{
                /*If at this point, the element has 0 node inside,
                 it might be tangencial to input mesh, in which case
                 it should be removed, or represent a feature of
                 the domain (e.g. all nodes outside, but there is
                 something like a pipeline crossing it). This algorithm
                 isn't yet "future sensitive", therefore the element
                 is simply removed.
                 */
                cerr << "warning at SurfHexahedron::getSubElements 0 inside nodes\n";
                break;
            }
            case 1: {
                SurfTemplate1 surf_t1;
                surf_t1.getSubelements(points,inpts,newsubs);
                break;
            }
            case 2: {
                //erase last argument ("possibles") to maximum avoid eventual quality issues.
                SurfTemplate2 surf_t2;
                surf_t2.getSubelements(points,inpts,newsubs,possibles,continuity);
                break;
            }
            case 3: {
                //erase last argument ("possibles") to maximum avoid eventual quality issues.
                SurfTemplate3 surf_t3;
                surf_t3.getSubelements(points,inpts,newsubs,possibles);
                break;
            }
            case 4: {
                SurfTemplate4 surf_t4;
                surf_t4.getSubelements(points,inpts,newsubs);
                break;
            }
            case 5: {
                SurfTemplate5 surf_t5;
                unsigned int old_size = newpts.size();
                surf_t5.getSubelements(points,inpts,meshpoints,newpts,newsubs,input,intersected_faces,e_idx);
                break;
            }
            case 6: {
                SurfTemplate6 surf_t6;
                surf_t6.getSubelements(points,outpts,newsubs);
                break;
            }
            case 7: {
                SurfTemplate7 surf_t7;
                surf_t7.getSubelements(points,outpts,newsubs);
                break;
            }
            case 8: {
                //If this happens the element is inside the overall
                //geometry, but intersects inner features.
                newsubs.push_back(points);
                break;
            }
            default: {
                cerr << " Error at EnhancedElement::applySurfacePatterns\n";
                cerr << " Number of inside nodes: " << inpts.size() << "\n";
                cerr << " Surface Patterns must be applied over elements";
                cerr << " with 1 to 7 inside nodes.\n";
                break;
            }
        }
        
        return newsubs;
    }
}

