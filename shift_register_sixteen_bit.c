#include "m_pd.h"
#include <math.h>

static t_class * shift_register_sixteen_bit_class;

typedef struct _shift_register_sixteen_bit{
  t_object    x_obj;
  t_int       state, max_state_value;
  t_inlet     * in_input, * in_state;
  t_outlet    *out1, *out2, *out3, *out4, *out5, *out6, *out7, *out8,
              *out9, *out10, *out11, *out12, *out13, *out14, *out15, *out16,
              *out_state;
}t_shift_register_sixteen_bit;


void shift_register_sixteen_bit_onbang(t_shift_register_sixteen_bit * x){
  x->state >>= 1;
}

float shift_register_sixteen_bit_get_bit(t_shift_register_sixteen_bit * x, int bit_index){
  return ( (float) ((x->state>>bit_index) & 1));
}

void shift_register_sixteen_bit_new_value(t_shift_register_sixteen_bit * x, t_float f){
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
    int mask = 1 << 15;
    x->state |= mask;
  }
  else{
    int mask = 1 << 15;
    mask = ~mask;
    x->state &= mask;
  }
}

void shift_register_sixteen_bit_output_values(t_shift_register_sixteen_bit * x){
  outlet_float(x->out1, shift_register_sixteen_bit_get_bit(x, 15));
  outlet_float(x->out2, shift_register_sixteen_bit_get_bit(x, 14));
  outlet_float(x->out3, shift_register_sixteen_bit_get_bit(x, 13));
  outlet_float(x->out4, shift_register_sixteen_bit_get_bit(x, 12));
  outlet_float(x->out5, shift_register_sixteen_bit_get_bit(x, 11));
  outlet_float(x->out6, shift_register_sixteen_bit_get_bit(x, 10));
  outlet_float(x->out7, shift_register_sixteen_bit_get_bit(x, 9));
  outlet_float(x->out8, shift_register_sixteen_bit_get_bit(x, 8));
  outlet_float(x->out9, shift_register_sixteen_bit_get_bit(x, 7));
  outlet_float(x->out10, shift_register_sixteen_bit_get_bit(x, 6));
  outlet_float(x->out11, shift_register_sixteen_bit_get_bit(x, 5));
  outlet_float(x->out12, shift_register_sixteen_bit_get_bit(x, 4));
  outlet_float(x->out13, shift_register_sixteen_bit_get_bit(x, 3));
  outlet_float(x->out14, shift_register_sixteen_bit_get_bit(x, 2));
  outlet_float(x->out15, shift_register_sixteen_bit_get_bit(x, 1));
  outlet_float(x->out16, shift_register_sixteen_bit_get_bit(x, 0));

  outlet_float(x->out_state, x->state);
}

void shift_register_sixteen_bit_update_state(t_shift_register_sixteen_bit * x, t_float f){
  int temp_int = (int) f;
  int temp_max = x->max_state_value;
  if (temp_int > temp_max){
    x->state = temp_max;
  }
  else if (temp_int < 0){
    x->state = 0;
  }
  else{
  x->state = temp_int;
  }
}

void * shift_register_sixteen_bit_new(t_floatarg f1, t_floatarg f2){

  t_shift_register_sixteen_bit * x = (t_shift_register_sixteen_bit *) pd_new(shift_register_sixteen_bit_class);

  x->state = 0;
  x->max_state_value = pow(2, 17)-1;

  x->in_input     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_input"));
  x->in_state     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_state"));
  x->out1         = outlet_new(&x->x_obj, &s_float);
  x->out2         = outlet_new(&x->x_obj, &s_float);
  x->out3         = outlet_new(&x->x_obj, &s_float);
  x->out4         = outlet_new(&x->x_obj, &s_float);
  x->out5         = outlet_new(&x->x_obj, &s_float);
  x->out6         = outlet_new(&x->x_obj, &s_float);
  x->out7         = outlet_new(&x->x_obj, &s_float);
  x->out8         = outlet_new(&x->x_obj, &s_float);
  x->out9         = outlet_new(&x->x_obj, &s_float);
  x->out10        = outlet_new(&x->x_obj, &s_float);
  x->out11        = outlet_new(&x->x_obj, &s_float);
  x->out12        = outlet_new(&x->x_obj, &s_float);
  x->out13        = outlet_new(&x->x_obj, &s_float);
  x->out14        = outlet_new(&x->x_obj, &s_float);
  x->out15        = outlet_new(&x->x_obj, &s_float);
  x->out16        = outlet_new(&x->x_obj, &s_float);
  x->out_state    = outlet_new(&x->x_obj, &s_float);

  return (void *) x;
}

void shift_register_sixteen_bit_free(t_shift_register_sixteen_bit * x){
  inlet_free(x->in_input);
  inlet_free(x->in_state);
  outlet_free(x->out1);
  outlet_free(x->out2);
  outlet_free(x->out3);
  outlet_free(x->out4);
  outlet_free(x->out5);
  outlet_free(x->out6);
  outlet_free(x->out7);
  outlet_free(x->out8);
  outlet_free(x->out9);
  outlet_free(x->out10);
  outlet_free(x->out11);
  outlet_free(x->out12);
  outlet_free(x->out13);
  outlet_free(x->out14);
  outlet_free(x->out15);
  outlet_free(x->out16);
  outlet_free(x->out_state);
}

void shift_register_sixteen_bit_setup(void){
  shift_register_sixteen_bit_class = class_new(gensym("shift_register_sixteen_bit"),
                             (t_newmethod) shift_register_sixteen_bit_new,
                             (t_method) shift_register_sixteen_bit_free,
                             sizeof(t_shift_register_sixteen_bit),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             0);

  class_addbang(shift_register_sixteen_bit_class, (t_method) shift_register_sixteen_bit_onbang);

  class_addmethod(shift_register_sixteen_bit_class,
                  (t_method) shift_register_sixteen_bit_output_values,
                  gensym("output"),
                  0);

  class_addmethod(shift_register_sixteen_bit_class,
                 (t_method) shift_register_sixteen_bit_new_value,
                  gensym("in_input"),
                  A_DEFFLOAT,
                  0);

  class_addmethod(shift_register_sixteen_bit_class,
                  (t_method) shift_register_sixteen_bit_update_state,
                   gensym("in_state"),
                   A_DEFFLOAT,
                   0);


}
