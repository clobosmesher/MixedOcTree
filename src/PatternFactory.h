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
#include <map>
#include <bitset>
#include "TransitionCube.h"


#ifndef PATTERN_FACTORY_H_
#define PATTERN_FACTORY_H_

using std::vector;
using std::map;
using std::bitset;
using std::cout;

namespace patterns {

typedef unsigned int Uint;
typedef vector< vector<Uint> > VTable;
typedef vector< vector<Uint> >::iterator VTableIter;

static const Uint TETRA_POINTS  =4;
static const Uint PYRAMID_POINTS=5;
static const Uint PRISM_POINTS  =6;
static const Uint HEXA_POINTS   =8;
static const Uint CORNER_POINTS =8;
static const Uint EDGE_POINTS   =12;
static const Uint FACE_POINTS   =6;
static const Uint TOTAL_POINTS  =27;
static const Uint PERMUTATIONS  =30;
static const Uint MAX_ELEMENTS  =71;
static const Uint NUM_PATTERNS  =26;


class PatternFactory {
public:
    
    // Struct to contain points and size of one element.
    struct item {
        Uint size;
        Uint points[CORNER_POINTS];
    };

    // Struct contains one element and the mask its edge point associated.
    struct Element {
        Uint mask;
        item items[MAX_ELEMENTS];
    };

    static PatternFactory *instance();
    static void deleteInstance();

    //virtual void createPattern(vector< vector<Uint> > & p);
    bool createPattern(Uint);

    void vectors(vector< vector<Uint> > & p ) { p = pattern; };
private:
    PatternFactory() { };

    virtual ~PatternFactory() { };
    PatternFactory(const PatternFactory &) { };
    PatternFactory& operator=(PatternFactory const&){ return *this; };

    static const Element elements[];

    static PatternFactory* m_Instance;
    static int m_NumInstances;

    vector< vector<Uint> > pattern;

};

}

#endif /* PATTERN_FACTORY_H_ */
