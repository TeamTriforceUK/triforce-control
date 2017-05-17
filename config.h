/**
* @file config.h
* @author Cameron A. Craig
* @date 15 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Configure pins and other parameters.
*/

#define VERSION "0.2.0"

#ifndef TC_CONFIG_H
#define TC_CONFIG_H

#define RECV_CHAN_1_PIN p5
#define RECV_CHAN_2_PIN p6
#define RECV_CHAN_3_PIN p7
#define RECV_CHAN_4_PIN p8
#define RECV_CHAN_5_PIN p11
#define RECV_CHAN_6_PIN p12
#define RECV_CHAN_7_PIN p13
#define RECV_CHAN_8_PIN p14

#define DRIVE_ESC_OUT_1_PIN p21
#define DRIVE_ESC_OUT_2_PIN p22
#define DRIVE_ESC_OUT_3_PIN p23

#define WEAPON_ESC_OUT_1_PIN p24
#define WEAPON_ESC_OUT_2_PIN p25

#define RC_NUMBER_CHANNELS 8

// 150ms increments
#define NO_SIGNAL_TIMEOUT 70

#define RC_ARM_CHANNEL_1 90
#define RC_ARM_CHANNEL_2 90
#define RC_ARM_CHANNEL_3 10
#define RC_ARM_CHANNEL_4 10
#define RC_ARM_DELAY 5

#define CHANNEL_MAX 85 //85
#define CHANNEL_MIN 3  //3
#define RC_ON_THRES 70 // ON threshold for switches
#define RC_OFF_THRES 30 // OFF threshold for switches
#define RC_FAILSAFE_THRES 150 // FAILSAFE threshold


#endif //TC_CONFIG_H
