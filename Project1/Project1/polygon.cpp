#include "polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace MatPlot;

polygon::polygon()
{
	head = 0;
	tail = 0;
	nedge = 0;
	is_inf = false;
	fignum = 0;
}


polygon::~polygon()
{
}

void polygon::read_polygon() {
	int nedge;
	char c;
	printf("Do you want to use the default polygon?(Y/N)\n");
	c = getchar();
	while (getchar() != 10);
	if (c == 'Y' || c == 'y') {
		int nset;
		printf("okay.\nWhich set do you want to use?(choose 1,2,3,4)\n");
		printf("set 4 is the example in Assignment\n");
		scanf("%d", &nset);
		double x1[8]{ 0,2,3,4,7,9,8,3 };
		double y1[8]{ 0,-2,-1,-2,2,2,5,10 };
		double x2[13]{ 0,5,3,8,7,9,10,6,7,2,2.5,-1,1 };
		double y2[13]{ 0,2,0,0,2,1,7,6,8,6,5,4,2 };
		double x3[3]{ 5, 76, 20 };
		double y3[3]{ 26, -23, 221 };
		double x[48];
		double y[48];
		double pi = 3.14159265358;
		double r[4]{ 2, 2.5, 2.5, 2 };
		double theta[4]{ -0.04,-0.037,0.037,0.04 };
		for (int i = 0; i < 48; i++) {
			x[i] = r[i % 4] * cos((i / 4)*pi / 6 + theta[i % 4]);
			y[i] = r[i % 4] * sin((i / 4)*pi / 6 + theta[i % 4]);
		}
		switch (nset) 
		{
		case 1: create_polygon(8, x1, y1, false); break;
		case 2: create_polygon(13, x2, y2, false); break;
		case 4: create_polygon(3, x3, y3, false); break;
		default:create_polygon(48, x, y, false);
			break;
		}

	}
	else {
		printf("How many edges are there in the polygon:\n");
		scanf("%d", &nedge);
		double x0[100];
		double y0[100];
		for (int j = 0; j < nedge; j++) {
			printf("input the %d-th coordinate of polygon:(x y)\n", nedge - j);
			scanf("%lf%lf", x0 + j, y0 + j);
			while (getchar() != 10);
		}
		create_polygon(nedge, x0, y0, false);
	}
		//create_polygon(4, x, y, true);
		print_polygon();

		//find_kernal();
		triangulation();
	return;
}

void polygon::create_polygon(int nedge_, double x[], double y[], bool is_inf_) {
	nedge = nedge_;
	head = new halfline(x[0], y[0], x[1], y[1], is_inf_, false);
	halfline *p = head, *curr;
	int i;
	for (i = 1; i < nedge-1; i++) {
		curr = new halfline(x[i], y[i], x[i + 1], y[i + 1], false, false);
		p->next = curr;
		curr->pre = p;
		p = curr;
	}
	if (is_inf_) {
		curr = new halfline(x[i], y[i], x[i + 1], y[i + 1], false, true);
		p->next = curr;
		curr->pre = p;
		p = curr;
		tail = new halfline(x[i + 1], y[i + 1], x[i], y[i], true, false);
	}else tail = new halfline(x[i], y[i], x[0], y[0], false, false);
	p->next = tail;
	tail->pre = p;
	if (!is_inf_) {
		head->pre = tail;
		tail->next = head;
	}
	is_inf = is_inf_;
	
}

void polygon::print_polygon() {
	double x[100];
	double y[100];
	int i;
	halfline *p=head;
	for (i = 0; i < nedge; i++) {
		x[i] = p->x1;
		y[i] = p->y1;
		p = p->next;
	}
	if (is_inf) {
		x[i] = tail->x1;
		y[i] = tail->y1;
	}
	else {
		x[i] = tail->x2;
		y[i] = tail->y2;
	}
	MatPlotInit();
	//fignum++;
	//figure(fignum);
	if (fignum >= 1) {
		clf();
	}
	//clf();
	plot(x, y, nedge+1, 'r');
	fignum++;
	p = head->next;

	if(fignum<=1) while (getchar() != 10);
//	MatPlotClose();
}

void polygon::print_polygon(halfline* khead, halfline* ktail) {
	double x[100];
	double y[100];
	int i=0;
	halfline *p = khead;
	while(p&&p!=ktail) {
		x[i] = p->x1;
		y[i] = p->y1;
		i++;
		p = p->next;
	}
	if (p) {
		if (p->next) {
			x[i] = p->x1;
			y[i] = p->y1;
			i++;
			x[i] = p->x2;
			y[i] = p->y2;
		}
		else {
			x[i] = p->x1;
			y[i] = p->y1;
		}
		i++;
	}
	if (fignum) {
	//	close(fignum);
		fignum++;
	}
	MatPlotInit();
	//figure(fignum);
	axis_auto();
//	clf();
	print_polygon();
	plot(x, y, i, 'b');
//	while (getchar() != 10);
//	MatPlotClose();
}

