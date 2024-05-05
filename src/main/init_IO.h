/*

*    -------
*    |  Z  Y
*    |  |  |
*    X1---X2

*/
#ifndef IOFILE
#define IOFILE

// #define ENA_X1 10
#define DIR_X1 9
#define PUL_X1 8

// #define ENA_X2 13
#define DIR_X2 12
#define PUL_X2 11

// #define ENA_Y 7
#define DIR_Y 6
#define PUL_Y 5

// #define ENA_Z 4
#define DIR_Z 3
#define PUL_Z 2

// TODO: Change Pin !
// #define StartButton 49
// #define Keep 50
#define KeepA_ZAxis 49
#define KeepB_ZAxis 50

#define JoyA_YAxis 52
#define JoyB_YAxis 53
#define JoyC_XAxis 48
#define JoyD_XAxis 51


// int pinIN[6] = {
//                 StartButton,Keep,
//                 JoyA_YAxis,JoyB_YAxis,
//                 JoyC_XAxis,JoyD_XAxis,
//                 };
int pinIN[6] = {
                KeepA_ZAxis,KeepB_ZAxis,
                JoyA_YAxis,JoyB_YAxis,
                JoyC_XAxis,JoyD_XAxis,
                };
/*
*   JoyStick : Common => GND

*       A
*       |
*  C -- | -- D
*       |
*       B

A
    13
    14 => GND

B
    33
    34 => GND

C
    23 => GND
    24

D
    43 => GND
    44


*/


// byte pinOUT[12] = {
//                     ENA_X1,DIR_X1,PUL_X1,
//                     ENA_X2,DIR_X2,PUL_X2,
//                     ENA_Y,DIR_Y,PUL_Y,
//                     ENA_Z,DIR_Z,PUL_Z,
//                     };

// int pinOUT[8] = {
//                 DIR_X1,PUL_X1,
//                 DIR_X2,PUL_X2,
//                 DIR_Y,PUL_Y,
//                 DIR_Z,PUL_Z,
//                 };

#endif