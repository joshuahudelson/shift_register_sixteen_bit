#include "m_pd.h"

static t_class * shift_register_class;

typedef struct _shift_register{
  t_object    x_obj;
  t_int       state, num_bits;
  t_inlet     * in_input, * in_state, * in_num_bits;
  t_outlet    * out_binary, * out_state;
}t_shift_register;


void shift_register_onbang(t_shift_register * x){
  post("Banged it.");
}

void * shift_register_new(t_floatarg f1, t_floatarg f2){

  t_shift_register * x = (t_shift_register *) pd_new(shift_register_class);

  x->state = 0;
  x->num_bits = 32;

  x->in_input     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_input"));
  x->in_state     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_state"));
  x->in_num_bits  = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_num_bits"));
  x->out_binary   = outlet_new(&x->x_obj, &s_float);
  x->out_state    = outlet_new(&x->x_obj, &s_float);

  return (void *) x;
}

void shift_register_free(t_shift_register * x){
  inlet_free(x->in_input);
  inlet_free(x->in_state);
  inlet_free(x->in_num_bits);
  outlet_free(x->out_binary);
  outlet_free(x->out_state);
}

void shift_register_setup(void){
  shift_register_class = class_new(gensym("shift_register"),
                             (t_newmethod) shift_register_new,
                             (t_method) shift_register_free,
                             sizeof(t_shift_register),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             0);

  class_addbang(shift_register_class, (t_method) shift_register_onbang);
}
