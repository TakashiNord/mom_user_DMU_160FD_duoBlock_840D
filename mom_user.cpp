//////////////////////////////////////////////////////////////////////////////
//
//  mom_user.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <uf.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_mom.h>

//#include <windows.h>

#include "mom_user.h"

//----------------------------------------------------------------------------

/*
 * Защита через UF_PART_ask_part_history:
 */

/*
 * Exported Tcl procedures:
 */
static int EXTN_rtv_mom_cp_part_attr
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
);

//----------------------------------------------------------------------------
//  Activation Methods
//----------------------------------------------------------------------------

//  Explicit Activation
//      This entry point is used to activate the application explicitly, as in
//      "File->Execute UG/Open->User Function..."
//----- (10001010) --------------------------------------------------------
extern "C" DllExport void entry_rtv_mom_cp_part_attr( char *parm, int *returnCode, int rlen )
{
  int errorCode ;
  UF_MOM_id_t mom ;
  void *interp = NULL ;

  if ( !UF_is_initialized() )
  {
    errorCode = UF_initialize();
	*returnCode = errorCode;
    if ( errorCode ) return ;
  }

  /* Get the TCL interpreter id from the ufusr param */
  UF_MOM_ask_interp_from_param ( parm, &interp ) ;
  /* Get the MOM id from the ufusr param */
  UF_MOM_ask_mom (parm, &mom) ;
  /*Создаем функции в интепретаторе*/
  UF_MOM_extend_xlator ( mom , "EXTN_rtv_mom_cp_part_attr", EXTN_rtv_mom_cp_part_attr ) ;

  *returnCode = 0;
  if ( errorCode == 1 )  errorCode = UF_terminate();
  *returnCode = errorCode;
}


//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

// Unload Handler
//     This function specifies when to unload your application from Unigraphics.
//     If your application registers a callback (from a MenuScript item or a
//     User Defined Object for example), this function MUST return
//     "UF_UNLOAD_UG_TERMINATE".
extern "C" int ufusr_ask_unload( void )
{
     /* unload immediately after application exits*/
     //return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     //return ( UF_UNLOAD_SEL_DIALOG );
     /*when UG terminates...              */
     return ( UF_UNLOAD_UG_TERMINATE );
}


/*-----------------------------------------------------------------------
 * Function Name: EXTN_rtv_mom_cp_part_attr
 *
 *
 -----------------------------------------------------------------------*/
static int EXTN_rtv_mom_cp_part_attr
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
)
{
  UF_MOM_id_t mom_id = ( UF_MOM_id_t ) client_data ;

  int     ii ;
  char    Dest[256] ;
  double  angle1 , angle2 ;
  double  angle11 , angle22 ;
  double  dy , C0 , B0;
  const  double c_pi = 3.14159265358979 ;
  double  x , y , z , i , j , k ;

  int errorCode = 1;  
  
  
    sprintf(Dest, "Number of args %ld", argc);
    if ( argc > 0 )
    {
      for (ii=0;ii<argc;ii++) sprintf(Dest, "Arg 0 %s", argv[ii]);
    }

  	if (0==strcmp(argv[1],"PLANE"))
   	{
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      sprintf(Dest, "MOM_output_literal \"AROT Z%.3lf Y%.3lf\" ", angle2,angle1);
      UF_MOM_execute_command(mom_id, Dest);	  
   	}

	if ( 0==strcmp(argv[1], "PLANE_moving") )
    {
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      sprintf(Dest, "MOM_output_literal \"G0 X%.3lf Y%.3lf \"", x, y);
      UF_MOM_execute_command(mom_id, Dest);
    }	
	
   if (0==strcmp(argv[1], "INPUT_ZERO") )
    {
      char ca2[133]="54\0" ;
	  int  ir3 ; 
	  UF_UI_close_listing_window();
      UF_UI_lock_ug_access(UF_UI_FROM_CUSTOM);
      uc1600(":Введите номер нуля программы G", ca2, &ir3);
      UF_MOM_set_string(mom_id, "zero", ca2);
      UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);
    }

	if ( !strcmp(argv[1], "M128") )
    {
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "0", x);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "1", y);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "2", z);
    }
	
   	if (0==strcmp(argv[1],"CALC_ANGLES"))
   	{
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1); // B
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2); // C
      if (angle1 = 90) {
		angle11 = 180.0 ; 
		angle22 = angle2 - 90.0 ;
	  } else {
        dy= cos ( angle1 * 3.14159265358979 / 180.0 ) ;
        y = 1.0 - dy ;
        x = sqrt ((dy*2.0) * (1.0-dy) ) ;
        C0 = acos(dy*2.0 - 1.0)  * 180.0 / 3.14159265358979 ;
        B0 = atan2(y, x) * 180.0 / 3.14159265358979 ;
        B0 = angle1 - B0 ;
        if (B0 > angle1) B0 = B0 + 180.0 ;		
	    angle11= B0 ;
	    angle22= C0 ;
	  }
	  sprintf(Dest, "MOM_output_literal \"G0 B%.3lf C=DC(%.3lf+SHIFT_ANGLE)\"", angle11, angle22);
      UF_MOM_execute_command(mom_id, Dest);
   	}	

 	if ( !strcmp(argv[1], "M128_init") )
    {
      i = 0 ;
	  j = 0.7071067811865 ;
	  k = 0.7071067811865 ;
	  UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "0", i);
      UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "1", j);
      UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "2", k);
      sprintf(Dest, "MOM_reload_kinematics");
      UF_MOM_execute_command(mom_id, Dest);
    }
	
    if ( 0==strcmp(argv[1], "FOR_ROT") )
    {
      sprintf(Dest, "MOM_output_literal \"ROT\"");
      UF_MOM_execute_command(mom_id, Dest);
    }

   	if (0==strcmp(argv[1],"M128_init_zero"))
   	{
      UF_MOM_ask_assoc_double_array(mom_id, "kin_4th_axis_vector", "0", &i);
      UF_MOM_ask_assoc_double_array(mom_id, "kin_4th_axis_vector", "1", &j);
      UF_MOM_ask_assoc_double_array(mom_id, "kin_4th_axis_vector", "2", &k);
      i = 0 ;
      j = 1 ;
      k = 0 ;
	  UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "0", i);
      UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "1", j);
      UF_MOM_set_assoc_double_array(mom_id, "kin_4th_axis_vector", "2", k);
   	}

    if ( !strcmp(argv[1], "UNLOAD") ) errorCode = 0;

    UF_MOM_set_string(mom_id, "var_unknow", Dest);
    UF_terminate();	
	
  return ( 0 ) ;
}

