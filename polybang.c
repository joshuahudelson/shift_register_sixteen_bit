#include "m_pd.h"

static t_class * polybang_class;

typedef struct _polybang {
  t_object    x_obj;
  t_int       init_count, current_count;
  t_int       mod_A, mod_B;
  t_outlet    * out_A, * out_B, * out_synch, * out_count;
  t_inlet     * in_mod_A, * in_mod_B;
}t_polybang;

void polybang_setMods(t_polybang * x, t_floatarg f1, t_floatarg f2){
  x->mod_A = (f1 <= 0) ? 1 : f1;
  x->mod_B = (f2 <=0) ? 1: f2;
}

void polybang_resetCount(t_polybang * x){
  x->init_count = 0;
  x->current_count = x->init_count;
}

void polybang_onBangMSG(t_polybang * x){
  post("Ready to go.");
  outlet_bang(x->out_A);
  outlet_bang(x->out_B);
  outlet_bang(x->out_synch);
  outlet_float(x->out_count, 1);
}

void polybang_onResetMsg(t_polybang * x){
  polybang_resetCount(x);
}

void polybang_onListMsg(t_polybang * x, t_symbol * s, t_int argc, t_atom * argv){
  switch(argc){
    case 2:
      polybang_setMods(x, atom_getfloat(argv), atom_getfloat(argv+1));
      polybang_resetCount(x);
      break;
    default:
      error("two arguments are needed");
  }
}

void polybang_onSet_A(t_polybang * x, t_floatarg f){
  polybang_setMods(x, f, x->mod_B);
}

void polybang_onSet_B(t_polybang * x, t_floatarg f){
  polybang_setMods(x, x->mod_A, f);
}

void * polybang_new(t_floatarg f1, t_floatarg f2){
  t_polybang * x = (t_polybang *)pd_new(polybang_class);

  polybang_resetCount(x);

  polybang_setMods(x, f1, f2);

  x->out_A = outlet_new(&x->x_obj, &s_bang);
  x->out_B = outlet_new(&x->x_obj, &s_bang);
  x->out_synch = outlet_new(&x->x_obj, &s_bang);
  x->out_count = outlet_new(&x->x_obj, &s_float);

  x->in_mod_A = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("ratio_A"));
  x->in_mod_B = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("ratio_B"));

  return (void *) x;
}

void polybang_free(t_polybang * x){
  outlet_free(x->out_A);
  outlet_free(x->out_B);
  outlet_free(x->out_synch);
  outlet_free(x->out_count);

  inlet_free(x->in_mod_A);
  inlet_free(x->in_mod_B);
}

void polybang_setup(void){
  polybang_class = class_new(gensym("polybang"),
                             (t_newmethod) polybang_new,
                             (t_method)polybang_free,
                             sizeof(t_polybang),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             0);

  class_addbang(polybang_class, (t_method) polybang_onBangMSG);

  class_addmethod(polybang_class,
                 (t_method) polybang_onResetMsg,
                 gensym("reset"),
                 0);

  class_addlist(polybang_class,
                (t_method) polybang_onListMsg);

  class_addmethod(polybang_class,
                   (t_method) polybang_onSet_A,
                   gensym("ratio_A"),
                   A_DEFFLOAT,
                   0);

 class_addmethod(polybang_class,
                  (t_method) polybang_onSet_B,
                  gensym("ratio_B"),
                  A_DEFFLOAT,
                  0);
}
