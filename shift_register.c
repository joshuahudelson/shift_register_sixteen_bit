#include "m_pd.h"
#include <math.h>

static t_class * shift_register_class;

typedef struct _shift_register{
  t_object    x_obj;
  t_int       state, num_bits;
  t_inlet     * in_input, * in_state, * in_num_bits;
  t_outlet    * out_binary, * out_state;
}t_shift_register;


void shift_register_onbang(t_shift_register * x){
  x->state >>= 1;
  outlet_float(x->out_binary, x->state & 1);
  outlet_float(x->out_state, x->state);
}

void shift_register_new_value(t_shift_register * x, t_float f){
  int temp_float = f;

  if (temp_float > 1){
    temp_float = 1;
  }
  else if (temp_float < 0){
    temp_float = 0;
  }
  else{
    temp_float = round(temp_float);
  }

  if (temp_float == 1){
    int mask = 1 << x->num_bits;
    x->state |= mask;
  }
  else{
    int mask = 1 << x->num_bits;
    mask = ~mask;
    x->state &= mask;
  }

  outlet_float(x->out_binary, x->state & 1);
  outlet_float(x->out_state, x->state);
}

void shift_register_update_state(t_shift_register * x, t_float f){
  int temp_int = (int) f;
  int temp_max = pow(2, (x->num_bits + 1)) - 1;
  if (temp_int > temp_max){
    x->state = temp_max;
  }
  else if (temp_int < 0){
    x->state = 0;
  }
  else{
  x->state = temp_int;
  }

  outlet_float(x->out_binary, x->state & 1);
  outlet_float(x->out_state, x->state);
}

void shift_register_set_num_bits(t_shift_register * x, t_float f){
  int temp_int = (int) f;

  if (temp_int > 16){
    x->num_bits = 16;
  }
  else if (temp_int < 0){
    x->num_bits = 0;
  }
  else {
    x->num_bits = temp_int;
  }

  int mask = (int) (pow(2, (x->num_bits + 1)) - 1);
  post("state: %i", x->state);
  post("mask: %i", mask);
  x->state &= mask;

  outlet_float(x->out_binary, x->state & 1);
  outlet_float(x->out_state, x->state);
}

void * shift_register_new(t_floatarg f1, t_floatarg f2){

  t_shift_register * x = (t_shift_register *) pd_new(shift_register_class);

  x->state = 0;
  x->num_bits = 16;

  x->in_input     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_input"));
  x->in_num_bits  = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_num_bits"));
  x->in_state     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_state"));
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

  class_addmethod(shift_register_class,
                 (t_method) shift_register_new_value,
                  gensym("in_input"),
                  A_DEFFLOAT,
                  0);

  class_addmethod(shift_register_class,
                 (t_method) shift_register_set_num_bits,
                  gensym("in_num_bits"),
                  A_DEFFLOAT,
                  0);

  class_addmethod(shift_register_class,
                  (t_method) shift_register_update_state,
                   gensym("in_state"),
                   A_DEFFLOAT,
                   0);


}
