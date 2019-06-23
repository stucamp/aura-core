#pragma once

#include <stdint.h>  // uint8_t, et. al.
#include <string.h>  // memcpy()

static inline int32_t intround(float f) {
    return (int32_t)(f >= 0.0 ? (f + 0.5) : (f - 0.5));
}

static inline uint32_t uintround(float f) {
    return (int32_t)(f + 0.5);
}

// Message id constants
const uint8_t message_id_simple_test = 0;
const uint8_t message_id_array_test = 1;
const uint8_t message_id_gps_v4 = 34;

// Message: simple_test (id: 0)
struct message_simple_test_t {
    // public fields
    int16_t a;

    // internal structure for packing
    #pragma pack(push, 1)
    struct {
        int16_t a;
    } _buf;
    #pragma pack(pop)

    const uint8_t id = 0;
    const uint16_t len = sizeof(_buf);

    uint8_t *pack() {
        _buf.a = a;
        return (uint8_t *)(&_buf);
    }

    void unpack(uint8_t *message) {
        memcpy(&_buf, message, len);
        a = _buf.a;
    }
};

// Message: array_test (id: 1)
struct message_array_test_t {
    // public fields
    double time;
    int8_t flags[4];
    float orientation[9];
    uint16_t something;

    // internal structure for packing
    #pragma pack(push, 1)
    struct {
        double time;
        int8_t flags[4];
        int16_t orientation[9];
        uint16_t something;
    } _buf;
    #pragma pack(pop)

    const uint8_t id = 1;
    const uint16_t len = sizeof(_buf);

    uint8_t *pack() {
        _buf.time = time;
        for (int _i=0; _i<4; _i++) _buf.flags[_i] = flags[_i];
        for (int _i=0; _i<9; _i++) _buf.orientation[_i] = intround(orientation[_i] * 53.3);
        _buf.something = something;
        return (uint8_t *)(&_buf);
    }

    void unpack(uint8_t *message) {
        memcpy(&_buf, message, len);
        time = _buf.time;
        for (int _i=0; _i<4; _i++) flags[_i] = _buf.flags[_i];
        for (int _i=0; _i<9; _i++) orientation[_i] = _buf.orientation[_i] / (float)53.3;
        something = _buf.something;
    }
};

// Message: gps_v4 (id: 34)
struct message_gps_v4_t {
    // public fields
    uint8_t index;
    float time_sec;
    double latitude_deg;
    double longitude_deg;
    float altitude_m;
    float vn_ms;
    float ve_ms;
    float vd_ms;
    double unixtime_sec;
    uint8_t satellites;
    float horiz_accuracy_m;
    float vert_accuracy_m;
    float pdop;
    uint8_t fix_type;

    // internal structure for packing
    #pragma pack(push, 1)
    struct {
        uint8_t index;
        float time_sec;
        double latitude_deg;
        double longitude_deg;
        float altitude_m;
        int16_t vn_ms;
        int16_t ve_ms;
        int16_t vd_ms;
        double unixtime_sec;
        uint8_t satellites;
        uint16_t horiz_accuracy_m;
        uint16_t vert_accuracy_m;
        uint16_t pdop;
        uint8_t fix_type;
    } _buf;
    #pragma pack(pop)

    const uint8_t id = 34;
    const uint16_t len = sizeof(_buf);

    uint8_t *pack() {
        _buf.index = index;
        _buf.time_sec = time_sec;
        _buf.latitude_deg = latitude_deg;
        _buf.longitude_deg = longitude_deg;
        _buf.altitude_m = altitude_m;
        _buf.vn_ms = intround(vn_ms * 100);
        _buf.ve_ms = intround(ve_ms * 100);
        _buf.vd_ms = intround(vd_ms * 100);
        _buf.unixtime_sec = unixtime_sec;
        _buf.satellites = satellites;
        _buf.horiz_accuracy_m = uintround(horiz_accuracy_m * 100);
        _buf.vert_accuracy_m = uintround(vert_accuracy_m * 100);
        _buf.pdop = uintround(pdop * 100);
        _buf.fix_type = fix_type;
        return (uint8_t *)(&_buf);
    }

    void unpack(uint8_t *message) {
        memcpy(&_buf, message, len);
        index = _buf.index;
        time_sec = _buf.time_sec;
        latitude_deg = _buf.latitude_deg;
        longitude_deg = _buf.longitude_deg;
        altitude_m = _buf.altitude_m;
        vn_ms = _buf.vn_ms / (float)100;
        ve_ms = _buf.ve_ms / (float)100;
        vd_ms = _buf.vd_ms / (float)100;
        unixtime_sec = _buf.unixtime_sec;
        satellites = _buf.satellites;
        horiz_accuracy_m = _buf.horiz_accuracy_m / (float)100;
        vert_accuracy_m = _buf.vert_accuracy_m / (float)100;
        pdop = _buf.pdop / (float)100;
        fix_type = _buf.fix_type;
    }
};

