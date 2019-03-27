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

#ifndef KALMAN_H
#define KALMAN_H
#include <Eigen/Dense>


class Kalman
{

	private:
	
	Eigen::MatrixXf A, C, Q, R, P, K, P0; // system/parameter matrices
	
	int m,n; // dimensions of system;
	
	float t0, t; // initial and current time
	
	float dt; // time step;
	
	bool initialized;
	
	Eigen::MatrixXf I; // identity matrix
	
	Eigen::VectorXf x_hat, x_hat_new; // state estimates

	public:
	
	Kalman(float dt, /* time step */
		const Eigen::MatrixXf& A, /* model gain */
		const Eigen::MatrixXf& C, /* sensor gain */
		const Eigen::MatrixXf& Q, /* process noise covariance */
		const Eigen::MatrixXf& R, /* sensor noise covariance */
		const Eigen::MatrixXf& P /* initial error covariance */);

	Kalman();
	
	void init(); //initialize with states as 0
	
	void init(float t0, const Eigen::VectorXf& x0); //initial guess
	
	void update(const Eigen::VectorXf& y); //update with measurements
	
	void update(const Eigen::VectorXf& y, float dt, const Eigen::MatrixXf A); //update with measurements, with given dt and A
	
	Eigen::VectorXf get_state()
	{
		return x_hat;
	};
	
	double get_time()
	{
		return t;
	};
	
};

#endif
