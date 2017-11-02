#include "m_pd.h"
#include <math.h>

static t_class *shift_register_sixteen_bit_tilde_class;

typedef struct _shift_register_sixteen_bit_tilde{
  t_object    x_obj;
  t_int       state, max_state_value;
  t_inlet     * in_state;
  t_outlet    *out, *out2, *out3, *out4, *out5, *out6, *out7, *out8, *out9,
              *out10, *out11, *out12, *out13, *out14, *out15, *out16;
  t_sample    input_signal_value, f;
} t_shift_register_sixteen_bit_tilde;

void shift_register_sixteen_bit_tilde_shift(t_shift_register_sixteen_bit_tilde * x){
  x->state >>= 1;
}

float shift_register_sixteen_bit_tilde_get_bit(t_shift_register_sixteen_bit_tilde * x, int bit_index){
  return ( (x->state >> bit_index) & 1);
}

void shift_register_sixteen_bit_tilde_put_most_significant_bit(t_shift_register_sixteen_bit_tilde * x, t_sample in1){
  t_sample temp_samp = in1;

  if (temp_samp > 1){
    temp_samp = 1;
  }
  else if (temp_samp < 0){
    temp_samp = 0;
  }
  else{
    temp_samp = round(temp_samp);
  }

  if (temp_samp == 1){
    int mask = 1<<15;
    x->state |= mask;
  }
  else{
    int mask = 1<<15;
    mask = ~mask;
    x->state &= mask;
  }
}

