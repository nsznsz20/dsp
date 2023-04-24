#ifndef DSP_DELAY_H
#define DSP_DELAY_H

#include "utilities.h"

#include <cmath>

namespace dsp {

struct Delay {
    explicit Delay(int maxdel) :
    	maxdel{maxdel}, size{maxdel+1},
        buffer{ new double[size]{0.} },
        l{-1}, m{0}, n{1}
    {}
    Delay(const Delay& other) : Delay(other.maxdel) {}
    ~Delay() {
        delete[] buffer;
    }
    
    const int maxdel;
    const int size;
    double* buffer;
    int l, m, n;
    
    void operator()(double x) {
        l = m; m = n; ++n;
    	if(n == size) n = 0;
    	buffer[l] = x;
    }
    // // assumes: 0 <= del <= maxdel
    double y() const {
    	return buffer[m];
    }
    double y(int delay) const {
    	int i = l - delay;
    	if (i < 0) i += size;
    	return buffer[i];
    }
    double y(double delay) const { // // spline interpolation
    	if (delay <= 1) { // // ... except in this case (linear interpolation)
            int y0 = l - 1;
            if (y0 < 0) y0 += size;
    		return buffer[l]*(1.-delay) + buffer[y0]*delay;
    	}
    	double floor_delay = std::floor(delay);
    	int y2 = l - (int) floor_delay;
        if (y2 < 0) y2 += size;
    	int y1 = y2 ? y2 - 1 : maxdel;
    	int y0 = y1 ? y1 - 1 : maxdel;
    	int y3 = y2 + 1;
        if (y3 == size) y3 = 0;
    	return spline_interpolate(delay - floor_delay,
    			buffer[y3], buffer[y2], buffer[y1], buffer[y0]);
    }
    double y_prev() const {
    	return buffer[n];
    }
    double y_prev(int delay) const {
    	// // assumes: 0 <= del <= maxdel
    	int i = m - delay;
    	if (i < 0) i += size;
    	return buffer[i];
    }
    double y_prev(double delay) const { // // minimum one sample of delay
        delay -= 1;
        if (delay < 0) delay = 0;
        return y(delay);
    }
    void reset(double value = 0.) {
        for (int i = 0; i < size; ++i) {
            buffer[i] = value;
        }
    }
};

} // // namespace

#endif // // include guard
