#ifndef PTG
#define PTG

#include "cPTG.h"

extern "C" char PTG_FILETYPETAG[];
extern "C" long PTG_MAGICNUMBER;
extern "C" int  PTG_PROP_XYZFLOAT;
extern "C" int  PTG_PROP_XYZDOUBLE;
extern "C" int  PTG_PROP_INTENSITY;
extern "C" int  PTG_PROP_RGBVALUE;

typedef  void (*TPTG_echo) (char *str,...);

typedef int TPTG_handle;

struct _PTG_point
{
  double x[3];             // cartesian 3D coordinates   8 byte  [-1000.0000,1000.0000]  24 byte
  float  I;                // intensity                  4 byte  [0.0,1.0]                4 byte
  unsigned char  rgb[3];   // RGB-color                  1 byte  [0,255]                  3 byte
};
typedef struct _PTG_point TPTG_point;

struct _PTG_metadata_mandatory
{
  long   version;
  long   cols;
  long   rows;
  long   properties;
  double tmatrix[4*4];
};

typedef struct _PTG_metadata_mandatory TPTG_metadata_mandatory;



extern "C" char* PTG__About(char *pstr);


extern "C" void PTG__nogeoreference(double *p);
extern "C" void PTG__copygeoreference(double *p,double *q);

extern "C" void PTG__Init(
                       void (*warnings) (char *str,...),
                       void (*errors)   (char *str,...),
                       int   debuglevel,
                       char *creator=NULL
                     );

extern "C" void PTG__Exit(void);


extern "C" TPTG_handle PTG__Create(char *fname,TPTG_metadata_mandatory *mdata);
extern "C" void PTG__AddPoint(TPTG_handle handle,TPTG_point *point);

extern "C" TPTG_handle PTG__Open(char *fname,TPTG_metadata_mandatory *mdata);
extern "C" bool PTG__GetPoint(TPTG_handle handle,TPTG_point *point);
extern "C" bool PTG__GetPointColRow(TPTG_handle handle,int col, int row, TPTG_point *point);

extern "C" void PTG__Close(TPTG_handle handle);
extern "C" void PTG__Local2World(TPTG_handle handle,TPTG_point *point,double *X);

#endif // PTG

