#include <Windows.h>
#include "MatPlot.h"
#include "polygon.h"

using namespace MatPlot;
int main() {
	polygon p;
	p.read_polygon();
	MatPlotClose();
	return 0;
}