#include "halfline.h"


halfline::halfline(double x1_, double y1_, double x2_, double y2_, bool start_is_inf_, bool end_is_inf_)
{
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;
	start_is_inf = start_is_inf_;
	end_is_inf = end_is_inf_;
	pre = 0;
	next = 0;
	dx = x2 - x1;
	dy = y2 - y1;
}

halfline::halfline(halfline *pre_, halfline *next_) {
	pre = pre_;
	next = next_;
	x1 = pre->x2;
	y1 = pre->y2;
	x2 = next->x1;
	y2 = next->y1;
	start_is_inf = pre->end_is_inf;
	end_is_inf = next->start_is_inf;
	dx = x2 - x1;
	dy = y2 - y1;
}

halfline::~halfline()
{
}

bool halfline::is_reflex() {
	return dx * pre->dy - dy * pre->dx > 0;
}

bool halfline::intersect_segement(halfline *p) {
	return is_point_on_the_left(p) != is_point_on_the_left(p->next);
}

bool halfline::is_point_on_the_left(double x, double y) {
	x = x - x1;
	y = y - y1;
	return dx * y - dy * x > 0;//intersection on start point.
}

bool halfline::is_point_on_the_left(halfline *p) {
	if (p->start_is_inf) return (-dx * p->dy + dy * p->dx > 0)|| ((-dx * p->dy + dy * p->dx == 0)&&is_point_on_the_left(p->x1,p->y1));
	else return is_point_on_the_left(p->x1, p->y1);
}

halfline* halfline::intersection(halfline *h) {
	double b[2];
	b[0] = dy * x1 - dx * y1;
	b[1] = h->dy*h->x1 - h->dx*h->y1;
	double det = dx * h->dy - dy * h->dx;
	halfline* p=new halfline((dx*b[1] - h->dx*b[0]) / det, (dy*b[1] - h->dy*b[0]) / det,0,0,false,false);
	return p;
}

void halfline::renew() {
	dx = x2 - x1;
	dy = y2 - y1;
}

void halfline::point_to_end_of(halfline *h) {
	if (h->end_is_inf) {
		x2 = x1 + h->dx;
		y2 = y1 + h->dy;
		end_is_inf = true;
	}
	else {
		x2 = h->x2;
		y2 = h->y2;
		end_is_inf = false;
	}
	renew();
	next = h->next;
}

void halfline::point_by_start_of(halfline *h) {
	x2 = x1;
	y2 = y1;
	if (h->start_is_inf) {
		x1 = x2 - h->dx;
		y1 = y2 - h->dy;
		start_is_inf = true;
	}
	else {
		x1 = h->x1;
		y1 = h->y1;
		start_is_inf = false;
	}
	renew();
	pre = h->pre;
}

void halfline::point_to_start_of(halfline* h) {
	x2 = h->x1;
	y2 = h->y1;
	if (start_is_inf) {
		x1 = x2 - dx;
		y1 = y2 - dy;
	}
	else {
		renew();
	}
	next = h;
	end_is_inf = false;
}