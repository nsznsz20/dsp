#ifndef DSP_LINE_H
#define DSP_LINE_H

namespace dsp {

struct Line {
	explicit Line() : destination{0.}, N{1}, n{-1}, dy{0.}, y{0.} {}
	Line(const Line& other) :
		destination{other.destination}, N{other.N}, n{other.n}, 
		dy{other.dy}, y{other.dy}
	{}
		
	double destination;
	int N;
	int n;
	double dy;
	double y;

	void operator()() {
		++n;
		if (n >= N) {
			n = N;
			y = destination;
		}
		else {
			y += dy;
		}
	}
	void init(double _destination, int time) {		
		destination = _destination;
		N = time;
		n = -1;
		dy = (destination - y) / N;
		y -= dy;
	}
	void set(double value) {
		y = value;
		destination = value;
		N = 0;
		n = 0;
		dy = 0.;
	}
};

} // // namespace

#endif // // include guard
