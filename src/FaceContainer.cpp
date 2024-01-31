#include "FaceContainer.h"

FaceContainer::FaceContainer(){
    size = 0;
    hashingsize = 0;
}

FaceContainer::~FaceContainer(){

}

void FaceContainer::updatePointIndex(unsigned int oldp, unsigned int newp,
                                     list<unsigned int> &fs){
	list<unsigned int>::iterator iter;
	for(iter=fs.begin();iter!=fs.end();iter++){
		facesvec.at(*iter).replacePoint(oldp,newp);
	}
}

Face &FaceContainer::getFace(const unsigned int &index){
	if(!outOfBoundary(index))
		return facesvec[index];
    cerr << "out of boundary Face index in FaceContainer::getFace";
	return facesvec[0];
}

unsigned int FaceContainer::length(){
	return size;
}

bool FaceContainer::getFaceIndex(Face &f, unsigned int &found){
	unsigned int idx = f.getMinIndex();
	if(hashingsize>idx){
		list<unsigned int> local = hashing[idx];
		for(hashiter=local.begin();hashiter!=local.end();hashiter++)
			if(facesvec[*hashiter]==f)
                found = *hashiter;
				return true;
	}
	return false;
}

unsigned int FaceContainer::addFace(Face &f){
    unsigned int found = 0;
	if(getIndex(f,found)){
		list<unsigned int> fe = f.getElements();
		list<unsigned int>::iterator iter;
		for (iter=fe.begin();iter!=fe.end();iter++)
			facesvec[found].addElement(*iter);
		return found;
	}
	facesvec.push_back(f);
	return size++;
}


//assigns an index for face f. If the face was previously inserted, it will update
//the found paramater with the index of the face. In this case it will return true.
//If the face wasn't inserted it will update all the structures so the addFace function
//will add safely add the new face to the container and the index will be size++.
bool FaceContainer::getIndex(Face &f, unsigned int &found){
    
	unsigned int idx = f.getMinIndex();
	if (hashingsize>idx) {
		list<unsigned int> local = hashing[idx];
		for(hashiter=local.begin();hashiter!=local.end();hashiter++){
			if(facesvec[*hashiter]==f){
                found = *hashiter;
				return true;
			}
		}
		local.push_back(size);
		hashing[idx]=local;
	}
	else{
		list<unsigned int> local, voidlist;
		for(unsigned int i=hashingsize; i<idx; i++)
			hashing.push_back(voidlist);
		local.push_back(size);
		hashing.push_back(local);
		hashingsize = idx+1;
	}
    return false;
}

bool FaceContainer::outOfBoundary(const unsigned int &index){
	if(index>=size){
		cout << "Index out of boundary in face container! (" << index << "/";
		cout << size << ")" << endl;
		return true;
	}
	return false;
}

