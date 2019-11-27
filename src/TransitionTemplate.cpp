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

/*
 * PatternsDefault.cpp
 *
 *  Created on: May 18, 2012
 *      Author: jsepulve
 */

#include "TransitionTemplate.h"

using namespace std;

namespace patterns {

const TransitionTemplate::Permutations_t TransitionTemplate::RotationMatrix[PERMUTATIONS] = {

    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1), /// Z --> 360 --> 0

    Permutations_t(Y,1), /// Y --> 90  
    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1), /// Z --> 360 --> 0

    Permutations_t(Y,1), /// Y --> 180  
    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1), /// Z --> 360 --> 0

    Permutations_t(Y,1), /// Y --> 270  
    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1), /// Z --> 360 --> 0

    Permutations_t(Y,1), /// Y --> 360 --> 0  
    Permutations_t(X,1), /// X --> 90  
    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1), /// Z --> 360

    Permutations_t(X,2), /// X --> 270 X-->180 is the same Y-->180  
    Permutations_t(Z,1), /// Z --> 90  
    Permutations_t(Z,1), /// Z --> 180 
    Permutations_t(Z,1), /// Z --> 270
    Permutations_t(Z,1)  /// Z --> 360
};


TransitionTemplate::TransitionTemplate()  { 

    m_cube = new TransitionCube();

    // builds a map with default values of corner points (0,1,..7)
    createDefaultMapping(); 

    factory = PatternFactory::instance();
}

TransitionTemplate::TransitionTemplate(const UintVec& nodes) {

    m_cube = new TransitionCube();
    // makes map with values of input vector
    createNodesMapping(nodes);

    factory = PatternFactory::instance();
}

TransitionTemplate::TransitionTemplate(const UintVec& nodes, const UintVec& edges) {

    if (edges.size() > 0 ) {
        // Creates a map with nodes and edge values.
        createEdgesMapping(nodes, edges);

        // Create an instance of cube containing edge nodes
        m_cube = new TransitionCube(m_localVector);
    }
    else {
        createNodesMapping(nodes);
        m_cube = new TransitionCube();
    }

    factory = PatternFactory::instance();
}

void TransitionTemplate::createDefaultMapping() {

    for (Uint i=0; i<CORNER_POINTS; i++) 
        m_NodesMap.insert( make_pair( i, CubeMapping_t(i , i)) );

}

/// Insert input vector into internal map.
void TransitionTemplate::createNodesMapping(const UintVec& nodes) {

    Uint SIZE = CORNER_POINTS;
    if (nodes.size() < SIZE)
        SIZE = nodes.size();

    /// This map creates a link of first 8 internal number and external points
    // 0 --> (0, 23)
    UintVecIt it = nodes.begin();
    for (Uint i=0; i< SIZE; i++)
        m_NodesMap.insert( make_pair( i, CubeMapping_t(i,*it++)) );
}

/// Insert the input vector of edge point into local vector.
void TransitionTemplate::createEdgesMapping(const UintVec& nodes, const UintVec& edges) {

    // insert first eight input nodes into an internal map.
    createNodesMapping(nodes);

    m_localVector.clear();

    UintVecIt itn = nodes.begin() + CORNER_POINTS;
    UintVecIt ite = edges.begin();

    int SIZE;
    if ( nodes.size() > CORNER_POINTS) {

        SIZE = nodes.size() - CORNER_POINTS;

        if ( edges.size() < SIZE )
            SIZE = edges.size();

        //for ( ;itn != nodes.end(); ++itn) {
        for ( int i=0; i<SIZE; i++) {

            // This map is a link of internal and external points
            // The constructor was created with point 123 which is linked to 11
            // Example: 11 --> (11,123)
            m_NodesMap.insert( make_pair( *ite, CubeMapping_t( *ite, *(itn++) ) ) );

            //Keep a local copy of edge nodes.
            m_localVector.push_back( *(ite++) );

        }
    }
}

