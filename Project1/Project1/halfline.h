#pragma once

class halfline
{
public:
	halfline(double x1_, double y1_, double x2_, double y2_, bool start_is_inf_, bool end_is_inf_);
	halfline(halfline *pre_, halfline *next_);
	~halfline();

	double x1, y1, x2, y2;
	double dx, dy;
	bool start_is_inf;
	bool end_is_inf;
	halfline *next, *pre;
	bool is_reflex();
	bool intersect_segement(halfline *p);
	bool is_point_on_the_left(double x, double y);
	bool is_point_on_the_left(halfline* p);
	halfline* intersection(halfline *h);
	void renew();
	void point_to_end_of(halfline* h);
	void point_by_start_of(halfline* h);
	void point_to_start_of(halfline* h);
};

