#include "math.hpp"


BoundingCube::BoundingCube(glm::vec3 min, float length) {
	this->min = min;
	this->length = length;
}

float BoundingCube::getLength() {
    return length;
}

glm::vec3 BoundingCube::getMin() {
    return min;
}

glm::vec3 BoundingCube::getMax() {
    return min+glm::vec3(length);
}

glm::vec3 BoundingCube::getCenter() {
    return min+glm::vec3(length*0.5);
}

float BoundingCube::getMaxX() {
    return min[0] + length;
}

float BoundingCube::getMaxY() {
    return min[1] + length;
}

float BoundingCube::getMaxZ() {
    return min[2] + length;
}

float BoundingCube::getMinX() {
    return min[0];
}

float BoundingCube::getMinY() {
    return min[1];
}

float BoundingCube::getMinZ() {
    return min[2];
}

void BoundingCube::setLength(float l) {
    this->length = l;
}

void BoundingCube::setMin(glm::vec3 v) {
    this->min = v;
}


   float check(float pn, float bmin,
         float bmax ) 
    {
        float out = 0;
        float v = pn;
 
        if ( v < bmin ) 
        {             
            float val = (bmin - v);             
            out += val * val;         
        }         
         
        if ( v > bmax )
        {
            float val = (v - bmax);
            out += val * val;
        }
 
        return out;
    };
 


float squaredDistPointAABB( glm::vec3 p, BoundingCube aabb )
{
 
    // Squared distance
    float sq = 0.0;
 
    sq += check( p[0], aabb.getMin()[0], aabb.getMax()[0] );
    sq += check( p[1], aabb.getMin()[1], aabb.getMax()[1] );
    sq += check( p[2], aabb.getMin()[2], aabb.getMax()[2] );
 
    return sq;
}


bool BoundingCube::intersects(BoundingSphere sphere) {
    float squaredDistance = squaredDistPointAABB( sphere.center, *this );
    return squaredDistance <= (sphere.radius * sphere.radius);
}

ContainmentResult BoundingCube::contains(BoundingSphere sphere) {



    ContainmentResult result;
    result.type = ContainmentType::Disjoint;
    
    // Classify corners
    unsigned char mask = 0;

    for(int i=0; i < 8; ++i) {
        glm::vec3 point(getMin()+ Octree::getShift(i)*getLength());
        if(sphere.contains(point)){
            mask |= (1 << i);
        }
    } 

    result.mask = mask;
    // Classifify type
    if(mask == 0xff) {
        result.type = ContainmentType::IsContained;
    }
    else if(mask > 0) {
        result.type = ContainmentType::Intersects;
    }
    else {
        float radius = sphere.radius;
        glm::vec3 center = sphere.center;
        
        glm::vec3 minSphere = (center-glm::vec3(radius));
        glm::vec3 maxSphere = (center+glm::vec3(radius));
        
        if ( getMin()[0] <=  minSphere[0] && maxSphere[0] <= getMax()[0] && 
             getMin()[1] <=  minSphere[1] && maxSphere[1] <= getMax()[1] && 
             getMin()[2] <=  minSphere[2] && maxSphere[2] <= getMax()[2] ) {
            result.type = ContainmentType::Contains;
        } else if(intersects(sphere)) {
            result.type = ContainmentType::Intersects;
        }
    }  

    return result;
}

ContainmentResult BoundingCube::contains(BoundingBox box) {
    ContainmentResult result;
    result.type = ContainmentType::Intersects;
    glm::vec3 min1 = getMin();
    glm::vec3 max1 = getMax();
    glm::vec3 min2 = box.getMin();
    glm::vec3 max2 = box.getMax();


    // Classify corners
    unsigned char mask = 0;
    result.mask = 0;

    for(int i=0; i < 8; ++i) {
        glm::vec3 sh = Octree::getShift(i);
        glm::vec3 p1(min1 + sh*getLength());
        glm::vec3 p2(min2 + sh*box.getLength());

        if(contains(p2)){
            mask |= (1 << i); 
        }
        if(box.contains(p1)){
            result.mask |= (1 << i); 
        }
    } 
   
    // Classifify type
    if(mask == 0xff) {
        result.type = ContainmentType::Contains;
    }
    else if(result.mask == 0xff) {
        result.type = ContainmentType::IsContained;
    }
    else {
        for(int i=0 ; i < 3 ; ++i){
            if(    (min1[i] <= min2[i] && min2[i] <= max1[i]) 
                || (min1[i] <= max2[i] && max2[i] <= max1[i]) 
                || (min2[i] <= min1[i] && min1[i] <= max2[i]) 
                || (min2[i] <= max1[i] && max1[i] <= max2[i])){
                // overlaps in one dimension
            } else {
                result.type = ContainmentType::Disjoint;
                break;
            }
        }
    }
    return result;
}


bool BoundingCube::contains(glm::vec3 point){
    return 
        Math::isBetween(point[0], min[0], getMax()[0]) &&
        Math::isBetween(point[1], min[1], getMax()[1]) &&
        Math::isBetween(point[2], min[2], getMax()[2]);
}
