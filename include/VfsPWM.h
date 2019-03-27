/*
 * vfsPWM.h
 *
 * Modified by Felipe Constantin Reyes-Miftajov
 *
 * Based on simpleGPIO.h modifications by Derek Molloy, School of 
 * Electronic Engineering, DCU -  www.derekmolloy.ie,
 * Almost entirely based on Software by RidgeRun:
 *
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WRRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * as(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VFSPWM_H
#define VFSPWM_H

#define MAX_BUF 64
#define VFS_PWM_DIR "/sys/devices/platform/ocp/48302000.epwmss/48302200.ehrpwm/pwm/pwmchip2"

#define NUM_PWMS 2

struct pwm_pin
{
	int number;
	unsigned long int dutyCycle, period;
	bool isRunning;
};

enum PWMtoggle
{
	TURN_ON = true,
	TURN_OFF = false
};

class VfsPWM
{

    private:
    
	pwm_pin pinList[NUM_PWMS];

    public:
    
    VfsPWM();
    
    ~VfsPWM();

    void export_pwm(int pwm_select);

    void unexport_pwm(int pwm_select);

    void set_period(int pwm_select, unsigned long int periodNS);

    void set_duty(int pwm_select, unsigned long int dutyNS);
    
    void set_duty_fraction(int pwm_select, float dutyFrac);

    void toggle_pwms(int pwm_select, PWMtoggle toggleOption);

};

#endif
