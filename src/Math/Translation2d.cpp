#include "Translation2d.h"

Translation2d::Translation2d() {
	x_ = 0;
	y_ = 0;
}

Translation2d::Translation2d(const double x, const double y) {
	x_ = x;
	y_ = y;
}

Translation2d::Translation2d(shared_ptr<Translation2d> other) {
	x_ = other->x_;
	y_ = other->y_;
}

Translation2d::Translation2d(shared_ptr<Translation2d> start, shared_ptr<Translation2d> end) {
	x_ = end->x_ - start->x_;
	y_ = end->y_ - start->y_;
}

double Translation2d::norm() {
	return hypot(x_, y_);
}

double Translation2d::norm2() {
	return pow(x_, 2) * pow(y_, 2);
}

double Translation2d::x() {
	return x_;
}

double Translation2d::y() {
	return y_;
}

void Translation2d::setX(double x) {
	x_ = x;
}

void Translation2d::setY(double y) {
	y_ = y;
}
