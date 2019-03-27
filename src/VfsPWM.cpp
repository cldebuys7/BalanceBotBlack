/*
 * vfsPWM.cpp
 *
 * Modified by Felipe Constantin Reyes-Miftajov
 *
 * Based on modifications by Zongyao Jin
 *
 * Based on simpleGPIO.cpp modifications by Derek Molloy, School of 
 * Electronic Engineering, DCU -  www.derekmolloy.ie,
 * and by Nagavenkat Adurthi - http://www.nagavenkat.adurthi.com
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

#include "VfsPWM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>


	VfsPWM::VfsPWM()
	{
		for (int i = 0; i<NUM_PWMS; i++)
		{
			pinList[i].number = i;
			pinList[i].dutyCycle = 0;
			pinList[i].period = 100000;
			pinList[i].isRunning = false;
			
			export_pwm(i);
		}
	}
	
	VfsPWM::~VfsPWM()
	{
		for (int i = 0; i<NUM_PWMS; i++)
		{
			toggle_pwms(i,TURN_OFF);
			unexport_pwm(i);
		}
	}

    void VfsPWM::export_pwm(int pwm_select)
    {
        int fileID, length;

        char buffer[MAX_BUF];

        struct stat st;
        //check if first pwm already exported
        /*
        if (stat(VFS_PWM_DIR "/pwm0",&st)==0)
        {
            printf("pwms already exported\n");
            return;
        }
        */

        //for (int i=0; i<NUM_PWMS; i++)
        //{
            //std::cout << VFS_PWM_DIR "/export" << std::endl;
            fileID = open(VFS_PWM_DIR "/export", O_WRONLY);
            if (fileID < 0)
            {
                perror("pwm/export");
                return;
            }
			
            length = snprintf(buffer, sizeof(buffer), "%d", pwm_select);
            write(fileID, buffer, length);
            close(fileID);
        //}

        return;
    }

    void VfsPWM::unexport_pwm(int pwm_select)
    {
		
        int fileID, length;

        char buffer[MAX_BUF];

        //for (int i=0; i<NUM_PWMS; i++)
        //{
            fileID = open(VFS_PWM_DIR "/unexport", O_WRONLY);
            if (fileID < 0)
            {
                perror("pwm/export");
                return;
            }

            length = snprintf(buffer, sizeof(buffer), "%d", pwm_select);
            write(fileID, buffer, length);
            close(fileID);
        //}

        return;
    }


    void VfsPWM::set_period(int pwm_select, unsigned long int periodNS)
    {
        int fileID, length;
        char buffer[MAX_BUF];

        //for (int i=0; i<NUM_PWMS; i++)
        //{
            snprintf(buffer, sizeof(buffer), VFS_PWM_DIR "/pwm%d/period",pwm_select);

            fileID = open(buffer, O_WRONLY);
            if (fileID <0)
            {
                perror("pwm/period");
                return;
            }

            length = snprintf(buffer, sizeof(buffer), "%lu", periodNS);
            std::cout << buffer  << std:: endl;
            write(fileID, buffer, length);
            close(fileID);
        //}

        pinList[pwm_select].period = periodNS;

        return;
    }

    void VfsPWM::set_duty(int pwm_select,unsigned long int dutyNS) //no longer assuming same duty cycle for all PWM
    {
        int fileID, length;
        char buffer[MAX_BUF];

        //for (int i=0; i<NUM_PWMS; i++)
        //{
            snprintf(buffer, sizeof(buffer), VFS_PWM_DIR "/pwm%d/duty_cycle",pwm_select);

            fileID = open(buffer, O_WRONLY);
            if (fileID < 0)
            {
                perror("pwm/duty");
				std::cout << "WTF IS GOING ONNNNNNNNNNNNNNNN duty" << pwm_select;
                return;
            }

            length = snprintf(buffer, sizeof(buffer), "%lu", dutyNS);
            std::cout << buffer  << std:: endl;
            write(fileID, buffer, length);
            close(fileID);
        //}

        pinList[pwm_select].dutyCycle = dutyNS;
        return;
    }
    
    void VfsPWM::set_duty_fraction(int pwm_select, float dutyFrac)
    {
    
    	unsigned long int dutyNS;
    	
    	//implement saturation
    	dutyFrac = std::max(std::min(dutyFrac, 1.0f), 0.0f);
    	
    	dutyNS = (unsigned long int)(dutyFrac*pinList[pwm_select].period);
    	this->set_duty(pwm_select, dutyNS);
    }

    void VfsPWM::toggle_pwms(int pwm_select, PWMtoggle toggleOption)
    {

        int msg = toggleOption; //implicit conversion

        int fileID, length;
        char buffer[MAX_BUF];

        //for (int i=0; i<NUM_PWMS; i++)
        //{
            snprintf(buffer, sizeof(buffer), VFS_PWM_DIR "/pwm%d/enable",pwm_select);

            fileID = open(buffer, O_WRONLY);
            if (fileID < 0)
            {
                perror("pwm/enable");
                return;
            }

            length = snprintf(buffer, sizeof(buffer), "%d", msg);
            write(fileID, buffer, length);
            close(fileID);
        //}

		pinList[pwm_select].isRunning = toggleOption;

        return;
    }
