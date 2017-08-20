#include "m_pd.h"

static t_class * polybang_class;

typedef struct _polybang {
  t_object    x_obj;
  t_int       init_count, current_count;
  t_int       mod_A, mod_B;
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
}

void * polybang_new(t_floatarg f1, t_floatarg f2){
  t_polybang * x = (t_polybang *)pd_new(polybang_class);

  polybang_resetCount(x);

  polybang_setMods(x, f1, f2);

  return (void *) x;
}

void polybang_setup(void){
  polybang_class = class_new(gensym("polybang"),
                             (t_newmethod) polybang_new,
                             0,
                             sizeof(t_polybang),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             A_DEFFLOAT,
                             0);

  class_addbang(polybang_class, (t_method) polybang_onBangMSG);
}