void polygon::triangulation() {
	double x[100];
	double y[100];
	halfline* curr = head;
	MatPlotInit();
	//figure(fignum);
	axis_auto();
	print_polygon();
	while (getchar() != 10);
	for (int i = nedge; i > 3; i--) {
		while (curr->is_reflex()) curr = curr->next;
		(curr->pre)->point_to_end_of(curr);
		(curr->next)->pre = curr->pre;
		curr = curr->next;
		x[0] = curr->pre->x1;
		x[1] = curr->pre->x2;
		y[0] = curr->pre->y1;
		y[1] = curr->pre->y2;
		plot(x, y, 2, 'b');
		while (getchar() != 10);
	}
}

halfline* polygon::find_kernal() {
	halfline *khead, *ktail;//kernal head and kernal tail
	halfline *f, *l;//support
	if (is_inf) {
		printf("we need a simple polygon\n");
		return 0;
	}
	//MatPlotInit();
	halfline *curr = head;
	while (!curr->is_reflex() && curr != tail) curr = curr->next;
	if (curr->is_reflex()) {
		head = curr;
		tail = head->pre;
	}else {
		print_polygon(head, tail);
		while (getchar() != 10);
		return head;
	}

	//initial step
	//halfline *kcurr;
	khead = new halfline(head->x1 - head->dx, head->y1 - head->dy, head->x1, head->y1, true, false);
	curr = new halfline(tail->x2, tail->y2, tail->x2 + tail->dx, tail->y2 + tail->dy, false, true);
	khead->next = curr;
	curr->pre = khead;
	ktail = new halfline(curr->x2, curr->y2, curr->x1, curr->y1, true, false);
	curr->next = ktail;
	ktail->pre = curr;
	f = khead;
	l = ktail;

	print_polygon(khead, ktail);

	curr = head->next;
	halfline *kcurr;
	halfline *w1,*w2,*w3;

	int two_intersection = 0;
	//general step
	while (curr != tail) {
		if (curr->is_reflex()) {
			if (!curr->is_point_on_the_left(f)) {
				kcurr = f;
				while (kcurr != l) {
					if (curr->intersect_segement(kcurr)) {
						break;
					}
					kcurr = kcurr->next;
				}
				if (kcurr == l) {
					double x10[100], y10[100];
					x10[0] = curr->x1;
					x10[1] = curr->x2;
					y10[0] = curr->y1 - 0.01;
					y10[1] = curr->y2 - 0.01;
					curr = curr->next;
					print_polygon();
					plot(x10, y10, 2, 'c');
					while (getchar() != 10);
					printf("kernal not exist.\n");
					return 0;
				}
				w1 = curr->intersection(kcurr);
				w1->point_to_end_of(kcurr);
				kcurr = f->pre;
				while (kcurr) {
					if (curr->intersect_segement(kcurr)) {
						break;
					}
					kcurr = kcurr->pre;
				}
				if (kcurr) {
					w2 = curr->intersection(kcurr);
					//w2->point_by_start_of(kcurr);
					kcurr->point_to_start_of(w2);
					//kcurr->point_to_start_of(w2);
					//if(w2->pre) (w2->pre)->next = w2;
					(w1->next)->pre = w1;
					w3 = new halfline(kcurr, w1);
					kcurr->next = w3;
					w1->pre = w3;
					w2 = w3;
					if (ktail->next) {
						khead = w1;
						ktail = w1->pre;
					}
					//f = w2;
					two_intersection = true;
				}
				else {
					kcurr = ktail->pre;
					while (kcurr != l->pre) {
						if (curr->intersect_segement(kcurr)) {
							break;
						}
						kcurr = kcurr->pre;
					}
					if (kcurr != l->pre) {
						w2 = curr->intersection(kcurr);
						//w2->point_by_start_of(kcurr);
						kcurr->point_to_start_of(w2);
						(w1->next)->pre = w1;
						w3 = new halfline(kcurr, w1);
						kcurr->next = w3;
						w1->pre = w3;
						khead = w1;
						ktail = w1->pre;
						w2 = w3;
						//f = w2;
						two_intersection = true;
					}
					else {
						w2 = new halfline(w1->x1 - curr->dx, w1->y1 - curr->dy, w1->x1, w1->y1, true, false);
						w2->next = w1;
						w1->pre = w2;
						(w1->next)->pre = w1;
						khead = w2;
						//f = w2;
						two_intersection = false;
					}
				}
				if (two_intersection) {
					f = w2;
					//f=w3;
				}
				else {
					f = w2;
				}
				//?
				while (l->next) {
					if (l->is_point_on_the_left(curr->x2, curr->y2)) {
						break;
					}
					l = l->next;
				}

			}
			else {
				while (f->next) {
					if (!f->is_point_on_the_left(curr->x2, curr->y2) ){
						break;
					}
					f = f->next;
				}

				while (l ->next) {
					if (l->is_point_on_the_left(curr->x2, curr->y2)) {
						break;
					}
					l = l->next;
				}

			}
		}
		else {
			if (!curr->is_point_on_the_left(l)) {
				kcurr = l->pre;
				while (kcurr != f->pre) {
					if (curr->intersect_segement(kcurr)) {
						break;
					}
					kcurr = kcurr->pre;
				}
				if (kcurr == f->pre) {
					printf("kernal not exist.\n");
					return 0;
				}
				w1 = curr->intersection(kcurr);
				//w1->point_by_start_of(kcurr);
				kcurr->point_to_start_of(w1);
				w1 = kcurr;

				kcurr = l;
				//ktail->next==0;
				while (kcurr->next) {
					if (curr->intersect_segement(kcurr)) {
						break;
					}
					kcurr = kcurr->next;
				}
				if (kcurr->next) {
					w2 = curr->intersection(kcurr);
					w2->point_to_end_of(kcurr);
					(w2->next)->pre = w2;
					w3 = new halfline(w1, w2);
					w1->next = w3;
					w2->pre = w3;
					//if(w1->pre) (w1->pre)->next = w1;
					if (ktail->next) {
						khead = w1;
						ktail = w1->pre;
					}
					w1 = w3;

					two_intersection = 1;
				}
				else {
					kcurr = khead;
					while (kcurr != f) {
						if (curr->intersect_segement(kcurr)) {
							break;
						}
						kcurr = kcurr->next;
					}
					if (kcurr != f) {
						w2 = curr->intersection(kcurr);
						w2->point_to_end_of(kcurr);
						//kcurr->point_to_start_of(w2);
						(w2->next)->pre = w2;
						w3 = new halfline(w1, w2);
						w1->next = w3;
						w2->pre = w3;
						if(w1->pre) (w1->pre)->next = w1;

						khead = w2;
						ktail = w2->pre;
						w1 = w3;
						two_intersection = 1;

					}
					else {
						w2 = new halfline(w1->x2, w1->y2, w1->x2 + curr->dx, w1->y2 + curr->dy, false, true);
						w1->next = w2;
						w2->pre = w1;
						w3 = new halfline(w2->x2, w2->y2, w2->x1, w2->y1, true, false);
						w3->pre = w2;
						w2->next = w3;
						//if(w1->pre) (w1->pre)->next = w1;

						ktail = w3;
						w1 = w2;
						w2 = w3;
						two_intersection = 0;
					}
				}
				
				//find f and l
				if (two_intersection) {
					if(two_is_middle(curr->x1,curr->y1,curr->x2,curr->y2,w1->x1,w1->y1)){
						while (f->next) {
							if (!f->is_point_on_the_left(curr->x2, curr->y2) ){
								break;
							}
							f = f->next;
						}
					}
					else {
						f = w1;
					}

					if (two_is_middle(curr->x1,curr->y1,curr->x2,curr->y2,w2->x1,w2->y1)) {
						l = w2;
					}
					else {
						l = w2;
						while (l->next) {
							if (l->is_point_on_the_left(curr->x2, curr->y2)) {
								break;
							}
							l = l->next;
						}
					}
				}
				else {
					if (two_is_middle(curr->x1, curr->y1, curr->x2, curr->y2, w1->x1, w1->y1)) {
						while (f->next) {
							if (!f->is_point_on_the_left(curr->x2, curr->y2)) {
								break;
							}
							f = f->next;
						}
					}
					else {
						f = w1;
					}

					l = ktail;
					//l=w3;
					//
				}
			}
			else {
				while (f->next) {
					if (!f->is_point_on_the_left(curr->x2, curr->y2) ){
						break;
					}
					f = f->next;
				}

				if (ktail->next) {
					while (l->next) {
						if (l->is_point_on_the_left(curr->x2, curr->y2)) {
							break;
						}
						l = l->next;
					}
				}
			}
		}
		double x10[100], y10[100];
		x10[0] = curr->x1;
		x10[1] = curr->x2;
		y10[0] = curr->y1-0.01;
		y10[1] = curr->y2-0.01;
		curr = curr->next;
		print_polygon(khead, ktail);
		plot(x10, y10, 2, 'c');
		while (getchar() != 10);
		
		/*
		if (f->start_is_inf) printf("f:%lf,%lf,%lf,%lf,is inf\n", f->x1, f->y1, f->x2, f->y2);
		else printf("f:%lf,%lf.\n", f->x1, f->y1);
		if (l->start_is_inf) printf("l:%lf,%lf,%lf,%lf,is inf\n", l->x1, l->y1, l->x2, l->y2);
		else printf("l:%lf,%lf.\n", l->x1, l->y1);
		*/
		//while (getchar() != 10);
	}
}


bool polygon::two_is_middle(double x1, double y1, double x2, double y2, double x3, double y3) {
	double dx1 = x1 - x2;
	double dy1 = y1 - y2;
	double dx3 = x3 - x2;
	double dy3 = y3 - y2;
	return dx1 * dx3 + dy1 * dy3 <= 0;
}