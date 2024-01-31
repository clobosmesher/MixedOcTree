#ifndef FaceContainer_h
#define FaceContainer_h 1

#include <iostream>
#include <vector>
#include <list>
#include "Face.h"

using namespace std;

class FaceContainer{
    
public:
    FaceContainer();
    
    virtual ~FaceContainer();
    
    virtual Face &getFace(const unsigned int &index);
    
    virtual unsigned int length();
    
    virtual unsigned int addFace(Face &f);
    
    virtual bool getFaceIndex(Face &f, unsigned int &found);
    
    virtual bool getIndex(Face &f, unsigned int &found);
    
    virtual void updatePointIndex(unsigned int oldp, unsigned int newp,
                                  list<unsigned int> &fs);
    
    virtual void clear();
    
protected:
    
    virtual bool outOfBoundary(const unsigned int &index);
    
protected:
    
    vector<Face> facesvec;
    unsigned int size, hashingsize;
    vector<list<unsigned int> > hashing;
    list<unsigned int>::iterator hashiter;
    
};

inline void FaceContainer::clear(){
    facesvec.clear();
    size=0;
    hashingsize=0;
    hashing.clear();
}
    
#endif
