#ifndef LIBPTG_H
#define LIBPTG_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdint.h>

class PTGException
{
private:
    std::string m_message;
public:
    PTGException(std::string msg) {m_message = msg;}
    ~PTGException() {}
    std::string getMessage() {return m_message;}
};

struct point {
    double xyz[3];
    float i;
    unsigned char rgb[3];
};

class libPTG
{
private:
    const uint32_t MAGIC;

    char header[4];
    char *key, *value;
    bool read;
    uint32_t magic;
    uint32_t cols;
    uint32_t rows;
    uint32_t rows_total;
    uint32_t version;
    uint32_t total_pts;
    double azim_min,azim_max,elev_min,elev_max,transform[16];
    uint32_t properties;
    std::vector<point> points;
    std::ifstream file;
    bool read_string(std::ifstream *f, char **str);
    //flags config
    bool is_float;
    bool is_intens;
    bool is_rgb;
public:
    libPTG(std::string fileName);
    ~libPTG();
    void readPoints();
    uint32_t getPointsCount();
    point getPoint(int i);
};

#endif // LIBPTG_H
