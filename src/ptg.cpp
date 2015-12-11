#include "ptg.h"

char PTG_FILETYPETAG[]    = FILETYPETAG;
long PTG_MAGICNUMBER    = MAGICNUMBER;
int  PTG_PROP_XYZFLOAT  = PROP_XYZFLOAT;
int  PTG_PROP_XYZDOUBLE = PROP_XYZDOUBLE;
int  PTG_PROP_INTENSITY = PROP_INTENSITY;
int  PTG_PROP_RGBVALUE  = PROP_RGBVALUE;

static const int    MAX_handle_idx=1;
static int          act_handle_idx=0;
static _cPTG_handle *handles[1]={NULL};

extern "C" char* PTG__About(char *pstr)
{
  return(pstr);
}

extern "C" void PTG__nogeoreference(double *p)
{
  _cPTG_nogeoreference(p);
}

extern "C" void PTG__copygeoreference(double *p,double *q)
{
  _cPTG_copygeoreference(p,q);
}


extern "C" void PTG__Init(
                       void (*warnings) (char *str,...),
                       void (*errors)   (char *str,...),
                       int   debuglevel,
                       char *creator
                     )
{
  act_handle_idx = 0;
  _cPTG_init(warnings,errors,debuglevel,creator);
}

extern "C" void PTG__Exit(void)
{
   _cPTG_exit();
}

extern "C" TPTG_handle PTG__Create(char *fname,TPTG_metadata_mandatory *mdata)
{
  if (act_handle_idx<MAX_handle_idx)
  {
    handles[act_handle_idx] =   _cPTG_create(fname,(T_cPTG_metadata_mandatory *) mdata);
    act_handle_idx++;
    return(act_handle_idx-1);
  }
  return(-1);
}

extern "C" void PTG__AddPoint(TPTG_handle handle,TPTG_point *point)
{
 if (point!=NULL)
 {
   T_cPTG_xyzIrgb pt;

   for (int k=0;k<3;k++) pt.x[k]   = point->x[k];
                         pt.I      = point->I;
   for (int k=0;k<3;k++) pt.rgb[k] = point->rgb[k];
   _cPTG_add_xyzIrgb(handles[handle],&pt);
 }
 else
 {
   _cPTG_add_xyzIrgb(handles[handle],NULL);
 }
}


extern "C" TPTG_handle PTG__Open(char *fname,TPTG_metadata_mandatory *mdata)
{
  if (act_handle_idx<MAX_handle_idx)
  {
    handles[act_handle_idx] = _cPTG_open(fname,(T_cPTG_metadata_mandatory *) mdata);
    act_handle_idx++;
    return(act_handle_idx-1);
  }
  return(-1);
}

extern "C" bool PTG__GetPoint(TPTG_handle handle,TPTG_point *point)
{
 T_cPTG_xyzIrgb pt;
 bool ret;

 ret = _cPTG_get_xyzIrgb(handles[handle],&pt);

 for (int k=0;k<3;k++) point->x[k]   = pt.x[k];
                       point->I      = pt.I;
 for (int k=0;k<3;k++) point->rgb[k] = pt.rgb[k];

 return(ret);
}

/*
    Liefert den an der Position col, row in der Datenmatrix gespeicherten Messpunkt
*/
extern "C" bool PTG__GetPointColRow(TPTG_handle handle,int col,int row, TPTG_point *point)
{
   T_cPTG_xyzIrgb pt;
   bool ret;

   ret = _cPTG_get_xyzIrgb(handles[handle],col,row,&pt);

   for (int k=0;k<3;k++) point->x[k]   = pt.x[k];
                         point->I      = pt.I;
   for (int k=0;k<3;k++) point->rgb[k] = pt.rgb[k];

   return(ret);
}

extern "C" void PTG__Close(TPTG_handle handle)
{
  if (act_handle_idx>=0)
  {
    _cPTG_close(handles[handle]);
    act_handle_idx--;
  }
}

extern "C" void PTG__Local2World(TPTG_handle handle,TPTG_point *point,double *X)
{
  T_cPTG_xyzIrgb pt;

  for (int k=0;k<3;k++) pt.x[k]   = point->x[k];

    _cPTG_Local2World(handles[handle],&pt,X);
}
