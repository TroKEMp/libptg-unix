#ifndef CPTG
#define CPTG

#include <stdio.h>

#define VERSION "1.00"

#define FILETYPETAG    "PTG"
#define MAGICNUMBER    0x928FA3C7

#define PROP_XYZFLOAT  0x0001
#define PROP_XYZDOUBLE 0x0002
#define PROP_INTENSITY 0x0004
#define PROP_RGBVALUE  0x0008

#define DATATYPE_xyz      PROP_XYZFLOAT
#define DATATYPE_xyzI     PROP_XYZFLOAT | PROP_INTENSITY
#define DATATYPE_xyzrgb   PROP_XYZFLOAT | PROP_RGBVALUE
#define DATATYPE_xyzIrgb  PROP_XYZFLOAT | PROP_INTENSITY | PROP_RGBVALUE
#define DATATYPE_XYZ      PROP_XYZDOUBLE
#define DATATYPE_XYZI     PROP_XYZDOUBLE | PROP_INTENSITY
#define DATATYPE_XYZrgb   PROP_XYZDOUBLE | PROP_RGBVALUE
#define DATATYPE_XYZIrgb  PROP_XYZDOUBLE | PROP_INTENSITY | PROP_RGBVALUE

typedef char       Tint8;  // 1 Byte
typedef long       Tint32;
typedef long long  Tint64;
typedef double     Tdouble;


typedef  void (*T_cPTG_echo) (char *str,...);

struct _cPTG_xyz
{
  float x[3];             // cartesian 3D coordinates   4 byte  [-1000.0000,1000.0000]  12 byte
};

typedef struct _cPTG_xyz T_cPTG_xyz;

struct _cPTG_xyzI
{
  float x[3];             // cartesian 3D coordinates   4 byte  [-1000.0000,1000.0000]  12 byte
  float I;                // intensity                  4 byte  [0.0,1.0]                4 byte
};

typedef struct _cPTG_xyzI T_cPTG_xyzI;

struct _cPTG_xyzrgb
{
  float x[3];             // cartesian 3D coordinates   4 byte  [-1000.0000,1000.0000]  12 byte
  unsigned char  rgb[3];  // RGB-color                  1 byte  [0,255]                   3 byte
};

typedef struct _cPTG_xyzrgb T_cPTG_xyzrgb;

struct _cPTG_xyzIrgb
{
  float x[3];             // cartesian 3D coordinates   4 byte  [-1000.0000,1000.0000]  12 byte
  float I;                // intensity                  4 byte  [0.0,1.0]                4 byte
  unsigned char  rgb[3];  // RGB-color                  1 byte  [0,255]                   3 byte
};

typedef struct _cPTG_xyzIrgb T_cPTG_xyzIrgb;

struct _cPTG_metadata_mandatory
{
  Tint32  version;
  Tint32  cols;
  Tint32  rows;
  Tint32  properties;
  Tdouble tmatrix[4*4];
};

typedef struct _cPTG_metadata_mandatory T_cPTG_metadata_mandatory;

struct _cPTG_handle {
  FILE  *fp;
  bool   ok;
  Tint32 act_col;
  Tint32 act_row;
  Tint32 nrow;

  /* informations and data from one row */
  Tint8  *bitmask;
  Tint32  numpts;
  Tint32  act_rowpos;
  bool           *is;
  T_cPTG_xyz     *xyz_points;
  T_cPTG_xyzI    *xyzI_points;
  T_cPTG_xyzrgb  *xyzrgb_points;
  T_cPTG_xyzIrgb *xyzIrgb_points;
};

typedef struct _cPTG_handle T_cPTG_handle;


// global variables

// helper functions
void  _cPTG_nogeoreference(double *p);
void  _cPTG_copygeoreference(double *p,double *q);
char *_cPTG_new_string(char *str);

// Level 0
void _cPTG_init(void (*warnings) (char *str,...),
                void (*errors)   (char *str,...),
                int   debuglevel,
                char *creator=NULL
               );
void _cPTG_exit(void);

// Level 1  -- creation --

T_cPTG_handle *_cPTG_create(char *fname,T_cPTG_metadata_mandatory *mdata);


bool _cPTG_add_xyzIrgb(T_cPTG_handle *handle,T_cPTG_xyzIrgb *point);

// Level 1 -- opening --

T_cPTG_handle *_cPTG_open(char *fname,
                          T_cPTG_metadata_mandatory *mdata
                          );

bool _cPTG_get_xyzIrgb(T_cPTG_handle *handle,T_cPTG_xyzIrgb *point);
bool _cPTG_get_xyzIrgb(T_cPTG_handle *handle,int u, int v, T_cPTG_xyzIrgb *point);

// Level 1 -- closing --

void           _cPTG_close(T_cPTG_handle *handle);



void _cPTG_Local2World(T_cPTG_handle *handle,T_cPTG_xyzIrgb *point,double *X);

#endif // CPTG

