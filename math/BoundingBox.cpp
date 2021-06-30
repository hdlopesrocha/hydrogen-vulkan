#include "math.hpp"


BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max) {
	this->min = min;
	this->max = max;
}

BoundingBox::BoundingBox() {
    this->min = glm::vec3(0,0,0);
    this->max = glm::vec3(0,0,0);
}

glm::vec3 BoundingBox::getMin() {
    return min;
}

glm::vec3 BoundingBox::getMax() {
    return max;
}

glm::vec3 BoundingBox::getLength() {
    return max - min;
}

glm::vec3 BoundingBox::getCenter() {
    return (min+max)*0.5f;
}

float BoundingBox::getMaxX() {
    return max[0];
}

float BoundingBox::getMaxY() {
    return max[1];
}

float BoundingBox::getMaxZ() {
    return max[2];
}

float BoundingBox::getMinX() {
    return min[0];
}

float BoundingBox::getMinY() {
    return min[1];
}

float BoundingBox::getMinZ() {
    return min[2];
}

void BoundingBox::setMin(glm::vec3 v) {
    this->min = v;
}

void BoundingBox::setMax(glm::vec3 v) {
    this->max = v;
}


bool BoundingBox::contains(glm::vec3 point){
    return 
        Math::isBetween(point[0], min[0], max[0]) &&
        Math::isBetween(point[1], min[1], max[1]) &&
        Math::isBetween(point[2], min[2], max[2]);
}
