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
 * PatternsDefault.h
 *
 *  Created on: May 18, 2012
 *      Author: jsepulve
 */

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include "TransitionCube.h"
#include "PatternFactory.h"
#include "MeshPoint.h"
#include "Point3D.h"

#ifndef TRANSITION_TEMPLATE_H_
#define TRANSITION_TEMPLATE_H_

using namespace std;
using namespace Clobscode;

/**
 * This is a base class for polyhedron
 * it defines basic operations
 */

namespace patterns {


/**
 * TransitionTemplate:
 * Receives a vector of corner and edge hexahedron points as a input 
 * parameters, it classifies into a known pattern, splits up and returns a 
 * vector of regular polyhedra corner points vectors. These vectors are 
 * composed by tetraedra, prisms or pyramids according to a previous
 * division of the known patterns.
 *
 * The corner, edge and mid points of the hexahedron have been defined by
 * the following arrangement.
 *
 *           4----19--------7
 *          /|             /|
 *         16|    25      18|
 *        / 12           /  15
 *       5---+---17-----6   |
 *       |  1|          |  3|
 *      13 2 |         14 2 |
 *       |   0-----11---|---3
 *       |  /           |  /
 *       | 8     20     |10
 *       |/             |/
 *       1------9-------2
 *
 *
 * Example:
 * std::vector<unsigned int> cp // {0,1,2,4,5,6,7}
 * std::vector<unsigned int> edge //{17,12} 
 * std::vector< std::vector<unsigned int> > result
 *
 * cube = new TransitionTemplate(cp,edge);
 * cube->getPatternResults(result);
 * delete cube;
 *
 */
class TransitionTemplate {

public:
    typedef map<Uint, Uint>         UintMap;
    typedef UintMap::iterator       UintMapIt;

    typedef vector<Uint>            UintVec;
    typedef UintVec::const_iterator UintVecIt;

    typedef vector< UintVec >       VectorTable;
    typedef VectorTable::iterator   VectorTableIt;

    typedef vector<MeshPoint> MeshPointVec;
	
	typedef list<Point3D>			Point3DList;
	typedef Point3DList::iterator	Point3DIt;

    
    enum Coordinate { X, Y, Z };

    /**
     * Internal struct to link external to local points.
     */
    struct CubeMapping_t {
        //Default constructor
        CubeMapping_t (): Local(0), External(0) { };

        //Default constructor
        CubeMapping_t (Uint p1, Uint p2): Local(p1), External(p2) { };

        //Copy constructor
        CubeMapping_t (const CubeMapping_t & m ): Local(m.Local), External(m.External) { };

        //Assignment operator
        CubeMapping_t & operator=( const CubeMapping_t & m ) {
            if (this != &m) {
                Local = m.Local;
                External = m.External;
            }
            return *this;
        }

        //Equals operator
        bool operator ==(const CubeMapping_t &m) const {
            return ((Local == m.Local)&&(External == m.External));
        }

        //Not Equals operator
        bool operator !=(const CubeMapping_t &m) const {
            return ((Local != m.Local) || (External != m.External));
        }

        Uint Local;
        Uint External;
    };

    struct Permutations_t {
        //Default constructor
        Permutations_t (): Axis(X), Step(0) { };

        //Default constructor
        Permutations_t (Coordinate c, int s): Axis(c), Step(s) { };

        //Copy constructor
        Permutations_t (const Permutations_t & p ): Axis(p.Axis), Step(p.Step) { };

        //Assignment operator
        Permutations_t & operator=( const Permutations_t & p ) {
            if (this != &p) {
                Axis = p.Axis;
                Step = p.Step;
            }
            return *this;
        }

        //Equals operator
        bool operator ==(const Permutations_t &p) const {
            return ((Axis == p.Axis)&&(Step == p.Step));
        }

        //Not Equals operator
        bool operator !=(const Permutations_t &p) const {
            return ((Axis != p.Axis) || (Step != p.Step));
        }

        Coordinate Axis;
        int Step;
    };
    
    /**
     * Default constructor.
     * Creates a default eight corner point cube.
     */
    TransitionTemplate();

    /**
     * Cube parametric constructor.
     * Takes first eight number as corner points, the other input number are
     * not considered.
     */
    TransitionTemplate( const UintVec& );

    /**
     * Cube parametric constructor.
     * Takes first eight number as corner points, the left number are taking
     * as edge points. 
     * The second vector is the mapping of edge points to internal convention.
     */
    TransitionTemplate(const UintVec&, const UintVec& );

    virtual ~TransitionTemplate() { delete m_cube; factory->deleteInstance(); };

    /**
     * Rotates the cube to left or right position in one of the 
     * three x,y,z axis.
     *
     * For each of the axis movement has been defined a default 
     * start position.
     *
     * Axis 'z' start position
     *      4----9------7     5-----------6      3-----------2
     *     /|          /|    /|          /|     /|          /|
     *    / |         / |   / |         / |    / 8         / |
     *   5-----------6  |  1-----------2  |   7-----------6  |
     *   |  |        |  |  |  |        |  |   |  |        |  |
     *   |  0----8---|--3  |  4----9---|--7   9  0--------|--1
     *   | /         | /   | /         | /    | /         | / 
     *   |/          |/    |/          |/     |/          |/  
     *   1-----------2     0----8------3      4-----------5  
     *
     * Saves the pattern found in a vector of integer vectors.
     */
    bool findPattern();

    /**
     * Reset both nodes map and local vector.
     */
    void reset();

    /**
     * Return final pattern result.
     */
    //void vectors(VectorTable &v) { v = m_result; }; 
    void vectors(VectorTable &); 
    void normal_vectors(VectorTable &); 

    bool getNewElements(VectorTable &);
    bool getNewElements(const UintVec&, 
            const UintVec&, 
            MeshPointVec &, 
            Point3DList&, 
            VectorTable&); 

private:

    /**
     * Initializes hexahedron pattern to default node positions.
     */
    void initialize();

    /** 
     *  Creates an internal map of the input nodes into defined vertexs
     *  numbered from 0 to 25 of a basic hexahedron. The first eight nodes
     *  (0-7) are the corner vertexs of this hexahedron, the other left
     *  nodes are vertexs located either at any edges or any of the faces
     *  of this hexahedron.
     */
    void createLocalMapping(const UintVec& );

    void createDefaultMapping();

    /**
     * Insert the first eight points of input vector and insert them
     * into the internal map of nodes.
     */
    void createNodesMapping(const UintVec&);

    /**
     * Takes all the edge input points and insert them into a local vector.
     */
    void createEdgesMapping(const UintVec&, const UintVec&);

    TransitionCube * m_cube;

    bitset<27> bitMask;

    //This vector will keep a local copy of edge nodes.
    UintVec m_localVector;


    typedef map<Uint, CubeMapping_t> PatternMap_t;
    typedef PatternMap_t::const_iterator MapIter;

    // Map: Point --> (Local, External)
    PatternMap_t m_NodesMap;

    VectorTable m_result;

    /**
     * Matrix of rotations
     * Return new vertex values after one turn in one of the three x,y,z axis.
     */
    static const Permutations_t RotationMatrix[];

    /**
     * Mask is the result to left shift number 1 by the value of the edge point.
     * Final outcome is a logical OR combination of all edge points in the pattern.
     */ 
    static const int PatternMask[];

    PatternFactory *factory;

};



}

#endif /* TRANSITION_TEMPLATE_H_ */
