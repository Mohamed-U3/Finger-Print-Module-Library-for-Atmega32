/*
 * fb_interface.h
 *
 * Created: 3/31/2022 5:22:09 PM
 *  Author: mmyra
 */ 
	
#ifndef FP_INTERFACE_H_
#define FP_INTERFACE_H_

#define FB_connect	1
#define F_find		2
#define F_im1		3
#define F_im2		4
#define F_cretModl	5
#define F_store		6
#define F_delete	7
#define FP_empty	8
#define F_search	9

#define success		0
#define fail		1
#define again		2

char sendcmd2fb(unsigned char order);
char GetID();
void enroll();
void Search();

#endif /* FB_INTERFACE_H_ */