/// Rotate cube until find a pattern
bool TransitionTemplate::findPattern() {

    Coordinate axis;
    int step;

    // Max cube rotation defined in PERMUTATIONS
    for (int i=0; i<PERMUTATIONS; i++) {

        axis = RotationMatrix[i].Axis;
        step = RotationMatrix[i].Step;
        if (axis == X)
            m_cube->rotX(step);
        else if (axis == Y)
            m_cube->rotY(step);
        else if (axis ==Z) 
            m_cube->rotZ(step);

        //Check if rotated cube mask matchs to one the masks in factory.
        if( factory->createPattern( m_cube->getEdgeTransitionPointsMask() ) ){
            factory->vectors(m_result);
            return true;
        }
    }

    // Just for debugging
    //cout << "PATRON NO ENCONTRADO " << m_cube->reportRotation() << endl;

    return false;
}


void TransitionTemplate::reset()
{

    if (m_NodesMap.size() > 0)
        m_NodesMap.clear();

    initialize();
}


void TransitionTemplate::initialize()
{
    if (m_localVector.size() > 0)
        m_localVector.clear();

    for(Uint i=0; i<TOTAL_POINTS; i++)
    {
        m_localVector.push_back(i);
    }
}

bool TransitionTemplate::getNewElements(const UintVec &hex_idxs,
										const UintVec & edge_idxs,
										MeshPointVec & mesh_point,
										Point3DList & tmp_pts,
										VectorTable & new_element){
    TransitionCube *cube;
    bool insert_new_point = true;
    Uint mid_idx;
    m_NodesMap.clear();

    if (edge_idxs.size() > 0 ) {
        // Creates a map with nodes and edge values.
        createEdgesMapping(hex_idxs, edge_idxs);

        // Create an instance of cube containing edge nodes
        cube = new TransitionCube(m_localVector);
    }
    else {
        createNodesMapping(hex_idxs);
        cube = new TransitionCube();
    }

    m_cube = cube;

    if (findPattern()) {
        // Determine mid point of this hexahedron
        Point3D p1 = mesh_point[hex_idxs[0]].getPoint();
        Point3D p2 = mesh_point[hex_idxs[6]].getPoint();
        Point3D mp = (p2-p1);
                mp /= 2;
                mp+= p1;

        UintMap points;
        // Returns a map of (rotated,init point)
        m_cube->getCurrentTransitionPoints(points);

        // The found pattern is returned in m_result
        for (VectorTableIt it=m_result.begin(); it != m_result.end(); ++it) {
            // Temporay vector
            UintVec temp_vector;
            for (UintVecIt it1=(*it).begin() ; it1 != (*it).end(); ++it1) {
                if (*it1 == 26 ) {
                    if (insert_new_point) {
						
						//Assign number to new inserted point
                        mid_idx = mesh_point.size() + tmp_pts.size();
						
                        //Insert new mid point
                        tmp_pts.push_back(mp);
                        insert_new_point = false;
                        
                    }
                    temp_vector.push_back(mid_idx);
                }
                else {
                    Uint internal = points.find(*it1)->second;
                    temp_vector.push_back( m_NodesMap.find(internal)->second.External );
                }
            }

            new_element.push_back(temp_vector);
        }

        return true;
    }


    return false;
}


void TransitionTemplate::vectors(VectorTable &v) {

    UintMap points;

    // Returns a map of (rotated,init point)
    m_cube->getCurrentTransitionPoints(points);

    if(!m_result.empty()) {
       for (VectorTableIt it=m_result.begin(); it != m_result.end(); ++it) {
   
           UintVec p;
   
           for (UintVecIt it1=(*it).begin(); it1!=(*it).end(); ++it1) {
               Uint internal = points.find(*it1)->second;
               p.push_back( m_NodesMap.find(internal)->second.External );
           }
   
           v.push_back(p);
       }
    }
}

void TransitionTemplate::normal_vectors(VectorTable &v) {

    UintMap points;

    // Returns: rotated --> init point
    m_cube->getCurrentTransitionPoints(points);


    for (VectorTableIt it=m_result.begin(); it != m_result.end(); ++it) {

        UintVec p;

        for (UintVecIt it1=(*it).begin(); it1!=(*it).end(); ++it1) 
            p.push_back(points.find(*it1)->second);

        v.push_back(p);
    }
}
}

