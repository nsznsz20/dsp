/*
hp = x - lp;
ap = lp - hp = 2*lp - x
*/

#ifndef DSP_ONEPOLE_H
#define DSP_ONEPOLE_H

#include "constants.h"

#include <cmath>

namespace dsp {

struct Onepole_lp {
	explicit Onepole_lp() : y_1{0.} {}
	explicit Onepole_lp(double _dy) : Onepole_lp() { dy(_dy); }
	Onepole_lp(const Onepole_lp& other) :
		dy_{other.dy_}, a0{other.a0}, b1{other.b1}, y_1{other.y}
	{}
	
	double dy_;
	double a0, b1;
	double y_1;
	double y;
	
	Onepole_lp& operator()(double x) {
		y_1 = a0 * x + b1 * y_1;
		y = y_1;
		return *this;
	}
	void dy(double value) { 
		dy_ = value;
		a0 = 1.0 - std::exp(-TWOPI*dy_);
		b1 = 1.0 - a0;
	}
	double dy() const { return dy_; }
	void reset(double y_1_ = 0) {
		y_1 = y_1_;
		y = 0;
	}
};

struct Onepole_hp : public Onepole_lp {
	using Onepole_lp::Onepole_lp;
	Onepole_hp& operator()(double x) {
		y_1 = a0 * x + b1 * y_1;
		y = x - y_1;
		return *this;
	}
};

struct Onepole_ap : public Onepole_lp {
	using Onepole_lp::Onepole_lp;
	Onepole_ap& operator()(double x) {
		y_1 = a0 * x + b1 * y_1;
		y = 2*y_1 - x;
		return *this;
	}
};

struct OnepoleRaw {
	explicit OnepoleRaw() {}
	explicit OnepoleRaw(double _ramp_time) {
		ramp_time(_ramp_time);
	}
	OnepoleRaw(const OnepoleRaw& other) :
		ramp_time_{other.ramp_time_}, k{other.k}, mkp1{other.mkp1}
	{}
	
	double ramp_time_;
	double k;
	double mkp1;
	double y;
	
	OnepoleRaw& operator()(double x) {
		y = k * x + mkp1 * y;
		return *this;
	}
	void ramp_time(double val) {
		ramp_time_ = val;
		k = 1 - std::pow(1. - 0.9, 1. / (ramp_time_ - 1.));
		mkp1 = 1. - k;
	}
	double ramp_time() const { return ramp_time_; };
};

} // // namespace

#endif // // include guard