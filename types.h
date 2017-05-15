#ifndef TC_TYPES_H
#define TC_TYPES_H

/* Positions of controls */
struct rc_controls{
    int channel_1;
    int channel_2;
    int channel_3;
    int channel_4;
    int channel_5;
    int channel_6;
    int channel_7;
    int channel_8;
};

/* Current movement command */
struct direction_vector{
    float rotation;
    float x_translation;
    float y_translation;
};

/* Outputs for devices */
struct rc_outputs{
    int wheel_1;
    int wheel_2;
    int wheel_3;
    int weapon_motor_1;
    int weapon_motor_2;
};

/* Ring definition */
struct ring{
    const int max_rpm;
    int rpm;
    const int max_energy;
    float energy;
};

/* Battery definition */
struct battery{
    int capacity;
    int used_capacity;
    const float max_voltage;
    float voltage;
    const int max_amps;
    int amps;
    const int max_temperature;
    int temperature;
};

/* Motor definition */
struct motor{
    const int max_rpm;
    int rpm;
    const int max_power;
    int power;
    const float max_voltage;
    float voltage;
    const float max_amps;
    float amps;
    const int max_temperature;
    int temperature;
};

/* Distance sensor definition */
struct distance_sensor{
    int id;
    const float max_range;
    float range;
};

#endif //TC_TYPES_H
