#include "ptg.h"

libPTG::libPTG(std::string fileName):
    MAGIC(2458887111)
{
    key = value = NULL;
    read = false;
    magic=0;
    cols=0;
    rows=0;
    rows_total=0;
    version=0;
    total_pts=0;
    azim_min = azim_max = elev_min = elev_max=0;
    is_float = false;
    is_intens = false;
    is_rgb = false;
    file.open(fileName.c_str(), std::ios::binary);
}

libPTG::~libPTG()
{
    file.close();
}

bool libPTG::read_string(std::ifstream *f, char **str)
{
    uint32_t length;
    if(*str != NULL)
        delete [] *str;
    f->read((char*)&length, sizeof(length));
    *str = new char[length];
    f->read(*str,length);
    if(length) return true;
    else
        return false;
}

void libPTG::readPoints()
{
    if(file.is_open())
    {
        file.read(header, 4);
        if(std::string(header) == "PTG")
        {
            file.read((char*)&magic, sizeof(magic));
            if(magic == MAGIC)
            {
                read_string(&file,&key);
                if(std::string(key)=="%%header_begin")
                {
                    read = true;
                    do
                    {
                        read_string(&file,&key);
                        if(std::string(key) == "%%header_end")
                        {
                            read = false;
                        }
                        else if(std::string(key) == "%%version")
                        {
                            file.read((char*)&version, sizeof(version));
                            //std::cout << version << std::endl;
                        }
                        else if(std::string(key) == "%%sw_name")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%scan_name")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%scanner_name")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%scanner_model")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%scanner_ip_addr")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%creation_date")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%creation_time")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%texte_*")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%text_*")
                        {
                            read_string(&file,&value);
                            //std::cout << value << std::endl;
                        }
                        else if(std::string(key) == "%%cols")
                        {
                            file.read((char*)&cols, sizeof(cols));
                            //std::cout << cols << std::endl;
                        }
                        else if(std::string(key) == "%%rows")
                        {
                            file.read((char*)&rows, sizeof(rows));
                            //std::cout << rows << std::endl;
                        }
                        else if(std::string(key) == "%%rows_total")
                        {
                            file.read((char*)&rows_total, sizeof(rows_total));
                            //std::cout << rows_total << std::endl;
                        }
                        else if(std::string(key) == "%%azim_min")
                        {
                            file.read((char*)&azim_min, sizeof(azim_min));
                        }
                        else if(std::string(key) == "%%azim_max")
                        {
                            file.read((char*)&azim_max, sizeof(azim_max));
                        }
                        else if(std::string(key) == "%%elev_min")
                        {
                            file.read((char*)&elev_min, sizeof(elev_min));
                        }
                        else if(std::string(key) == "%%elev_max")
                        {
                            file.read((char*)&elev_max, sizeof(elev_max));
                        }
                        else if(std::string(key) == "%%transform")
                        {
                            file.read((char*)&transform, sizeof(transform));
                        }
                        else if(std::string(key) == "%%properties")
                        {
                            file.read((char*)&properties, sizeof(properties));
                            //std::cout << properties << std::endl;
                        }
                        else if(std::string(key) == "%%header_end")
                        {
                            read = false;
                        }
                    }
                    while(read);

                    uint64_t absolute_col_offsets[cols];
                    file.read((char*)&absolute_col_offsets, sizeof(absolute_col_offsets));

                    if(properties & 0x1)
                        is_float = true;
                    if(properties & 0x2)
                        is_float = false;
                    if(properties & 0x4)
                        is_intens = true;
                    if(properties & 0x8)
                        is_rgb = true;

                    uint32_t nrow = ceil((double)rows/8.0);
                    uint32_t numpts = 0;
                    total_pts=0;
                    uint8_t bitmask[nrow];
                    for(int m=0;m<cols;m++)
                    {
                        numpts = 0;
                        file.read((char*)&bitmask, sizeof(bitmask));
                        for(int j=0; j<nrow; j++)
                        {
                            if (bitmask[j] & 0x80) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x40) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x20) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x10) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x08) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x04) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x02) {
                              numpts++;
                            }
                            if (bitmask[j] & 0x01) {
                              numpts++;
                            }
                        }
                        for(int j=0; j<numpts; j++)
                        {
                            point p;
                            float xyz[3];
                            double XYZ[3];
                            float intens;
                            unsigned char rgb[3];
                            if(is_float)
                            {
                                file.read((char*)&xyz, sizeof(xyz));
                                p.xyz[0] = xyz[0];
                                p.xyz[1] = xyz[1];
                                p.xyz[2] = xyz[2];
                            }
                            else
                            {
                                file.read((char*)&XYZ, sizeof(XYZ));
                                p.xyz[0] = XYZ[0];
                                p.xyz[1] = XYZ[1];
                                p.xyz[2] = XYZ[2];
                            }
                            if(is_intens)
                            {
                                file.read((char*)&intens, sizeof(intens));
                                p.i = intens;
                            }
                            if(is_rgb)
                            {
                                file.read((char*)&rgb, sizeof(rgb));
                                p.rgb[0] = rgb[0];
                                p.rgb[1] = rgb[1];
                                p.rgb[2] = rgb[2];
                            }
                            points.emplace_back(p);
                        }
                        total_pts += numpts;
                    }
                }
                else
                {
                    throw PTGException("header not found.");
                }
            }
            else
            {
                throw PTGException("magic number not equals.");
            }
        }
        else
        {
            throw PTGException("not PTG format.");
        }
    }
    else
    {
        throw PTGException("can't open file.");
    }
}

uint32_t libPTG::getPointsCount()
{
    return total_pts;
}

point libPTG::getPoint(int i)
{
    if(!points.empty())
    {
        return points.at(i);
    }
}
