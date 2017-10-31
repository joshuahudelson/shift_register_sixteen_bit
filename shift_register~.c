#include "m_pd.h"
#include <math.h>

static t_class *shift_register_tilde_class;

typedef struct _shift_register_tilde{
  t_object    x_obj;
  t_int       state, num_bits;
  t_inlet     * in_input, * in_state, * in_num_bits;
  t_outlet    * out_binary, * out_state;

  t_sample    input_signal_value, f;
} t_shift_register_tilde;

void shift_register_tilde_shift(t_shift_register_tilde * x){
  x->state >>= 1;
}

int shift_register_tilde_get_least_significant_bit(t_shift_register_tilde * x){
  return (x->state & 1);
}

void shift_register_tilde_put_most_significant_bit(t_shift_register_tilde * x, t_sample in1){
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
    int mask = 1 << x->num_bits;
    x->state |= mask;
  }
  else{
    int mask = 1 << x->num_bits;
    mask = ~mask;
    x->state &= mask;
  }
}

void shift_register_tilde_update_state(t_shift_register_tilde * x, t_float f){
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

//  outlet_float(x->out_binary, x->state & 1);
//  outlet_float(x->out_state, x->state);
}

void shift_register_tilde_set_num_bits(t_shift_register_tilde * x, t_float f){
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

//  outlet_float(x->out_binary, x->state & 1);
//  outlet_float(x->out_state, x->state);
}

t_int * shift_register_tilde_perform(t_int * w){

  t_shift_register_tilde *x   = (t_shift_register_tilde *)(w[1]);
  t_sample *in1               = (t_sample *)(w[2]);
  t_sample *out               = (t_sample *)(w[3]);
  t_sample *out2               = (t_sample *)(w[4]);
  int n                       = (int)(w[5]);


  while (n--) {

    t_sample output = (t_sample) shift_register_tilde_get_least_significant_bit(x);
    shift_register_tilde_shift(x);
    shift_register_tilde_put_most_significant_bit(x, *in1++);

    *out++ = output;
    *out2++ = output;
  }

  return (w+6);
}

void shift_register_tilde_dsp(t_shift_register_tilde *x, t_signal **sp){
  // 5 or 4?
  dsp_add(shift_register_tilde_perform, 5, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void * shift_register_tilde_new(t_floatarg f1, t_floatarg f2){

  t_shift_register_tilde * x = (t_shift_register_tilde *) pd_new(shift_register_tilde_class);

  x->state = 0;
  x->num_bits = 16;

  x->in_input     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
  x->in_num_bits  = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_num_bits2"));
  x->in_state     = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_state2"));
  outlet_new(&x->x_obj, &s_signal);
  outlet_new(&x->x_obj, &s_signal); // Don't need to assign to anything?  Interesting...

//  x->out_state    = outlet_new(&x->x_obj, &s_float);

  return (void *) x;
}

void shift_register_tilde_free(t_shift_register_tilde * x){
  inlet_free(x->in_input);
  inlet_free(x->in_state);
  inlet_free(x->in_num_bits);
  outlet_free(x->out_binary);
//  outlet_free(x->out_state);
}

void shift_register_tilde_setup(void){
  shift_register_tilde_class = class_new(gensym("shift_register~"),
                             (t_newmethod) shift_register_tilde_new,
                             (t_method) shift_register_tilde_free,
                             sizeof(t_shift_register_tilde),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             0);


  class_addmethod(shift_register_tilde_class,
                  (t_method) shift_register_tilde_dsp,
                  gensym("dsp"),
                  0);

  CLASS_MAINSIGNALIN(shift_register_tilde_class, t_shift_register_tilde, f);

  class_addmethod(shift_register_tilde_class,
                 (t_method) shift_register_tilde_set_num_bits,
                  gensym("in_num_bits2"),
                  A_DEFFLOAT,
                  0);

  class_addmethod(shift_register_tilde_class,
                  (t_method) shift_register_tilde_update_state,
                   gensym("in_state2"),
                   A_DEFFLOAT,
                   0);
}
