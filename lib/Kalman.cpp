/**
* Felipe Constantin Reyes-Miftajov
* Modification of:
* Kalman filter implementation using Eigen. Based on the following
* introductory paper:
*
*     http://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf
*
* @author: Hayk Martirosyan
* @date: 2014.11.15
*/

#include "Kalman.h"

#include <iostream>
#include <stdexcept>


Kalman::Kalman(float dt,
	const Eigen::MatrixXf& A,
	const Eigen::MatrixXf& C,
	const Eigen::MatrixXf& Q,
	const Eigen::MatrixXf& R,
	const Eigen::MatrixXf& P)
	: A(A), C(C), Q(Q), R(R), P0(P), m(C.rows()), n(A.rows()), dt(dt), initialized(false), I(n,n), x_hat(n), x_hat_new(n)
{
	I.setIdentity();
}

Kalman::Kalman()
{ // this space intentionally left blank
}

void Kalman::init() //initialize with states as 0
{
	x_hat.setZero();
	P = P0;
	t0 = 0;
	t = t0;
	initialized = true;
}

void Kalman::init(float t0, const Eigen::VectorXf& x0) //initial guess
{
	x_hat = x0;
	P = P0;
	this->t0 = t0;
	t = t0;
	initialized = true;
}

void Kalman::update(const Eigen::VectorXf& y) //update with measurements
{
	if(!initialized)
	{
		throw std::runtime_error("Filter is not initialized");
	}
	
	x_hat_new = A * x_hat;
	P = A*P*A.transpose() + Q;
	K = P*C.transpose()*(C*P*C.transpose() + R).inverse(); // optimal gain
	x_hat_new += K * (y - C*x_hat_new);
	P = (I - K*C)*P;
	x_hat = x_hat_new;
	
	t += dt;	
}

void Kalman::update(const Eigen::VectorXf& y, float dt, const Eigen::MatrixXf A) //update with measurements, with given dt and A
{
	this->A = A;
	this->dt = dt;
	update(y);
}
