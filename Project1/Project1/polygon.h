#pragma once
#include "halfline.h"
#include <vector>
#include <Windows.h>
#include "MatPlot.h"


using namespace std;
class polygon
{
public:
	polygon();
	~polygon();

	halfline *head, *tail;
	void create_polygon(int nedge_, double x[], double y[], bool is_inf_);
	void read_polygon();
	void print_polygon();
	int nedge;
	bool is_inf;
	halfline* find_kernal();
	void print_polygon(halfline* khead, halfline* ktail);
	bool two_is_middle(double x1, double y1, double x2, double y2, double x3, double y3);
	void triangulation();
	int fignum;
};

