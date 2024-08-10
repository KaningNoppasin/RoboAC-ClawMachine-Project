#ifndef IOFILE
#define IOFILE

/* ---------- STEPPER MOTOR ---------- */
/*

*    -------
*    |  Z  Y
*    |  |  |
*    X1---X2

*/
#define DIR_X1 51
#define PUL_X1 50

#define DIR_X2 49
#define PUL_X2 48

#define DIR_Y 47
#define PUL_Y 46

#define DIR_Z 53
#define PUL_Z 52

/* ---------- LIMIT SWITCH ---------- */
#define Limit_XAxis A0
#define Limit_YAxis A1
#define Limit_ZAxis A2

/* ---------- JOY ---------- */

#define JoyA_YAxis 5   // LEFT -
#define JoyB_YAxis 4   // RIGHT +
#define JoyC_XAxis 6   // FORWARD
#define JoyD_XAxis 7   // BACKWARD

#define Joy_Keep 13   // KEEP

/* ---------- RELAY ---------- */
#define Relay_Keep 8   // KEEP

/* ---------- SLECTOR SWITCH ---------- */
/*

*        ---       ---       ---
*       /   \     /   \     /   \
*      |  A  |   |  B  |   |  C  |
*       \   /     \   /     \   /
*        ---       ---       ---

*/

#define SLECTOR_SWITCH_A1 33
#define SLECTOR_SWITCH_A2 31
#define SLECTOR_SWITCH_A3 29

#define SLECTOR_SWITCH_B1 39
#define SLECTOR_SWITCH_B2 37
#define SLECTOR_SWITCH_B3 35

#define SLECTOR_SWITCH_C1 45
#define SLECTOR_SWITCH_C2 43
#define SLECTOR_SWITCH_C3 41

#define SEND_DATA_FORM_SLECTOR_SWITCH 12

int pinIN[18] = {
                JoyA_YAxis, JoyB_YAxis,
                JoyC_XAxis, JoyD_XAxis,
                Joy_Keep,
                SLECTOR_SWITCH_A1, SLECTOR_SWITCH_A2, SLECTOR_SWITCH_A3,
                SLECTOR_SWITCH_B1, SLECTOR_SWITCH_B2, SLECTOR_SWITCH_B3,
                SLECTOR_SWITCH_C1, SLECTOR_SWITCH_C2, SLECTOR_SWITCH_C3,
                SEND_DATA_FORM_SLECTOR_SWITCH,
                Limit_XAxis, Limit_YAxis, Limit_ZAxis,
                };
/*
*   JoyStick : Common => GND

*       A
*       |
*  C -- | -- D
*       |
*       B

*/


#endif