void shift_register_sixteen_bit_tilde_update_state(t_shift_register_sixteen_bit_tilde * x, t_float f){
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

t_int * shift_register_sixteen_bit_tilde_perform(t_int * w){

  t_shift_register_sixteen_bit_tilde *x   = (t_shift_register_sixteen_bit_tilde *)(w[1]);
  t_sample *in1              = (t_sample *)(w[2]);
  t_sample *out              = (t_sample *)(w[3]);
  t_sample *out2              = (t_sample *)(w[4]);
  t_sample *out3              = (t_sample *)(w[5]);
  t_sample *out4              = (t_sample *)(w[6]);
  t_sample *out5              = (t_sample *)(w[7]);
  t_sample *out6              = (t_sample *)(w[8]);
  t_sample *out7              = (t_sample *)(w[9]);
  t_sample *out8              = (t_sample *)(w[10]);
  t_sample *out9              = (t_sample *)(w[11]);
  t_sample *out10              = (t_sample *)(w[12]);
  t_sample *out11              = (t_sample *)(w[13]);
  t_sample *out12              = (t_sample *)(w[14]);
  t_sample *out13              = (t_sample *)(w[15]);
  t_sample *out14              = (t_sample *)(w[16]);
  t_sample *out15              = (t_sample *)(w[17]);
  t_sample *out16              = (t_sample *)(w[18]);
  int n                        = (int)(w[19]);

  while (n--) {
    shift_register_sixteen_bit_tilde_shift(x);
    shift_register_sixteen_bit_tilde_put_most_significant_bit(x, *in1++);

    *out++    = shift_register_sixteen_bit_tilde_get_bit(x, 15);
    *out2++   = shift_register_sixteen_bit_tilde_get_bit(x, 14);
    *out3++   = shift_register_sixteen_bit_tilde_get_bit(x, 13);
    *out4++   = shift_register_sixteen_bit_tilde_get_bit(x, 12);
    *out5++   = shift_register_sixteen_bit_tilde_get_bit(x, 11);
    *out6++   = shift_register_sixteen_bit_tilde_get_bit(x, 10);
    *out7++   = shift_register_sixteen_bit_tilde_get_bit(x, 9);
    *out8++   = shift_register_sixteen_bit_tilde_get_bit(x, 8);
    *out9++   = shift_register_sixteen_bit_tilde_get_bit(x, 7);
    *out10++  = shift_register_sixteen_bit_tilde_get_bit(x, 6);
    *out11++  = shift_register_sixteen_bit_tilde_get_bit(x, 5);
    *out12++  = shift_register_sixteen_bit_tilde_get_bit(x, 4);
    *out13++  = shift_register_sixteen_bit_tilde_get_bit(x, 3);
    *out14++  = shift_register_sixteen_bit_tilde_get_bit(x, 2);
    *out15++  = shift_register_sixteen_bit_tilde_get_bit(x, 1);
    *out16++  = shift_register_sixteen_bit_tilde_get_bit(x, 0);
  }
  return (w+20);
}

void shift_register_sixteen_bit_tilde_dsp(t_shift_register_sixteen_bit_tilde *x, t_signal **sp){
  dsp_add(shift_register_sixteen_bit_tilde_perform, 19, x,
          sp[0]->s_vec,
          sp[1]->s_vec,
          sp[2]->s_vec,
          sp[3]->s_vec,
          sp[4]->s_vec,
          sp[5]->s_vec,
          sp[6]->s_vec,
          sp[7]->s_vec,
          sp[8]->s_vec,
          sp[9]->s_vec,
          sp[10]->s_vec,
          sp[11]->s_vec,
          sp[12]->s_vec,
          sp[13]->s_vec,
          sp[14]->s_vec,
          sp[15]->s_vec,
          sp[16]->s_vec,
          sp[0]->s_n);
}

void * shift_register_sixteen_bit_tilde_new(t_floatarg f1, t_floatarg f2){

  t_shift_register_sixteen_bit_tilde * x = (t_shift_register_sixteen_bit_tilde *) pd_new(shift_register_sixteen_bit_tilde_class);

  x->state = 0;
  x->max_state_value = pow(2, 17)-1;

  x->in_state = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_state"));

  x->out = outlet_new(&x->x_obj, &s_signal);
  x->out2 = outlet_new(&x->x_obj, &s_signal);
  x->out3 = outlet_new(&x->x_obj, &s_signal);
  x->out4 = outlet_new(&x->x_obj, &s_signal);
  x->out5 = outlet_new(&x->x_obj, &s_signal);
  x->out6 = outlet_new(&x->x_obj, &s_signal);
  x->out7 = outlet_new(&x->x_obj, &s_signal);
  x->out8 = outlet_new(&x->x_obj, &s_signal);
  x->out9 = outlet_new(&x->x_obj, &s_signal);
  x->out10 = outlet_new(&x->x_obj, &s_signal);
  x->out11 = outlet_new(&x->x_obj, &s_signal);
  x->out12 = outlet_new(&x->x_obj, &s_signal);
  x->out13 = outlet_new(&x->x_obj, &s_signal);
  x->out14 = outlet_new(&x->x_obj, &s_signal);
  x->out15 = outlet_new(&x->x_obj, &s_signal);
  x->out16 = outlet_new(&x->x_obj, &s_signal);

  return (void *) x;
}

void shift_register_sixteen_bit_tilde_free(t_shift_register_sixteen_bit_tilde * x){
  inlet_free(x->in_state);
  outlet_free(x->out);
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
}

void shift_register_sixteen_bit_tilde_setup(void){
  shift_register_sixteen_bit_tilde_class = class_new(gensym("shift_register_sixteen_bit~"),
                             (t_newmethod) shift_register_sixteen_bit_tilde_new,
                             (t_method) shift_register_sixteen_bit_tilde_free,
                             sizeof(t_shift_register_sixteen_bit_tilde),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             0);

  class_addmethod(shift_register_sixteen_bit_tilde_class,
                  (t_method) shift_register_sixteen_bit_tilde_dsp,
                  gensym("dsp"),
                  0);

  class_addmethod(shift_register_sixteen_bit_tilde_class,
                  (t_method) shift_register_sixteen_bit_tilde_update_state,
                   gensym("in_state"),
                   A_DEFFLOAT,
                   0);

  CLASS_MAINSIGNALIN(shift_register_sixteen_bit_tilde_class, t_shift_register_sixteen_bit_tilde, f);
}
