#!/bin/bash
sudo -s <<EOF
# load DTO file
echo bone_meen652 >  ${SLOTS}
#test results
cat ${SLOTS}
sleep 1
cd /sys/devices/platform/ocp/48302000.epwmss/48302200.ehrpwm/pwm/pwmchip2
echo 0 > export
cd /sys/devices/platform/ocp/48302000.epwmss/48302200.ehrpwm/pwm/pwmchip2/pwm0
echo 100000 > period
echo 0 > duty_cycle
echo 1 > enable
cd /sys/devices/platform/ocp/48302000.epwmss/48302200.ehrpwm/pwm/pwmchip2
echo 1 > export
cd /sys/devices/platform/ocp/48302000.epwmss/48302200.ehrpwm/pwm/pwmchip2/pwm1
echo 100000 > period
echo 0 > duty_cycle
echo 1 > enable
cd /sys/class/gpio
echo 30 > export
cd /sys/class/gpio/gpio30
echo out > direction
cd /sys/class/gpio
echo 31 > export
cd /sys/class/gpio/gpio31
echo out > direction
EOF