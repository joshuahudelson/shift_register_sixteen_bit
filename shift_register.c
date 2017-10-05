#include "m_pd.h"

static t_class * shift_register_class

typedef Struct _shift_register{
  t_object    x_obj;
  t_int       num_elems, current_exponent;
  t_float     element_array[100];
  t_inlet     * in_elems, * in_exp;
  t_outlet    * out;

}t_shift_register;


void reset_Reg(t_shift_register * x){
  for (i=0; i<x->num_elems; i++){
    x->element_array[i] = 0.0;
  }
}


void * shift_register_new(t_floatarg f1, t_floatarg f2){

  t_shift_register * x = (t_shift_register *) pd_new(shift_register_class);

  reset_Reg(x);

  x->in_elems = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &sfloat, gensym("in_elems"));
  x->in_exp   = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &sfloat, gensym("in_exp"));
  x->out      = outlet_new(&x->x_obj, &s_float);

  return (void *) x;
}

void shift_register_free(t_shift_register * x){


}
